
#ifndef Player_hpp
#define Player_hpp

    class Player {

    protected :

        static sf::Texture Texture[8];

        sf::Sprite Sprite;

        int ID;
        int X;
        int Y;
        int Gravity;

        bool Direction;

        sf::SoundBuffer SoundBuffer;
        sf::Sound Sound;

        sf::SoundBuffer BufferCry;
        sf::Sound SoundCry;

        static bool Enable;
        int Swim;
        int Water;

        int LastX;
        int LastY;

    public :

        Player(int, int, int);
        bool Move(sf::Event, int[], int, int, bool, bool);
        void Draw(sf::RenderWindow*, int, int, int, int =0, int =-1);

        static void DrawXY(sf::RenderWindow*, int, int, int, int, int, int);
        void PlayCry(int);

        const int Get_ID();
        const int Get_X();
        const int Get_Y();
        const int Get_Direction();
        const int Get_Gravity();

        void Translate(int, int);
        void OnGround(int);
        void Return(bool);

    };

#endif
