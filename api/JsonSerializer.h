#pragma once

#include <string>
#include <crow.h>

#include "../game/ChessGame.h"

// ---------------------------------------------------------------------------
// JsonSerializer
// ---------------------------------------------------------------------------
// Pure utility functions that convert ChessGame state -> crow::json::wvalue.
// Contains NO chess logic and NO HTTP handling.
// ---------------------------------------------------------------------------
namespace JsonSerializer
{
    // Full board response (used by GET /board and move responses).
    crow::json::wvalue boardResponse(const ChessGame& game,
                                     const std::string& gameId);

    // Status-only response (used by GET /status).
    crow::json::wvalue statusResponse(const ChessGame& game,
                                      const std::string& gameId);

    // Move response — wraps board + move outcome.
    crow::json::wvalue moveResponse(bool valid,
                                    const ChessGame& game,
                                    const std::string& gameId,
                                    const std::string& from,
                                    const std::string& to,
                                    const std::string& error = "");

    // Simple error response (HTTP 4xx payloads).
    crow::json::wvalue errorResponse(const std::string& message);

    // Helper: GameStatus enum -> JSON string
    std::string statusToString(GameStatus s);

    // Helper: Color enum -> JSON string
    std::string colorToString(Color c);
}
