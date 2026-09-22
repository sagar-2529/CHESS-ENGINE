#include "JsonSerializer.h"

#include <vector>
#include <string>

namespace JsonSerializer
{

// ---------------------------------------------------------------------------
// Enum helpers
// ---------------------------------------------------------------------------

std::string statusToString(GameStatus s)
{
    switch (s)
    {
        case GameStatus::IN_PROGRESS: return "IN_PROGRESS";
        case GameStatus::CHECK:       return "CHECK";
        case GameStatus::CHECKMATE:   return "CHECKMATE";
        case GameStatus::STALEMATE:   return "STALEMATE";
        case GameStatus::DRAW:        return "DRAW";
    }
    return "UNKNOWN";
}

std::string colorToString(Color c)
{
    return (c == Color::WHITE) ? "WHITE" : "BLACK";
}

// ---------------------------------------------------------------------------
// Private helper: determine the winner string (null-equivalent = "")
// ---------------------------------------------------------------------------
static std::string winnerString(const ChessGame& game)
{
    if (game.getStatus() == GameStatus::CHECKMATE)
    {
        // The player who just moved won; it is now the losing player's turn.
        // So the winner is the opposite of the current turn.
        return (game.getTurn() == Color::WHITE) ? "BLACK" : "WHITE";
    }
    return "";  // no winner yet; callers map "" -> null
}

// ---------------------------------------------------------------------------
// Private helper: build the 8x8 board JSON array
// ---------------------------------------------------------------------------
static crow::json::wvalue buildBoardJson(const ChessGame& game)
{
    auto grid = game.getBoardGrid();           // vector<vector<string>>

    crow::json::wvalue boardJson;
    for (int r = 0; r < 8; ++r)
    {
        crow::json::wvalue row;
        for (int c = 0; c < 8; ++c)
            row[c] = grid[r][c];
        boardJson[r] = std::move(row);
    }
    return boardJson;
}

// ---------------------------------------------------------------------------
// Public serializers
// ---------------------------------------------------------------------------

crow::json::wvalue boardResponse(const ChessGame& game,
                                 const std::string& gameId)
{
    crow::json::wvalue resp;
    resp["gameId"]  = gameId;
    resp["turn"]    = colorToString(game.getTurn());
    resp["status"]  = statusToString(game.getStatus());
    resp["inCheck"] = game.isInCheck();
    resp["board"]   = buildBoardJson(game);

    std::string winner = winnerString(game);
    if (winner.empty())
        resp["winner"] = nullptr;
    else
        resp["winner"] = winner;

    return resp;
}

crow::json::wvalue statusResponse(const ChessGame& game,
                                  const std::string& gameId)
{
    crow::json::wvalue resp;
    resp["gameId"]  = gameId;
    resp["turn"]    = colorToString(game.getTurn());
    resp["status"]  = statusToString(game.getStatus());
    resp["inCheck"] = game.isInCheck();

    std::string winner = winnerString(game);
    if (winner.empty())
        resp["winner"] = nullptr;
    else
        resp["winner"] = winner;

    return resp;
}

crow::json::wvalue moveResponse(bool valid,
                                const ChessGame& game,
                                const std::string& gameId,
                                const std::string& from,
                                const std::string& to,
                                const std::string& error)
{
    crow::json::wvalue resp;
    resp["valid"]   = valid;
    resp["from"]    = from;
    resp["to"]      = to;
    resp["gameId"]  = gameId;
    resp["turn"]    = colorToString(game.getTurn());
    resp["status"]  = statusToString(game.getStatus());
    resp["inCheck"] = game.isInCheck();
    resp["board"]   = buildBoardJson(game);

    std::string winner = winnerString(game);
    if (winner.empty())
        resp["winner"] = nullptr;
    else
        resp["winner"] = winner;

    if (!error.empty())
        resp["error"] = error;

    return resp;
}

crow::json::wvalue errorResponse(const std::string& message)
{
    crow::json::wvalue resp;
    resp["error"] = message;
    return resp;
}

} // namespace JsonSerializer
