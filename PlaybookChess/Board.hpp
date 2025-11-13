// Board.hpp
// Square struct
// Board class

#pragma once
#include <SFML/Graphics.hpp>
#include <array>

struct Square {
	sf::RectangleShape shape;
	int rank = 0;
	int file = 0;
	bool isHighlighted = false; // Check is square is selected by user

	void draw(sf::RenderWindow& window) const { // Draws square using reference to window
		window.draw(shape);
	}
};

class Board {
private:
	std::array<std::array<Square, 8>, 8> squares; // 8x8 array of squares
	std::optional< std::pair<int, int> > selectedSquare;
	std::optional< std::pair<int, int> > moveSquare;
	std::optional< std::pair<int, int> > checkHighlight;
	std::optional< std::pair<int, int> > checkmateHighlight;

public:
	void initialize(); // Initialize board function
	void clearHighlights();
	void draw(sf::RenderWindow& window); // Draw function, takes window reference
	void setSelectedSquare(int file, int rank);
	void setMoveSquare(int file, int rank);
	void setCheckHighlight(int file, int rank);
	void setCheckmateHighlight(int file, int rank);
};