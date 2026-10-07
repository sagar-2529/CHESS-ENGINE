#include <crow.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "GameManager.h"
#include "Routes.h"

// ---------------------------------------------------------------------------
// Chess Engine — Crow REST API Server
// ---------------------------------------------------------------------------
// This is the entry point for the HTTP backend.
// The original terminal entry point lives in ../main.cpp (unchanged).
//
// Build:
//   g++ -std=c++17 \
//       $(find .. -name "*.cpp" ! -path "../main.cpp" ! -path "../api/main.cpp") \
//       api/main.cpp \
//       -I/opt/homebrew/include \
//       -L/opt/homebrew/lib \
//       -lpthread \
//       -o chess_server
//
// Run:
//   ./chess_server
// ---------------------------------------------------------------------------

int main()
{
    crow::SimpleApp app;
    GameManager manager;

    registerRoutes(app, manager);

    int port = 8080;
    if (const char* portValue = std::getenv("PORT"))
    {
        try
        {
            port = std::stoi(portValue);
        }
        catch (const std::exception&)
        {
            std::cerr << "Invalid PORT value; using 8080 instead.\n";
        }
    }

    std::cout << "Chess API server starting on port " << port << "\n";
    std::cout << "Press Ctrl+C to stop.\n";

    app.bindaddr("0.0.0.0")
       .port(port)
       .multithreaded()
       .run();

    return 0;
}
