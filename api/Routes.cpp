#include "Routes.h"
#include "JsonSerializer.h"

#include <crow.h>
#include <set>
#include <string>

// ---------------------------------------------------------------------------
// CORS helper
// ---------------------------------------------------------------------------
// Adds Access-Control headers to every response so a React dev server
// running on http://localhost:3000 or http://localhost:5173 can call us.
// ---------------------------------------------------------------------------
static void addCorsHeaders(crow::response &res) {
  res.add_header("Access-Control-Allow-Origin", "*");
  res.add_header("Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
  res.add_header("Access-Control-Allow-Headers", "Content-Type");
}

// Wrap a wvalue body with CORS headers and a status code.
static crow::response makeResponse(int code, crow::json::wvalue body) {
  crow::response res(code, body.dump());
  res.add_header("Content-Type", "application/json");
  addCorsHeaders(res);
  return res;
}

// ---------------------------------------------------------------------------
// Input helpers
// ---------------------------------------------------------------------------

// Validate a chess square string like "e2": exactly 2 chars, col a-h, row 1-8.
static bool isValidSquare(const std::string &s) {
  if (s.size() != 2)
    return false;
  if (s[0] < 'a' || s[0] > 'h')
    return false;
  if (s[1] < '1' || s[1] > '8')
    return false;
  return true;
}

// Valid promotion piece letters.
static const std::set<std::string> VALID_PROMOTIONS = {"q", "r", "b", "n"};

// ---------------------------------------------------------------------------
// registerRoutes
// ---------------------------------------------------------------------------
void registerRoutes(crow::SimpleApp &app, GameManager &manager) {
  // --------------------------------------------------------
  // OPTIONS pre-flight catch-all (CORS)
  // --------------------------------------------------------
  CROW_ROUTE(app, "/<path>")
      .methods(crow::HTTPMethod::OPTIONS)(
          [](const crow::request &, crow::response &res, const std::string &) {
            res.code = 204;
            addCorsHeaders(res);
            res.end();
          });

  // ========================================================
  // POST /api/game
  // Create a new game and return its ID.
  // ========================================================
  CROW_ROUTE(app, "/api/game")
      .methods(crow::HTTPMethod::POST)([&manager](const crow::request &) {
        std::string id = manager.createGame();
        ChessGame *game = manager.getGame(id);

        crow::json::wvalue resp;
        resp["gameId"] = id;
        resp["status"] = JsonSerializer::statusToString(game->getStatus());
        resp["turn"] = JsonSerializer::colorToString(game->getTurn());
        resp["inCheck"] = false;
        resp["winner"] = nullptr;

        return makeResponse(201, std::move(resp));
      });

  // ========================================================
  // GET /api/game/:id/board
  // Return the complete board state.
  // ========================================================
  CROW_ROUTE(app, "/api/game/<string>/board")
      .methods(crow::HTTPMethod::GET)(
          [&manager](const crow::request &, const std::string &id) {
            ChessGame *game = manager.getGame(id);
            if (!game)
              return makeResponse(
                  404, JsonSerializer::errorResponse("Game not found: " + id));

            return makeResponse(200, JsonSerializer::boardResponse(*game, id));
          });

  // ========================================================
  // POST /api/game/:id/move
  // Make a move.  Body: { "from": "e2", "to": "e4" }
  //              Optional: { "promotion": "q" }  (required if promotion)
  // ========================================================
  CROW_ROUTE(app, "/api/game/<string>/move")
      .methods(crow::HTTPMethod::POST)([&manager](const crow::request &req,
                                                  const std::string &id) {
        // --- Locate game ---
        ChessGame *game = manager.getGame(id);
        if (!game)
          return makeResponse(
              404, JsonSerializer::errorResponse("Game not found: " + id));

        // --- Parse JSON body ---
        crow::json::rvalue body;
        try {
          body = crow::json::load(req.body);
        } catch (...) {
          return makeResponse(
              400, JsonSerializer::errorResponse("Malformed JSON body."));
        }

        if (!body || body.t() != crow::json::type::Object)
          return makeResponse(400, JsonSerializer::errorResponse(
                                       "Request body must be a JSON object."));

        // --- Validate required fields ---
        if (!body.has("from") || !body.has("to"))
          return makeResponse(
              400, JsonSerializer::errorResponse(
                       "Missing required fields: \"from\" and \"to\"."));

        std::string from, to;
        try {
          from = body["from"].s();
          to = body["to"].s();
        } catch (...) {
          return makeResponse(400, JsonSerializer::errorResponse(
                                       "\"from\" and \"to\" must be strings."));
        }

        // --- Validate square format ---
        if (!isValidSquare(from))
          return makeResponse(
              400,
              JsonSerializer::errorResponse(
                  "\"from\" is not a valid square (expected a1-h8, got \"" +
                  from + "\")."));

        if (!isValidSquare(to))
          return makeResponse(
              400, JsonSerializer::errorResponse(
                       "\"to\" is not a valid square (expected a1-h8, got \"" +
                       to + "\")."));

        // --- Promotion field handling ---
        // Detect whether this is a promotion move BEFORE calling makeMove.
        std::string promotion;
        bool needsPromotion = game->isPromotionMove(from, to);

        if (needsPromotion) {
          if (!body.has("promotion"))
            return makeResponse(
                400,
                JsonSerializer::errorResponse(
                    "This move promotes a pawn. Please specify \"promotion\": "
                    "\"q\" (queen), \"r\" (rook), \"b\" (bishop), or \"n\" "
                    "(knight)."));

          try {
            promotion = body["promotion"].s();
          } catch (...) {
            return makeResponse(400, JsonSerializer::errorResponse(
                                         "\"promotion\" must be a string: "
                                         "\"q\", \"r\", \"b\", or \"n\"."));
          }

          if (VALID_PROMOTIONS.find(promotion) == VALID_PROMOTIONS.end())
            return makeResponse(400,
                                JsonSerializer::errorResponse(
                                    "Invalid promotion piece \"" + promotion +
                                    "\". "
                                    "Use \"q\" (queen), \"r\" (rook), \"b\" "
                                    "(bishop), or \"n\" (knight)."));
        }
        // For non-promotion moves, ignore the promotion field entirely.

        // --- Attempt the move (delegates to existing ChessGame logic) ---
        bool valid = game->makeMove(from, to, promotion);

        if (!valid) {
          // The engine rejected the move.  Return 200 with valid:false so the
          // React client can display the error without treating it as an HTTP
          // error.
          return makeResponse(
              200, JsonSerializer::moveResponse(false, *game, id, from, to,
                                                "Invalid move."));
        }

        return makeResponse(
            200, JsonSerializer::moveResponse(true, *game, id, from, to));
      });

  // ========================================================
  // GET /api/game/:id/status
  // Return turn, status, inCheck, winner.
  // ========================================================
  CROW_ROUTE(app, "/api/game/<string>/status")
      .methods(crow::HTTPMethod::GET)(
          [&manager](const crow::request &, const std::string &id) {
            ChessGame *game = manager.getGame(id);
            if (!game)
              return makeResponse(
                  404, JsonSerializer::errorResponse("Game not found: " + id));

            return makeResponse(200, JsonSerializer::statusResponse(*game, id));
          });

  // ========================================================
  // POST /api/game/:id/reset
  // Reset the game to the initial board position.
  // ========================================================
  CROW_ROUTE(app, "/api/game/<string>/reset")
      .methods(crow::HTTPMethod::POST)(
          [&manager](const crow::request &, const std::string &id) {
            if (!manager.resetGame(id))
              return makeResponse(
                  404, JsonSerializer::errorResponse("Game not found: " + id));

            ChessGame *game = manager.getGame(id);

            crow::json::wvalue resp;
            resp["gameId"] = id;
            resp["status"] = JsonSerializer::statusToString(game->getStatus());
            resp["turn"] = JsonSerializer::colorToString(game->getTurn());
            resp["inCheck"] = false;
            resp["winner"] = nullptr;
            resp["message"] = "Game reset successfully.";

            return makeResponse(200, std::move(resp));
          });

  // ========================================================
  // DELETE /api/game/:id
  // Remove the game from memory.
  // ========================================================
  CROW_ROUTE(app, "/api/game/<string>")
      .methods(crow::HTTPMethod::DELETE)(
          [&manager](const crow::request &, const std::string &id) {
            if (!manager.deleteGame(id))
              return makeResponse(
                  404, JsonSerializer::errorResponse("Game not found: " + id));

            crow::json::wvalue resp;
            resp["gameId"] = id;
            resp["message"] = "Game deleted.";

            return makeResponse(200, std::move(resp));
          });
}
