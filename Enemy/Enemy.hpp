
#ifndef Enemy_hpp
#define Enemy_hpp

    class Enemy : public Player {

    protected :

        static sf::Texture Texture[9];

        int Explosion;

        static bool Enable;

    public :

        Enemy(int, int, int, int);
        void Move(int[], int, int);
        void Draw(sf::RenderWindow*, int, int, int);
        void Explode();

        int Get_ID();
        int Get_Explosion();
    };

#endif
