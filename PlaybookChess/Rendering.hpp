// Rendering.hpp
// Texture mapping
// PieceType enum
// Global constants

#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

extern const float squareSize;
extern std::map<std::string, sf::Texture> pieceTextures; // Maps piece texture to the name of the piece for easier rendering

enum class PieceType {
	Pawn, Knight, Bishop, Rook, Queen, King
};

bool loadPieceTextures(); // Loads all textures, returns error if a texture could not be loaded