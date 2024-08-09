import os
import ctypes
import subprocess
import chess

# If the shared library does not exist, compile it
if not os.path.isfile('evaluation/eval.so'):
    print('Compiling the shared library...')
    subprocess.run(['g++', '-shared', '-fPIC', '-std=c++17', '-o', 'evaluation/eval.so', 'evaluation/eval.cpp'], check=True)

# Load the shared library
lib = ctypes.CDLL('evaluation/eval.so')

# Define the functions
lib.init_tables.argtypes = []  # No arguments
lib.init_tables.restype = None

lib.evaluateBoard.argtypes = [ctypes.c_char_p]  # FEN string as argument
lib.evaluateBoard.restype = ctypes.c_int

# Initialize tables
lib.init_tables()

def evaluate_board(fen: str) -> int:
    return lib.evaluateBoard(fen.encode('utf-8'))

def search_best_move(board: chess.Board, seconds_left: float) -> chess.Move:
    
    # For now select the move with best score. (Depth 1 currently)
    best_score = -999999999
    best_move = chess.Move.null()
    for current_move in list(board.legal_moves):
        board_copy = board.copy()
        board_copy.push(current_move)
        current_score = evaluate_board(board_copy.fen())
        if board_copy.is_checkmate():
                print("---> Returning due to putting enemy in checkmate: " + current_move.uci() + " with score: " + str(best_score))
                return current_move
        if current_score > best_score:
            best_score = current_score
            best_move = current_move
        
    print("---> Best move selected: " + best_move.uci() + " with score: " + str(best_score))
    return best_move