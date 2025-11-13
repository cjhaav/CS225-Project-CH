// Rendering.cpp
// Texture mapping
// Global constants

#include "Rendering.hpp"
#include <iostream>
#include <vector>

const float squareSize = 80.f;
std::map<std::string, sf::Texture> pieceTextures;

bool loadPieceTextures() {
    std::vector<std::pair<std::string, std::string>> paths = { // Matches piece name (key) to texture path
        {"white_pawn",  "Sprites/white_pawn.png"},
        {"white_rook",  "Sprites/white_rook.png"},
        {"white_knight","Sprites/white_knight.png"},
        {"white_bishop","Sprites/white_bishop.png"},
        {"white_queen", "Sprites/white_queen.png"},
        {"white_king",  "Sprites/white_king.png"},
        {"black_pawn",  "Sprites/black_pawn.png"},
        {"black_rook",  "Sprites/black_rook.png"},
        {"black_knight","Sprites/black_knight.png"},
        {"black_bishop","Sprites/black_bishop.png"},
        {"black_queen", "Sprites/black_queen.png"},
        {"black_king",  "Sprites/black_king.png"}
    };

    for (auto& [key, path] : paths) { // Checks if each texture was loaded correctly
        if (!pieceTextures[key].loadFromFile(path)) {
            std::cerr << "ERROR: Could not load " << path << std::endl;
            return false;
        }
    }
    return true;
}