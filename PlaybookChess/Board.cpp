// Board.cpp
// Handles squares, coordinates, and drawing
// Board class functions

#include "Board.hpp"
#include "Rendering.hpp"

// Build chess board squares
void Board::initialize() {
	for (int rank = 0; rank < 8; rank++) {
		for (int file = 0; file < 8; file++) {
			Square& sq = squares[rank][file]; // Reference to the current square
			sq.rank = rank + 1;
			sq.file = file + 1;
			sq.shape.setSize({ squareSize, squareSize });
			sq.shape.setPosition({ file * squareSize + 64.f, rank * squareSize + 64.f });
			bool isLight = (rank + file) % 2 == 0; // Checkerboard pattern
			sq.shape.setFillColor(isLight ? sf::Color(238, 238, 210) : sf::Color(118, 150, 86)); // In line if statement with isLight bool to set square colors
		}
	}
}

// Clear highlight from all squares
void Board::clearHighlights() {
	for (auto& row : squares)
		for (auto& sq : row)
			sq.isHighlighted = false;

	checkHighlight.reset();
	checkmateHighlight.reset();
}

void Board::setSelectedSquare(int file, int rank) {
	selectedSquare = std::make_pair(file, rank);
}

void Board::setMoveSquare(int file, int rank) {
	moveSquare = std::make_pair(file, rank);
}

void Board::setCheckHighlight(int file, int rank) {
	checkHighlight = std::make_pair(file, rank);
}

void Board::setCheckmateHighlight(int file, int rank) {
	checkmateHighlight = std::make_pair(file, rank);
}

// Draw squares
void Board::draw(sf::RenderWindow& window) {
	for (auto& row : squares) // Reference to row in squares array
		for (auto& sq : row) // Reference to each square in row
			window.draw(sq.shape);

	auto drawHighlight = [&](int file, int rank, sf::Color color) {
		if (file > 0 && file < 9 && rank > 0 && rank < 9) {
			sf::RectangleShape highlight;
			highlight.setSize({ squareSize, squareSize });
			highlight.setPosition({ squares[rank - 1][file - 1].shape.getPosition() });
			highlight.setFillColor(color);
			window.draw(highlight);
		}
	};

	if (selectedSquare)
		drawHighlight(selectedSquare->first, selectedSquare->second, sf::Color(255, 255, 0, 80)); // Yellow highlight
	if (moveSquare)
		drawHighlight(moveSquare->first, moveSquare->second, sf::Color(255, 255, 0, 180)); // More transparent yellow
	if (checkHighlight)
		drawHighlight(checkHighlight->first, checkHighlight->second, sf::Color(255, 165, 0, 120)); // Orange highlight
	if (checkmateHighlight)
		drawHighlight(checkmateHighlight->first, checkmateHighlight->second, sf::Color(255, 0, 0, 80)); // Red highlight
}