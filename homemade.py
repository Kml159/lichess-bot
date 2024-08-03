"""
Some example classes for people who want to create a homemade bot.

With these classes, bot makers will not have to implement the UCI or XBoard interfaces themselves.
"""
import chess
from chess.engine import PlayResult, Limit
import random
from lib.engine_wrapper import MinimalEngine
from lib.types import MOVE, HOMEMADE_ARGS_TYPE
import logging


# Use this logger variable to print messages to the console or log files.
# logger.info("message") will always print "message" to the console or log file.
# logger.debug("message") will only print "message" if verbose logging is enabled.
logger = logging.getLogger(__name__)


class ExampleEngine(MinimalEngine):
    """An example engine that all homemade engines inherit."""

    pass

###
# Dictionary for piece values
piece_values = {
    chess.PAWN: 100,
    chess.KNIGHT: 300,
    chess.BISHOP: 300,
    chess.ROOK: 500,
    chess.QUEEN: 900,
    chess.KING: 10000000
}

class MyEngine(MinimalEngine):
        
    def board_evaluation(self, board: chess.Board) -> int:
        # Check if pieces in threat
        score = 0
        return score
    
    def get_my_time(self, board: chess.Board, time_limit: Limit):
        ret = 999999999
        if board.turn == chess.WHITE and time_limit.white_clock != None:
            ret = time_limit.white_clock
        elif board.turn == chess.BLACK and time_limit.black_clock != None:
            ret = time_limit.black_clock
        return ret
                
    def search(self, board: chess.Board, time_limit: Limit, *args: HOMEMADE_ARGS_TYPE) -> PlayResult:
            
            # If time is running out, return a random move
            if self.get_my_time(board, time_limit) < 0.1:
                print("---> Returning due to time/inc limit")
                return PlayResult(random.choice(list(board.legal_moves)), None)
                
            # If move is check mate
            for move in list(board.legal_moves):
                board_copy = board.copy()
                board_copy.push(move)
                board_copy.turn = not board_copy.turn  # Switch to the opponent's turn
                if board_copy.is_checkmate():
                    print("---> Returning due to putting enemy in checkmate")
                    return PlayResult(move, None)
                
            # If move puts enemy in check
            for move in list(board.legal_moves):
                board_copy = board.copy()
                board_copy.push(move)
                board_copy.turn = not board_copy.turn  # Switch to the opponent's turn
                if board_copy.is_check():
                    print("---> Returning due to putting enemy in check")
                    return PlayResult(move, None)
            
            legal_captures = list(board.generate_legal_captures())
            
            # If queen capture available
            for move in legal_captures:
                captured_piece = board.piece_at(move.to_square)
                if captured_piece and captured_piece.piece_type == chess.QUEEN:
                    print("---> Returning due to queen capture")
                    return PlayResult(move, None)
            
            # If rook capture available
            for move in legal_captures:
                captured_piece = board.piece_at(move.to_square)
                if captured_piece and captured_piece.piece_type == chess.ROOK:
                    print("---> Returning due to rook capture")
                    return PlayResult(move, None)
            
            # If bishop capture available
            for move in legal_captures:
                captured_piece = board.piece_at(move.to_square)
                if captured_piece and captured_piece.piece_type == chess.BISHOP:
                    print("---> Returning due to bishop capture")
                    return PlayResult(move, None)
            
            # If knight capture available
            for move in legal_captures:
                captured_piece = board.piece_at(move.to_square)
                if captured_piece and captured_piece.piece_type == chess.KNIGHT:
                    print("---> Returning due to knight capture")
                    return PlayResult(move, None)
            
            # If pawn capture available
            for move in legal_captures:
                captured_piece = board.piece_at(move.to_square)
                if captured_piece and captured_piece.piece_type == chess.PAWN:
                    print("---> Returning due to pawn capture")
                    return PlayResult(move, None)
            
            
            print("---> Returning random legal move")
            return PlayResult(random.choice(list(board.legal_moves)), None)
