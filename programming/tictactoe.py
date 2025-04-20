class TicTacToeBoard:
    def __init__(self):
        self.board = [" " for _ in range(9)]

    def __print_row(self, row_number):
        """Prints a single row of the board."""
        print("|".join(self.board[row_number * 3:(row_number + 1) * 3]))

    def __print_row_divider(self):
        """Prints a divider between rows."""
        print("-" * 5)

    def print(self):
        """Prints current state."""
        for row_number in range(3):
            self.__print_row(row_number)
            needs_divider = row_number < 2
            if needs_divider:
                self.__print_row_divider()

    def __empty_position(self, position):
        """Checks if a position is empty."""
        return self.board[position] == " "

    def mark_position(self, position, mark):
        """Marks a position on the board."""
        if self.__empty_position(position):
            self.board[position] = mark
            return True
        return False

class TicTacToe:
    def __init__(self, board: TicTacToeBoard):
        self.board = board
        self.current_player = "X"  # The current player, either "X" or "O"

    def print_board(self):
        """Prints the current state of the board."""
        print("Current board:")
        self.board.print()

    def make_move(self, position: int) -> bool:
        """Places a move on the board at the given position."""
        return self.board.mark_position(position, self.current_player)

    def check_winner(self):
        """Checks if there is a winner."""
        winning_combinations = [
            (0, 1, 2), (3, 4, 5), (6, 7, 8),  # Horizontal
            (0, 3, 6), (1, 4, 7), (2, 5, 8),  # Vertical
            (0, 4, 8), (2, 4, 6)              # Diagonal
        ]
        for combo in winning_combinations:
            if self.board[combo[0]] == self.board[combo[1]] == self.board[combo[2]] != " ":
                return self.board[combo[0]]
        return None

    def is_draw(self):
        """Checks if the game is a draw."""
        return all(space != " " for space in self.board)

    def switch_player(self):
        """Switches the current player."""
        self.current_player = "O" if self.current_player == "X" else "X"

    def play_game(self):
        """Main method to play the game."""
        while True:
            self.print_board()
            try:
                move = int(input(f"Player {self.current_player}, enter your move (0-8): "))
                if move < 0 or move > 8:
                    raise ValueError("Invalid move. Please enter a number between 0 and 8.")
                if not self.make_move(move):
                    raise ValueError("Invalid move. The position is already occupied.")
            except ValueError as
