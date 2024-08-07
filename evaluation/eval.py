import os
import ctypes
import subprocess

# If the shared library does not exist, compile it
if not os.path.isfile('./eval.so'):
    subprocess.run(['g++', '-shared', '-fPIC', '-std=c++17', '-o', 'eval.so', 'eval.cpp'], check=True)

# Load the shared library
lib = ctypes.CDLL('./eval.so')

# Define the functions
lib.init_tables.argtypes = []  # No arguments
lib.init_tables.restype = None

lib.evaluateBoard.argtypes = [ctypes.c_char_p]  # FEN string as argument
lib.evaluateBoard.restype = ctypes.c_int

# Initialize tables
lib.init_tables()

# fen = "1n2k3/ppp5/8/8/8/2N2N2/PPPPPPPP/R1BQKB2 w Q - 0 1"
# score = lib.evaluateBoard(fen.encode('utf-8'))
# print(f"Evaluation score: {score}")

def evaluate_board(fen: str) -> int:
    return lib.evaluateBoard(fen.encode('utf-8'))