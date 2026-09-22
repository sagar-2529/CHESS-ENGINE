#include "GameManager.h"

#include <random>
#include <sstream>
#include <iomanip>

// ---------------------------------------------------------------------------
// Private helpers
// ---------------------------------------------------------------------------

std::string GameManager::generateId()
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist;

    uint64_t hi = dist(gen);
    uint64_t lo = dist(gen);

    std::ostringstream oss;
    oss << std::hex << std::setfill('0')
        << std::setw(16) << hi
        << std::setw(16) << lo;
    return oss.str();
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

std::string GameManager::createGame()
{
    std::lock_guard<std::mutex> lock(mutex_);

    std::string id = generateId();
    // Extremely unlikely collision, but guard against it.
    while (games_.count(id))
        id = generateId();

    games_[id] = std::make_unique<ChessGame>();
    return id;
}

ChessGame* GameManager::getGame(const std::string& id)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = games_.find(id);
    if (it == games_.end())
        return nullptr;

    return it->second.get();
}

bool GameManager::resetGame(const std::string& id)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = games_.find(id);
    if (it == games_.end())
        return false;

    it->second->reset();
    return true;
}

bool GameManager::deleteGame(const std::string& id)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = games_.find(id);
    if (it == games_.end())
        return false;

    games_.erase(it);
    return true;
}
