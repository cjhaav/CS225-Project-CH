// File: main.cpp
// Author: Casey Haavind
// Purpose: 
// Version: 1.0 Oct 12, 2025: Rendered board as rectangles and prototype shapes as circles with text
//			1.1 Oct 18, 2025: Added piece sprites
//			1.2 Oct 20, 2025: Added rank and file text
//			1.3 Oct 27, 2025: Added piece movement
//			1.4 Nov 11, 2025: Added selected piece highlighting and turns
//			1.5 Nov 12, 2025: Added move verification
// Resources: Used info from
//			https://www.sfml-dev.org/tutorials/3.0/: for SFML setup, shapes, and text rendering
//			Used ChatGPT to find what file/line was the root cause for an error

#include "Game.hpp"

int main() {
	Game game;
	game.run();
	return 0;
}