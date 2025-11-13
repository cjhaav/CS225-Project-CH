// Piece.hpp
// Piece class

#pragma once
#include <SFML/Graphics.hpp>
#include "Rendering.hpp"

class Piece {
private:
	int file, rank;
	bool isWhite;
	PieceType type;
	const sf::Texture* texture; // Pointer to texture location in memory, because textures take up a lot of memory it shouldn't be copied

public:
	Piece(int f, int r, bool white, PieceType t, sf::Texture& tex); // Constructor for Piece, takes reference to texture
	void draw(sf::RenderWindow& window); // Draw piece function, takes reference to window
	int getFile() const;
	int getRank() const;
	bool isWhitePiece() const;
	PieceType getType() const;
	void setPosition(int f, int r);

	// Move validation
	bool isValidMove(int destFile, int destRank, const std::vector<std::unique_ptr<Piece>>& pieces) const;
	static bool isSquareOccupied(int file, int rank, const std::vector<std::unique_ptr<Piece>>& pieces);
	static bool isPathClear(int fileStart, int rankStart, int fileEnd, int rankEnd, const std::vector<std::unique_ptr<Piece>>& pieces);
};