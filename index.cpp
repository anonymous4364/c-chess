#include <iostream>
#include <cctype>
#include <vector>
#include <cmath>  

// global variables
char grid[8][8]; // chess board 2d array
std::vector<char> whiteCaptured; // vector to store white captured pieces 
std::vector<char> blackCaptured;  // vector to store black captured pieces 

void setBoard() {
	// black pieces
	grid[0][0] = grid[0][7] = 'r'; grid[0][1] = grid[0][6] = 'n'; grid[0][2] = grid[0][5] = 'b';
	grid[0][3] = 'q'; grid[0][4] = 'k'; for (int i = 0; i < 8; i++) grid[1][i] = 'p';

	// empty spots
	for (int i = 2; i < 6; i++) {
		for (int j = 0; j < 8; j++) {
			grid[i][j] = '.';
		}
	}

	//white pieces
	grid[7][0] = grid[7][7] = 'R'; grid[7][1] = grid[7][6] = 'N'; grid[7][2] = grid[7][5] = 'B';
	grid[7][3] = 'Q'; grid[7][4] = 'K'; for (int i = 0; i < 8; i++) grid[6][i] = 'P';
}

void printBoard() {
	std::cout << "    a   b   c   d   e   f   g   h\n\n";
	for (int i = 0; i < 8; i++) {
		std::cout << 8 - i << "   ";
		for (int j = 0; j < 8; j++) {
			std::cout << grid[i][j] << "   ";
		}
		std::cout << 8 - i << "\n\n";
	}
	std::cout << "    a   b   c   d   e   f   g   h\n\n";
}

void notationToIndex(std::string pos, int &row, int &col) {
	col = pos[0] - 'a';       // 'a'->0, 'b'->1, ..., 'h'->7
	row = 8 - (pos[1] - '0'); // '1'->7, '2'->6, ..., '8'->0
}

void move(int fromRow, int fromCol, int toRow, int toCol) {
	grid[toRow][toCol] = grid[fromRow][fromCol]; // whatever piece is in grid[fromRow][fromCol]; move it to grid[toRow][toCol]
	grid[fromRow][fromCol] = '.'; // replace the piece with '.'
}

bool isValidMove(int fromRow, int fromCol, int toRow, int toCol) {
	// Allow valid positions only
	if (fromRow < 0 || fromRow > 7 || fromCol < 0 || fromCol > 7 ||
		toRow < 0 || toRow > 7 || toCol < 0 || toCol > 7) {
		std::cout << "Invalid Move\n"; 	return false;
	}
	// Allow pieces to move only 
	if (grid[fromRow][fromCol] == '.') {
		std::cout << "No piece is there!\n"; return false;
	}

	char pieceFrom = grid[fromRow][fromCol];
	char pieceTo = grid[toRow][toCol];

	// Don't capture your own piece
	if ((std::isupper(pieceFrom) && std::isupper(pieceTo)) ||
		(std::islower(pieceFrom) && std::islower(pieceTo))) {
		std::cout << "Can't capture your own piece!\n";
		return false;
	}

	return true;
}

bool capture(int fromRow, int fromCol, int toRow, int toCol) {
	char pieceFrom = grid[fromRow][fromCol];
	char pieceTo = grid[toRow][toCol];

	if (pieceTo == '.') return false; // no capture

	if (std::isupper(pieceFrom) && std::islower(pieceTo)) { // if white captures
		whiteCaptured.push_back(pieceTo); // push to array
		grid[toRow][toCol] = '.'; // remove the captured piece
		return true;
	}
	else if (std::islower(pieceFrom) && std::isupper(pieceTo)) { // if black captures
		blackCaptured.push_back(pieceTo);  // push to array
		grid[toRow][toCol] = '.'; // remove the captured piece
		return true;
	}
	return false;
}

std::string pieceName(char p) {
	switch (std::tolower(p)) {
		case 'p': return "Pawn";
		case 'r': return "Rook";
		case 'n': return "Knight";
		case 'b': return "Bishop";
		case 'q': return "Queen";
		case 'k': return "King";
		default: return "Unknown";
	}
}

void printCaptured() {
	std::cout << "White captured: ";
	for (char p : whiteCaptured) std::cout << pieceName(p) << ' ';
	std::cout << "\nBlack captured: ";
	for (char p : blackCaptured) std::cout << pieceName(p) << ' ';
	std::cout << "\n\n";
}

// move checks
bool isValidPawnMove(int fromRow, int fromCol, int toRow, int toCol, bool white) {
	int direction = white ? -1 : 1; // direction on how the pawn should move. white upwards black downwards

	// 1. Moving forward one square
	if (toCol == fromCol && toRow == fromRow + direction && grid[toRow][toCol] == '.') {
		return true;
	}

	// 2. Moving forward two squares (only from starting row)
	if (toCol == fromCol && toRow == fromRow + 2 * direction && grid[toRow][toCol] == '.') {
		int startRow = white ? 6 : 1;
		if (fromRow == startRow && grid[fromRow + direction][toCol] == '.') {
			return true;
		}
	}

	// 3. Capturing diagonally
	if ((toCol == fromCol + 1 || toCol == fromCol - 1) && (toRow == fromRow + direction)) {
		if (grid[toRow][toCol] != '.' && std::isupper(grid[toRow][toCol]) != white) {
			return true;
		}
	}

	return false;
}

bool isValidKingMove(int fromRow, int fromCol, int toRow, int toCol, bool white) {
	int rowDiff = std::abs(toRow - fromRow);
	int colDiff = std::abs(toCol - fromCol);

	// King moves only one square in any direction
	if (rowDiff <= 1 && colDiff <= 1) {
		// Can't capture own piece:
		if (grid[toRow][toCol] == '.' || std::isupper(grid[toRow][toCol]) != white) {
			return true; 
		}
	}
	return false;
}

bool isValidKnightMove(int fromRow, int fromCol, int toRow, int toCol, bool white) {
	int rowDiff = std::abs(toRow - fromRow);
	int colDiff = std::abs(toCol - fromCol);

	// knight moves in L shape: 2 by 1 or 1 by 2
	if ((rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2)) {
		// Can't capture own piece:
		if (grid[toRow][toCol] == '.' || std::isupper(grid[toRow][toCol]) != white) {
			return true;
		}
	}
	return false;
}

bool isPathClear(int fromRow, int fromCol, int toRow, int toCol) {
	int rowStep = 0, colStep = 0;

	// direction of movememnt 1 is top -1 is buttom 
	if (toRow > fromRow) rowStep = 1;
	else if (toRow < fromRow) rowStep = -1;

	// direction of movememnt 1 is right -1 is left
	if (toCol > fromCol) colStep = 1;
	else if (toCol < fromCol) colStep = -1;

	int currentRow = fromRow + rowStep; 
	int currentCol = fromCol + colStep;

	while (currentRow != toRow || currentCol != toCol) { // while piece place != choosen place keep checking if theres another pice based on direction
		if (grid[currentRow][currentCol] != '.') {
			return false;
		}
		currentRow += rowStep;
		currentCol += colStep;
	}

	return true;
}

bool isValidRookMove(int fromRow, int fromCol, int toRow, int toCol, bool white) {
	// Rook moves in straight line: either same row or same column
	if (fromRow != toRow && fromCol != toCol) return false;

	// Check if path is clear
	if (!isPathClear(fromRow, fromCol, toRow, toCol)) return false;

	// Can't capture own piece
	if (grid[toRow][toCol] != '.' && std::isupper(grid[toRow][toCol]) == white) return false;

	return true;
}


bool isValidBishopMove(int fromRow, int fromCol, int toRow, int toCol, bool white) {
	// Bishop moves in diagonal line formula is absolute difference between the rows equals the absolute difference between the columns:: 
	if (abs(toRow - fromRow) != abs(toCol - fromCol)) return false;

	// Check if path is clear
	if (!isPathClear(fromRow, fromCol, toRow, toCol)) return false;

	// Can't capture own piece
	if (grid[toRow][toCol] != '.' && std::isupper(grid[toRow][toCol]) == white) return false;

	return true;
}

bool isValidQueenMove(int fromRow, int fromCol, int toRow, int toCol, bool white) {
	// Queen is basically rook + bishop
	return isValidBishopMove(fromRow, fromCol, toRow, toCol, white) ||
		isValidRookMove(fromRow, fromCol, toRow, toCol, white);
}

bool isValidPieceMove(int fromRow, int fromCol, int toRow, int toCol, bool isWhitePiece, char piece) {
	bool validMove = false;

	switch (std::toupper(piece)) {
	case 'P':
		validMove = isValidPawnMove(fromRow, fromCol, toRow, toCol, isWhitePiece); break;
	case 'K':
		validMove = isValidKingMove(fromRow, fromCol, toRow, toCol, isWhitePiece); break;
	case 'N':
		validMove = isValidKnightMove(fromRow, fromCol, toRow, toCol, isWhitePiece); break;
	case 'R':
		validMove = isValidRookMove(fromRow, fromCol, toRow, toCol, isWhitePiece); break;
	case 'B':
		validMove = isValidBishopMove(fromRow, fromCol, toRow, toCol, isWhitePiece); break;
	case 'Q':
		validMove = isValidQueenMove(fromRow, fromCol, toRow, toCol, isWhitePiece); break;
	}
	if (!validMove) {
		std::cout << "Invalid move for " << pieceName(piece) << "!\n";
	}

	return validMove;
}

int main() {
	setBoard();
	printBoard();

	std::string from, to;
	bool running = true;
	bool white = true;  

	while (running) {
		std::cout << "Enter move (e.g. e2 e4 or 'q q' to quit): ";
		std::cin >> from >> to; std::cout << "\n";

		if (from == "q" && to == "q") {
			running = false;
			break;
		}

		if (from.length() != 2 || to.length() != 2) {
			std::cout << "Invalid input format. Try again.\n";
			continue;
		}

		int fromRow, fromCol, toRow, toCol;

		notationToIndex(from, fromRow, fromCol);
		notationToIndex(to, toRow, toCol);

		if (!isValidMove(fromRow, fromCol, toRow, toCol)) {
			continue;
		}

		char piece = grid[fromRow][fromCol];
		bool isWhitePiece = std::isupper(piece);

		if ((white && !isWhitePiece) || (!white && isWhitePiece)) {
			std::cout << (white ? "It's White's turn. " : "It's Black's turn. ") << "Please move your own piece.\n";
			continue;
		}

		if (!isValidPieceMove(fromRow, fromCol, toRow, toCol, isWhitePiece, piece)) {
			continue;
		}

		if (capture(fromRow, fromCol, toRow, toCol)) {
			std::cout << "Piece captured!\n\n";
		}

		move(fromRow, fromCol, toRow, toCol);
		printBoard();
		printCaptured();

		white = !white;  
	}

	std::cout << "Thanks for playing!\n";
	return 0;
}
