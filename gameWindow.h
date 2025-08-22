#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>
#include "Tile.h"

class gameWindow {
public:
  gameWindow();
  void run();
  void resetGame();
  void showLeaderboard(sf::RenderWindow& window);
  void setPlayerName(std::string name);
  void drawDigits(sf::RenderWindow& window, int number, int x, int y);

private:
  int row, col, mineNums, width, height;
  bool gameOver, smileAble, timeLock;
  int savedSeconds;
  std::vector<std::vector<Tile> > grid;


  sf::Texture hiddenTexPictura;
  sf::Texture revealedTex;
  sf::Texture mineTextura;
  sf::Texture flagTextura;
  sf::Texture smile;
  sf::Texture faceLose;
  sf::Texture faceWin;
  sf::Texture debugTex;
  sf::Texture boardTex;
  sf::Texture digitsTex;
  sf::Texture pauseTex;
  sf::Texture playTex;

  sf::Sprite smileButton;
  sf::Sprite debugButton;
  sf::Sprite leaderboardButton;
  sf::Sprite pauseButton;

  std::vector<sf::Texture> numberTextures;

  sf::Clock gameClock;
  sf::Clock resumedClock;
  std::string playerNameGlobal;
  bool paused;
};
