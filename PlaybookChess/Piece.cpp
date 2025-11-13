// Piece.cpp
// Handles piece class

#include "Piece.hpp"
#include <cmath>
#include <algorithm>

// Piece constructor
Piece::Piece(int f, int r, bool white, PieceType t, sf::Texture& tex) : file(f), rank(r), isWhite(white), type(t), texture(&tex) {}

// Draw piece
void Piece::draw(sf::RenderWindow& window) {
	sf::Sprite sprite(*texture);
	float scale = squareSize / texture->getSize().x;
	sprite.setScale({ scale, scale });
	sprite.setOrigin({ texture->getSize().x / 2.f, texture->getSize().y / 2.f });
	sprite.setPosition({
		(file - 1) * squareSize + 64.f + squareSize / 2.f,
		(rank - 1) * squareSize + 64.f + squareSize / 2.f
		});
	window.draw(sprite);
}

// Getter functions
int Piece::getFile() const { return file; }
int Piece::getRank() const { return rank; }
bool Piece::isWhitePiece() const { return isWhite; }
PieceType Piece::getType() const { return type; }

void Piece::setPosition(int f, int r) {
	file = f;
	rank = r;
}

bool Piece::isSquareOccupied(int file, int rank, const std::vector<std::unique_ptr<Piece>>& pieces) {
	for (const auto& p : pieces) {
		if (p->getFile() == file && p->getRank() == rank)
			return true;
	}
	return false;
}

// Helper sign function
static int sign(int x) { return (x > 0) - (x < 0); }

bool Piece::isPathClear(int fileStart, int rankStart, int fileEnd, int rankEnd, const std::vector<std::unique_ptr<Piece>>& pieces) {
	int df = fileEnd - fileStart;
	int dr = rankEnd - rankStart;

	int stepF = sign(df);
	int stepR = sign(dr);

	int steps = std::max(std::abs(df), std::abs(dr)); // Sets steps to file or rank if horizontal
	if (steps <= 1) return true;

	for (int i = 1; i < steps; i++) { // iterates step by step checking for pieces
		int f = fileStart + stepF * i;
		int r = rankStart + stepR * i;
		if (isSquareOccupied(f, r, pieces))
			return false;
	}
	return true;
}

bool Piece::isValidMove(int destFile, int destRank, const std::vector<std::unique_ptr<Piece>>& pieces) const {
	if (destFile < 1 || destFile > 8 || destRank < 1 || destRank > 8) // Checks for move outside of board
		return false;

	if (destFile == file && destRank == rank) // Checks for move to same square
		return false;

	for (const auto& p : pieces) {
		if (p->getFile() == destFile && p->rank == destRank && p->isWhitePiece() == isWhite) // Checks if square is occupied by the same color
			return false;
	}

	int df = destFile - file;
	int dr = destRank - rank;

	switch (type) { // Switch case for piece type and their respective movement
	case PieceType::Pawn: {
		int dir = isWhite ? 1 : -1; // Direction of move
		int startRank = isWhite ? 2 : 7; // Defines starting rank to check if pawn can move one or two spaces

		// Move one space forward
		if (df == 0 && dr == dir && !isSquareOccupied(destFile, destRank, pieces))
			return true;

		// Double move, only possible from starting rank
		if (df == 0 && dr == 2 * dir && rank == startRank && !isSquareOccupied(file, rank + dir, pieces) && !isSquareOccupied(destFile, destRank, pieces))
			return true;

		// Diagonal capturing
		if (std::abs(df) == 1 && dr == dir) { // Checks if user attempts to move diagonally
			for (const auto& p : pieces)
				if (p->getFile() == destFile && p->getRank() == destRank && p->isWhitePiece() != isWhite) // Checks there is an opposite colored piece on destination square
					return true;
		}

		return false; // If all checks fail
	}

	case PieceType::Rook:
		if ((df == 0 || dr == 0) && isPathClear(file, rank, destFile, destRank, pieces)) // checks for straight line movement and clear path
			return true;
		break;

	case PieceType::Bishop:
		if (std::abs(df) == std::abs(dr) && isPathClear(file, rank, destFile, destRank, pieces)) // checks for diagonal movement and clear path
			return true;
		break;

	case PieceType::Queen:
		if ((std::abs(df) == std::abs(dr) || df == 0 || dr == 0) && isPathClear(file, rank, destFile, destRank, pieces)) // checks for diagonal or straight movement and clear path
			return true;
		break;

	case PieceType::Knight:
		if ((std::abs(df) == 2 && std::abs(dr) == 1) || (std::abs(df) == 1 && std::abs(dr) == 2)) // checks for L shaped movement, does not need clear path
			return true;
		break;

	case PieceType::King:
		if (std::abs(df) <= 1 && std::abs(dr) <= 1) // Checks for 1 square movement
			return true;
		break;
	}

	return false;
}