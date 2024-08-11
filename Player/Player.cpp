
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Player.hpp"

#define BRICKS 36
#define F(X) (-0.25*(X)*(X)+16.0*(X))

extern std::string Path;

    sf::Texture Player::Texture[8];

    bool Player::Enable =false;
    bool Test =false;
    bool KeyUp =false;
    int WaitKey =0;
    int Lava =0;

    Player::Player(int ID, int X, int Y) : ID(ID), X(X), Y(Y), Gravity(0), Direction(true), Swim(0), Water(false), LastX(0), LastY(0) {
        if(!Player::Enable) {
        Player::Enable =true;
        Player::Texture[0].loadFromFile(Path +"Arcanine.png");
        Player::Texture[1].loadFromFile(Path +"Alakazam.png");
        Player::Texture[2].loadFromFile(Path +"Machamp.png");
        Player::Texture[3].loadFromFile(Path +"Milotic.png");
        Player::Texture[4].loadFromFile(Path +"Metagross.png");
        Player::Texture[5].loadFromFile(Path +"Asleep.png");
        Player::Texture[6].loadFromFile(Path +"Dusknoir.png");
        Player::Texture[7].loadFromFile(Path +"Froslass.png");
        this->SoundBuffer.loadFromFile(Path +"Jump.wav");
        }

    Test =false;
    KeyUp =false;
    WaitKey =0;
    }

    bool Player::Move(sf::Event Event, int Level[], int Jump, int Turbo, bool Focus, bool NewEvent) {
    bool KeyNewPressed =false;
    static int AquaJump =0;

        if(Event.type ==sf::Event::KeyPressed && (Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) && WaitKey ==0) Test =true, KeyNewPressed =true;
        else if(Event.type ==sf::Event::KeyReleased && (Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return)) Test =false;

        if(Focus && Test) WaitKey ++;
        else if(Focus && !Test) WaitKey =0;

        if(WaitKey >5) KeyUp =true;
        else KeyUp =false;

        if(!Focus) KeyUp =false;

    int Top =int((this->Y -1) /64) *256 +this->X /64;
    int Bottom =int(this->Y /64) *256 +this->X /64 +512;

        if(Level[Bottom] ==19 && this->Swim ==0) this->Swim =-1;
        else if(Level[Bottom] ==18 && this->Swim ==-1) this->Swim =6;
        else if(Level[Bottom] <18 || Level[Bottom] >19) this->Swim =0;

        if(this->Swim >0) this->Y --, this->Swim --, this->Gravity =0;

        if((Level[Top] >23 || Level[Top +1] >23 || (Level[Top +2] >23 && this->X %64 !=0)) && (Level[Bottom] >23 || Level[Bottom +1] >23 || (Level[Bottom +2] >23 && this->X %64 !=0))) KeyUp =false;

    this->Water =(Level[Bottom -255] >14 && Level[Bottom -255] <23) || (Level[Bottom -255 -1] >14 && Level[Bottom -255 -1] <23) || (Level[Bottom -255 +1] >14 && Level[Bottom -255 +1] <23);

        if(AquaJump !=15 && Focus) AquaJump ++;
        if(AquaJump ==15 && this->ID ==3 && this->Water && KeyUp && this->Gravity >31 && Level[Top] <24 && Level[Top +1] <24 && (Level[Top +2] <24 || this->X %64 ==0)) this->Gravity =0, AquaJump =0;
        if(this->Gravity <32 && this->Gravity && (Level[Top] >23 || Level[Top +1] >23 || (Level[Top +2] >23 && this->X %64 !=0)) && this->ID !=6) this->Gravity =48;
        if(this->Gravity <32 && this->Gravity && ((Level[Top] >23 && Level[Top] !=35) || (Level[Top +1] >23 && Level[Top +1] !=35) || (Level[Top +2] >23 && Level[Top +2] !=35 && this->X %64 !=0)) && this->ID ==6) this->Gravity =48;
        if(Jump !=-1 && !KeyUp && this->Gravity <32 && this->Gravity) this->Gravity =32;

        if(((Jump ==-1 || KeyUp) && Focus && this->Gravity <32) || ((Level[Bottom] <20 && Level[Bottom +1] <20 && (Level[Bottom +2] <20 || this->X %64 ==0)) && this->ID !=6)) this->Y -=(this->ID !=5 || this->Gravity <32 || !KeyUp) ? (F(this->Gravity +1) -F(this->Gravity)) /(this->Water +1) *(Jump <1 ? 1 : Jump >15 ? 4 : 2) : -1, this->Gravity +=Jump ==-1 ? 4 : 1;
        else if(((Jump ==-1 || KeyUp) && Focus && this->Gravity <32) || (((Level[Bottom] <20 || Level[Bottom] ==35) && (Level[Bottom +1] <20 || Level[Bottom +1] ==35) && (Level[Bottom +2] <20 || Level[Bottom +2] ==35 || this->X %64 ==0)) && this->ID ==6)) this->Y -=(this->ID !=5 || this->Gravity <32 || !KeyUp) ? (F(this->Gravity +1) -F(this->Gravity)) /(this->Water +1) *(Jump <1 ? 1 : Jump >15 ? 4 : 2) : -1, this->Gravity +=Jump ==-1 ? 4 : 1;

        else {
        this->Gravity =0;

            while((Level[int(this->Y /64) *256 +this->X /64 +512] >19 && (this->ID !=6 || Level[int(this->Y /64) *256 +this->X /64 +512] !=35)) || (Level[int(this->Y /64) *256 +this->X /64 +513] >19 && (this->ID !=6 || Level[int(this->Y /64) *256 +this->X /64 +513] !=35)) || (Level[int(this->Y /64) *256 +this->X /64 +514] >19 && (this->ID !=6 || Level[int(this->Y /64) *256 +this->X /64 +514] !=35) && this->X %64 !=0)) this->Y --;

        this->Y ++;
        }

    if(this->Y <0) this->Y =0, this->Gravity =48;

    if(KeyUp && this->Gravity ==1) this->Sound.setBuffer(this->SoundBuffer), this->Sound.play();

    if(this->ID ==5 && this->Gravity >31 && KeyUp) this->Gravity =32;

    int Right =int(this->Y /64) *256 +this->X /64 +2;
    int Left =int(this->Y /64) *256 +(this->X -1) /64;

        if(!this->Water && this->X %4) this->X +=this->Direction ? 2 : -2;

        if(Focus) {
            for(int Z =0; Z <(!Turbo ? 1 : Turbo <48 ? 4 : 2); Z ++) {
            Right =int(this->Y /64) *256 +this->X /64 +2;
            Left =int(this->Y /64) *256 +(this->X -1) /64;

                if(this->Direction && (Level[Right] <24 || (Level[Right] ==35 && this->ID ==6)) && (Level[Right +256] <24 || (Level[Right +256] ==35 && this->ID ==6)) && (Level[Right +512] <24 || (Level[Right +512] ==35 && this->ID ==6) || this->Y %64 ==0)) this->X +=this->Water ? 2 : 4;
                else if(!this->Direction && (Level[Left] <24 || (Level[Left] ==35 && this->ID ==6)) && (Level[Left +256] <24 || (Level[Left +256] ==35 && this->ID ==6)) && (Level[Left +512] <24 || (Level[Left +512] ==35 && this->ID ==6) || this->Y %64 ==0)) this->X -=this->Water ? 2 : 4;
            }
        }

        if(KeyNewPressed && !Lava && (this->Gravity >0 || (Level[Top] >23 && (this->ID !=6 || Level[Top] !=35)) || (Level[Top +1] >23 && (this->ID !=6 || Level[Top +1] !=35)) || ((Level[Top +2] >23 && (this->ID !=6 || Level[Top +2] !=35)) && this->X %64 !=0))) {
            if(this->Direction && Level[Right +256] >23 && (Level[Right +256] !=BRICKS || Level[Top] !=BRICKS) && (this->Get_ID() !=6 || Level[Right +256] !=35)) {
            this->Direction =false;
            return true;
            }
            else if(!this->Direction && Level[Left +256] >23 && (Level[Left +256] !=BRICKS || Level[Top] !=BRICKS) && (this->Get_ID() !=6 || Level[Left +256] !=35)) {
            this->Direction =true;
            return true;
            }
        }

        if(this->Gravity >96) this->Gravity =96;

    return false;
    }

    void Player::Draw(sf::RenderWindow *Window, int Level_X, int Level_Y, int Heart, int Attack, int Direction) {
    static sf::Clock Clock;

    this->Sprite.setTexture(Player::Texture[this->ID]);

        if(Direction !=-1) this->Direction =Direction;

        if(!this->Gravity || Lava !=0) this->Sprite.setTextureRect(sf::IntRect((!Attack || Attack >15 ? int(this->X %(!this->Water ? 128 : 64) /(!this->Water ? 32 : 16)) *128 : Attack <8 ? 512 : 640) +(this->Direction ? 0 : 128), 0, this->Direction ? 128 : -128, 128));
        else if(this->Gravity <32) this->Sprite.setTextureRect(sf::IntRect(128 +(this->Direction ? 0 : 128), 0, this->Direction ? 128 : -128, 128));
        else this->Sprite.setTextureRect(sf::IntRect(384 +(this->Direction ? 0 : 128), 0, this->Direction ? 128 : -128, 128));

        if(Heart %120 >96) this->Sprite.setTextureRect(sf::IntRect(this->Direction ? 768 : 896, 0, this->Direction ? 128 : -128, 128));

        if(Attack && Attack <16) this->Sprite.setTextureRect(sf::IntRect((Attack <8 ? 512 : 640) +(this->Direction ? 0 : 128), 0, this->Direction ? 128 : -128, 128));

        if(this->X ==this->LastX && this->Y ==this->LastY && (!Attack || Attack >=32)) this->Sprite.setTextureRect(sf::IntRect(this->Direction ? 0 : 128, 0, this->Direction ? 128 : -128, 128));

    this->LastX =this->X;
    this->LastY =this->Y;

        if(Lava !=0) this->Sprite.setTextureRect(sf::IntRect((int(Clock.getElapsedTime().asMilliseconds() /125) %4 *128) +(this->Direction ? 0 : 128), 0, this->Direction ? 128 : -128, 128));

        if(this->Gravity ==1 && this->ID ==0) this->Sprite.setTextureRect(sf::IntRect(0 +(this->Direction ? 0 : 128), 0, this->Direction ? 128 : -128, 128));

    this->Sprite.setPosition(this->X -Level_X +896, this->Y -Level_Y +476);
    Window->draw(this->Sprite);

        if(Attack && Attack <32) Attack =std::min(Attack, 16), this->Sprite.setTextureRect(sf::IntRect(896 +(this->Direction ? 0 : Attack *16), 0, Attack *(this->Direction ? 16 : -16), 256)), this->Sprite.setPosition(this->Direction ? 1024 : 896 -Attack *16, 476), Window->draw(this->Sprite);
        else if(Attack && this->ID ==1 && Attack <128) this->Sprite.setTextureRect(sf::IntRect(896 +(this->Direction ? 0 : 256), 0, (this->Direction ? 256 : -256), 256)), this->Sprite.setPosition((this->Direction ? 896 +Attack *4 : 768 -Attack *4), 476), Window->draw(this->Sprite);
    }

    void Player::DrawXY(sf::RenderWindow* Window, int ID, int LevelX, int LevelY, int X, int Y, int dir) {
    sf::Sprite MySprite(Player::Texture[ID]);
    MySprite.setTextureRect(sf::IntRect(dir !=0 ? 0 : 128, 0, dir !=0 ? 128 : -128, 128));
    MySprite.setPosition(X -LevelX +1920 /2 -64, Y -LevelY +1080 /2 -64);
    MySprite.setColor(sf::Color(127, 127, 127, 127));
    Window->draw(MySprite);
    }

    void Player::PlayCry(int Cry) {
        if(Cry ==0) this->BufferCry.loadFromFile(Path +"Arcanine.wav");
        else if(Cry ==1) this->BufferCry.loadFromFile(Path +"Alakazam.wav");
        else if(Cry ==2) this->BufferCry.loadFromFile(Path +"Machamp.wav");
        else if(Cry ==3) this->BufferCry.loadFromFile(Path +"Milotic.wav");
        else if(Cry ==4) this->BufferCry.loadFromFile(Path +"Metagross.wav");
        else if(Cry ==5) this->BufferCry.loadFromFile(Path +"Asleep.wav");
        else if(Cry ==6) this->BufferCry.loadFromFile(Path +"Dusknoir.wav");
        else if(Cry ==7) this->BufferCry.loadFromFile(Path +"Froslass.wav");

    this->SoundCry.setBuffer(this->BufferCry);
    this->SoundCry.play();
    }
 

    const int Player::Get_ID() {
        return this->ID;
    }

    const int Player::Get_X() {
        return this->X;
    }

    const int Player::Get_Y() {
        return this->Y;
    }

    const int Player::Get_Direction() {
        return this->Direction;
    }

    const int Player::Get_Gravity() {
        return this->Gravity;
    }

    void Player::Translate(int X, int Y) {
        if(this->X +X >-1 && this->X +X <16384) this->X +=X;
        if(this->Y +Y >-1 && this->Y +Y <4096) this->Y +=Y;
    }

    void Player::OnGround(int Gravity) {
        if(Gravity >-1 && Gravity <64) this->Gravity =Gravity;
    }

    void Player::Return(bool Direction) {
        if(Direction) this->Direction =!this->Direction;
    }
