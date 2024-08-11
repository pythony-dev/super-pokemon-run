
#include <SFML/Graphics.hpp>

#include "Coin.hpp"

extern std::string Path;

    sf::Texture Coin::Texture;
    bool Coin::Enable =false;

    Coin::Coin(int X, int Y) : X(X), Y(Y) {

        if(!Coin::Enable) Coin::Enable =true, Coin::Texture.loadFromFile(Path +"Coin.png");

    this->Sprite.setTexture(Coin::Texture);
    }

    bool Coin::Draw(sf::RenderWindow *Window, int Level_X, int Level_Y, bool Screenshot) {
    this->Sprite.setTextureRect(sf::IntRect(!Screenshot ? int(this->Clock.getElapsedTime().asMilliseconds() %512 /128) *64 : 64, 0, 64, 64));
    this->Sprite.setPosition(this->X -Level_X +896, this->Y -Level_Y +476);

    Window->draw(this->Sprite);

    return abs(int(this->Sprite.getPosition().x) -928) <48 && abs(int(this->Sprite.getPosition().y) -508) <96;
    }

    void Coin::Draw(sf::RenderWindow *Window, int Level_X, int Level_Y, int Frame) {
    this->Sprite.setTextureRect(sf::IntRect(Frame *64, 0, 64, 64));
    this->Sprite.setPosition(Level_X, Level_Y);

    Window->draw(this->Sprite);
    }

    const int Coin::Get_X() {
        return this->X;
    }

    const int Coin::Get_Y() {
        return this->Y;
    }
