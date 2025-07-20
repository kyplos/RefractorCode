#include <iostream>
#include <vector>
#include <limits>
#include <random>
#include <algorithm>

class TicTacToe {
private:
    static const int BOARD_SIZE = 3;
    static const char PLAYER_X = 'X';
    static const char PLAYER_O = 'O';

    std::vector<std::vector<char>> board;
    char currentPlayer;
    char humanPlayer;
    char aiPlayer;
    bool gameEnded;
    bool isDraw;
    bool vsAI;
    std::mt19937 rng;

public:
    // Constructor
    TicTacToe() : board(BOARD_SIZE, std::vector<char>(BOARD_SIZE)),
                  currentPlayer(PLAYER_X), gameEnded(false), isDraw(false),
                  vsAI(false), rng(std::random_device{}()) {
        initializeBoard();
    }

    // Initialize board with numbers 1-9
    void initializeBoard() {
        char cellNumber = '1';
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                board[i][j] = cellNumber++;
            }
        }
    }

    // Choose game mode
    void chooseGameMode() {
        int choice;
        std::cout << "\n=== GAME MODE ===\n";
        std::cout << "1. Human vs Human\n";
        std::cout << "2. Human vs AI\n";
        std::cout << "Choose (1 or 2): ";

        while (!(std::cin >> choice) || (choice != 1 && choice != 2)) {
            std::cout << "Invalid choice! Please enter 1 or 2: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (choice == 2) {
            vsAI = true;
            choosePlayerSymbol();
        } else {
            vsAI = false;
        }
    }

    // Let human choose X or O when playing vs AI
    void choosePlayerSymbol() {
        char choice;
        std::cout << "\nChoose your symbol:\n";
        std::cout << "X - You go first\n";
        std::cout << "O - AI goes first\n";
        std::cout << "Enter X or O: ";

        while (!(std::cin >> choice) || (choice != 'X' && choice != 'x' && choice != 'O' && choice != 'o')) {
            std::cout << "Invalid choice! Please enter X or O: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        humanPlayer = (choice == 'X' || choice == 'x') ? PLAYER_X : PLAYER_O;
        aiPlayer = (humanPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
        currentPlayer = PLAYER_X; // X always goes first
    }

    // Display the current board state
    void displayBoard() const {
        std::cout << "\n=== TIC TAC TOE ===\n";
        if (vsAI) {
            std::cout << "You: [" << humanPlayer << "]  AI: [" << aiPlayer << "]\n\n";
        } else {
            std::cout << "Player 1: [X]  Player 2: [O]\n\n";
        }

        for (int i = 0; i < BOARD_SIZE; ++i) {
            std::cout << "     |     |     \n";
            std::cout << "  " << board[i][0] << "  |  "
                      << board[i][1] << "  |  " << board[i][2] << " \n";

            if (i < BOARD_SIZE - 1) {
                std::cout << "_____|_____|_____\n";
            }
        }
        std::cout << "     |     |     \n\n";
    }

    // Get valid input from human player
    int getPlayerInput() const {
        int choice;
        if (vsAI) {
            std::cout << "Your turn [" << humanPlayer << "], enter your move (1-9): ";
        } else {
            std::cout << "Player " << (currentPlayer == PLAYER_X ? "1 [X]" : "2 [O]")
                      << ", enter your move (1-9): ";
        }

        while (!(std::cin >> choice) || choice < 1 || choice > 9) {
            std::cout << "Invalid input! Please enter a number between 1-9: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        return choice;
    }

    // Convert choice (1-9) to board coordinates
    void getCoordinates(int choice, int& row, int& col) const {
        row = (choice - 1) / BOARD_SIZE;
        col = (choice - 1) % BOARD_SIZE;
    }

    // Check if a move is valid
    bool isValidMove(int row, int col) const {
        return board[row][col] != PLAYER_X && board[row][col] != PLAYER_O;
    }

    // Get all available moves
    std::vector<int> getAvailableMoves() const {
        std::vector<int> moves;
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (isValidMove(i, j)) {
                    moves.push_back(i * BOARD_SIZE + j + 1);
                }
            }
        }
        return moves;
    }

    // AI makes a move using minimax algorithm
    int getBestMove() {
        int bestScore = -1000;
        int bestMove = -1;

        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (isValidMove(i, j)) {
                    // Try this move
                    board[i][j] = aiPlayer;
                    int score = minimax(false, 0);
                    board[i][j] = char('1' + i * BOARD_SIZE + j); // Undo move

                    if (score > bestScore) {
                        bestScore = score;
                        bestMove = i * BOARD_SIZE + j + 1;
                    }
                }
            }
        }

        return bestMove;
    }

    // Minimax algorithm for AI decision making
    int minimax(bool isMaximizing, int depth) {
        if (checkWin()) {
            if (isMaximizing) {
                return -10 + depth; // Human won (bad for AI)
            } else {
                return 10 - depth;  // AI won (good for AI)
            }
        }

        if (isBoardFull()) {
            return 0; // Draw
        }

        if (isMaximizing) {
            int bestScore = -1000;
            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    if (isValidMove(i, j)) {
                        board[i][j] = aiPlayer;
                        int score = minimax(false, depth + 1);
                        board[i][j] = char('1' + i * BOARD_SIZE + j);
                        bestScore = std::max(score, bestScore);
                    }
                }
            }
            return bestScore;
        } else {
            int bestScore = 1000;
            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    if (isValidMove(i, j)) {
                        board[i][j] = humanPlayer;
                        int score = minimax(true, depth + 1);
                        board[i][j] = char('1' + i * BOARD_SIZE + j);
                        bestScore = std::min(score, bestScore);
                    }
                }
            }
            return bestScore;
        }
    }

    // Make a move on the board
    bool makeMove(int choice) {
        int row, col;
        getCoordinates(choice, row, col);

        if (!isValidMove(row, col)) {
            std::cout << "That position is already taken! Choose another.\n\n";
            return false;
        }

        board[row][col] = currentPlayer;
        return true;
    }

    // Switch to the other player
    void switchPlayer() {
        currentPlayer = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;
    }

    // Check for winning condition
    bool checkWin() const {
        // Check rows
        for (int i = 0; i < BOARD_SIZE; ++i) {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2] &&
                (board[i][0] == PLAYER_X || board[i][0] == PLAYER_O)) {
                return true;
            }
        }

        // Check columns
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[0][j] == board[1][j] && board[1][j] == board[2][j] &&
                (board[0][j] == PLAYER_X || board[0][j] == PLAYER_O)) {
                return true;
            }
        }

        // Check diagonals
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2] &&
            (board[0][0] == PLAYER_X || board[0][0] == PLAYER_O)) {
            return true;
        }

        if (board[0][2] == board[1][1] && board[1][1] == board[2][0] &&
            (board[0][2] == PLAYER_X || board[0][2] == PLAYER_O)) {
            return true;
        }

        return false;
    }

    // Check if board is full
    bool isBoardFull() const {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                if (board[i][j] != PLAYER_X && board[i][j] != PLAYER_O) {
                    return false;
                }
            }
        }
        return true;
    }

    // Check if game is over
    void checkGameStatus() {
        if (checkWin()) {
            gameEnded = true;
            isDraw = false;
        } else if (isBoardFull()) {
            gameEnded = true;
            isDraw = true;
        }
    }

    // Display game result
    void displayResult() const {
        displayBoard();

        if (isDraw) {
            std::cout << "It's a draw! Good game!\n";
        } else {
            char winner = (currentPlayer == PLAYER_X) ? PLAYER_O : PLAYER_X;

            if (vsAI) {
                if (winner == humanPlayer) {
                    std::cout << "Congratulations! You beat the AI! [" << winner << "]\n";
                } else {
                    std::cout << "AI wins this time! [" << winner << "] Better luck next time!\n";
                }
            } else {
                int playerNumber = (winner == PLAYER_X) ? 1 : 2;
                std::cout << "Player " << playerNumber << " [" << winner
                          << "] wins! Congratulations!\n";
            }
        }
    }

    // Main game loop
    void playGame() {
        std::cout << "Welcome to Tic Tac Toe!\n";
        chooseGameMode();
        std::cout << "Enter numbers 1-9 to make your move.\n";

        while (!gameEnded) {
            displayBoard();

            if (vsAI && currentPlayer == aiPlayer) {
                std::cout << "AI is thinking...\n";
                int aiMove = getBestMove();
                std::cout << "AI chooses position " << aiMove << "\n";
                makeMove(aiMove);
            } else {
                int choice = getPlayerInput();
                if (!makeMove(choice)) {
                    continue; // Invalid move, try again
                }
            }

            checkGameStatus();
            if (!gameEnded) {
                switchPlayer();
            }
        }

        displayResult();
    }

    // Ask if players want to play again
    bool playAgain() const {
        char response;
        std::cout << "\nWould you like to play again? (y/n): ";
        std::cin >> response;
        return (response == 'y' || response == 'Y');
    }

    // Reset game for new round
    void resetGame() {
        gameEnded = false;
        isDraw = false;
        currentPlayer = PLAYER_X;
        vsAI = false;
        initializeBoard();
    }
};

int main() {
    TicTacToe game;

    do {
        game.playGame();

        if (game.playAgain()) {
            game.resetGame();
        } else {
            break;
        }
    } while (true);

    std::cout << "Thanks for playing! Goodbye!\n";
    return 0;
}
