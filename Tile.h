#ifndef TILE_H
#define TILE_H

#include <SFML/Graphics.hpp>

class Tile {
public:
  Tile() {}
bool revealed = false;
  bool isMine;
  bool clicked;
  bool flagged=false;
  int neighbors;
  int adjacentMines;
  bool markedFlag;

  sf::Vector2f position;
  sf::Sprite sprite; // la lugar para la imagina
};

#endif // TILE_H