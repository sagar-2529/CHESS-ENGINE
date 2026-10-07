cmake_minimum_required(VERSION 3.20)

project(chess_api LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# Pin Crow so production builds are repeatable. Crow is header-oriented, but
# its CMake target resolves its Boost/Asio and thread dependencies for us.
set(CROW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CROW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    Crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG v1.3.3
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Crow)

add_executable(chess_server
    api/main.cpp
    api/GameManager.cpp
    api/JsonSerializer.cpp
    api/Routes.cpp
    board/Board.cpp
    board/Move.cpp
    board/Position.cpp
    board/StandardBoardInitializer.cpp
    game/ChessGame.cpp
    pieces/Bishop.cpp
    pieces/King.cpp
    pieces/Knight.cpp
    pieces/Pawn.cpp
    pieces/Piece.cpp
    pieces/Queen.cpp
    pieces/Rook.cpp
    rules/MoveValidator.cpp
    user/Player.cpp
    utlis/MoveGenerator.cpp
)

target_link_libraries(chess_server PRIVATE Crow::Crow)
