#include <crow.h>
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

    std::cout << "Chess API server starting on http://localhost:8080\n";
    std::cout << "Press Ctrl+C to stop.\n";

    app.port(8080)
       .multithreaded()
       .run();

    return 0;
}
