import sys
import chess

def testFunction() -> float:
    return 12.34

def is_checkmate(fen_board: str) -> bool:
    board = chess.Board(fen=fen_board)
    return board.is_checkmate()
    
def is_stalemate(fen_board: str) -> bool:
    board = chess.Board(fen=fen_board)
    return board.is_stalemate()

def is_insufficient_material(fen_board: str) -> bool:
    board = chess.Board(fen=fen_board)
    return board.is_insufficient_material()

def generate_legal_moves(fen_board: str) -> list:
    board = chess.Board(fen=fen_board)
    return [move.uci() for move in board.legal_moves]

def make_move(fen_board: str, move: str) -> str:
    board = chess.Board(fen=fen_board)
    board.push(chess.Move.from_uci(move))
    return board.fen()

