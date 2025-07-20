# RefractorCode


the code came from a video that i found on youtube :https://www.youtube.com/watch?v=_6H49euHmJc

Original Problems Fixed
1. Removed Global Variables
Before: All variables were floating around globally

char board[3][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
int choice;
int row, column;
char turn = 'X';
bool draw = false;
After: Everything organized inside a TicTacToe class

class TicTacToe {
private:
    std::vector<std::vector<char>> board;
    char currentPlayer;
    bool gameEnded;
    bool isDraw;
    // etc...
};
2. Fixed Input Validation
Before: Game crashed on bad input

cin >> choice; // No checking if input is valid
After: Bulletproof input handling

while (!(std::cin >> choice) || choice < 1 || choice > 9) {
    std::cout << "Invalid input! Please enter a number between 1-9: ";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
3. Fixed Backwards Game Logic
Before: gameover() returned true when game should continue

bool gameover(){
    // Check win
    for(int i=0; i<3; i++)
        if(board[i][0]==board[i][1] && board[i][0]==board[i][2])
            return false; // This is backwards!
}
After: Clear, logical functions

bool checkWin() const {
    // Returns true if someone actually won
}
bool isBoardFull() const {
    // Returns true if board is actually full
}
4. Eliminated Dangerous Recursion
Before: Used recursion for error handling (could crash)

void player_turn(){
    // ... code ...
    if(board[row][column] != 'X' && board[row][column] != 'O'){
        // valid move
    } else {
        cout << "Box already filled! Please choose another.\n\n";
        player_turn(); // Dangerous recursion!
    }
}
After: Safe loop-based error handling

while (!gameEnded) {
    int choice = getPlayerInput();
    if (!makeMove(choice)) {
        continue; // Just try again, no recursion
    }
}
5. Replaced Magic Numbers
Before: Random numbers everywhere

for(int i=0; i<3; i++) // What does 3 mean?
    for(int j=0; j<3; j++) // Why 3 again?
After: Named constants

static const int BOARD_SIZE = 3;
for (int i = 0; i < BOARD_SIZE; ++i)
    for (int j = 0; j < BOARD_SIZE; ++j)
6. Improved Function Names
Before: Confusing names

bool gameover() // Returns true when game continues?
void player_turn() // Does way too many things
void display_board() // Only displays
After: Clear, descriptive names

bool checkWin() // Actually checks for wins
bool isBoardFull() // Checks if board is full
void makeMove() // Makes a move
void displayBoard() // Displays the board
void switchPlayer() // Switches players
7. Fixed Redundant Function Calls
Before: Called same function twice

while(gameover()){
    display_board();
    player_turn();
    gameover(); // Called again for no reason!
}
After: Each function called once when needed

while (!gameEnded) {
    displayBoard();
    // ... get move ...
    checkGameStatus(); // Called once after move
}
8. Better Code Organization
Before: Giant switch statement

switch(choice){
    case 1: row=0; column=0; break;
    case 2: row=0; column=1; break;
    case 3: row=0; column=2; break;
    // ... 9 cases total
}
After: Simple math

void getCoordinates(int choice, int& row, int& col) const {
    row = (choice - 1) / BOARD_SIZE;
    col = (choice - 1) % BOARD_SIZE;
}
9. Added Modern C++ Features
Before: Old C-style code

using namespace std; // Bad practice
char board[3][3]; // Raw arrays
After: Modern C++ practices

std::vector<std::vector<char>> board; // Safe containers
// No namespace pollution
10. Enhanced User Experience
Before: Basic, confusing output

cout << "Player - 1 [X] turn: ";
cout << "Invalid Move\n";
After: Clear, helpful messages

std::cout << "Player 1 [X], enter your move (1-9): ";
std::cout << "That position is already taken! Choose another.\n";
std::cout << "Would you like to play again? (y/n): ";
Major Additions
11. Added and option to play against a Ai or another human
