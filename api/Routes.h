#pragma once

#include <crow.h>
#include "GameManager.h"

// Register all REST routes + CORS on the given Crow app.
void registerRoutes(crow::SimpleApp& app, GameManager& manager);
