#include <string>
#include "main.cpp"

extern "C" {
    const char* get_best_move(const char* fen) {
        static std::string bestMove;
        bestMove = chess::getBestMove(fen);
        return bestMove.c_str();
    }
}