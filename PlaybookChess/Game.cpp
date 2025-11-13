// Game.cpp
// Handles window and main game loop
// Game class functions

#include "Game.hpp"
#include <iostream>
#include <optional> // An optional variable, does not have to store a value
#include <algorithm>
#include <memory>

Game::Game() : window(sf::VideoMode({ 800, 800 }), "Chess Board") // In line constructor for window
{
	window.setFramerateLimit(60);

	if (!loadPieceTextures()) {
		std::cerr << "ERROR: Could not load piece textures." << std::endl;
		std::exit(-1);
	}

	if (!font.openFromFile("Typography Times Regular.ttf")) {
		std::cerr << "ERROR: Could not load font." << std::endl;
		std::exit(-1);
	}

	board.initialize(); // Initialize board, text, and pieces when game is constructed
	initText();
	initPieces();
}

void Game::initText() {
	char files[8] = { 'A','B','C','D','E','F','G','H' };

	for (int i = 0; i < 8; i++) {
		// Initialize files text
		sf::Text ftext(font);
		ftext.setString(std::string(1, files[i]));
		ftext.setCharacterSize(40);

		// Center text at coordinate
		sf::FloatRect rbounds = ftext.getLocalBounds();
		ftext.setOrigin(rbounds.position + rbounds.size / 2.f);
		ftext.setPosition({ i * squareSize + 104.f, squareSize / 2.f });

		rankText.push_back(ftext); // Add to vector

		// Initialize rank text
		sf::Text rtext(font);
		rtext.setString(std::to_string(i+1));
		rtext.setCharacterSize(40);

		// Center text
		sf::FloatRect fbounds = rtext.getLocalBounds();
		rtext.setOrigin(fbounds.position + fbounds.size / 2.f);
		rtext.setPosition({ squareSize / 2.f, i * squareSize + 104.f });

		fileText.push_back(rtext); // Add to vector
	}
}

void Game::initPieces() {
	// Initialize pawns
	for (int file = 1; file < 9; file++) {
		pieces.push_back(std::make_unique<Piece>(file, 2, true, PieceType::Pawn, pieceTextures["white_pawn"]));
		pieces.push_back(std::make_unique<Piece>(file, 7, false, PieceType::Pawn, pieceTextures["black_pawn"]));
	}

	// Rooks
	pieces.push_back(std::make_unique<Piece>(1, 1, true, PieceType::Rook, pieceTextures["white_rook"]));
	pieces.push_back(std::make_unique < Piece>(8, 1, true, PieceType::Rook, pieceTextures["white_rook"]));
	pieces.push_back(std::make_unique<Piece>(1, 8, false, PieceType::Rook, pieceTextures["black_rook"]));
	pieces.push_back(std::make_unique<Piece>(8, 8, false, PieceType::Rook, pieceTextures["black_rook"]));

	// Knights
	pieces.push_back(std::make_unique<Piece>(2, 1, true, PieceType::Knight, pieceTextures["white_knight"]));
	pieces.push_back(std::make_unique<Piece>(7, 1, true, PieceType::Knight, pieceTextures["white_knight"]));
	pieces.push_back(std::make_unique<Piece>(2, 8, false, PieceType::Knight, pieceTextures["black_knight"]));
	pieces.push_back(std::make_unique<Piece>(7, 8, false, PieceType::Knight, pieceTextures["black_knight"]));

	// Bishops
	pieces.push_back(std::make_unique<Piece>(3, 1, true, PieceType::Bishop, pieceTextures["white_bishop"]));
	pieces.push_back(std::make_unique<Piece>(6, 1, true, PieceType::Bishop, pieceTextures["white_bishop"]));
	pieces.push_back(std::make_unique<Piece>(3, 8, false, PieceType::Bishop, pieceTextures["black_bishop"]));
	pieces.push_back(std::make_unique<Piece>(6, 8, false, PieceType::Bishop, pieceTextures["black_bishop"]));

	// Queens
	pieces.push_back(std::make_unique<Piece>(4, 1, true, PieceType::Queen, pieceTextures["white_queen"]));
	pieces.push_back(std::make_unique<Piece>(4, 8, false, PieceType::Queen, pieceTextures["black_queen"]));

	// Kings
	pieces.push_back(std::make_unique<Piece>(5, 1, true, PieceType::King, pieceTextures["white_king"]));
	pieces.push_back(std::make_unique<Piece>(5, 8, false, PieceType::King, pieceTextures["black_king"]));
}

std::optional<sf::Vector2i> Game::getSquareFromMouse(const sf::Vector2i& mousePos) {
	float x = mousePos.x - 64.f;
	float y = mousePos.y - 64.f;

	if (x < 0 || y < 0) return std::nullopt; // Returns nullopt if mouse is outside of window

	int file = static_cast<int>(x / squareSize) + 1; // Static cast takes the mouse position and sets the rank and file to the square that has that position
	int rank = static_cast<int>(y / squareSize) + 1; 

	if (file < 1 || file > 8 || rank < 1 || rank > 8) // Returns nullopt if mouse is outside of board
		return std::nullopt;

	return sf::Vector2i(file, rank); // Returns the file and rank of the mouse
}

bool Game::isKingInCheck(bool whiteKing) {
	int kingFile = -1, kingRank = -1;

	// Find the king
	for (const auto& p : pieces) {
		if (p->getType() == PieceType::King && p->isWhitePiece() == whiteKing) {
			kingFile = p->getFile();
			kingRank = p->getRank();
			break;
		}
	}

	if (kingFile == -1) std::cerr << "ERROR: King not found in King in Check function" << std::endl;

	// Check if king is threatened
	for (const auto& p : pieces) {
		if (p->isWhitePiece() != whiteKing) {
			if (p->isValidMove(kingFile, kingRank, pieces)) {
				return true;
			}
		}
	}
	return false;
}

bool Game::isCheckmate(bool whiteKing) {
	int kingFile = -1;
	int kingRank = -1;

	// Find King location
	for (auto& p : pieces) {
		if (p->getType() == PieceType::King && p->isWhitePiece() == whiteKing) {
			kingFile = p->getFile();
			kingRank = p->getRank();
		}
	}

	// Check if King is in check at all
	if (!isKingInCheck(whiteKing)) return false;

	// Iterate through all possible moves
	for (int i = 0; i <= pieces.size(); i++) {
		auto& p = pieces[i];
		if (p->isWhitePiece() != whiteKing) continue;
		int origFile = p->getFile();
		int origRank = p->getRank();

		for (int file = 1; file <= 8; file++) {
			for (int rank = 1; rank <= 8; rank++) {
				if (file == origFile && rank == origRank) continue;
				if (!p->isValidMove(file, rank, pieces)) continue;

				// Simulate move
				std::unique_ptr<Piece> capturedPiece = nullptr;
				int capturedIndex = -1;

				for (auto x = pieces.begin(); x != pieces.end(); x++) {
					if ((*x)->getFile() == file && (*x)->getRank() == rank && (*x)->isWhitePiece() != p->isWhitePiece()) {
						capturedIndex = std::distance(pieces.begin(), x);
						capturedPiece = std::move(*x);
						pieces.erase(x);
						break;
					}
				}

				int adjustedIndex = i;
				if (capturedIndex != -1 && static_cast<int>(capturedIndex) < i) --adjustedIndex;
				if (adjustedIndex >= pieces.size()) continue;

				// Move piece
				pieces[adjustedIndex]->setPosition(file, rank);

				// Check if king is in check
				bool inCheck = isKingInCheck(whiteKing);

				// Undo simulated piece move
				pieces[adjustedIndex]->setPosition(origFile, origRank);

				// Replace captured piece if applicable
				if (capturedPiece && capturedIndex != -1) {
					pieces.insert(pieces.begin() + capturedIndex, std::move(capturedPiece));
				}

				if (!inCheck) return false;
			}
		}
	}

	return true;
}

// Convert file/rank to chess notation
static std::string toNotation(int file, int rank) {
	std::string notation;
	notation += static_cast<char>('a' + file - 1);
	notation += std::to_string(rank);
	return notation;
}

// Convert piece type to notation
static std::string pieceSymbol(PieceType type) {
	switch (type) {
	case PieceType::King:	return "K";
	case PieceType::Queen:	return "Q";
	case PieceType::Rook:	return "R";
	case PieceType::Bishop:	return "B";
	case PieceType::Knight:	return "N";
	default:				return "";
	}
}

void Game::handleClick(int file, int rank) {
	if (gameOver) return;

	char files[8] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };

	if (!selectedPiece.has_value()) { // No piece selected yet
		for (int i = 0; i < pieces.size(); i++) {
			auto& tempP = pieces[i];
			if (tempP->getFile() == file && tempP->getRank() == rank && tempP->isWhitePiece() == whiteTurn) {
				board.clearHighlights();
				selectedPiece = i;
				board.setSelectedSquare(file, rank);
				std::cout << (whiteTurn ? "White" : "Black") << " selects " << pieceSymbol(pieces[i]->getType()) << toNotation(file, rank) << std::endl;
			}
		}
		return;
	}

	// Try to move piece
	if (!selectedPiece.has_value()) return;
	int index = selectedPiece.value();
	if (index >= pieces.size()) { selectedPiece.reset(); return; }
	Piece* piece = pieces[index].get();
	if (!piece) { selectedPiece.reset(); return; }

	if (piece->isValidMove(file, rank, pieces)) {
		// Simulate the move
		int oldFile = piece->getFile();
		int oldRank = piece->getRank();

		// Temporarily capture piece until validity is checked
		std::unique_ptr<Piece> capturedPiece = nullptr;
		bool isCapture = false;
		int capturedIndex = -1;

		for (auto captPiece = pieces.begin(); captPiece != pieces.end(); captPiece++) {
			if ((*captPiece)->getFile() == file && (*captPiece)->getRank() == rank && (*captPiece)->isWhitePiece() != piece->isWhitePiece()) {
				capturedIndex = std::distance(pieces.begin(), captPiece);
				capturedPiece = std::move(*captPiece);
				pieces.erase(captPiece);
				isCapture = true;
				break;
			}
		}

		if (isCapture && capturedIndex != -1 && static_cast<int>(capturedIndex) < index)
			--index;

		pieces[index]->setPosition(file, rank);

		// Check legality of move
		bool inCheck = isKingInCheck(piece->isWhitePiece());

		if (inCheck) {
			std::cout << "Illegal move, King in check" << std::endl;
			pieces[index]->setPosition(oldFile, oldRank); // Reset position
			if (isCapture && capturedPiece)
				pieces.insert(pieces.begin() + capturedIndex, std::move(capturedPiece)); // Replaces captured piece
		}
		else {
			std::string moveNotation; // Build notation
			moveNotation += pieceSymbol(piece->getType());
			if (isCapture) {
				if (piece->getType() == PieceType::Pawn)
					moveNotation = std::string(1, static_cast<char>('a' + oldFile - 1));
				moveNotation += "x"; // Add x to notation for a capture
			}
			moveNotation += toNotation(file, rank);

			// Check if opponent is in check after move
			bool opponentIsWhite = !piece->isWhitePiece();
			bool opponentInCheck = isKingInCheck(opponentIsWhite);
			if (opponentInCheck) moveNotation += "+";
			std::cout << moveNotation << std::endl;

			board.setMoveSquare(file, rank);
			whiteTurn = !whiteTurn; // Alternate turns
			

			// Find opponent king
			int kingFile = -1, kingRank = -1;
			for (const auto& k : pieces) {
				if (k->getType() == PieceType::King && k->isWhitePiece() == opponentIsWhite) {
					kingFile = k->getFile();
					kingRank = k->getRank();
					break;
				}
			}

			// Check for checkmate and check
			if (isCheckmate(opponentIsWhite)) {
				std::cout << (piece->isWhitePiece() ? "White" : "Black") << " wins by checkmate." << std::endl;
				if (kingFile != -1) board.setCheckmateHighlight(kingFile, kingRank);
				gameOver = true;
			}
			else if (opponentInCheck) {
				if (kingFile != -1) board.setCheckHighlight(kingFile, kingRank);
			}
		}
	}
	else {
		std::cout << "Illegal move" << std::endl;
	}

	selectedPiece.reset();
}

void Game::run() {
	// While loop that runs every frame
	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) // Close window if user closes it
				window.close();

			if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) { // Checks if user left clicks on square, then handles the click
				if (mousePressed->button == sf::Mouse::Button::Left) {
					if (gameOver) continue; // Ignore inputs once game is over

					// Get mouse position
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);

					// Convert mouse position to rank and file
					float x = mousePos.x - 64.f;
					float y = mousePos.y - 64.f;
					
					if (x >= 0 && y >= 0) {
						int file = static_cast<int>(x / squareSize) + 1;
						int rank = static_cast<int>(y / squareSize) + 1;

						if (file >= 1 && file <= 8 && rank >= 1 && rank <= 8) {
							handleClick(file, rank);
						}
					}
				}
			}
		}

		window.clear(sf::Color(50, 50, 50));
		board.draw(window);

		for (auto& r : rankText) window.draw(r); // Draw text and pieces using references
		for (auto& f : fileText) window.draw(f);
		for (auto& p : pieces) p->draw(window);

		window.display();
	}
}