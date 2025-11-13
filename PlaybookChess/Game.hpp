// Game.hpp
// Game class

#pragma once
#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "Piece.hpp"
#include "Rendering.hpp"
#include <vector>

class Game {
private:
	sf::RenderWindow window; // Game window
	sf::Font font; // Rank and file text font
	Board board; // Board class
	std::vector<std::unique_ptr<Piece>> pieces; // Pieces vector, uses unique_ptr so pointers do not move when pieces are captured and removed from the vector
	std::vector<sf::Text> rankText; // Rank text vector
	std::vector<sf::Text> fileText; // File text vector
	std::optional<int> selectedPiece; // Currently selected piece
	bool whiteTurn = true; // White starts

	std::optional<sf::Vector2i> getSquareFromMouse(const sf::Vector2i& mousePos); // Gets the square the mouse clicks on by taking the position as an integer vector
	void handleClick(int file, int rank); // Handles what to do when the user clicks on a position

	bool isKingInCheck(bool whiteKing); // Checks if king is in check, takes bool for if the king is white
	bool isCheckmate(bool whiteKing); // Checks for checkmate

	void initText(); // Initialize text prototype
	void initPieces(); // Initialize pieces prototype

	bool gameOver = false; // Ends the game if checkmated

public:
	Game(); // Constructor prototype
	void run(); // Main game loop prototype
};