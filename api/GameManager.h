#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <mutex>

#include "../game/ChessGame.h"

// ---------------------------------------------------------------------------
// GameManager
// ---------------------------------------------------------------------------
// Thread-safe in-memory registry of active ChessGame instances.
// Sits between Crow route handlers and ChessGame; contains NO chess logic.
// ---------------------------------------------------------------------------
class GameManager
{
public:
    // Create a new game and return its unique ID.
    std::string createGame();

    // Return a raw pointer to the game, or nullptr if the ID is unknown.
    ChessGame* getGame(const std::string& id);

    // Reset the game to its initial board state.
    // Returns false if the ID is unknown.
    bool resetGame(const std::string& id);

    // Delete the game from the registry.
    // Returns false if the ID was not found.
    bool deleteGame(const std::string& id);

private:
    std::unordered_map<std::string, std::unique_ptr<ChessGame>> games_;
    std::mutex mutex_;

    // Generate a random hex-string game ID.
    static std::string generateId();
};
