//
// Created by Xiaoyu Chen on 2025/4/12.
//

//
// Created by Xiaoyu Chen on 2025/4/12.
//

#include "WelcomeWindow.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <cctype> // for std::isalpha

WelcomeWindow::WelcomeWindow(int w, int h) : width(w), height(h) {}

// helper function from spec
void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

// user input name
std::string holdGiven;

std::string WelcomeWindow::run() {
    sf::RenderWindow window(sf::VideoMode(width, height), "Welcome Window", sf::Style::Close);
    window.setKeyRepeatEnabled(true); // allows holding backspace



    // loadFont, assign
    sf::Font a;
    if (!a.loadFromFile("files/calibri.ttf")) {
        //std::cout << "Font failed to load" << std::endl;
    }

    // the welcome message.
    sf::Text temp;
    temp.setString("Welcome to Minesweeper!");
    temp.setFont(a);
    temp.setCharacterSize(24);
    temp.setFillColor(sf::Color::White);
    setText(temp, window.getSize().x / 2, 100);

    // name prompt
    sf::Text name;
    name.setString("Enter Your Name");
    name.setCharacterSize(20);
    name.setFont(a);
    name.setFillColor(sf::Color::White);
    setText(name, window.getSize().x / 2, height / 2 - 75);

    // typed name display
    sf::Text z;
    z.setFont(a);
    z.setCharacterSize(18);
    z.setFillColor(sf::Color::Yellow);
    z.setPosition(window.getSize().x / 2, height / 2 - 45);

    // loop
    while (window.isOpen()) {
        sf::Event flag;
        while (window.pollEvent(flag)) {
            if (flag.type == sf::Event::Closed) {
                window.close();
            }

            if (flag.type == sf::Event::TextEntered) {
                char letter = static_cast<char>(flag.text.unicode);
                if (std::isalpha(letter) && holdGiven.length() < 10) {
                    letter = std::tolower(letter);
                    if (holdGiven.empty()) letter = std::toupper(letter);
                    holdGiven += letter;
                    z.setString(holdGiven + "|");
                    setText(z, window.getSize().x / 2, height / 2 - 45);
                }
            }

            if (flag.type == sf::Event::KeyPressed) {
                if (flag.key.code == sf::Keyboard::Backspace && !holdGiven.empty()) {
                    holdGiven = holdGiven.substr(0, holdGiven.length() - 1);
                    z.setString(holdGiven + "|");
                    setText(z, window.getSize().x / 2, height / 2 - 45);
                } else if (flag.key.code == sf::Keyboard::Enter) {
                    if (!holdGiven.empty()) {
                        window.close(); // name valid, proceed
                        return holdGiven;
                    } else {
                      //  std::cout << "Name invalid" << std::endl;
                    }
                }
            }
        }

        window.clear(sf::Color::Blue);
        window.draw(temp);
        window.draw(name);
        window.draw(z);
        window.display();
    }
    return holdGiven;
//must have this line. because it error if non void
}


/* another bad version... endless revises
//cpp file
#include "WelcomeWindow.h"
#include <SFML/Graphics.hpp>
#include <string> // for std::string
WelcomeWindow::WelcomeWindow(int w, int h) : width(w), height(h) {}
//when testing, met font problem. tried changing the file but window wont let me.
//so I changed the required file name to successfully add a new file
// function given directly from document
//still just cant solve font issue. dont know

//my god. 30mins later I knew that clion uses the debug folder but I put it in project folder
//heart feels painful. hopeless as cs. hard major but job market is terrible
//I guess no one will check those files anyway.
void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

std::string holdGiven;

void WelcomeWindow::run() {
    sf::RenderWindow window(sf::VideoMode(width, height), "Welcome Window", sf::Style::Close);

    // loadFont, assign
    sf::Font a;
    a.loadFromFile("files/calibri.ttf");

    // the welcome message.
    sf::Text temp;
    temp.setString("Welcome to Minesweeper!");
    temp.setFont(a);
    temp.setCharacterSize(24);
    temp.setFillColor(sf::Color::White);
    setText(temp, window.getSize().x / 2, 100); // positioning

    // name prompt
    sf::Text name;
    name.setString("Enter Your Name");
    name.setCharacterSize(20);
    name.setFont(a);
    name.setFillColor(sf::Color::White);
    setText(name, window.getSize().x / 2, height / 2 - 75);

    // typed name display
    sf::Text z;
    z.setFont(a);
    z.setCharacterSize(18);
    z.setFillColor(sf::Color::Yellow);
    z.setPosition(window.getSize().x / 2, height / 2 - 45);

    // loop
    while (window.isOpen()) {
        sf::Event flag;
        while (window.pollEvent(flag)) {
            if (flag.type == sf::Event::Closed) {
                window.close();
            }

            if (flag.type == sf::Event::TextEntered) {
                char letter = static_cast<char>(flag.text.unicode);
                if (std::isalpha(letter) && holdGiven.length() < 10) {
                    letter = std::tolower(letter);
                    if (holdGiven.empty()) letter = std::toupper(letter);
                    holdGiven += letter;
                    z.setString(holdGiven + "|");
                    setText(z, window.getSize().x / 2, height / 2 - 45);
                }

            }
            {//bllock to handle delete, enter
                /*char letter=name.substr(0,1);
  letter.capitalize();

  if (key==delete) {
      name=name.substr(0,name.size()-1;
  }
  else if (key==enter) {
      if (name) {
          //proceed
      }
      else {
          cout<<"name invalid"<<endl;
      }
  }
   //this version work but dont force capital. detect user
if (flag.type == sf::Event::KeyPressed) {

                    if (flag.key.code == sf::Keyboard::Backspace && !holdGiven.empty()) {
                        holdGiven = holdGiven.substr(0, holdGiven.size() - 1);
                        z.setString(holdGiven + "|");
                        setText(z, window.getSize().x / 2, height / 2 - 45);
                    }
                    else if (flag.key.code == sf::Keyboard::Enter) {
                        if (!holdGiven.empty()) {
                            window.close(); // valid name, proceed
                        } else {
                           // std::cout << "name invalid" << std::endl;
                        }
                    }*/

//so just check each letter as enter in text entered block


  /*

        window.clear(sf::Color::Blue);
        window.draw(temp);
        window.draw(name);
        window.draw(z);
        window.display();
    }
*/

/*
WelcomeWindow::WelcomeWindow(int w, int h) : width(w), height(h) {}

void WelcomeWindow::run() {
    sf::RenderWindow window(sf::VideoMode(width, height), "Welcome Window", sf::Style::Close);
while(window.isOpen()){
    sf::Event flag;
    while(window.pollEvent(flag)){
            window.draw(temp);
        if(sf::Event::Closed==flag.type){ window.close()}
    }

    window.clear(sf::Color::Blue);
    window.display
}
}

//the welcome message.
sf::Text temp;
temp.setString("Welcome to Minesweeper!");
//loadFont, assign

sf::Font a;
a.loadFromFile("files/font.ttf");
//get from file
temp.setFont(a);
temp.setCharacterSize(24);
int w=window.getWidth();
temp.setPosition(w/2,100);
//positioning

sf::Text name;
name.setString("Enter Your Name");
name.setCharacterSize(20);

name.setFont(a);
centerText(name);
name.setFillColor(sf::Color::White);

// typed name display
string holdGiven;
sf::Text typed;
typed.setFont(a);
typed.setCharacterSize(18);
typed.setPosition(window.getSize().x / 2, height / 2 - 45);
typed.setFillColor(sf::Color::Yellow);
typed.setString(holdGiven + "|");
centerText(typed);

sf:: Text z;
if(z.length()<10){
    holdGiven().add(alpha(letter));
    window.draw(z+"|");
}

char letter = static_cast<char>(flag.text.unicode);

sf::Text z;
static_cast<char>holdGiven();
if(holdGiven.length()<10&&isAlpha(z)){
holdGiven+=z;
z.setString(holdGiven+letter);
centerText(z);
}
draw

// are drafts
WelcomeWindow::WelcomeWindow(int w, int h) : width(w), height(h) {}

//function given directly from document
void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}
std::string holdGiven;

void WelcomeWindow::run() {
    sf::RenderWindow window(sf::VideoMode(width, height), "Welcome Window", sf::Style::Close);

    // loadFont, assign
    sf::Font a;
    a.loadFromFile("files/font.ttf");

    // the welcome message.
    sf::Text temp;
    temp.setString("Welcome to Minesweeper!");
    temp.setFont(a);
    temp.setCharacterSize(24);
    temp.setFillColor(sf::Color::White);
    //name prompt
sf::Text name;
name.setString("Enter Your Name");
name.setCharacterSize(20);

name.setFont(a);
centerText(name);
name.setFillColor(sf::Color::White);

name.setPosition(window.getSize().x / 2, height / 2 - 75);
    int w = window.getSize().x;
    temp.setPosition(w / 2, 100); // positioning


string holdGiven;
sf::Text typed;
typed.setFont(a);
typed.setCharacterSize(18);
typed.setPosition(window.getSize().x / 2, height / 2 - 45);
typed.setFillColor(sf::Color::Yellow);
typed.setString(holdGiven + "|");
centerText(typed);


   sf::Text z;
z.setFont(a);
z.setCharacterSize(18);
z.setFillColor(sf::Color::Yellow);
z.setPosition(window.getSize().x / 2, height / 2 - 45);

// loop
while (window.isOpen()) {
    sf::Event flag;
    while (window.pollEvent(flag)) {
        if (flag.type == sf::Event::Closed) {
            window.close();
        }

        if (flag.type == sf::Event::TextEntered) {
            char letter = static_cast<char>(flag.text.unicode);

            if (std::isalpha(letter) && holdGiven.length() < 10) {
                holdGiven += letter;
                z.setString(holdGiven + "|");
                centerText(z);
            }
        }
    }

    window.clear(sf::Color::Blue);
    window.draw(temp);
    window.draw(name);
    window.draw(z);
    window.display();
}

}

*/
