#include <Python.h>
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <thread>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include "eval.hpp"

const char *D_INTERPRETER_PATH = std::getenv("D_INTERPRETER_PATH");
const char *D_SOURCE_PATH = std::getenv("D_SOURCE_PATH");

namespace py
{

    PyObject *name = nullptr;
    PyObject *load_module = nullptr;

    void initializePython()
    {
        // Check if env. vars valid
        if (D_INTERPRETER_PATH == nullptr || D_SOURCE_PATH == nullptr)
        {
            throw std::runtime_error("---> Environment variables D_INTERPRETER_PATH or D_SOURCE_PATH are not set");
        }

        // Set env. vars
        Py_Initialize();
        PyObject *sys_path = PySys_GetObject("path");
        if (sys_path == nullptr)
        {
            PyErr_Print();
            throw std::runtime_error("---> Failed to get sys.path");
        }
        PyList_Append(sys_path, PyUnicode_FromString(D_SOURCE_PATH));
        PyList_Append(sys_path, PyUnicode_FromString(D_INTERPRETER_PATH));

        // Load module
        name = PyUnicode_FromString((char *)"script");
        load_module = PyImport_Import(name);

        if (name == nullptr || load_module == nullptr)
        {
            PyErr_Print();
            throw std::runtime_error("---> Failed to load module");
        }
    }

    void finalizePython()
    {
        // Set reference counts to 0 for python gc
        Py_SET_REFCNT(name, 0);
        Py_SET_REFCNT(load_module, 0);
        Py_Finalize();
    }

    void testFunction()
    {
        // Call function
        PyObject *func = PyObject_GetAttrString(py::load_module, (char *)"testFunction");
        PyObject *callfunc = PyObject_CallObject(func, NULL);
        if (PyErr_Occurred())
        {
            PyErr_Print();
            throw std::runtime_error("---> Error in testFunction");
        }
        double result = PyFloat_AsDouble(callfunc);
        std::cout << "Result: " << result << std::endl;

        // Set reference counts to 0 for python gc,
        Py_XDECREF(callfunc);
        Py_XDECREF(func);
    }

    bool isCheckmate(const std::string &fenBoard)
    {
        // Call function with fen arg
        PyObject *func = PyObject_GetAttrString(py::load_module, (char *)"is_checkmate");
        PyObject *args = PyTuple_Pack(1, PyUnicode_FromString(fenBoard.c_str()));
        PyObject *callfunc = PyObject_CallObject(func, args);
        if (PyErr_Occurred())
        {
            PyErr_Print();
            throw std::runtime_error("---> Error in isCheckmate");
        }
        bool res = PyObject_IsTrue(callfunc);

        // Set reference counts
        Py_XDECREF(callfunc);
        Py_XDECREF(args);
        Py_XDECREF(func);

        return res;
    }

    bool isStalemate(const std::string &fenBoard)
    {
        // Call function with fen arg
        PyObject *func = PyObject_GetAttrString(py::load_module, (char *)"is_stalemate");
        PyObject *args = PyTuple_Pack(1, PyUnicode_FromString(fenBoard.c_str()));
        PyObject *callfunc = PyObject_CallObject(func, args);
        if (PyErr_Occurred())
        {
            PyErr_Print();
            throw std::runtime_error("---> Error in is_stalemate");
        }
        bool res = PyObject_IsTrue(callfunc);

        // Set reference counts
        Py_XDECREF(callfunc);
        Py_XDECREF(args);
        Py_XDECREF(func);

        return res;
    }

    bool isInsufficientMaterial(const std::string &fenBoard)
    {
        // Call function with fen arg
        PyObject *func = PyObject_GetAttrString(py::load_module, (char *)"is_insufficient_material");
        PyObject *args = PyTuple_Pack(1, PyUnicode_FromString(fenBoard.c_str()));
        PyObject *callfunc = PyObject_CallObject(func, args);
        if (PyErr_Occurred())
        {
            PyErr_Print();
            throw std::runtime_error("---> Error in is_insufficient_material");
        }
        bool res = PyObject_IsTrue(callfunc);

        // Set reference counts
        Py_XDECREF(callfunc);
        Py_XDECREF(args);
        Py_XDECREF(func);

        return res;
    }
        
    bool isGameOver(const std::string &fenBoard)
    {
        // Call function with fen arg
        PyObject *func = PyObject_GetAttrString(py::load_module, (char *)"is_game_over");
        PyObject *args = PyTuple_Pack(1, PyUnicode_FromString(fenBoard.c_str()));
        PyObject *callfunc = PyObject_CallObject(func, args);
        if (PyErr_Occurred())
        {
            PyErr_Print();
            throw std::runtime_error("---> Error in is_game_over");
        }
        bool res = PyObject_IsTrue(callfunc);

        // Set reference counts
        Py_XDECREF(callfunc);
        Py_XDECREF(args);
        Py_XDECREF(func);

        return res;
    }
    
    bool isDraw(const std::string &fenBoard){
        // Call function with fen arg
        PyObject *func = PyObject_GetAttrString(py::load_module, (char *)"is_draw");
        PyObject *args = PyTuple_Pack(1, PyUnicode_FromString(fenBoard.c_str()));
        PyObject *callfunc = PyObject_CallObject(func, args);
        if (PyErr_Occurred())
        {
            PyErr_Print();
            throw std::runtime_error("---> Error in is_draw");
        }
        bool res = PyObject_IsTrue(callfunc);

        // Set reference counts
        Py_XDECREF(callfunc);
        Py_XDECREF(args);
        Py_XDECREF(func);

        return res;
    }

    bool isTurnWhite(const std::string &fen) {
        return fen.find("w") != std::string::npos;
    }

    std::vector<std::string> generateLegalMoves(const std::string &fenBoard) {
        // Call function with fen arg
        PyObject *func = PyObject_GetAttrString(py::load_module, "generate_legal_moves");
        if (!func || !PyCallable_Check(func)) {
            PyErr_Print();
            throw std::runtime_error("Error: generate_legal_moves function not found or not callable");
        }
    
        PyObject *args = PyTuple_Pack(1, PyUnicode_FromString(fenBoard.c_str()));
        if (!args) {
            PyErr_Print();
            Py_XDECREF(func);
            throw std::runtime_error("Error: Failed to create arguments tuple");
        }
    
        PyObject *callfunc = PyObject_CallObject(func, args);
        if (!callfunc) {
            PyErr_Print();
            Py_XDECREF(args);
            Py_XDECREF(func);
            throw std::runtime_error("Error: Failed to call generate_legal_moves");
        }
    
        if (!PyList_Check(callfunc)) {
            PyErr_Print();
            Py_XDECREF(callfunc);
            Py_XDECREF(args);
            Py_XDECREF(func);
            throw std::runtime_error("Error: Expected a list as return value");
        }
    
        std::vector<std::string> res;
        Py_ssize_t size = PyList_Size(callfunc);
        for (Py_ssize_t i = 0; i < size; ++i) {
            PyObject *item = PyList_GetItem(callfunc, i);
            if (!item) {
                PyErr_Print();
                Py_XDECREF(callfunc);
                Py_XDECREF(args);
                Py_XDECREF(func);
                throw std::runtime_error("Error: Failed to get item from list");
            }
            const char *move = PyUnicode_AsUTF8(item);
            if (!move) {
                PyErr_Print();
                Py_XDECREF(callfunc);
                Py_XDECREF(args);
                Py_XDECREF(func);
                throw std::runtime_error("Error: Failed to convert item to string");
            }
            res.push_back(move);
        }
    
        // Check for errors
        if (PyErr_Occurred()) {
            PyErr_Print();
            Py_XDECREF(callfunc);
            Py_XDECREF(args);
            Py_XDECREF(func);
            throw std::runtime_error("Error in generateLegalMoves");
        }
    
        // Decrease reference counts
        Py_XDECREF(callfunc);
        Py_XDECREF(args);
        Py_XDECREF(func);
    
        return res;
    }
    
    std::string makeMove(const std::string &fenBoard, const std::string &move) // Returns new board
    {
        // Call function with fen arg
        PyObject *func = PyObject_GetAttrString(py::load_module, (char *)"make_move");
        PyObject *args = PyTuple_Pack(2, PyUnicode_FromString(fenBoard.c_str()), PyUnicode_FromString(move.c_str()));
        PyObject *callfunc = PyObject_CallObject(func, args);
        if (PyErr_Occurred())
        {
            PyErr_Print();
            throw std::runtime_error("---> Error in makeMove");
        }
        std::string res = PyUnicode_AsUTF8(callfunc);

        // Set reference counts
        Py_XDECREF(callfunc);
        Py_XDECREF(args);
        Py_XDECREF(func);

        return res;
    }

    void printAllMoves(const std::vector<std::string> &moves)
    {
        for (int i = 0; i < moves.size(); i++)
        {
            std::cout << moves[i] << std::endl;
        }
    }
   
}

namespace chess
{
    enum class Player{
        WHITE_PLAYER,
        BLACK_PLAYER
    };

    enum class GameState{
        ONGOING,
        WIN,
        LOSE,
        DRAW
    };

    std::string getCurrentTimeStamp() {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%d.%m.%Y_%H:%M:%S");
        return oss.str();
    }
    
    const std::string timeStamp = getCurrentTimeStamp();
    const int MAX_SEARCH_DEPTH = 3;
    const int64_t WIN_SCORE = 1000000000;
    const int64_t DRAW_SCORE = 0;
    const int64_t LOST_SCORE = -1000000000;
    bool stopSearch = false;
    Player player;

    int evaluate(const std::string &fenBoard){ 
        if(player == Player::BLACK_PLAYER){ return -eval::evaluateBoard(fenBoard); }
        return eval::evaluateBoard(fenBoard); 
    }

    int depthToBeSearched(const float secondsLeftToMakeMove){
        throw std::runtime_error("Not implemented yet");
    }

    struct boardState{
        static int bsCounter;
        static int64_t bestScoresForDepth[MAX_SEARCH_DEPTH] ; // index: depth, value score

        std::vector<std::string> legalMoves;
        std::vector<boardState*> subBoardStates;
        std::string fenBoard;
        std::string prevMoveMade = "NULL";
        
        Player turn;
        boardState* parentBS;
        GameState gameState;
        int64_t evaluationScore = -123456789;
        signed short depth = -1;
        bool worthSearching = true;

        boardState(const std::string &fenBoard, int depth, boardState* parentBS = nullptr, std::string moveMade = "NULL"){
            bsCounter++;
            // Initialize vars
            this->fenBoard = fenBoard;
            this->depth = depth;
            this->parentBS = parentBS;
            this->prevMoveMade = moveMade;
            legalMoves = py::generateLegalMoves(fenBoard);

            // Check turn and evaluate board state
            turn = py::isTurnWhite(fenBoard) ? Player::WHITE_PLAYER : Player::BLACK_PLAYER;
            if(depth == 0){ chess::player = turn; }

            if(depth == MAX_SEARCH_DEPTH){
                evaluationScore = evaluate(fenBoard);
            }
            
            if(legalMoves.empty()){
                if(py::isCheckmate(fenBoard)){
                    evaluationScore = WIN_SCORE;
                    gameState = GameState::WIN;
                }
                else if(py::isDraw(fenBoard)){ 
                    evaluationScore = DRAW_SCORE;
                    gameState = GameState::DRAW;
                }
                else{
                    evaluationScore = LOST_SCORE;
                    gameState = GameState::LOSE;
                }
            }

            
        }


        boardState* makeMove(const std::string moveToMake){
            boardState *newBS = new boardState(py::makeMove(fenBoard, moveToMake), this->depth + 1, this, moveToMake);
            subBoardStates.push_back(newBS);
            return newBS;
        }

        void printToTextFile() {
            std::string fileName = "ST/output_" + timeStamp + ".txt";
            std::ofstream outFile(fileName, std::ios::app);
            if (outFile.is_open()) {
                // Add title line if the file is empty
                if (outFile.tellp() == 0) {
                    outFile << "ParentBoard\tFEN Board\tTurn\tGame State\tEvaluation Score\tDepth\tMoveMade\n";
                }
                outFile << (parentBS ? "0x" + std::to_string(reinterpret_cast<uintptr_t>(parentBS)) : "0x0000000000") << "\t"
                        << "\"" << fenBoard << "\"" << "\t"
                        << (turn == Player::WHITE_PLAYER ? "WHITE" : "BLACK") << "\t"
                        << (gameState == GameState::WIN ? "WIN" : 
                            gameState == GameState::DRAW ? "DRAW" : 
                            gameState == GameState::LOSE ? "LOSE" : "ONGOING") << "\t"
                        << evaluationScore << "\t"
                        << depth << "\t"
                        << prevMoveMade << "\n";
                outFile.close();
            } else {
                std::cerr << "Unable to open file" << std::endl;
            }
        }

        bool isBranchWorthSearching(){
            throw std::runtime_error("Not implemented");
            return 0;
        }

        bool isTurnMine(){ return turn == chess::player; }

        ~boardState(){
            for(int i=0; i < subBoardStates.size(); i++){
                delete subBoardStates[i];
            }
            subBoardStates.clear();
        }

    };
    
    // Definitions for static variables
    int boardState::bsCounter = 0;
    int64_t boardState::bestScoresForDepth[MAX_SEARCH_DEPTH] = { INT64_MIN };

    void stampTextFile(std::chrono::system_clock::time_point begin, std::chrono::system_clock::time_point end) {
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        std::time_t begin_time_t = std::chrono::system_clock::to_time_t(begin);
        std::time_t end_time_t = std::chrono::system_clock::to_time_t(end);
        std::tm begin_local_time = *std::localtime(&begin_time_t);
        std::tm end_local_time = *std::localtime(&end_time_t);
        std::string fileName = "ST/output_" + chess::timeStamp + ".txt";
        std::ofstream outFile(fileName, std::ios::app);
        if (outFile.is_open()) {
            outFile << "Begin Timestamp: " << std::put_time(&begin_local_time, "%H:%M:%S") << "." 
                    << std::setw(6) << std::setfill('0') << (std::chrono::duration_cast<std::chrono::microseconds>(begin.time_since_epoch()).count() % 1000000) << "\n"
                    << "End Timestamp: " << std::put_time(&end_local_time, "%H:%M:%S") << "." 
                    << std::setw(6) << std::setfill('0') << (std::chrono::duration_cast<std::chrono::microseconds>(end.time_since_epoch()).count() % 1000000) << "\n"
                    << "Duration: " << (float)duration/1000000 << " second\n"
                    << "Board State Counter: " << boardState::bsCounter << "\n"
                    << "Max Search Depth: " << MAX_SEARCH_DEPTH << "\n"
                    << "Search Stopped Prematurely: " << (stopSearch ? "YES" : "NO") << "\n";
            outFile.close();
        } else {
            std::cerr << "Unable to open file" << std::endl;
        }
        std::cout << "Duration: " << (float)duration/1000000 << " second\n";
        std::cout << "Games Searched: " << boardState::bsCounter << "\n";
    }
    

    struct blindSearch{

        static void updateBestScoresForDepth(){
            // Check my notebook
        }

        static void getBestMoveHelper(boardState* board){

            if(board->gameState != GameState::ONGOING && board->depth == 1){ // If there move to win do it
                stopSearch = true;
                return;
            } 
            else if(stopSearch || board->depth == MAX_SEARCH_DEPTH){
                return;
            }

            if(board->isTurnMine()){ // If turn is mine then best move goes up
                int64_t bestScore = INT64_MIN;
                for(int i=0; i < board->legalMoves.size(); i++){
                    boardState* subBS = board->makeMove(board->legalMoves.at(i));
                    getBestMoveHelper(subBS);

                    if(bestScore < subBS->evaluationScore){
                        bestScore = subBS->evaluationScore;
                    }
                }
                board->evaluationScore = bestScore;
            }
            else{ // If opponent turn then best move for opponent goes up
                int64_t worseScore = INT64_MIN;
                for(int i=0; i < board->legalMoves.size(); i++){
                    boardState* subBS = board->makeMove(board->legalMoves.at(i));
                    getBestMoveHelper(subBS);

                    if(worseScore > subBS->evaluationScore){
                        worseScore = subBS->evaluationScore;
                    }
                }
                board->evaluationScore = worseScore;
            }

            // *** Comment this in actual run
            board->printToTextFile();
            // ***
            
        }

        static std::string getBestMove(std::string fenBoard){
            bool stopSearch = false;
            std::string bestMove = "NULL";
            boardState* board = new boardState(fenBoard, 0);
            getBestMoveHelper(board);
            int64_t bestScore = INT64_MIN;
            for(int i=0; i < board->subBoardStates.size(); i++){
                boardState* subBoard = board->subBoardStates.at(i);
                if(bestScore < subBoard->evaluationScore){
                    bestScore = subBoard->evaluationScore;
                    bestMove = subBoard->prevMoveMade;
                }
            }
            delete board;
            std::cout << "\nBest Move Found: " << bestMove << "\n";
            return bestMove;
        }
    
    };
    
    std::chrono::time_point<std::chrono::system_clock> timeBegin;
    std::chrono::time_point<std::chrono::system_clock> timeEnd;

    void initialize(){
        py::initializePython();
        std::fill(boardState::bestScoresForDepth, boardState::bestScoresForDepth + MAX_SEARCH_DEPTH, INT64_MIN);
        timeBegin = std::chrono::system_clock::now();
    }

    void finalize(){
        timeEnd = std::chrono::system_clock::now();
        py::finalizePython();
        stampTextFile(timeBegin, timeEnd);
    }

}

int main(int argc, char *argv[])
{
    
    chess::initialize();
    chess::blindSearch::getBestMove("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    chess::finalize();
    
    
    return 0;
}
