
#ifndef Coin_hpp
#define Coin_hpp

    class Coin {

    protected :

        sf::Clock Clock;

        static sf::Texture Texture;

        sf::Sprite Sprite;

        int X;
        int Y;

        static bool Enable;

    public :

        Coin(int, int);
        bool Draw(sf::RenderWindow*, int, int, bool);
        void Draw(sf::RenderWindow*, int, int, int);

        const int Get_X();
        const int Get_Y();

    };

#endif
