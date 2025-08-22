#include "GameWindow.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <random>
#include <queue>
#include <utility>
#include <cstring>
#include <algorithm>
// 8/5 final chck. no global variable. need to zip
/*why I ont need to include sfml? need this
#include <SFML/Graphics/Sprite.cpp>
sf::Texture debugTex;
debugTex.loadFromFile("files/images/debug.png");
sf::Sprite debugButton;
debugButton.setTexture(debugTex);
debugButton.setPosition(width/2 + 64, height - 90);
bool debug=false;

*/

//7/25 debug prob
//spot some problems. here. 1.board cant change size, width,landmine num
  // based on config file, as I have tried. 2. flagged tiles cant be revealed til flag is removed.
  // currenly they can. 3. the winner time is compared to the leaderboard. new records are marked with "*". now the good
  // records are appended to jeaderboard, but with no name. and no *. solve this. and is in format of seconds, not min:sec.
  // 4.the pause button is not implemented. it should pause time, only leaderboard and face work, and all tiles to tile_revealed.
  // 5.the leaderboard button, when clicked, should show the tile revealed of all tiles. then return to prior state when exit leaderboard.
  // currently I cant see the bord when its leaderboard. it all goes black in background
gameWindow::gameWindow() {
  std::string name;
  std::ifstream readFile("files/config.cfg");
  col = 25; row = 16; mineNums = 50;
  //do default, read data, resize
  readFile>>col>>row>>mineNums;
//auto not geneate if mine> col*row
  /*not the right format
  while (readFile >> name) {
    if (name == "width") readFile >> col;
    else if (name == "height") readFile >> row;
    else if (name == "mines") readFile >> mineNums;
  }*/
  height = row * 32 + 100;
  width = col * 32;
  paused = false;
  savedSeconds = 0;
  resumedClock.restart();
  //got errored because I used literal string name.
  //cant use literal

  gameOver = false; smileAble = false; timeLock = false; savedSeconds = 0;

  grid.resize(row, std::vector<Tile>(col));
  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++) {
      grid[i][j].isMine = false;
      grid[i][j].markedFlag = false;
    }
  //here we got random
  //searched online use random. basically 0-1 equal interval
  std::random_device rd; std::mt19937 gen(rd());
  std::uniform_int_distribution<> rowDist(0, row - 1);
  std::uniform_int_distribution<> colDist(0, col - 1);

  //those are what I learned rather than rand
  //rd is like seed, while the meline twister stuff is classic generator
  //then, get equal probability between the possible ranges
  //use this gen to get randomm row,col

  int minesPlaced = 0;
  while (minesPlaced < mineNums) {
    int a = rowDist(gen), b = colDist(gen);
    if (!grid[a][b].isMine) {
      //check to avoid double mine
      grid[a][b].isMine = true;
      minesPlaced++;
    }
    /*break wrong. it will exit loop completely
    while (numNow<mineNums) {
      int a,b=random(0,row),random(0,col)
      if (grid.[a][b].mine_status) {
        break;
      }
      grid[a][b].setMine;
    }*/

  }
}

void gameWindow::run() {
  bool showDebug = false;
  sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper");
//loading textures froom photos
  hiddenTexPictura.loadFromFile("files/images/tile_hidden.png");
  revealedTex.loadFromFile("files/images/tile_revealed.png");
  mineTextura.loadFromFile("files/images/mine.png");
  flagTextura.loadFromFile("files/images/flag.png");
  smile.loadFromFile("files/images/face_happy.png");
  faceLose.loadFromFile("files/images/face_lose.png");
  faceWin.loadFromFile("files/images/face_win.png");
  debugTex.loadFromFile("files/images/debug.png");
  boardTex.loadFromFile("files/images/leaderboard.png");
  digitsTex.loadFromFile("files/images/digits.png");
  pauseTex.loadFromFile("files/images/pause.png");
  playTex.loadFromFile("files/images/play.png");
  //create window, get texture, collect user doing, respoond, exit.
  numberTextures.resize(9);
  for (int i = 1; i <= 8; ++i)
    numberTextures[i].loadFromFile("files/images/number_" + std::to_string(i) + ".png");

  smileButton.setTexture(smile);
  smileButton.setPosition(width / 2 - 32, height - 90);
  debugButton.setTexture(debugTex);
  debugButton.setPosition(width / 2 + 64, height - 90);
  leaderboardButton.setTexture(boardTex);
  leaderboardButton.setPosition(width - 64, height - 90);
  pauseButton.setTexture(pauseTex);
  pauseButton.setPosition(width / 2 + 128, height - 90);


  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++) {
      grid[i][j].sprite.setTexture(hiddenTexPictura);
      grid[i][j].position = sf::Vector2f(j * 32, i * 32);
      grid[i][j].sprite.setPosition(grid[i][j].position);
    }
  //to add to form new coordin
  int flagCount = 0;
  std::vector<std::pair<int, int> > directions = {//bfs help
      {-1,-1}, {-1,0}, {-1,1}, {0,-1}, {0,1}, {1,-1}, {1,0}, {1,1}
  };//for bfs reveal

  gameClock.restart();

  while (window.isOpen()) {

    sf::Event signal;
    while (window.pollEvent(signal)) {    //keep doing unless signal
      if (signal.type == sf::Event::Closed) window.close();
      sf::Vector2i pixel = sf::Mouse::getPosition(window);

      if (signal.type == sf::Event::MouseButtonReleased && signal.mouseButton.button == sf::Mouse::Right) {
        int x = pixel.y / 32, y = pixel.x / 32;
        if (x >= 0 && x < row && y >= 0 && y < col && !grid[x][y].revealed)
          grid[x][y].markedFlag = !grid[x][y].markedFlag;
      }//right to flag
      if (signal.type == sf::Event::MouseButtonReleased && signal.mouseButton.button == sf::Mouse::Left) {
        if (debugButton.getGlobalBounds().contains(pixel.x, pixel.y) && !gameOver && !smileAble&&!paused)
          showDebug = !showDebug;
        //gotta reverse here! trap
        //check win condition
        //get off the last newline
        else if (smileButton.getGlobalBounds().contains(pixel.x, pixel.y)) resetGame();
        else if (leaderboardButton.getGlobalBounds().contains(pixel.x, pixel.y)) {//now about the leaderboard show.
          if (!paused && !gameOver && !smileAble) {
      //if click on mr smily face
            savedSeconds += resumedClock.getElapsedTime().asSeconds();
            paused = true;
          }

          /*
          if (debugButton.getGlobalBounds().contains(pixel.x, pixel.y)) {
            showBug=!showBug;
          }
          if (debugButton.getGlobalBounds().contains(pixel.x, pixel.y)) {
            showDebug = !showDebug;
          }
          */

          showLeaderboard(window);
          if (!gameOver && !smileAble) {
            resumedClock.restart();
            paused = false;
          }

        }
//debug here. leaderboard dont pause time. surround with pause logic.
        else if (pauseButton.getGlobalBounds().contains(pixel.x, pixel.y) && !gameOver && !smileAble) {
          if (!paused) {
            savedSeconds += resumedClock.getElapsedTime().asSeconds();
            paused = true;
            pauseButton.setTexture(playTex);
          } else {
            resumedClock.restart();  // restart clean!
            paused = false;
            pauseButton.setTexture(pauseTex);
          }
        }
        //all safe, just reveal
        // reveal al the mines. and swich image


        /*else if (pauseButton.getGlobalBounds().contains(pixel.x, pixel.y)&& !gameOver&&!smileAble) {

          paused = !paused;;

          if (!paused) {
            gameClock.restart();
            pauseButton.setTexture(pauseTex);
            //gameClock-=sf::seconds(savedSeconds);
            //original plan but cant. illegal. so changed to modify at seconds itself
          }
          else {
            pauseButton.setTexture(playTex);
            resumedClock.restart();
            savedSeconds=static_cast<int>(gameClock.getElapsedTime().asSeconds());
          }
        }*/
        else if (!gameOver && !smileAble&&!paused) {  //win game or lose
          // clear and re-initialize grid
          //signature,load image,title text,read name,go bback,draw
          int x = pixel.y / 32, y = pixel.x / 32;
          if (x >= 0 && x < row && y >= 0 && y < col) {
            if (grid[x][y].isMine) {
              gameOver = true; timeLock = true;
              savedSeconds += static_cast<int>(resumedClock.getElapsedTime().asSeconds());
              timeLock = true;
              smileButton.setTexture(faceLose);
              for (int i = 0; i < row; i++)
                for (int j = 0; j < col; j++)
                  if (grid[i][j].isMine) grid[i][j].revealed = true;
              /*spite:button revealed
                setImage
                if (ButtonPessed:event &&event=left) {
                  if (mouse.position in button) {
                    for (auto ) {
                      for () {
                        mine.textue=reveal
                      }
                    }
                  }
                }
                */
            } else if (!grid[x][y].revealed&& !grid[x][y].markedFlag) {//debug here. now flagged wont reveal
              std::queue<std::pair<int, int> > reveal;
              reveal.push(std::make_pair(x, y));
              while (!reveal.empty()) {
                std::pair<int, int> temp = reveal.front(); reveal.pop();
                int tempX = temp.first, tempY = temp.second;
                if (grid[tempX][tempY].revealed) continue;
                grid[tempX][tempY].revealed = true;
                //circle around to see
                //meet all the neighbors
                //dont allow strutured binding had to change to pair here
                //check win condition

                //how to die

                /*
                if (position.isMine()) {
                  gameover=true;
                }
                if (gameover) {
                  for (int i=0;i<m;i++) {
                    for (int j=0;j<n;j++) {
                      if (grid[i][j].isMine) {
                        grid[i][j].revealMine;
                        cout>>"Game Over">>endl;
                      }
                    }
                  }
                }
                */
                int count = 0;
                for (size_t i = 0; i < directions.size(); i++) {
                  int dx = directions[i].first, dy = directions[i].second;
                  int xn = tempX + dx, yn = tempY + dy;
                  if (xn >= 0 && xn < row && yn >= 0 && yn < col && grid[xn][yn].isMine) count++;
                }
                //met problem of many cols of flag and mine
                //turns out flag auto marked. and mine not leared so left to next
                //basically doing mine from 40 not 50. also cause spots overflow. 2probs
                grid[tempX][tempY].adjacentMines = count;
                if (count == 0) {
                  for (size_t i = 0; i < directions.size(); i++) {
                    int dx = directions[i].first, dy = directions[i].second;
                    int xn = tempX + dx, yn = tempY + dy;
                    if (xn >= 0 && xn < row && yn >= 0 && yn < col &&
                      !grid[xn][yn].revealed && !grid[xn][yn].isMine)
                      reveal.push(std::make_pair(xn, yn));
                  }//get off the last newline
                  //all safe, just reveal
                  //now about the leaderboard show.
                  //reveal al the mines. and swich image
                }
              }

              bool win = true;
              for (int i = 0; i < row; i++)
                for (int j = 0; j < col; j++)
                  if (!grid[i][j].revealed && !grid[i][j].isMine) win = false;
              if (win) {
                smileAble = true; timeLock = true;
                smileButton.setTexture(faceWin);
                savedSeconds += static_cast<int>(resumedClock.getElapsedTime().asSeconds());
                timeLock = true;
                /*bad version
                bool win=true;
                for (int q=0;q<grid.size();q++) {
                  for (int w=0;q<grid.size();w++) {
                    if (!grid[q][w].revealed&&!grid[q][w].isMine) {
                      win=false;
                    }
                  }
                }
                */


                int seconds = savedSeconds;
                int minutes = seconds / 60, secs = seconds % 60;

                //sequence matter here. move to bottom to not be overriden by sprite
                //sf::clock countSec;
                // int seconds=static_cast<int>(countSec.getElapsedTime().asSeconds());
                // sf::Text seconds...

                std::vector<std::pair<int, std::string> > scores;
                std::ifstream fin("files/leaderboard.txt");

                //signature,load image,title text,read name,go bback,draw
                //the leaderboard I originally understood wrong to write to a local file to compete with myself
                std::string line;
                while (getline(fin, line)) {
                  size_t comma = line.find(',');
                  if (comma != std::string::npos) {
                    std::string timeStr = line.substr(0, comma);
                    std::string name = line.substr(comma + 1);
                    int mm = std::stoi(timeStr.substr(0, 2));
                    int ss = std::stoi(timeStr.substr(3, 2));
                    scores.push_back(std::make_pair(mm * 60 + ss, name));
                  }
                }
                fin.close();
                                /*
                                scores.push_back(std::make_pair(minutes * 60 + secs, playerNameGlobal));
                                std::sort(scores.begin(), scores.end());
                                if (scores.size() > 5) scores.pop_back();

                                std::ofstream fout("files/leaderboard.txt");
                                for (size_t i = 0; i < scores.size(); ++i) {
                                  int mm = scores[i].first / 60;
                                  int ss = scores[i].first % 60;
                                  fout << (mm < 10 ? "0" : "") << mm << ":" << (ss < 10 ? "0" : "") << ss << ", " << scores[i].second << "\n";
                                }*/
                auto newest=std::make_pair(minutes*60+secs,playerNameGlobal);//to make sure * only new. so move out
                scores.push_back(newest);
                std::sort(scores.begin(),scores.end());
                bool inserted=false;
                std::ofstream outFile("files/leaderboard.txt");
                int num=0;
                for (auto i=0;i<scores.size()&&num<5;i++) {
                  int mtemp=scores[i].first/60;
                  int sTemp=scores[i].first%60;

                  std::string name = scores[i].second;
                  if (!name.empty() && name.back() == '*')
                    name.pop_back();  // actively remove old *

                  std::string converted=(mtemp<10?"0":"")+std::to_string(mtemp)+":"+(sTemp<10?"0":"")+std::to_string(sTemp)+",";
                  //first see if need 0 in front ig 04 than 4. then convert to string, and put tognther
                  //wrong draft if (.find(name)=.end()&&scores[i].second==playerNameGlobal&&scores[i].first==minutes*60+secs){
                  if (!inserted &&!inserted && scores[i] == newest&&scores[0]==newest){//but what if tme and name both same?...
                    //see if exactly the new one
                    name+="*";
                    inserted=true;
                  }outFile<<converted<<name<<"\n";
                  num++;


                }outFile.close();//need it outside. Im stupid
              }
            }
          }
        }
      }

      //double structure to encorporate pause. if paused, save time, freeze.
      //int seconds = !timeLock ? static_cast<int>(gameClock.getElapsedTime().asSeconds()) : savedSeconds;
      //int seconds = !timeLock ? (paused ? savedSeconds : static_cast<int>(gameClock.getElapsedTime().asSeconds())) : savedSeconds;
      //int seconds = !timeLock? (paused ? savedSeconds : savedSeconds + static_cast<int>(gameClock.getElapsedTime().asSeconds())): savedSeconds;
//failed plan. basically cant just do shortcut of pause clock. need to do a new clock and add time when its 0...
    }
    window.clear();
    /* skele
      to count mines,
      for(){
      for(){
      if(isMine){
      count++.
      sprite box(mineCounter){
      Sprite::Text("mine Number",count),size}
      */

    flagCount=0;
    //twinkling issue. turned out I placed the for loops inside event so they will redraw everytime when I do it.
    for (int i = 0; i < row; i++)
      for (int j = 0; j < col; j++) {
        Tile& tile = grid[i][j];
        if (tile.revealed||paused) tile.sprite.setTexture(tile.isMine ? mineTextura : revealedTex);
        else if (tile.markedFlag) { tile.sprite.setTexture(flagTextura); flagCount++; }
        else tile.sprite.setTexture(hiddenTexPictura);
        if (showDebug && tile.isMine && !tile.revealed) tile.sprite.setTexture(mineTextura);
        window.draw(tile.sprite);

        if (tile.revealed && !tile.isMine && tile.adjacentMines > 0) {
          if (tile.adjacentMines >= 1 && tile.adjacentMines <= 8) {
            sf::Sprite numSprite;
            numSprite.setTexture(numberTextures[tile.adjacentMines]);
            numSprite.setPosition(tile.position);
            window.draw(numSprite);
          }
        }
      }


    int seconds = !timeLock
  ? (paused ? savedSeconds
            : savedSeconds + static_cast<int>(resumedClock.getElapsedTime().asSeconds()))
    : savedSeconds;
    /*
    if (win) {
      stop further

      reset()
    }
    */


    drawDigits(window, mineNums - flagCount, 33, static_cast<int>(32 * (row + 0.5f) + 16));
    drawDigits(window, seconds, width - 150, height - 90);//do the number by sf text
    // align it nicely

    window.draw(smileButton); window.draw(leaderboardButton); window.draw(debugButton);window.draw(pauseButton);
    window.display();
  }
}
void gameWindow::setPlayerName(std::string name) {
  playerNameGlobal = name;
}
//for printing name at leaderboard. mend.
void gameWindow::resetGame() {
  // clear and re-initialize grid
  //simply the do game logic heere
  //win game or lose
  paused = false;
  savedSeconds = 0;
  resumedClock.restart();

  gameOver = false; smileAble = false; timeLock = false; savedSeconds = 0;
  smileButton.setTexture(smile);
  gameClock.restart();

  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++) {
      grid[i][j].isMine = false;
      grid[i][j].markedFlag = false;
      grid[i][j].revealed = false;
      grid[i][j].adjacentMines = 0;
    }

  std::random_device rd; std::mt19937 gen(rd());
  std::uniform_int_distribution<> rowDist(0, row - 1);
  std::uniform_int_distribution<> colDist(0, col - 1);
  int minesPlaced = 0;
  while (minesPlaced < mineNums) {
    int r = rowDist(gen), c = colDist(gen);
    if (!grid[r][c].isMine) {
      grid[r][c].isMine = true;
      minesPlaced++;
    }
  }

  for (int i = 0; i < row; i++) for (int j = 0; j < col; j++) {
    grid[i][j].sprite.setTexture(hiddenTexPictura);
    grid[i][j].position = sf::Vector2f(j * 32, i * 32);
    grid[i][j].sprite.setPosition(grid[i][j].position);
  }

}

void gameWindow::drawDigits(sf::RenderWindow& window, int number, int x, int y) {
  int digitWidth = 21, digitHeight = 32;
  if (number < 0) {
    sf::Sprite minus;
    minus.setTexture(digitsTex);
    minus.setTextureRect(sf::IntRect(10 * digitWidth, 0, digitWidth, digitHeight));
    minus.setPosition(x, y); window.draw(minus);
    x += digitWidth; number = abs(number);
  }
  std::string numStr = std::to_string(number);
  for (size_t i = 0; i < numStr.size(); ++i) {
    int digit = numStr[i] - '0';
    sf::Sprite sprite;
    sprite.setTexture(digitsTex);
    sprite.setTextureRect(sf::IntRect(digit * digitWidth, 0, digitWidth, digitHeight));
    sprite.setPosition(x, y); window.draw(sprite);
    x += digitWidth;
  }
}

void gameWindow::showLeaderboard(sf::RenderWindow& window) {
  sf::Font font; font.loadFromFile("files/font.ttf");
  std::ifstream file("files/leaderboard.txt");
  std::vector<std::string> storage; std::string data;
  while (std::getline(file, data)) storage.push_back(data);

  sf::Text title("Leaderboard", font, 36);
  title.setPosition(width / 2 - 100, 45);
  //tested 90 best. bigger means upper
  //oh actually specified in file
  title.setFillColor(sf::Color::Green);

  std::vector<sf::Text> lines;
  for (size_t i = 0; i < storage.size(); i++) {
    std::string line = std::to_string(i+1) + ". " + storage[i];
    sf::Text entry(line, font, 24);
    entry.setFillColor(sf::Color::White);
    entry.setPosition(100, 100 + i * 40);
    lines.push_back(entry);
  }/*switch
case "width"
break*/
  //do default, read data, resize

  sf::Text exitMsg("Click anywhere to return", font, 30);
  exitMsg.setFillColor(sf::Color::Red);
  exitMsg.setPosition(width / 2 - 150, height - 50);

  while (window.isOpen()) {
    sf::Event e;
    while (window.pollEvent(e)) {
      if (e.type == sf::Event::MouseButtonPressed) return;
      if (e.type == sf::Event::Closed) window.close();
    }
    window.clear(sf::Color::Black);
    window.draw(title);
    for (size_t i = 0; i < lines.size(); i++) window.draw(lines[i]);
    window.draw(exitMsg);
    window.display();
  }
}

