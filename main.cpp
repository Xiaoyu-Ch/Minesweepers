#include <SFML/Graphics.hpp>

#include "WelcomeWindow.h"
#include "GameWindow.h"
#include <string>
#include <iostream>

int main() {
    WelcomeWindow welcome(800, 600);
    std::string name = welcome.run();

    std::cout << "Welcome, " << name << "!" << std::endl;

    gameWindow game;
    game.setPlayerName(name);
    game.run();

    return 0;
}


