#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define SIZE 9
#define AMOVE 3

// Struct for board positions (row and column)
typedef struct {
    int row;
    int col;
} Position;

// Function declarations
void splashScreen();
void initialsBoard(char board[SIZE][SIZE]);
void drawBoard();
void clearMarkers();
void playerDisplay(int player);
Position parsePosition(const char input[AMOVE]);
Position getPosition(const char *selectedCell, int player, bool checkPlayer);
void markValidPawnMoves(Position from, char piece);
void markValidRookMoves(Position from, char piece);
void markValidKnightMoves(Position from, char piece);
void markValidBishopMoves(Position from, char piece);
void markValidQueenMoves(Position from, char piece);
void markValidKingMoves(Position from, char piece);
int move(int player);
int gameStatus(int player);
bool isKingInCheck(char player);
bool isLegalMove(Position from, Position to, char piece);
bool isCheckmate(char player);
bool isStalemate(char player);

/*SPLASH SCREEN*/
void splashScreen() {
    printf("\n");
    printf("\t\t\t  ██████╗██╗  ██╗███████╗███████╗███████╗\n");
    printf("\t\t\t ██╔════╝██║  ██║██╔════╝██╔════╝██╔════╝\n");
    printf("\t\t\t ██║     ███████║█████╗  ███████╗███████╗\n");
    printf("\t\t\t ██║     ██╔══██║██╔══╝  ╚════██║╚════██║\n");
    printf("\t\t\t ╚██████╗██║  ██║███████╗███████║███████║\n");
    printf("\t\t\t  ╚═════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n");
    printf("\n");
    printf("  ─────────────────────────────────────────────────────────────────────────────────────────────\n");
    printf("\t  Welcome to Terminal Chess | Built in C by a Student Testing His Skills 🧠\n");
    printf("\t  White = UPPERCASE   |   Black = lowercase\n");
    printf("\t  Make your move using chess notation (e.g., e2 e4)\n");
    printf("\t  Type carefully — this is battle of minds!\n");
    printf("  ─────────────────────────────────────────────────────────────────────────────────────────────\n");
    printf("\n");
    printf("The Game Will Start Shortly\n");
}

/*BOARD*/

// The chess board (9x9 for labels)
char board[SIZE][SIZE] = {
    {'8', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'7', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'6', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'5', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'4', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'3', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'2', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'1', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'W', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}
};

// For restoring board after marking moves
char markerBackup[8][9] = {{0}};
// For en passant logic
int enPassantRow = -1, enPassantCol = -1;

// Set up the initial board state
void initialsBoard(char board[SIZE][SIZE]) {
    // Place black pieces
    board[0][1] = 'r'; // rook
    board[0][2] = 'n'; // knight
    board[0][3] = 'b'; // bishop
    board[0][4] = 'q'; // queen
    board[0][5] = 'k'; // king
    board[0][6] = 'b'; // bishop
    board[0][7] = 'n'; // knight
    board[0][8] = 'r'; // rook

    // Place pawns
    for(int i = 1; i <= SIZE - 1; i++) {
        board[1][i] = 'p'; //Place black pawns
        board[6][i] = 'P'; // Place white pawns
    }

    // Place white pieces
    board[7][1] = 'R'; // rook
    board[7][2] = 'N'; // knight
    board[7][3] = 'B'; // bishop
    board[7][4] = 'Q'; // queen
    board[7][5] = 'K'; // king
    board[7][6] = 'B'; // bishop
    board[7][7] = 'N'; // knight
    board[7][8] = 'R'; // rook
}

// Print the board to the console
void drawBoard() {
    for(int i = 0; i < SIZE - 1; i++){
        if (i == 0) {
            printf("\t\t\t    +-------------------------------+\n"); //Top row
        }
        printf("\t\t\t %c || %c | %c | %c | %c | %c | %c | %c | %c |\n", board[i][0], board[i][1], board[i][2], board[i][3], board[i][4], board[i][5], board[i][6], board[i][7], board[i][8]);
        if(i < 7) {
            printf("\t\t\t---||———+———+———+———+———+———+———+———|\n"); //Piece seperator
        }
        if(i == 7) {
            printf("\t\t\t---||===-===-===-===-===-===-===-===+\n"); //End of the board
        }
    }
    printf("\t\t\t %c || %c | %c | %c | %c | %c | %c | %c | %c \n", board[8][0], board[8][1], board[8][2], board[8][3], board[8][4], board[8][5], board[8][6], board[8][7], board[8][8]); //Bottom row
    printf("\n");
}

// Display the current player
void playerDisplay(int player) {
    char playerChar = (player % 2 != 0) ? 'W' : 'B';
    if(playerChar == 'W') {
        board[8][0] = 'W';
    }
    else {
        board[8][0] = 'B';
    }
}

/*TRANSLATOR*/

// Convert user input to board position
Position parsePosition(const char input[AMOVE]) {
    Position pos;
    // Convert column letter to number
    switch(input[0]) {
        case 'A': case 'a': pos.col = 1; break;
        case 'B': case 'b': pos.col = 2; break;
        case 'C': case 'c': pos.col = 3; break;
        case 'D': case 'd': pos.col = 4; break;
        case 'E': case 'e': pos.col = 5; break;
        case 'F': case 'f': pos.col = 6; break;
        case 'G': case 'g': pos.col = 7; break;
        case 'H': case 'h': pos.col = 8; break;
        default: pos.col = -1;
    }
    // Convert row number to array index
    switch(input[1]) {
        case '8': pos.row = 0; break;
        case '7': pos.row = 1; break;
        case '6': pos.row = 2; break;
        case '5': pos.row = 3; break;
        case '4': pos.row = 4; break;
        case '3': pos.row = 5; break;
        case '2': pos.row = 6; break;
        case '1': pos.row = 7; break;
        default: pos.row = -1;
    }
    return pos;
}

// Get a position from the user, with validation
Position getPosition(const char *selectedCell, int player, bool checkPlayer) {
    char input[AMOVE];
    Position pos;
    while (1) {
        printf("%s", selectedCell);
        scanf("%2s", input);
        pos = parsePosition(input);
        if(pos.row >= 0 && pos.row <= 7 && pos.col >= 1 && pos.col <= 8) {
            if(checkPlayer) { // If selecting a piece to move
                if(board[pos.row][pos.col] != ' ') {
                    char piece = board[pos.row][pos.col];
                    char playerToMove = (player % 2 != 0) ? 'W' : 'B';
                    if((playerToMove == 'W' && isupper(piece)) || (playerToMove == 'B' && islower(piece))) {
                        return pos;
                    }
                }
                else {
                    printf("Invalid cell\n");
                }
            }
            else {
                return pos;
            }
        }
        printf("Invalid cell entered! Please try again.\n");
    }
}

/*PIECE MOVEMENT AND VALIDATION*/

// Clear all '*' and 'X' markers from the board
void clearMarkers() {
    for(int i = 0; i < 8; i++) {
        for(int j = 1; j <= 8; j++) {
            if(board[i][j] == '*' || board[i][j] == 'X') {
                if(markerBackup[i][j] != 0) {
                    board[i][j] = markerBackup[i][j];
                }
                else {
                    board[i][j] = ' ';
                }
                markerBackup[i][j] = 0;
            }
        }
    }
}

// Mark valid moves for a pawn, including en passant
void markValidPawnMoves(Position from, char piece) {
    clearMarkers();
    // White pawn
    if(isupper(piece)) {
        // Forward move
        if(from.row > 0 && board[from.row - 1][from.col] == ' ') {
            board[from.row - 1][from.col] = '*';
            // Two squares from starting position
            if(from.row == 6 && board[from.row - 2][from.col] == ' ') { 
                board[from.row - 2][from.col] = '*';
            }
        }
        // Capture left
        if(from.row > 0 && from.col > 1 && islower(board[from.row - 1][from.col - 1])) {
            markerBackup[from.row - 1][from.col - 1] = board[from.row - 1][from.col - 1];
            board[from.row - 1][from.col - 1] = 'X';
        }
        // Capture right
        if(from.row > 0 && from.col < 8 && islower(board[from.row - 1][from.col + 1])) {
            markerBackup[from.row - 1][from.col + 1] = board[from.row - 1][from.col + 1];
            board[from.row - 1][from.col + 1] = 'X';
        }
        // En passant
        if(from.row == 3) {
            // Left
            if(from.col > 1 && enPassantRow == 2 && enPassantCol == from.col - 1 && board[from.row][from.col - 1] == 'p') {
                board[from.row - 1][from.col - 1] = '*';
                markerBackup[from.row][from.col - 1] = board[from.row][from.col - 1];
                board[from.row][from.col - 1] = 'X';
            }
            // Right
            if(from.col < 8 && enPassantRow == 2 && enPassantCol == from.col + 1 && board[from.row][from.col + 1] == 'p') {
                board[from.row - 1][from.col + 1] = '*';
                markerBackup[from.row][from.col + 1] = board[from.row][from.col + 1];
                board[from.row][from.col + 1] = 'X';
            }
        }
    }
    // Black pawn
    else if(islower(piece)) {
        // Forward move
        if(from.row < 7 && board[from.row + 1][from.col] == ' ') {
            board[from.row + 1][from.col] = '*';
            // Two squares from starting position
            if(from.row == 1 && board[from.row + 2][from.col] == ' ') {
                board[from.row + 2][from.col] = '*';
            }
        }
        // Capture left
        if(from.row < 7 && from.col > 1 && isupper(board[from.row + 1][from.col - 1])) {
            markerBackup[from.row + 1][from.col - 1] = board[from.row + 1][from.col - 1];
            board[from.row + 1][from.col - 1] = 'X';
        }
        // Capture right
        if(from.row < 7 && from.col < 8 && isupper(board[from.row + 1][from.col + 1])) {
            markerBackup[from.row + 1][from.col + 1] = board[from.row + 1][from.col + 1];
            board[from.row + 1][from.col + 1] = 'X';
        }
        // En passant
        if(from.row == 4) {
            // Left
            if(from.col > 1 && enPassantRow == 5 && enPassantCol == from.col - 1 && board[from.row][from.col - 1] == 'P') {
                board[from.row + 1][from.col - 1] = '*';
                markerBackup[from.row][from.col - 1] = board[from.row][from.col - 1];
                board[from.row][from.col - 1] = 'X';
            }
            // Right
            if(from.col < 8 && enPassantRow == 5 && enPassantCol == from.col + 1 && board[from.row][from.col + 1] == 'P') {
                board[from.row + 1][from.col + 1] = '*';
                markerBackup[from.row][from.col + 1] = board[from.row][from.col + 1];
                board[from.row][from.col + 1] = 'X';
            }
        }
    }
    drawBoard();
}

// Mark valid moves for a rook
void markValidRookMoves(Position from, char piece) {
    clearMarkers();
    int directions[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    for(int d = 0; d < 4; d++) {
        int r = from.row, c = from.col;
        while(1) {
            r += directions[d][0];
            c += directions[d][1];
            if(r < 0 || r > 7 || c < 1 || c > 8) {
                break;
            }
            if(board[r][c] == ' ') {
                markerBackup[r][c] = board[r][c];
                board[r][c] = '*';
            }
            else {
                if((isupper(piece) && islower(board[r][c])) || (islower(piece) && isupper(board[r][c]))) {
                    markerBackup[r][c] = board[r][c];
                    board[r][c] = 'X';
                }
                break;
            }
        }
    }
    drawBoard();
}

// Mark valid moves for a knight
void markValidKnightMoves(Position from, char piece) {
    clearMarkers();
    int moves[8][2] = {{-2,-1}, {-2,1}, {-1,-2}, {-1,2}, {1,-2}, {1,2}, {2,-1}, {2,1}};
    for(int i = 0; i < 8; i++) {
        int r = from.row + moves[i][0];
        int c = from.col + moves[i][1];
        if(r >= 0 && r <= 7 && c >= 1 && c <= 8) {
            if(board[r][c] == ' ') {
                markerBackup[r][c] = board[r][c];
                board[r][c] = '*';
            }
            else if((isupper(piece) && islower(board[r][c])) || (islower(piece) && isupper(board[r][c]))) {
                markerBackup[r][c] = board[r][c];
                board[r][c] = 'X';
            }
        }
    }
    drawBoard();
}

// Mark valid moves for a bishop
void markValidBishopMoves(Position from, char piece) {
    clearMarkers();
    int directions[4][2] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
    for(int d = 0; d < 4; d++) {
        int r = from.row, c = from.col;
        while(1) {
            r += directions[d][0];
            c += directions[d][1];
            if(r < 0 || r > 7 || c < 1 || c > 8) {
                break;
            }
            if(board[r][c] == ' ') {
                markerBackup[r][c] = board[r][c];
                board[r][c] = '*';
            }
            else {
                if((isupper(piece) && islower(board[r][c])) || (islower(piece) && isupper(board[r][c]))) {
                    markerBackup[r][c] = board[r][c];
                    board[r][c] = 'X';
                }
                break;
            }
        }
    }
    drawBoard();
}

// Mark valid moves for a queen
void markValidQueenMoves(Position from, char piece) {
    clearMarkers();
    int directions[8][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};
    for(int d = 0; d < 8; d++) {
        int r = from.row, c = from.col;
        while(1) {
            r += directions[d][0];
            c += directions[d][1];
            if(r < 0 || r > 7 || c < 1 || c > 8) {
                break;
            }
            if(board[r][c] == ' ') {
                markerBackup[r][c] = board[r][c];
                board[r][c] = '*';
            }
            else {
                if((isupper(piece) && islower(board[r][c])) || (islower(piece) && isupper(board[r][c]))) {
                    markerBackup[r][c] = board[r][c];
                    board[r][c] = 'X';
                }
                break;
            }
        }
    }
    drawBoard();
}

// Mark valid moves for a king, including castling (Castling is not yet working)
void markValidKingMoves(Position from, char piece) {
    clearMarkers();
    int moves[8][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};
    for(int i = 0; i < 8; i++) {
        int r = from.row + moves[i][0];
        int c = from.col + moves[i][1];
        if(r >= 0 && r <= 7 && c >= 1 && c <= 8) {
            if(board[r][c] == ' ') {
                markerBackup[r][c] = board[r][c];
                board[r][c] = '*';
            }
            else if((isupper(piece) && islower(board[r][c])) || (islower(piece) && isupper(board[r][c]))) {
                markerBackup[r][c] = board[r][c];
                board[r][c] = 'X';
            }
        }
    }

    // Castling for White (row 7, king at col 5)
    if(piece == 'K' && from.row == 7 && from.col == 5) {
        // Kingside castling: rook at [7][8], squares [7][6] and [7][7] empty
        if(board[7][8] == 'R' && board[7][6] == ' ' && board[7][7] == ' ') {
            markerBackup[7][7] = board[7][7];
            board[7][7] = '*'; // King will move to [7][7]
        }
        // Queenside castling: rook at [7][1], squares [7][2], [7][3], [7][4] empty
        if(board[7][1] == 'R' && board[7][2] == ' ' && board[7][3] == ' ' && board[7][4] == ' ') {
            markerBackup[7][3] = board[7][3];
            board[7][3] = '*'; // King will move to [7][3]
        }
    }

    // Castling for Black (row 0, king at col 5)
    if(piece == 'k' && from.row == 0 && from.col == 5) {
        // Kingside castling: rook at [0][8], squares [0][6] and [0][7] empty
        if(board[0][8] == 'r' && board[0][6] == ' ' && board[0][7] == ' ') {
            markerBackup[0][7] = board[0][7];
            board[0][7] = '*'; // King will move to [0][7]
        }
        // Queenside castling: rook at [0][1], squares [0][2], [0][3], [0][4] empty
        if(board[0][1] == 'r' && board[0][2] == ' ' && board[0][3] == ' ' && board[0][4] == ' ') {
            markerBackup[0][3] = board[0][3];
            board[0][3] = '*'; // King will move to [0][3]
        }
    }
    drawBoard();
}

// Main move function: handles user input, move validation, and special moves
int move(int player) {
    while (1) {
        Position from = getPosition("Select piece that you want to move: ", player, true);
        char piece = board[from.row][from.col];

        // Mark valid moves for the selected piece
        switch(piece) {
            case 'P': case 'p': markValidPawnMoves(from, piece); break;
            case 'R': case 'r': markValidRookMoves(from, piece); break;
            case 'N': case 'n': markValidKnightMoves(from, piece); break;
            case 'B': case 'b': markValidBishopMoves(from, piece); break;
            case 'Q': case 'q': markValidQueenMoves(from, piece); break;
            case 'K': case 'k': markValidKingMoves(from, piece); break;
            default: clearMarkers(); break;
        }

        Position to = getPosition("Select where you want to move it: ", player, false);

        // Only allow move to a marked cell
        if((board[to.row][to.col] == '*' || board[to.row][to.col] == 'X') && isLegalMove(from, to, piece)) {
            // En passant capture (must be before moving the pawn)
            if(piece == 'P' && from.row == 3 && to.row == 2 && abs(to.col - from.col) == 1 && enPassantRow == 2 && enPassantCol == to.col && board[from.row][to.col] == 'p') {
                board[from.row][to.col] = ' ';
            }
            if(piece == 'p' && from.row == 4 && to.row == 5 && abs(to.col - from.col) == 1 && enPassantRow == 5 && enPassantCol == to.col && board[from.row][to.col] == 'P') {
                board[from.row][to.col] = ' ';
            }

            // Move the piece
            board[to.row][to.col] = board[from.row][from.col];
            board[from.row][from.col] = ' ';
            clearMarkers();

            // Set en passant target
            if(piece == 'P' && from.row == 6 && to.row == 4) {
                enPassantRow = 5;
                enPassantCol = from.col;
            }
            else if(piece == 'p' && from.row == 1 && to.row == 3) {
                enPassantRow = 2;
                enPassantCol = from.col;
            }
            else {
                enPassantRow = -1;
                enPassantCol = -1;
            }

            // Pawn promotion
            if((piece == 'P' && to.row == 0) || (piece == 'p' && to.row == 7)) {
                char promote;
                printf("Pawn promotion! Choose (Q)ueen, (R)ook, (B)ishop, or k(N)ight: ");
                while (1) {
                    scanf(" %c", &promote);
                    if(piece == 'P') {
                        promote = toupper(promote);
                    }
                    else {
                        promote = tolower(promote);
                    }
                    if(promote == 'Q' || promote == 'R' || promote == 'B' || promote == 'N' || promote == 'q' || promote == 'r' || promote == 'b' || promote == 'n') {
                        board[to.row][to.col] = promote;
                        break;
                    }
                    else {
                        printf("Invalid choice. Please enter Q, R, B, or N: ");
                    }
                }
            }

            // After moving the king, handle castling rook move (Not yet working)
            if(piece == 'K' && from.row == 7 && from.col == 5) {
                // Kingside castling
                if(to.row == 7 && to.col == 7 && board[7][8] == 'R') {
                    board[7][6] = 'R'; // Move rook to f1
                    board[7][8] = ' ';
                }
                // Queenside castling
                if(to.row == 7 && to.col == 3 && board[7][1] == 'R') {
                    board[7][4] = 'R'; // Move rook to d1
                    board[7][1] = ' ';
                }
            }
            if(piece == 'k' && from.row == 0 && from.col == 5) {
                // Kingside castling
                if(to.row == 0 && to.col == 7 && board[0][8] == 'r') {
                    board[0][6] = 'r'; // Move rook to f8
                    board[0][8] = ' ';
                }
                // Queenside castling
                if(to.row == 0 && to.col == 3 && board[0][1] == 'r') {
                    board[0][4] = 'r'; // Move rook to d8
                    board[0][1] = ' ';
                }
            }
            break;
        }
        else {
            printf("Invalid move! Please select a highlighted cell.\n");
        }
    }
    return 0;
}

// Check if the player's king is in check
bool isKingInCheck(char player) {
    char king = (player == 'W') ? 'K' : 'k';
    int kingRow = -1, kingCol = -1;
    // Find king position
    for(int i = 0; i < 8; i++) {
        for(int j = 1; j <= 8; j++) {
            if(board[i][j] == king) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
    }
    if(kingRow == -1 || kingCol == -1) {
        return false; // King not found
    }

    // Check for pawn attacks
    if(player == 'W') {
        if(kingRow > 0) {
            if(kingCol > 1 && board[kingRow-1][kingCol-1] == 'p') {
                return true;
            }
            if(kingCol < 8 && board[kingRow-1][kingCol+1] == 'p') {
                return true;
            }
        }
    }
    else {
        if(kingRow < 7) {
            if(kingCol > 1 && board[kingRow+1][kingCol-1] == 'P') {
                return true;
            }
            if(kingCol < 8 && board[kingRow+1][kingCol+1] == 'P') {
                return true;
            }
        }
    }

    // Check for knight attacks
    int knightMoves[8][2] = {{-2,-1}, {-2,1}, {-1,-2}, {-1,2}, {1,-2}, {1,2}, {2,-1}, {2,1}};
    char oppKnight = (player == 'W') ? 'n' : 'N';
    for(int k = 0; k < 8; k++) {
        int r = kingRow + knightMoves[k][0];
        int c = kingCol + knightMoves[k][1];
        if(r >= 0 && r <= 7 && c >= 1 && c <= 8 && board[r][c] == oppKnight) {
            return true;
        }
    }

    // Check for king attacks
    int kingMoves[8][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};
    char oppKing = (player == 'W') ? 'k' : 'K';
    for(int k = 0; k < 8; k++) {
        int r = kingRow + kingMoves[k][0];
        int c = kingCol + kingMoves[k][1];
        if(r >= 0 && r <= 7 && c >= 1 && c <= 8 && board[r][c] == oppKing) {
            return true;
        }
    }

    // Check for rook/queen attacks (straight lines)
    char oppRook = (player == 'W') ? 'r' : 'R';
    char oppQueen = (player == 'W') ? 'q' : 'Q';
    int directions[4][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    for(int d = 0; d < 4; d++) {
        int r = kingRow, c = kingCol;
        while(1) {
            r += directions[d][0];
            c += directions[d][1];
            if(r < 0 || r > 7 || c < 1 || c > 8) {
                break;
            }
            if(board[r][c] == oppRook || board[r][c] == oppQueen) {
                return true;
            }
            if(board[r][c] != ' ') {
                break;
            }
        }
    }

    // Check for bishop/queen attacks (diagonals)
    char oppBishop = (player == 'W') ? 'b' : 'B';
    int diag[4][2] = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
    for(int d = 0; d < 4; d++) {
        int r = kingRow, c = kingCol;
        while(1) {
            r += diag[d][0];
            c += diag[d][1];
            if(r < 0 || r > 7 || c < 1 || c > 8) {
                break;
            }
            if(board[r][c] == oppBishop || board[r][c] == oppQueen) {
                return true;
            }
            if(board[r][c] != ' ') {
                break;
            }
        }
    }

    return false;
}

// Check if a move is legal (does not leave king in check) (Not yet working)
bool isLegalMove(Position from, Position to, char piece) {
    char tempFrom = board[from.row][from.col];
    char tempTo = board[to.row][to.col];

    board[to.row][to.col] = piece;
    board[from.row][from.col] = ' ';

    char player = isupper(piece) ? 'W' : 'B';
    bool inCheck = isKingInCheck(player);

    // Undo move
    board[from.row][from.col] = tempFrom;
    board[to.row][to.col] = tempTo;

    return !inCheck;
}

// Check if the player's king is in checkmate (no legal moves to escape check) (Not yet fully working)
bool isCheckmate(char player) {
    // Find king position
    char king = (player == 'W') ? 'K' : 'k';
    int kingRow = -1, kingCol = -1;
    for(int i = 0; i < 8; i++) {
        for(int j = 1; j <= 8; j++) {
            if(board[i][j] == king) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
    }
    if(kingRow == -1 || kingCol == -1) {
        return false; // King not found
    }

    // If not in check, not checkmate
    if(!isKingInCheck(player)) {
        return false;
    }

    // Try all king moves to see if any escape check
    int kingMoves[8][2] = {{1,0}, {-1,0}, {0,1}, {0,-1}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};
    for(int k = 0; k < 8; k++) {
        int r = kingRow + kingMoves[k][0];
        int c = kingCol + kingMoves[k][1];
        if(r >= 0 && r <= 7 && c >= 1 && c <= 8) {
            char dest = board[r][c];
            // Only move to empty or opponent's piece
            if(dest == ' ' || (isupper(king) && islower(dest)) || (islower(king) && isupper(dest))) {
                // Try move
                char temp = board[r][c];
                board[kingRow][kingCol] = ' ';
                board[r][c] = king;
                bool stillInCheck = isKingInCheck(player);
                // Undo move
                board[r][c] = temp;
                board[kingRow][kingCol] = king;
                if(!stillInCheck) {
                    return false;
                }
            }
        }
    }
    return true;
}

// Check if the player is stalemated
bool isStalemate(char player) {
    if(isKingInCheck(player)) {
        return false; // Not stalemate if in check
    }

    // Trying all moves for all pieces of this player
    for(int i = 0; i < 8; i++) {
        for(int j = 1; j <= 8; j++) {
            char piece = board[i][j];
            if((player == 'W' && isupper(piece)) || (player == 'B' && islower(piece))) {
                Position from = {i, j};
                // Trying all possible destinations
                for(int r = 0; r < 8; r++) {
                    for(int c = 1; c <= 8; c++) {
                        Position to = {r, c};
                        if((board[r][c] == ' ' || (player == 'W' && islower(board[r][c])) || (player == 'B' && isupper(board[r][c]))) && isLegalMove(from, to, piece)) {
                            return false; // Has a legal move
                        }
                    }
                }
            }
        }
    }
    return true; // No legal moves and not in check
}

// Check the game status after each move
int gameStatus(int player) {
    // After a move, it's the opponent's king that could be in check
    char nextPlayerChar = (player % 2 == 0) ? 'W' : 'B';
    if(isCheckmate(nextPlayerChar)) {
        printf("Checkmate! Game over.\n");
        return 1; // End the game
    }
    if(isStalemate(nextPlayerChar)) {
        printf("Stalemate! Game drawn.\n");
        return 1;
    }
    if(isKingInCheck(nextPlayerChar)) {
        printf("Check!\n");
        Sleep(2000);
    }
    return 0; // Never stop unless checkmate
}

/*MAIN*/

// Main game loop
int main() {
    splashScreen();
    Sleep(3000);
    printf("\t\t\t\t\tBEGIN!...\n\n");
    int player = 1;
    initialsBoard(board);
    drawBoard();

    while (1) {
        if(player != 1) {
            playerDisplay(player);
            drawBoard();
            printf("\n");
        }
        playerDisplay(player);
        move(player);
        printf("\n");
        drawBoard();

        // Only stop if checkmate or stalemate
        if(gameStatus(player) == 1) {
            break;
        }
        player++;
    }
    return 0;
}