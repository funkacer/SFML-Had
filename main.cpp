#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <SFML/Window.hpp>
#include <vector>


int sizePlaygroundX,
    sizePlaygroundY,
    sizeSprite,
    sizeTextY,
    sizeBorder,
    sizeWindowX,
    sizeWindowY,
    stateOfTheGame,
    headPositionX,
    headPositionY,
    timeDelay,
    score,
    direction,
    appleX,
    appleY,
    lastClanekX,
    lastClanekY;

//sf::RectangleShape clanek;
sf::CircleShape clanek;

bool vysledek, pause;

class Letter {
public:
    sf::Font font;
    int code;
    int x;
    double pii;
    int sizeOfFont;
    int minY;
    int maxY;

    void nastav_hodnoty(sf::Font, int, int, double, int, int, int);
    sf::Text text() {
        sf::Text text;
        text.setString(sf::Uint32(code));
        // select the font
        text.setFont(font); // font is a sf::Font
        // set the string to display
        //text.setString("Hello world");
        // set the character size
        text.setCharacterSize(sizeOfFont); // in pixels, not points!
        // set the color
        text.setFillColor(sf::Color::Red);
        // set the text style
        //text.setStyle(sf::Text::Bold | sf::Text::Underlined);
        text.setStyle(sf::Text::Bold);
        int y = sin(pii) * (maxY - minY)/2 + (maxY - minY)/2 + minY;
        text.setPosition(x, y); //(x, 200) to stop
        //std::cout << sin(pii);
        pii += .1;

    return text;};
};

void Letter::nastav_hodnoty(sf::Font font, int code, int x, double pii, int sizeOfFont, int minY, int maxY) {
    this->font = font;
    this->code = code;
    this->x = x;
    this->pii = pii;
    this->sizeOfFont = sizeOfFont;
    this->minY = minY;
    this->maxY = maxY;
}

class Clanek {
public:
    int x, y, pozice;
    //sf::RectangleShape shape;
    sf::CircleShape shape;
    sf::Color colorRed = sf::Color::Red;
    sf::Color colorBlue = sf::Color::Blue;
    sf::Color color;

    Clanek (int x_, int y_, int pozice_) {
        x = x_;
        y = y_;
        pozice = pozice_;
        //sf::RectangleShape shape;
        //shape.setSize(sf::Vector2f(sizeSprite, sizeSprite));
        shape.setRadius(sizeSprite/2);
        if (pozice == 0) {
            color = colorRed;
        } else {
            color = colorBlue;
        }
        shape.setFillColor(color);
    }
    //virtual ~Clanek(){}

    void posun (int direction);

    //sf::RectangleShape get() {
    sf::CircleShape get() {
        shape.setPosition(x * sizeSprite, y * sizeSprite + sizeTextY);
        //std::cout << x * sizeSprite << std::endl;
        //std::cout << y * sizeSprite + sizeTextY << std::endl;
        return shape;}

    void changeColor() {
        if (color == colorRed) {
            color = colorBlue;
        } else {
            color = colorRed;
        }
        shape.setFillColor(color);
    }
};

std::vector <Clanek*> Had;

void Clanek::posun(int direction) {
    if (pozice == 0) {
        //hlava
        switch (direction) {
            case 0: x++; break; //doprava
            case 1: x--; break; //doleva
            case 2: y++; break; //dolu
            case 3: y--; break; //nahoru
        }

        if (x > sizePlaygroundX) {
            x = 1;
        }else if (x < 1) {
            x = sizePlaygroundX;
        }
        if (y > sizePlaygroundY) {
            y = 1;
        }else if (y < 1) {
            y = sizePlaygroundY;
        }
    } else {
        //ostatni clanky
        x = Had.at(pozice - 1)->x;
        y = Had.at(pozice - 1)->y;
    }
}

class Apple {
public:
    int x, y;
    Apple () {
        place();
    }

    void place() {
        while (true) {
            x = rand() % sizePlaygroundX + 1;
            y = rand() % sizePlaygroundY + 1;
            //kontrola zda jablko neni na hadovi, jinak pregeneruj
            int OK = 1;
            for (int i = 0; i < Had.size(); i++) {
                if (x == Had.at(i)->x && y == Had.at(i)->y) {
                    OK = 0;
                }
            }
            if (OK) break;
        }
    }

    sf::CircleShape get() {
        sf::CircleShape apple;
        apple.setRadius(sizeSprite/2);
        apple.setFillColor(sf::Color(150,255,100));
        apple.setOutlineColor(sf::Color::Red);
        apple.setOutlineThickness(5);
        apple.setPosition(x*sizeSprite, y*sizeSprite+sizeTextY); //50-,150-900
        return apple;
    }
};

bool isAnyKeyPressed() {
     for (int k = -1; k < sf::Keyboard::KeyCount; ++k) {
            if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k)))
            return true;
     } return false; }

int main()
{
    sizePlaygroundX = 15; //18
    sizePlaygroundY = 15; //16
    sizeSprite = 50; //stejné jako sizeBorder
    sizeBorder = 50; //stejné jako sizeSprite
    sizeTextY = 100;

    sizeWindowX = sizePlaygroundX * sizeSprite + 2*sizeBorder;
    sizeWindowY = sizePlaygroundY * sizeSprite + 2*sizeBorder + sizeTextY;

    stateOfTheGame = 1;
    timeDelay = 10;
    score = 0;
    direction = 0;
    pause = false;

    /*
    std::cout << sizeWindowX;
    std::cout << "\n";
    std::cout << sizeWindowY;
    std::cout << "\n";
    */

    sf::RenderWindow window(sf::VideoMode(sizeWindowX, sizeWindowY), "Wonderful snake!");
    sf::CircleShape shape(std::min(sizeWindowX, sizeWindowY)/2);
    shape.setFillColor(sf::Color::Green);
    //int i = 100;

    sf::Font font;
    if (!font.loadFromFile("fonts/arial.ttf"))
    {
        std::cout << "Error";
    }

    int sizeOfLetters = 9;
    Letter letters[sizeOfLetters];
    letters[0].nastav_hodnoty(font, 218, 110, 4, 100, 200, 700); //Ú 200
    letters[1].nastav_hodnoty(font, 382, 190, 3.5, 100, 200, 700); //ž 500
    letters[2].nastav_hodnoty(font, 97, 250, 3, 100, 200, 700); //a 250
    letters[3].nastav_hodnoty(font, 115, 310, 2.5, 100, 200, 700); //s 300
    letters[4].nastav_hodnoty(font, 110, 370, 2, 100, 200, 700); //n 200
    letters[5].nastav_hodnoty(font, 253, 430, 1.5, 100, 200, 700); //ý 400
    letters[6].nastav_hodnoty(font, 104, 550, 1, 100, 200, 700); //h 350
    letters[7].nastav_hodnoty(font, 97, 610, 0.5, 100, 200, 700); //a 450
    letters[8].nastav_hodnoty(font, 100, 670, 0, 100, 200, 700); //d 250

    //sf::Text text = letters[0].text();
    //std::cout << letters[0].obsah();

    sf::Text text;
    // select the font
    text.setFont(font); // font is a sf::Font
    // set the string to display
    text.setString("Press any key to start...");
    // set the character size
    text.setCharacterSize(40); // in pixels, not points!
    // set the color
    text.setFillColor(sf::Color::Red);
    // set the text style
    //text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text.setPosition(100, 50);


    /*
    sf::Text text1;
    // select the font
    text1.setFont(font); // font is a sf::Font
    // set the string to display
    text1.setString("Hello world");
    // set the character size
    text1.setCharacterSize(40); // in pixels, not points!
    // set the color
    text1.setFillColor(sf::Color::Red);
    // set the text style
    text1.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text1.setPosition(500, 550);
    */

    sf::Image icon;
    icon.loadFromFile("images/dragon.png"); // File/Image/Pixel
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    //std::string color = "Green";

    sf::RectangleShape rectangleShape;
    rectangleShape.setSize(sf::Vector2f(500, 50));
    rectangleShape.setOutlineColor(sf::Color::Green);
    rectangleShape.setOutlineThickness(5);
    rectangleShape.setOrigin(rectangleShape.getLocalBounds().width/2, rectangleShape.getLocalBounds().height/2);
    rectangleShape.setPosition(sizeWindowX / 2, 50);

    sf::RectangleShape snakePlayground;
    snakePlayground.setSize(sf::Vector2f(sizeWindowX - 2*sizeBorder, sizeWindowY - 2*sizeBorder - sizeTextY));
    snakePlayground.setFillColor(sf::Color::Green);
    snakePlayground.setOutlineColor(sf::Color::Yellow);
    snakePlayground.setOutlineThickness(sizeBorder);
    snakePlayground.setOrigin(snakePlayground.getLocalBounds().width/2, snakePlayground.getLocalBounds().height/2);
    snakePlayground.setPosition(sizeWindowX/2 + sizeBorder, sizeWindowY/2 + sizeBorder + sizeTextY / 2);

    text.setOrigin(rectangleShape.getLocalBounds().width/2, rectangleShape.getLocalBounds().height/2);
    text.setPosition(rectangleShape.getLocalBounds().width + sizeBorder, rectangleShape.getLocalBounds().height);

    Had.push_back(new Clanek(4, 4, 0));
    Had.push_back(new Clanek(3, 3, 1));
    Had.push_back(new Clanek(2, 2, 2));
    Had.push_back(new Clanek(1, 1, 3)); //pozor pole je >= 1 a <= sizePlaygroundX/Y

    Apple* apple = new Apple(); //zavola apple place

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Green);
        window.draw(rectangleShape);
        window.draw(snakePlayground);

        if (stateOfTheGame == 1) {

            timeDelay = 10;

            text.setString("Press any key to start...");
            text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
            text.setPosition(sizeWindowX / 2, 40);
            window.draw(text);
            //window.draw(text1);
            //for (int i = 0; i < sizeof(myNumbers) / sizeof(int) / 3; i++)
            for (int i = 0; i < sizeOfLetters; i++) {
                window.draw(letters[i].text());
            }
            if (isAnyKeyPressed()) {
                stateOfTheGame = 2;
            }

        } else if (stateOfTheGame == 2) {

            timeDelay = 100;

            std::stringstream ss; //musi byt tady jinak se text neustale prodluzuje
            ss << "Apples eaten: " << std::to_string(score);
            if (!pause) text.setString(ss.str());
            if (pause) text.setString("Pause! (press arrow key)");
            text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
            text.setPosition(sizeWindowX / 2, 40);
            window.draw(text);
            window.draw(apple->get());

            if (event.type == sf::Event::EventType::KeyPressed){
                // Up and down to control volume
                if (event.key.code == sf::Keyboard::Key::Down) {
                    direction = 2;
                    pause = false;
                }
                if (event.key.code == sf::Keyboard::Key::Up) {
                    direction = 3;
                    pause = false;
                }
                if (event.key.code == sf::Keyboard::Key::Right){
                    direction = 0;
                    pause = false;
                }
                if (event.key.code == sf::Keyboard::Key::Left){
                    direction = 1;
                    pause = false;
                }
            }

            if (event.key.code == sf::Keyboard::Key::Space){
                    pause = true;
            }

            for (int i=(int) Had.size() - 1; i >= 0; i--) {

                if (!pause) Had.at(i)->posun(direction);
                clanek = Had.at(i)->get();
                //std::cout << clanek.getSize().x << std::endl;
                //std::cout << clanek.getPosition().x << std::endl;
                window.draw(clanek);
            }

            if (Had.at(0)->x == apple->x && Had.at(0)->y == apple->y) {
                //jez
                score++;
                Had.push_back(new Clanek(Had.at(Had.size()-1)->x, Had.at(Had.size()-1)->y, Had.size()));
                apple->place();
            } else {
                //kolize hlavy s ostatnimi clanky
                for (int i=(int) Had.size() - 1; i >= 1; i--) {
                    if (Had.at(0)->x == Had.at(i)->x && Had.at(0)->y == Had.at(i)->y) {
                        stateOfTheGame = 3;
                    }
                }
            }

        } else if (stateOfTheGame == 3) {

            timeDelay = 200;

            std::stringstream ss; //musi byt tady jinak se text neustale prodluzuje
            ss << "Collision! Your score: " << std::to_string(score);
            text.setString(ss.str());
            text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
            text.setPosition(sizeWindowX / 2, 40);
            window.draw(text);

            for (int i=(int) Had.size() - 1; i >= 0; i--) {

                //if (!pause)vysledek = Had.at(i)->posun(direction);
                //std::cout << vysledek << std::endl;
                clanek = Had.at(i)->get();
                //std::cout << clanek.getSize().x << std::endl;
                //std::cout << clanek.getPosition().x << std::endl;
                window.draw(clanek);
            }
            //flash
            Had.at(0)->changeColor();

            if (isAnyKeyPressed()) {
                stateOfTheGame = 1;
                //reset
                score = 0;
                direction = 0;
                pause = false;
                Had.resize(0);
                Had.push_back(new Clanek(4, 4, 0));
                Had.push_back(new Clanek(3, 3, 1));
                Had.push_back(new Clanek(2, 2, 2));
                Had.push_back(new Clanek(1, 1, 3)); //pozor pole je >= 1 a <= sizePlaygroundX/Y
                apple->place();
            }

        }


        window.display();

        //i++;

        sf::sleep(sf::milliseconds(timeDelay));

    }

    return 0;
}
