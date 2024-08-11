
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Player.hpp"
#include "Enemy.hpp"

#define F(X) (-0.25*(X)*(X)+16.0*(X))

extern std::string Path;

    sf::Texture Enemy::Texture[];
    bool Enemy::Enable;

    Enemy::Enemy(int ID, int X, int Y, int Pos) : Player(ID, X, Y) {
        if(!Enemy::Enable) {
        Enemy::Enable =true;
        this->Texture[0].loadFromFile(Path +"Goomba.png");
        this->Texture[1].loadFromFile(Path +"Boo.png");
        this->Texture[2].loadFromFile(Path +"Cheep.png");
        this->Texture[3].loadFromFile(Path +"Wiggler.png");
        this->Texture[4].loadFromFile(Path +"Bobomb.png");
        this->Texture[5].loadFromFile(Path +"Spiny.png");
        this->Texture[6].loadFromFile(Path +"Piranha.png");
        this->Texture[7].loadFromFile(Path +"Thwomp.png");
        this->Texture[8].loadFromFile(Path +"Bullet.png");
        }

    this->Gravity =31;

        if(Pos ==-1) this->Direction =rand() %2;
        else this->Direction =this->X <Pos;

    this->Explosion =0;
    }

    void Enemy::Move(int Level[], int Player_X, int Player_Y) {
    int Top =int((this->Y -1) /64) *256 +this->X /64;
    int Bottom =int(this->Y /64) *256 +this->X /64 +512;

    bool Water =Level[Bottom -255] >14 && Level[Bottom -255] <23;

        if((this->ID <7 && this->ID !=2) || (this->ID ==7 && ((abs(this->X -Player_X) <64 && this->Y <Player_Y && abs(this->Y -Player_Y) <641) || Level[Top] <20 || Level[Top +1] <20 || (Level[Top +2] <20 && this->X %64 !=0)))) {
            if((Level[Bottom] <20 || this->ID ==6) && Level[Bottom +1] <20 && (Level[Bottom +2] <20 || this->X %64 ==0 || this->ID ==6) && this->Gravity ==31) this->Direction =!this->Direction, this->Gravity =32;
            else if((Level[Bottom] <20 || this->ID ==6) && Level[Bottom +1] <20 && (Level[Bottom +2] <20 || this->X %64 ==0 || this->ID ==6)) this->Y -=(F(this->Gravity +1) -F(this->Gravity)) /(Water +1), this->Gravity ++;
            else {
            this->Gravity =31;

                while((Level[int(this->Y /64) *256 +this->X /64 +512] >19 && this->ID !=6) || Level[int(this->Y /64) *256 +this->X /64 +513] >19 || (Level[int(this->Y /64) *256 +this->X /64 +514] >19 && this->X %64 !=0 && this->ID !=6)) this->Y --;

            this->Y ++;
            }
        }

        if(this->ID <6 || this->ID ==8) {
            if(this->ID !=8) {
            int Right =int(this->Y /64) *256 +this->X /64 +2;
            int Left =int(this->Y /64) *256 +(this->X -1) /64;

                if(this->Direction && Level[Right +(this->ID !=3 ? 256 : 256 +1)] >23) this->Direction =false;
                if(!this->Direction && Level[Left +(this->ID !=3 ? 256 : 256 -1)] >23) this->Direction =true;
            }

            if(!this->Explosion) {
                if(!Water && this->X %2) this->X +=this->Direction ? 1 : -1;
                else if(this->Direction) this->X +=(this->ID !=8 ? 1 : 2) *(Water ? 1 : 2);
                else this->X -=(this->ID !=8 ? 1 : 2) *(Water ? 1 : 2);
            }
        }

        if(this->Explosion !=0) this->Explosion ++;
    }

    void Enemy::Draw(sf::RenderWindow *Window, int Level_X, int Level_Y, int Frame) {
    this->Sprite.setTexture(this->Texture[this->ID]);
    this->Sprite.setTextureRect(sf::IntRect((int(this->X %128 /32) *128 +(this->Direction ? 0 : 128)) %(this->ID !=3 ? 1024 : 256) +((this->ID !=3) || this->Direction ? 0 : 128), 0, this->Direction ? 128 : -128, 128));
    this->Sprite.setPosition(this->X -Level_X +896 +(this->ID !=3 ? 0 : this->Direction ? -96 : 96) +(this->ID !=3 ? 0 : (this->Direction ? 192 : -192)), this->Y -Level_Y +476 -(this->ID !=3 ? 0 : 16));

        if(this->ID ==4 && this->Explosion) this->Sprite.setTextureRect(sf::IntRect(this->Explosion <210 ? this->Direction ? 512 : 640 : 640 +int((this->Explosion -210) /10) *128, 0, this->Explosion <210 ? this->Direction ? 128 : -128 : 128, 128));

        if(this->ID ==6) this->Sprite.setTextureRect(sf::IntRect(Frame *128, 0, 128, 128));

        if(this->X >=0) Window->draw(this->Sprite);

        if(this->ID ==3) {
        this->Sprite.setTextureRect(sf::IntRect(this->Sprite.getTextureRect().left +256, this->Sprite.getTextureRect().top, this->Sprite.getTextureRect().width, this->Sprite.getTextureRect().height));
        this->Sprite.move(0, 16);

            for(int Z =0; Z <3; Z ++) {
            this->Sprite.move(this->Direction ? -64 : 64, 0);
            Window->draw(this->Sprite);
            }
        }
     }

    void Enemy::Explode() {
        if(!this->Explosion) this->Explosion =1;
    }

    int Enemy::Get_ID() {
        return this->ID;
    }

    int Enemy::Get_Explosion() {
        return this->Explosion;
    }
