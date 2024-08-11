
#include <thread>
#include <fstream>
#include <cmath>

#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Save.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Coin.hpp"
#include "Network.hpp"

#define SCREENSHOTS_MODE (Pause.getScale().x !=0.0)
#define LEVEL_DEBUG false

#define IS_JUMPING (Event.type ==sf::Event::KeyPressed && (Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return))
// LAVE X3
#define IS_PLAYING (!RecordTermine && !AffichagePodium && !Shape_1.getRadius())
#define ID_BEGIN 0
#define LEVEL_MAIN 2

#define DEC_EXPT_LEFT (Level[Z -1] >24 && Level[Z -1] <48 ? +32 : 0)
#define DEC_EXPT_RIGHT (Level[Z +1] >24 && Level[Z +1] <48 ? -32 : 0)

extern std::string Path;

    int EMPTY_OR_WATER(int Level[], int Z) {
        if(Z >255) { // SI ON EST PAS HORS CHAMP
            if(Level[Z -256] <15 || Level[Z -256] >22) { // SI LE BLOC AU DESSUS EST HORS DE L'EAU
                if((Level[Z -1] >14 && Level[Z -1] <23) || (Level[Z +1] >14 && Level[Z +1] <23)) return 17;

                else if(Level[Z +256] ==15 || Level[Z +256] ==16) return Level[Z +256] +1;
                else if(Level[Z +256] ==17) return 1;

            return 1;
            } else return 16;
        } else return 1;
    }

    int Play(sf::RenderWindow *Window, int LevelID, int Mode, int *Scoring, int Session, int PlayerID) {
    //std::cout << "Start Level " << LevelID << "; Mode " << Mode << "; Session " << Session << "; PID " << PlayerID << std::endl;
    //LevelID =LEVEL_MAIN;

    // SFML Variables
    sf::Clock Clock;
    sf::Clock Laps;
    sf::Clock EnemyAnim;
    sf::Event Event;

    sf::Font Font;
    Font.loadFromFile(Path +"Font_3.ttf");

    sf::Text Text("Hello", Font, 60), Score("0", Font, 45), MiniScore("0", Font, 24), RecordText("0", Font, 48), TextMenu("Continuer", Font, 60);
    Text.setColor(sf::Color::White);
    Text.setPosition(32 +64, 16);
    Score.setPosition(800, 1080 -128);
    Score.setColor(sf::Color::Black);
    MiniScore.setPosition(800, 1080 -36);
    MiniScore.setColor(sf::Color::Black);
    RecordText.setColor(sf::Color::Black);
    TextMenu.setColor(sf::Color::Black);

    sf::Texture Texture[20];
    Texture[0].loadFromFile(Path+ "Tileset_" +(LevelID !=0 ? std::to_string((LevelID +9) /10) : "1") +".png");
    Texture[1].loadFromFile(Path +"Background_" +(LevelID !=0 ? std::to_string((LevelID +9) /10) : "1") +".png");
    Texture[2].loadFromFile(Path +"Items.png");
    //Texture[3].loadFromFile("/Users/AnthonyMew/Desktop/Multi.png");
    Texture[4].loadFromFile(Path +"Record.png");
    Texture[5].loadFromFile(Path +"MyRecord.png");
    Texture[6].loadFromFile(Path +"Podium.png");
    Texture[7].loadFromFile(Path +"Toads.png");
    Texture[8].loadFromFile(Path +"PokéCoin.png");
    Texture[9].loadFromFile(Path +"Pause.png");
    Texture[10].loadFromFile(Path +"Choice.png");
    Texture[11].loadFromFile(Path +"Fireworks_1.png");
    Texture[12].loadFromFile(Path +"Fireworks_2.png");
    Texture[13].loadFromFile(Path +"Fireworks_3.png");
    Texture[14].loadFromFile(Path +"Fireworks_4.png");

    sf::Sprite Tileset(Texture[0]), Background(Texture[1]), Items(Texture[2]), MultiBG(Texture[3]), Record(Texture[4]), MyRecord(Texture[5]), Podium(Texture[6]), Toad(Texture[7]), PokeCoin(Texture[8]), Pause(Texture[9]), PauseChoice(Texture[10]), Firework_1(Texture[11]), Firework_2(Texture[12]), Firework_3(Texture[13]), Firework_4(Texture[14]);

    Pause.setScale(0, 0);
    PauseChoice.setScale(0, 0);

    sf::CircleShape Shape_1(1089, 60);
    Shape_1.setFillColor(sf::Color::Black);
    sf::RectangleShape Shape_2(sf::Vector2f(1920, 1080));
    Shape_2.setFillColor(sf::Color(0, 0, 0, 0));

    sf::Music Music, MusicRecord;
    Music.openFromFile(Path +"Theme_" +std::to_string(int((LevelID -1) /10) +1) +".ogg");
    Music.setLoop(true);
    MusicRecord.openFromFile(Path +"Record.ogg");
    MusicRecord.setLoop(true);

    sf::SoundBuffer Buffer[20];
    Buffer[0].loadFromFile(Path +"Coin.wav");
    Buffer[1].loadFromFile(Path +"Block.wav");
    Buffer[2].loadFromFile(Path +"Brick.wav");
    Buffer[3].loadFromFile(Path +"Water.wav");
    Buffer[4].loadFromFile(Path +"EnemyDie.wav");
    Buffer[5].loadFromFile(Path +"Explosion.wav");
    Buffer[6].loadFromFile(Path +"CourseClear.wav");
    Buffer[7].loadFromFile(Path +"Switch.wav");
    Buffer[8].loadFromFile(Path +"Cannon.wav");
    Buffer[9].loadFromFile(Path +"LittleJump.wav");
    Buffer[10].loadFromFile(Path +"BigJump.wav");
    Buffer[11].loadFromFile(Path +"PokéCoin.wav");
    Buffer[12].loadFromFile(Path +"AllPokéCoin.wav");
    Buffer[13].loadFromFile(Path +"Block?.wav");
    Buffer[14].loadFromFile(Path +"Hurt.wav");
    Buffer[15].loadFromFile(Path +"Dead.wav");
    Buffer[16].loadFromFile(Path +"OpenMenu.wav");
    Buffer[17].loadFromFile(Path +"CloseMenu.wav");
    Buffer[18].loadFromFile(Path +"Pipe.wav");

    sf::Sound Sound_1(Buffer[0]), Sound_2(Buffer[1]), Sound_3(Buffer[2]), Sound_4(Buffer[3]), SoundDie(Buffer[4]), SoundExplosion(Buffer[5]), CourseClear(Buffer[6]), SwitchSound(Buffer[7]), Cannon(Buffer[8]), PetitJump(Buffer[9]), GrandJump(Buffer[10]), PokeSound(Buffer[11]), AllPokeSound(Buffer[12]), BlockB(Buffer[13]), Hurt(Buffer[14]), Dead(Buffer[15]), OpenMenu(Buffer[16]), CloseMenu(Buffer[17]), Pipe(Buffer[18]);

    // Primitives Variables
    std::vector<sf::Vector2f> Keys;
    std::vector<Player> Players;
    std::vector<Enemy> Enemies;
    std::vector<Coin> Coins;
    std::vector<sf::Vector3i> Toads;
    std::vector<sf::Vector2i> PokeCoins;
    std::vector<sf::Vector3i> AnimCoins;

    std::string List[3*7];

    Player PlayerCry(0, 0, 0);
    Coin AnimCoin(0, 0);

    static int q =0;
    long PointsAbs =(Mode ==0 ? (unsigned int)(std::stoul(ReadSave(20 +8, 8, false))) : (*Scoring)), PointsRel =0;
    int Level[16384] ={0}, ID =-1, Heart =600, Attack =0, Switch =0, Door =0, Jump =0, Turbo =0, End =0, Multi[16] ={}, AttackDelay =-1, PokeCount =0, RecordTermine =0, intTombe =0, AffichagePodium =0, t =0, endless =0, WaitBlockB =0, PauseState =0, Transition =0, sens =-1, FireworkDelay =120;
    bool KeyEnable =false, c =true, isRecordSended =false, valueExit =false;
    bool Language =(std::stoi(ReadSave(156, 1, false)) >>2) %2;

    // LEVEL LOADING
    LoadLevel(Level, LevelID);

        //if(LEVEL_DEBUG) return 0; // ERROR
        if(LEVEL_DEBUG) {
        std::ifstream Stream(LEVEL_DEBUG ? "/Users/AnthonyMew/Desktop/Super Pokémon Run - Level Editor/Level.sav" : Path +(LevelID !=0 ? "Level_" +std::to_string(LevelID) : "EasterEgg") +".sav");

            if(!Stream) return -1; // ERROR NOT CATCHED !!!

        Stream.seekg(0, std::ios::end);

            if(Stream && Stream.tellg() ==32769) {
            Stream.seekg(0, std::ios::beg);

            char Character =0;

                for(int Z =0; Z <16384; Z ++) {
                Stream.get(Character);

                    if(Character >='0' && Character <='9') Character -='0';
                    else if(Character >='A' && Character <='F') Character -=('A' -10);
                    else Character =0;

                Level[Z] =Character *16;
                Stream.get(Character);

                    if(Character >='0' && Character <='9') Character -='0';
                    else if(Character >='A' && Character <='F') Character -=('A' -10);
                    else Character =0;
                
                Level[Z] +=Character;
                }
            }
        }

        for(int Z =0; Z <16384; Z ++) {
            if(Level[Z] >55) Players.push_back(Player(Level[Z] -56, Z %256 *64 -32, Z /256 *64 -64)), Level[Z] =Z >255 ? Level[Z -256] <15 || Level[Z -256] >22 ? 1 : 16 : 1;
            else if(Level[Z] >47) Enemies.push_back(Enemy(Level[Z] -48, Z %256 *64 -32 +DEC_EXPT_LEFT +DEC_EXPT_RIGHT, Z /256 *64 -64, !SCREENSHOTS_MODE || ID ==-1 ? -1 : Players[ID].Get_X())), Level[Z] =Level[Z] !=50 ? 1 : 16;
            if(Level[Z] ==9) End ++, Toads.push_back(sf::Vector3i(Z %256 *64, Z /256 *64, rand() %5));
            else if(Level[Z] ==2 && Mode !=1 && Mode !=3) Coins.push_back(Coin(Z %256 *64, Z /256 *64));

            if(Level[Z] ==2) Level[Z] =EMPTY_OR_WATER(Level, Z);
            else if(Level[Z] ==0) ID =int(Players.size()), Players.push_back(Player(LevelID !=0 ? (LevelID -1) /5 : 0, Z %256 *64 -32, Z /256 *64 -64));
            else if(Level[Z] ==11) PokeCoins.push_back(sf::Vector2i(Z %256 *64, Z /256 *64)), Level[Z] =EMPTY_OR_WATER(Level, Z), PokeCount ++;

            if(!SCREENSHOTS_MODE && Z >=256 && Level[Z] >23 && Level[Z] <27 && Level[Z -256] ==1 && rand() %8 ==0) Level[Z -256] =12;
            else if(!SCREENSHOTS_MODE && Z >=512 && Level[Z] >23 && Level[Z] <27 && Level[Z -256] ==1 && Level[Z -512] ==1 && rand() %8 ==0) Level[Z -256] =13, Level[Z -512] =14;
            else if(!SCREENSHOTS_MODE && Z >=256 && Z <65536 -256 && Level[Z] ==16 && Level[Z +256] >23 && Level[Z +256] <27 && rand() %4 ==0) Level[Z] =15;

            if(SCREENSHOTS_MODE && Level[Z] ==0) Level[Z] =16;
        }

    // Starting Threads
    bool networkIsRunning =false, networkIsRunning2 =false, needSwitch =false;

        if(Session >0 && PlayerID >0) {
        networkIsRunning =true;
        std::thread tSetPosition(SetPosition, &Players, &ID, Multi, Session, PlayerID, &c, &networkIsRunning);
        tSetPosition.detach();

        networkIsRunning2 =true;
        std::thread tSwitchLevel(GetLevel, &networkIsRunning2, &c, &needSwitch, Session, LevelID);
        tSwitchLevel.detach();
        }

        //for(int Z =0; Z !=20; Z ++) WriteSave(8 +Z, 1, 15); // ONLY FOR UNLOCKING ALL LEVELS !!!

        if(Mode ==3) PointsRel =102 +LevelID *10;

    Music.play();

        // Play Super Pokémon Run
        while(Window->isOpen() && Shape_1.getRadius() !=1104 && !needSwitch) {
        bool NewEvent =false;

            if((NewEvent =Window->pollEvent(Event)) && Event.type ==sf::Event::Closed) Window->close();
            else if(Shape_1.getRadius() ==1) Shape_1.setRadius(0);

            else if(Heart ==0 && !Shape_1.getRadius() && (Mode ==0 || Session ==0)) Shape_1.setRadius(16);

            if(PauseState ==0 && Music.getStatus() ==1) Music.play();

            else if(!Shape_1.getRadius() && End >0) {
                if(Event.type ==sf::Event::KeyReleased && (Event.key.code ==sf::Keyboard::Escape || Event.key.code ==sf::Keyboard::BackSpace) && q ==0) {
                
                    if(PauseState ==0) PauseState =1, Music.pause(), OpenMenu.play();
                    else if(PauseState ==2) PauseState =3, CloseMenu.play();

                    //PauseState =(PauseState ==0 ? 1 : PauseState ==2 ? 3 : PauseState);
                q =1;
                }
                else if(Event.type ==sf::Event::KeyPressed && (Event.key.code ==sf::Keyboard::Escape || Event.key.code ==sf::Keyboard::BackSpace)) q =0;
                else if(Event.type ==sf::Event::KeyPressed && Event.key.code ==sf::Keyboard::Down && PauseState ==2 && !Transition) sens =1, Transition =-2;
                else if(Event.type ==sf::Event::KeyPressed && Event.key.code ==sf::Keyboard::Up && PauseState ==2 && Transition ==270) sens =-1, Transition =272;

                else if(Event.type ==sf::Event::KeyPressed && (Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) && PauseState ==2 && sens ==1) Shape_1.setRadius(16), valueExit =true;
                else if(Event.type ==sf::Event::KeyPressed && (Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) && PauseState ==2 && sens ==-1) PauseState =3;
            }

            if(Shape_1.getRadius() ==32) Pipe.play();

            // Affichage des records
            if(End ==0 && RecordTermine ==0 && endless ==0 && !valueExit && Window->isOpen()) {
            Attack =0;
            Music.stop();
            CourseClear.play();

                if(Mode ==0) {
                unsigned long TotalPoints =PointsAbs +PointsRel;

                    if(TotalPoints >4294967295) TotalPoints =4294967295;

                RecordTermine =1;

                int Medal =0;

                    if(Window->isOpen() && Heart >=120) {
                        if(!End && PokeCoins.size() ==0) Medal =3;
                        else if(!End) Medal =2;
                        else Medal =1;
                    } else Medal =Window->isOpen() && Heart >=120;

                isRecordSended =true;
                std::thread ThreadRecord(SendRecords, LevelID, PointsRel, Mode, Medal ,List, (unsigned int)std::stoul(ReadSave(0, 8, false)), ReadUsername());
                ThreadRecord.detach();
                } else endless =1;
            }
            else if(Session >0 && Heart <120 && AffichagePodium ==0) {
            AffichagePodium +=16;
            }
            else if((RecordTermine ==328 || AffichagePodium ==1920) && IS_JUMPING && !Shape_1.getRadius()) Shape_1.setRadius(16);
            else if(RecordTermine >328) RecordTermine =328, MusicRecord.play();
            else if(RecordTermine && RecordTermine !=328) RecordTermine ++;

            if(endless ==180 && !Shape_1.getRadius()) Shape_1.setRadius(16);
            else if(endless !=0) endless ++;

            if(RecordTermine ==176) MusicRecord.play();
            if(AffichagePodium !=0 && AffichagePodium !=1920) AffichagePodium +=16;

        int Current =int(Players[ID].Get_Y() /64) *256 +Players[ID].Get_X() /64 +513 +(Players[ID].Get_ID() !=7 ? (Players[ID].Get_ID() ==1 ? 1 : -1) : 0);
        int Bottom =int(Players[ID].Get_Y() /64) *256 +Players[ID].Get_X() /64 +512;
        int Top =int((Players[ID].Get_Y() -1) /64) *256 +Players[ID].Get_X() /64;
        int Left =int((Players[ID].Get_Y() -0) /64) *256 +(Players[ID].Get_X() -1) /64;
        int Right =int((Players[ID].Get_Y() -0) /64) *256 +Players[ID].Get_X() /64 +2;

            if(Bottom <0 || Bottom >16383) Bottom =32;
            else if(Top <0 || Top >16383) Top =32;
            else if(Left <0 || Left >16383) Left =32;
            else if(Right <0 || Right >16383) Right =32;

            if(Players[ID].Get_ID() ==7 && Level[Current] ==17) Level[Current] =21, PointsRel ++;
            else if(Players[ID].Get_ID() ==7 && Level[Current] ==19) Level[Current] =20, PointsRel ++;

            if(Players[ID].Get_ID() ==0 && Level[Bottom] ==21) Level[Bottom] =17;
            if(Players[ID].Get_ID() ==0 && Level[Bottom +1] ==21) Level[Bottom +1] =17;
            if(Players[ID].Get_ID() ==0 && Level[Bottom +2] ==21 && Players[ID].Get_X() %64 !=0) Level[Bottom +2] =17;

            if(Players[ID].Get_ID() ==0 && Level[Bottom] ==20) Level[Bottom] =19;
            if(Players[ID].Get_ID() ==0 && Level[Bottom +1] ==20) Level[Bottom +1] =19;
            if(Players[ID].Get_ID() ==0 && Level[Bottom +2] ==20 && Players[ID].Get_X() %64 !=0) Level[Bottom +2] =19;

            // FOREACH PLAYERS
            for(int Z =0; Z <Players.size() && !Shape_1.getRadius(); Z ++) {

                if(!SCREENSHOTS_MODE && (Z !=ID || !Door) && (Z !=ID || (!AffichagePodium && Heart >=120)) && Players[Z].Move(Event, Level, Z !=ID ? -1 : Jump, Turbo, Z ==ID && !endless && !Door && !RecordTermine/* && Window->hasFocus()*/, NewEvent)) Attack =Players[ID].Get_ID() !=1 ? 32 : 128;

                if(Players[ID].Get_X() ==Players[Z].Get_X() && Players[ID].Get_Y() ==Players[Z].Get_Y() && ID !=Z) Players[Z].Translate(Players[ID].Get_Direction() ? 4 : -4, 0), Players[Z].Return(Players[Z].Get_Direction() !=Players[ID].Get_Direction()), ID =Z, Attack =0, AttackDelay =-1, PlayerCry.PlayCry(Players[ID].Get_ID());
            }

            // FOREACH ENEMIES
            for(int Z =0; Z <Enemies.size() && !Shape_1.getRadius() && !SCREENSHOTS_MODE; Z ++) {
            Enemies[Z].Move(Level, Players[ID].Get_X(), Players[ID].Get_Y());

                if(abs(Players[ID].Get_X() -Enemies[Z].Get_X()) <(Enemies[Z].Get_ID() !=3 ? 96 : 196)) {
                    if(abs(Players[ID].Get_Y() -Enemies[Z].Get_Y() +112) <16 && (Enemies[Z].Get_ID() <4 || (Enemies[Z].Get_ID() ==4 && Enemies[Z].Get_Explosion() <210) || Enemies[Z].Get_ID() ==8 || (Players[ID].Get_ID() ==4 && (Enemies[Z].Get_ID() !=4 || Enemies[Z].Get_Explosion() <210)))) Players[ID].OnGround(0);
                    else if(abs(Players[ID].Get_Y() -Enemies[Z].Get_Y()) <112 && Heart %120 ==0 && IS_PLAYING && !endless) Heart --, Hurt.play();
                }

                if((Attack && (Attack <32 || (Players[ID].Get_ID() ==1 && Attack <128)) && Enemies[Z].Get_ID() !=3 && abs(Players[ID].Get_X() -Enemies[Z].Get_X() +(Players[ID].Get_Direction() ? Attack <32 ? 192 : Attack *4 +64 : Attack <32 ? -192 : -Attack *4 -64)) <(Enemies[Z].Get_ID() !=3 ? 160 : 280) && abs(Players[ID].Get_Y() -Enemies[Z].Get_Y()) <112) || Enemies[Z].Get_Y() >3967) {

                    if(Enemies[Z].Get_ID() !=4 && (Players[ID].Get_ID() ==1 || Enemies[Z].Get_ID() !=1)) Enemies.erase(Enemies.begin() +Z), PointsRel +=(Mode ==0 || Mode ==1 || Mode ==4 ? 25 : 0), SoundDie.play();
                    else if(Enemies[Z].Get_ID() ==4) Enemies[Z].Explode();

                } else if(Enemies[Z].Get_ID() ==8 &&
                    Level[int(Enemies[Z].Get_Y() /64) *256 +(Enemies[Z].Get_X() +(Enemies[Z].Get_Direction() ? 48 : 16)) /64 +(Enemies[Z].Get_Direction() ? 1 : 0) +256] >23 &&
                    Level[int(Enemies[Z].Get_Y() /64) *256 +(Enemies[Z].Get_X() +(Enemies[Z].Get_Direction() ? 48 : 16)) /64 +(Enemies[Z].Get_Direction() ? 1 : 0) +256] !=44) Enemies.erase(Enemies.begin() +Z);

                if(Enemies[Z].Get_ID() ==4 && Enemies[Z].Get_Explosion() ==210) {
                int Top =int((Enemies[Z].Get_Y() -1) /64) *256 +Enemies[Z].Get_X() /64;
                int Bottom =int(Enemies[Z].Get_Y() /64) *256 +Enemies[Z].Get_X() /64 +512;
                int Right =int(Enemies[Z].Get_Y() /64) *256 +Enemies[Z].Get_X() /64 +2;
                int Left =int(Enemies[Z].Get_Y() /64) *256 +(Enemies[Z].Get_X() -1) /64;

                    if(Level[Top] ==36) Level[Top] =1;
                    if(Level[Top +1] ==36) Level[Top +1] =1;
                    if(Level[Top +2] ==36) Level[Top +2] =1;
                    if(Level[Bottom] ==36) Level[Bottom] =1;
                    if(Level[Bottom +1] ==36) Level[Bottom +1] =1;
                    if(Level[Bottom +2] ==36) Level[Bottom +2] =1;
                    if(Level[Right] ==36) Level[Right] =1;
                    if(Level[Right +1] ==36) Level[Right +1] =1;
                    if(Level[Right +256] ==36) Level[Right +256] =1;
                    if(Level[Right +257] ==36) Level[Right +257] =1;
                    if(Level[Right +512] ==36) Level[Right +512] =1;
                    if(Level[Right +513] ==36) Level[Right +513] =1;
                    if(Level[Left] ==36) Level[Left] =1;
                    if(Level[Left -1] ==36) Level[Left -1] =1;
//                    if(Level[Left +256] ==36) Level[Left +256] =1;
                    if(Level[Left +255] ==36) Level[Left +255] =1;
                    if(Level[Left +512] ==36) Level[Left +512] =1;
                    if(Level[Left +511] ==36) Level[Left +511] =1;

                SoundExplosion.play();
                } if(Enemies[Z].Get_ID() ==4 && Enemies[Z].Get_Explosion() ==240) Enemies.erase(Enemies.begin() +Z), PointsRel +=(Mode ==0 || Mode ==1 || Mode ==4 ? 25 : 0);
            }

            if(Heart ==119) Music.stop(), Dead.play();
            if(Heart %120) Heart --;

            if(Attack && Attack <32 && Players[ID].Get_ID() ==2) {
                // BRICKS TO EMPTY
                if(Level[Players[ID].Get_Direction() ? Right : Left] ==36) Level[Players[ID].Get_Direction() ? Right : Left] =EMPTY_OR_WATER(Level, Players[ID].Get_Direction() ? Right : Left), PointsRel +=(Mode ==0 || Mode ==4 ? 5 : 0);
                else if(Level[(Players[ID].Get_Direction() ? Right : Left) +256] ==36) Level[(Players[ID].Get_Direction() ? Right : Left) +256] =EMPTY_OR_WATER(Level, (Players[ID].Get_Direction() ? Right : Left) +256), PointsRel +=(Mode ==0 || Mode ==4 ? 5 : 0);
                else if(Level[(Players[ID].Get_Direction() ? Right : Left) +512] ==36 && Players[ID].Get_Y() %64 !=0) Level[(Players[ID].Get_Direction() ? Right : Left) +512] =EMPTY_OR_WATER(Level, (Players[ID].Get_Direction() ? Right : Left) +512), PointsRel +=(Mode ==0 || Mode ==4 ? 5 : 0);

                // BLOCK TO EMPTY BLOCK
                if((Level[Players[ID].Get_Direction() ? Right : Left] | 0x1) ==35) Level[Players[ID].Get_Direction() ? Right : Left] =33, PointsRel +=(Mode ==0 || Mode ==4 ? 7 : Mode ==2 ? 2 : 0), t ++;
                else if((Level[(Players[ID].Get_Direction() ? Right : Left) +256] | 0x1) ==35) Level[(Players[ID].Get_Direction() ? Right : Left) +256] =33, PointsRel +=(Mode ==0 || Mode ==4 ? 7 : Mode ==2 ? 2 : 0), t ++;
                else if(((Level[(Players[ID].Get_Direction() ? Right : Left) +512] | 0x1) ==35) && Players[ID].Get_Y() %64 !=0) Level[(Players[ID].Get_Direction() ? Right : Left) +512] =33, PointsRel +=(Mode ==0 || Mode ==4 ? 7 : Mode ==2 ? 2 : 0), t ++;

                // BLOC TO BLOC EMPTY
                }


            if(IS_JUMPING && !endless) KeyEnable =true;
            else if(Event.type ==sf::Event::KeyReleased && (Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) && PauseState ==0) KeyEnable =false;

            //if(Event.type ==sf::Event::KeyPressed && Event.key.code ==sf::Keyboard::X && Attack <=0) Attack =1, AttackDelay =-2;
            //if(Event.type ==sf::Event::KeyReleased && Event.key.code ==sf::Keyboard::X) Attack =0, AttackDelay =-1;

            if(!PauseState) {
            if((Attack && (Attack <32 || (Players[ID].Get_ID() ==1 && Attack <128))) || (AttackDelay ==-2 && Attack !=-1 && !RecordTermine && !AffichagePodium)) Attack ++;
            if(KeyEnable || AttackDelay >0) AttackDelay ++, Attack =(Attack <(Players[ID].Get_ID() !=1 ? 32 : 128) ? Attack : 0);
            if(!KeyEnable && AttackDelay >0) AttackDelay =(AttackDelay <5 ? -2 : -1);
            }

            if(Level[Bottom] ==37) Level[Bottom] =3, Switch =!Switch ? 1 : 3, PointsRel +=(Mode ==0 || Mode ==4 ? 20 : 0), Music.setVolume(0), SwitchSound.play();
            else if(Level[Bottom +1] ==37) Level[Bottom +1] =3, Switch =!Switch ? 1 : 3, PointsRel +=(Mode ==0 || Mode ==4 ? 20 : 0), Music.setVolume(0), SwitchSound.play();
            else if((Level[Bottom +2] ==37 && Players[ID].Get_Y() %64 !=0)) Level[Bottom +2] =3, Switch =!Switch ? 1 : 3, PointsRel +=(Mode ==0 || Mode ==4 ? 20 : 0), Music.setVolume(0), SwitchSound.play();
            else if(Switch ==2 || Switch ==512) {

                while(Coins.size() !=0) Level[int(Coins[Coins.size() -1].Get_Y() /64) *256 +Coins[Coins.size() -1].Get_X() /64] =2, Coins.pop_back();

                for(int Z =0; Z <16384; Z ++) {
                    if(Level[Z] ==2) {
                        if((Players[ID].Get_X() >=Z %256 *64 +64 || Players[ID].Get_X() <=Z %256 *64 +64 -192) || (Players[ID].Get_Y() >=Z /256 *64 +64 || Players[ID].Get_Y() <Z /256 *64 +64 -192)) Level[Z] =36;
                        else Coins.push_back(Coin(Z %256 *64, Z /256 *64)), Level[Z] =EMPTY_OR_WATER(Level, Z);
                    } else if(Level[Z] ==36) Coins.push_back(Coin(Z %256 *64, Z /256 *64)), Level[Z] =EMPTY_OR_WATER(Level, Z);
                }

            Switch =Switch ==2 ? 3 : 0;
            } else if(Switch) Switch ++;

            if(Switch !=0 && Switch <25) Music.setVolume(100 -Switch *4);
            else if(Switch >512 -25) Music.setVolume((Switch -512 +25) *4);

            if(Level[Bottom -255] ==4 && (Players[ID].Get_X() %64 ==32 || Turbo !=0) && Players[ID].Get_Y() %64 ==0 && Door ==0 && !Players[ID].Get_Gravity()) Door =1;
            else if(Door ==1 && Shape_2.getFillColor().a !=240) Shape_2.setFillColor(sf::Color(0, 0, 0, Shape_2.getFillColor().a +16));
            else if(Door ==1) {
            int Count =0;

                for(int Z =0; Z <16384; Z ++) if(Level[Z] ==5) Count ++;

                if(Count !=0) {
                Count -=rand() %Count +1;

                    for(int Z =0; Z <16384; Z ++) {
                        if(Level[Z] ==5 && !Count) Players[ID].Translate(Z %256 *64 -32 -Players[ID].Get_X(), Z /256 *64 -64 -Players[ID].Get_Y()), Z =16384;
                        else if(Level[Z] ==5) Count --;
                    }
                }

            Turbo =0;
            Attack =Players[ID].Get_ID() !=1 ? 32 : 128;
            Players[ID].Translate(Players[ID].Get_Direction() ? 4 : -4, 0);
            Door =2;
            } else if(Door ==2) Shape_2.setFillColor(sf::Color(0, 0, 0, Shape_2.getFillColor().a -16)), Door =Shape_2.getFillColor().a ? 2 : 0;

            if(Keys.size() && ((Level[Bottom -256] & 0x3E) ==6 || (Level[Bottom -255] & 0x3E) ==6 || ((Level[Bottom -254] & 0x3E) ==6 && Players[ID].Get_Y() %64 !=0))) {
            Keys.erase(Keys.begin(), Keys.begin() +16);
                
                if((Level[Bottom -256] & 0x3E) ==6) Level[Bottom -256] -=2;
                else if((Level[Bottom -255] & 0x3E) ==6) Level[Bottom -255] -=2;
                else Level[Bottom -254] -=2;
            }

            if(Level[Bottom -256] ==8 || Level[Bottom -255] ==8 || (Level[Bottom -254] ==8 && Players[ID].Get_Y() %64 !=0)) {

                for(int Z =0; Z <16; Z ++) Keys.push_back(sf::Vector2f(Players[ID].Get_X(), Players[ID].Get_Y()));

                if(Level[Bottom -256] ==8) Level[Bottom -256] =1;
                else if(Level[Bottom -255] ==8) Level[Bottom -255] =1;
                else Level[Bottom -254] =1;
            }

            if(Level[Bottom -511] ==10) Level[Bottom -511] ++, Level[Bottom -511+256] +=2, End --, PointsRel +=(Mode ==0 || Mode ==4 ? 100 : 0);
            else if(Level[Bottom -255] ==10) Level[Bottom -255] ++, Level[Bottom -255+256] +=2, End --, PointsRel +=(Mode ==0 || Mode ==4 ? 100 : 0);

            if(Level[Bottom -255] >17 && Level[Bottom -255] <21 && Players[ID].Get_ID() !=0) Heart =119;
            //else if(Level[Bottom -255] ==19) Players[ID].OnGround(0);

            if((Level[Bottom] <20 && Level[Bottom +1] <20 && (Level[Bottom +2] <20 || Players[ID].Get_X() %64 ==0)) || IS_JUMPING) {
                if(Level[Top] ==34 || (Level[Top] ==35 && Players[ID].Get_ID() !=6)) Level[Top] =33, t ++, Sound_1.play(), PointsRel +=(Mode ==0 || Mode ==4 ? 7 : Mode ==2 ? 2 : 0), AnimCoins.push_back(sf::Vector3i(Top %256 -1, int(Top /256) -1, 0)), BlockB.play();
                if(Level[Top +1] ==34 || (Level[Top +1] ==35 && Players[ID].Get_ID() !=6)) Level[Top +1] =33, t ++, Sound_1.play(), PointsRel +=(Mode ==0 || Mode ==4 ? 7 : Mode ==2 ? 2 : 0), AnimCoins.push_back(sf::Vector3i((Top +1) %256 -1, int((Top +1) /256) -1, 0)), BlockB.play();
                if((Level[Top +2] ==34 || (Level[Top +2] ==35 && Players[ID].Get_ID() !=6)) && Players[ID].Get_X() %64 !=0) Level[Top +2] =33, t ++, Sound_1.play(), PointsRel +=(Mode ==0 || Mode ==4 ? 7 : Mode ==2 ? 2 : 0), AnimCoins.push_back(sf::Vector3i((Top +2) %256 -1, int((Top +2) /256) -1, 0)), BlockB.play();

                if(Level[Top] ==33 && WaitBlockB ==0) BlockB.play(), WaitBlockB +=1;

                if(WaitBlockB ==5) WaitBlockB =0;
                else if(WaitBlockB) WaitBlockB ++;

                //if(Level[Top +1] ==34 || (Level[Top +1] ==35 && Players[ID].Get_ID() !=6)) Level[Top +1] =33, t ++, Sound_1.play(), PointsRel +=(Mode ==0 || Mode ==4 ? 7 : Mode ==2 ? 2 : 0), AnimCoins.push_back(sf::Vector3i((Top +1) %256 -1, int((Top +1) /256) -1, 0)), BlockB.play();
               //if((Level[Top +2] ==34 || (Level[Top +2] ==35 && Players[ID].Get_ID() !=6)) && Players[ID].Get_X() %64 !=0) Level[Top +2] =33, t ++, Sound_1.play(), PointsRel +=(Mode ==0 || Mode ==4 ? 7 : Mode ==2 ? 2 : 0), AnimCoins.push_back(sf::Vector3i((Top +2) %256 -1, int((Top +2) /256) -1, 0)), BlockB.play();

                if(Level[Top] ==36) Level[Top] =EMPTY_OR_WATER(Level, Top), Sound_3.play(), PointsRel +=(Mode ==0 || Mode ==4 ? 5 : 0);
                if(Level[Top +1] ==36) Level[Top +1] =EMPTY_OR_WATER(Level, Top +1), Sound_3.play(), PointsRel +=(Mode ==0 || Mode ==4 ? 5 : 0);
                if(Level[Top +2] ==36 && Players[ID].Get_X() %64 !=0) Level[Top +2] =EMPTY_OR_WATER(Level, Top +2), Sound_3.play(), PointsRel +=(Mode ==0 || Mode ==4 ? 5 : 0);
            }

            if(Players[ID].Get_Gravity() ==1 && ((Level[Bottom] ==38 && Players[ID].Get_X() %64 <32) || Level[Bottom +1] ==38 || (Level[Bottom +2] ==38 && Players[ID].Get_X() %64 >32))) Jump =17, PetitJump.play();
            else if(Players[ID].Get_Gravity() ==1 && ((Level[Bottom] ==39 && Players[ID].Get_X() %64 <32) || Level[Bottom +1] ==39 || (Level[Bottom +2] ==39 && Players[ID].Get_X() %64 >32))) Jump =23, GrandJump.play();
            else if(Jump) Jump --;

            if((Level[Bottom +1] | 0x1) ==41 && (Players[ID].Get_X() %64 ==0 || Turbo) && Level[Bottom +1] %2 ==Players[ID].Get_Direction()) Players[ID].Return(true), Attack =Players[ID].Get_ID() !=1 ? 32 : 128;
            // (!Turbo ? 1 : Turbo <48 ? 4 : 2)
            else if(Level[Bottom +1] ==42 && Players[ID].Get_X() %64 ==32) Players[ID].Translate(Players[ID].Get_Direction() ? -4 : 4, 0);
            if(Level[Bottom +1] ==42 && Turbo) Turbo =0, Players[ID].Translate((Turbo <48 ? 4 : 2) *(Players[ID].Get_Direction() ? -1 : 1), 0);
            else if(Level[Bottom +1] ==43 && Players[ID].Get_X() %64 ==0) Turbo =1;
            else if(Turbo >=64) Turbo =0;
            else if(Turbo) Turbo ++;

            if(Players[ID].Get_ID() !=4 && Heart %120 ==0 && IS_PLAYING && !endless) {
                if(((Level[Bottom] | 0x1) ==45 && Players[ID].Get_X() %64 <32) || (Level[Bottom +1] | 0x1) ==45 || ((Level[Bottom +2] | 0x1) ==45 && Players[ID].Get_X() %64 >32)) Heart --, Hurt.play();
                if((((Level[Top] | 0x1) ==45 && Players[ID].Get_X() %64 <32) || (Level[Top +1] | 0x1) ==45 || ((Level[Top +2] | 0x1) ==45 && Players[ID].Get_X() %64 >32)) && IS_JUMPING) Heart --, Hurt.play();

                if(((Level[Left] | 0x1) ==45 || (Level[Left +256] | 0x1) ==45 || ((Level[Left +512] | 0x1) ==45 && Players[ID].Get_Y() %64 >32))) Heart --, Hurt.play();
                if(((Level[Right] | 0x1) ==45 || (Level[Right +256] | 0x1) ==45 || ((Level[Right +512] | 0x1) ==45 && Players[ID].Get_Y() %64 >32))) Heart --, Hurt.play();
            }

            if(Clock.getElapsedTime().asMilliseconds() >4096) {
                for(int Z =0; Z <16384; Z ++) {
                    if(Level[Z] ==47) {
                        if(Level[Z -1] ==1) {
                        Enemies.push_back(Enemy(8, Z %256 *64 -32 -96, Z /256 *64 -64, -1));
                        Enemies[Enemies.size() -1].Return(Enemies[Enemies.size() -1].Get_Direction());

                        int dec_Y =Players[ID].Get_Y() -Enemies[Enemies.size() -1].Get_Y();

                            if(dec_Y >=-512 && dec_Y <640 && Players[ID].Get_X() -Enemies[Enemies.size() -1].Get_X() >-892) Cannon.play();
                        }
                        if(Level[Z +1] ==1) {
                        Enemies.push_back(Enemy(8, Z %256 *64 -32 +96, Z /256 *64 -64, -1));
                        Enemies[Enemies.size() -1].Return(!Enemies[Enemies.size() -1].Get_Direction());

                        int dec_Y =Players[ID].Get_Y() -Enemies[Enemies.size() -1].Get_Y();

                            if(dec_Y >=-512 && dec_Y <640 && Players[ID].Get_X() -Enemies[Enemies.size() -1].Get_X() <860 +32) Cannon.play();
                        }
                    }
                }

            Clock.restart();
            }

            if(abs(Players[ID].Get_Y() %64 -4) <=4 && Level[int(Players[ID].Get_Y() /64 +9.5) *256 +Players[ID].Get_X() /64 +15 -2062] ==17) Sound_4.play();

            if(Mode ==3) PointsRel =102 +LevelID *10 -Laps.getElapsedTime().asSeconds();
            if(PointsRel <0)  PointsRel =0;

        // CLEARING WINDOW
        Window->clear(sf::Color::Black);
        Background.setPosition(-Players[ID].Get_X() *1920 /16384, -Players[ID].Get_Y() *1080 /4096/*-(Players[ID].Get_Y() *1080 /4096)*/);
        Window->draw(Background);

            for(int Z =0; Z !=Toads.size(); Z ++) {
            Toad.setTextureRect(sf::IntRect(Toads[Z].z *64 %320, 0, 64, 128));
            Toad.setPosition(Toads[Z].x -Players[ID].Get_X() +960 -64, Toads[Z].y -Players[ID].Get_Y() +540 -128);
            Window->draw(Toad);
            }

            for(int Z =0; Z !=AnimCoins.size(); Z ++) {
                if(AnimCoins[Z].z !=64) {
                AnimCoin.Draw(Window, AnimCoins[Z].x *64 -Players[ID].Get_X() +960, AnimCoins[Z].y *64 -Players[ID].Get_Y() +540 -AnimCoins[Z].z, AnimCoins[Z].z /16);
                AnimCoins[Z].z +=4;
                }
            }

            for(int Y =0; Y <19; Y ++) {
                for(int X =0; X <31; X ++) {
                int Current =int(Players[ID].Get_Y() /64 +Y) *256 +Players[ID].Get_X() /64 +X -2062;
                int HorizontalCenter =Players[ID].Get_X() +X *64 >=960 -64 && Players[ID].Get_X() +X *64 <256 *64 +(960 -64);
                int BottomCenter =Players[ID].Get_Y() +Y *64 <64 *64 +540 +36 -64;
                int TopCenter =Players[ID].Get_Y() +Y *64 >=512;

                Tileset.setPosition(X *64 -Players[ID].Get_X() %64, Y *64 -Players[ID].Get_Y() %64 -36);

                    if(Level[Current] ==3 && Current +256 <16384 && Level[Current +256] ==18) Level[Current] =19;

                    if(HorizontalCenter && !TopCenter) {
                    Tileset.setTextureRect(sf::IntRect(Players[ID].Get_Y() +Y *64 >=512 -64 ? 32 *64 : 64, 0, 64, 64));
                    } else if(HorizontalCenter && BottomCenter) {
                        if(Level[Current] ==36 && Level[Current -256] >14 && Level[Current -256] <23) Tileset.setTextureRect(sf::IntRect(Level[Current -256] <20 ? Level[Current -256] *64 : Level[Current -256] ==20 ? 19 *64 : Level[Current -256] ==21 ? 17 *64 : 16 *64, 0, 64, 32)), Window->draw(Tileset);

                        if(Level[Current] ==36 && Level[Current +256] >14 && Level[Current +256] <23) Tileset.setTextureRect(sf::IntRect(Level[Current +256] <20 ? Level[Current +256] *64 : Level[Current +256] ==20 ? 19 *64 : Level[Current +256] ==21 ? 17 *64 : 16 *64, 32, 64, 32)), Tileset.move(0, 32), Window->draw(Tileset), Tileset.move(0, -32);

                        if(Level[Current] >14 && Level[Current] <23 && Level[Current +1] >23 && Level[Current +1] <37) Tileset.setTextureRect(sf::IntRect(Level[Current] <20 ? Level[Current] *64 : Level[Current] ==20 ? 19 *64 : Level[Current] ==21 ? 17 *64 : 16 *64, 0, 32, 64)), Tileset.move(64, 0), Window->draw(Tileset), Tileset.move(-64, 0);

                        if(Level[Current +1] >14 && Level[Current +1] <23 && Level[Current] >23 && Level[Current] <37) Tileset.setTextureRect(sf::IntRect((Level[Current +1] <20 ? Level[Current +1] *64 : Level[Current +1] ==20 ? 19 *64 : Level[Current +1] ==21 ? 17 *64 : 16 *64) +32, 0, 32, 64)), Tileset.move(32, 0), Window->draw(Tileset), Tileset.move(-32, 0);

                    Tileset.setTextureRect(sf::IntRect((Level[Current]) *64, 0, 64, 64));

                        if(Level[Current] >23 && Level[Current] <30 && Current >0 && Current <16383 && ((Level[Current -1] >14 && Level[Current -1] <23) || (Level[Current +1] >14 && Level[Current +1] <23))) Window->draw(Tileset); //Tileset.setTextureRect(sf::IntRect(((Level[Current -1] <18 || Level[Current -1] >20) && (Level[Current +1] <18 || Level[Current +1] >20) ? 1024 : 1152) +(Level[Current -1] !=17 && Level[Current -1] !=19 && Level[Current +1] !=17 && Level[Current +1] !=19 ? 0 : 64), 0, 64, 64));

                        else if(Level[Current] /2 ==2) Items.setTextureRect(sf::IntRect(Level[Current] ==4 ? 256 +128 : 384 +128, 0, 64, 64)), Items.setPosition(X *64 -Players[ID].Get_X() %64, Y *64 -Players[ID].Get_Y() %64 -36), Window->draw(Items), Items.setTextureRect(sf::IntRect(Level[Current] ==4 ? 320 +128 : 448 +128,  0, 64, 64)), Items.move(0, -64), Window->draw(Items);

                        else if(Level[Current] ==6 || Level[Current] ==7) Tileset.setTextureRect(sf::IntRect(384, 0, 64, 64)), Window->draw(Tileset), Tileset.move(0, -64), Tileset.setTextureRect(sf::IntRect(448, 0, 64, 64));
                        else if((Level[Current] ==3 || Level[Current] ==37) && Current <16128 && Level[Current +256] <22 && intTombe !=64) intTombe +=8, Tileset.move(0, intTombe);
                        else if((Level[Current] ==3 || Level[Current] ==37) && Current <16128 && Level[Current +256] <22) Tileset.move(0, 64), Level[Current +256] =Level[Current], Level[Current] =1, intTombe =0;

                        else if((Level[Current] ==38 || Level[Current] ==39 || Level[Current] ==44 || Level[Current] ==45) && Current >255 && Level[Current -256] >14 && Level[Current -256] <23) Tileset.setTextureRect(sf::IntRect(16 *64, 0, 64, 64)), Window->draw(Tileset), Tileset.setTextureRect(sf::IntRect((Level[Current]) *64, 0, 64, 64));

                        if(((Level[Current] | 0x1) ==39) && (Current <65536 -256 && Level[Current +256] >26 && Level[Current +256] <30)) Tileset.setTextureRect(sf::IntRect(25 *64, 0, 64, 64)), Window->draw(Tileset), Tileset.setTextureRect(sf::IntRect((Level[Current]) *64, 0, 64, 64));

                        if((Level[Current] ==0 || Level[Current] ==9) && ((Level[Current -1] >14 && Level[Current -1] <23) || (Level[Current +1] >14 && Level[Current +1] <23))) Tileset.setTextureRect(sf::IntRect(16 *64, 0, 64, 64)), Window->draw(Tileset), Tileset.setTextureRect(sf::IntRect(Level[Current] *64, 0, 64, 64));


                    } else if(Players[ID].Get_Y() +Y *64 >=64 *64 +540 +36) Tileset.setTextureRect(sf::IntRect(18 *64, 0, 64, 64));
                    else Tileset.setTextureRect(sf::IntRect(Players[ID].Get_Y() +Y *64 >=-64 ? 64 : 0, 0, 64, 64));

                Window->draw(Tileset);

                    if(int(Players[ID].Get_Y() +Y *64) /64 ==72) Tileset.setTextureRect(sf::IntRect(HorizontalCenter ? Level[Current -256] *64 : 64, 0, 64, 64)), Window->draw(Tileset), Tileset.setTextureRect(sf::IntRect(19 *64, 0, 64, 64)), Window->draw(Tileset);
                }
            }

            for(int Z =0; Z <PokeCoins.size(); Z ++) {

                if(abs(PokeCoins[Z].x -36 -Players[ID].Get_X()) <96 && abs(PokeCoins[Z].y -36 -Players[ID].Get_Y()) <128) {
                PokeCoins.erase(PokeCoins.begin() +Z);
                PointsRel +=(Mode ==0 || Mode ==2 || Mode ==4 ? 50 : 0);
                
                    if(PokeCoins.size() !=0) PokeSound.play();
                    else AllPokeSound.play();
                }

            PokeCoin.setTextureRect(sf::IntRect(int(EnemyAnim.getElapsedTime().asMilliseconds() /125 %4) *128, 0, 128, 128));
            PokeCoin.setPosition(PokeCoins[Z].x -Players[ID].Get_X() +960 -96, PokeCoins[Z].y -Players[ID].Get_Y() +540 -96);
            Window->draw(PokeCoin);
            }

            for(int Z =int(Keys.size()) -1; Z !=-1; Z --) {
            Keys[Z] =Z ? Keys[Z -1] : sf::Vector2f(Players[ID].Get_X(), Players[ID].Get_Y());
            Items.setTextureRect(sf::IntRect(192 +128, 0, 64, 64));
            Items.setPosition(Keys[Z].x -Players[ID].Get_X() +928, Keys[Z].y -Players[ID].Get_Y() +508);

                if(Z %16 ==15) Window->draw(Items);
            }

        Items.setColor(sf::Color(255, 255, 255, 255 -255.0 *Pause.getScale().x));

            for(int Z =0; Z <Coins.size(); Z ++) if(Coins[Z].Draw(Window, Players[ID].Get_X(), Players[ID].Get_Y(), SCREENSHOTS_MODE)) Coins.erase(Coins.begin() +Z), Z --, t ++, Heart +=(t %50 !=0 || Heart >=481 ? 0 : 120), Sound_1.play(), PointsRel +=(Mode ==0 || Mode ==2 || Mode ==4 ? 5 : 0);
            for(int Z =0; Z <Players.size(); Z ++) if(Z !=ID || Heart %2 ==0) Players[Z].Draw(Window, Players[ID].Get_X(), Players[ID].Get_Y(), Z ==ID ? (Heart >=120 ? Heart : 100) : 120, Z ==ID ? Attack : 0, Z !=ID ? Players[Z].Get_X() < Players[ID].Get_X() : -1);
            for(int Z =0; Z <Enemies.size(); Z ++) Enemies[Z].Draw(Window, Players[ID].Get_X(), Players[ID].Get_Y(), EnemyAnim.getElapsedTime().asMilliseconds() /250 %4);
            for(int Z =0; Z <5; Z ++) Items.setTextureRect(sf::IntRect(Heart /120 >Z ? 0 : 64, 0, 64, 64)), Items.setPosition(720 +Z *96, 32), Window->draw(Items);

            for(int Z =0; Z <PokeCount; Z ++) Items.setTextureRect(sf::IntRect(PokeCoins.size() <PokeCount -Z ? 192 : 256, 0, 64, 64)), Items.setPosition(1400 +Z *64, 32), Window->draw(Items); // BUG BIZARRE MULTIPLES SI POKEPIECE EN HAUT LEVEL 24 ET STD::COUT UTILISE AVANT !!!

        if(PointsRel >4294967295) PointsRel =4294967295;

        // SCORE AND ITEMS
        if(true) {
            Items.setTextureRect(sf::IntRect(128, 0, 64, 64));
            Items.setPosition(32, 32);
            Window->draw(Items);
            MiniScore.setColor(sf::Color(0, 0, 0, Items.getColor().a));

            MiniScore.setPosition(940 -MiniScore.getGlobalBounds().width /2, 1080 -24 -MiniScore.getGlobalBounds().height /2);
            unsigned long TotalPoints =PointsAbs +PointsRel;

                if(TotalPoints >4294967295) TotalPoints =4294967295;

            sf::String myScore =std::to_string(Mode ==0 ? PointsRel : TotalPoints);
            std::string add ="";
            Window->draw(MultiBG);

                while(std::to_string(Mode ==0 ? PointsRel : TotalPoints).size() +add.size() !=10) add +="0";

            Score.setColor(MiniScore.getColor());
            Score.setString(add +myScore);
            Window->draw(Score);

            Text.setString("x " +std::to_string(t));
            Text.setColor(Items.getColor());
            Window->draw(Text);

                if(Mode ==0) {
                MiniScore.setPosition(940 -MiniScore.getGlobalBounds().width /2, 1080 -24 -MiniScore.getGlobalBounds().height /2);
                std::string add2 ="";

                    while((add2 +std::to_string(TotalPoints)).size() !=10) add2 +="0";

                MiniScore.setString(add2 +std::to_string(TotalPoints));
                Window->draw(MiniScore);
                }
            }

            if(Session >0 && PlayerID >0) for(int Z =0; Z !=4; Z ++) Player::DrawXY(Window, Multi[Z *4 +2], Players[ID].Get_X(), Players[ID].Get_Y(), Multi[Z *4], Multi[Z *4 +1], Multi[Z *4 +3]);

            if(RecordTermine >=180) {
                for(int Z =0; Z !=7; Z ++) {
                Record.setPosition(std::min(RecordTermine *16 -3840 -Z *144, 480), Z *144 +60);
                Window->draw(Record);

                    if(Z ==3) MyRecord.setPosition(Record.getPosition()), Window->draw(MyRecord);

                RecordText.setString(List[Z *3] +" - " +List[Z *3 +1] +" (" +List[Z *3 +2] +")");
                RecordText.setPosition(std::min(RecordTermine *16 -3840 -Z *144 +480, 960) -RecordText.getGlobalBounds().width /2, Z *144 +60 +48 -RecordText.getCharacterSize() /2);

                    if(List[Z *3 +2] !="") Window->draw(RecordText);

                }
            }

            else if(AffichagePodium >0) {
            Podium.setPosition(AffichagePodium -1920, 0);
            Window->draw(Podium);
            }

            if(Players[ID].Get_Y() >=3968 && Heart >=120) Music.stop(), Dead.play(), Heart =119;

            if(End ==0) {
                if(FireworkDelay %8 ==0) Firework_1.setTextureRect(sf::IntRect(int(FireworkDelay /8) *256, 0, 256, 256)), Firework_2.setTextureRect(Firework_1.getTextureRect()), Firework_3.setTextureRect(Firework_1.getTextureRect()), Firework_4.setTextureRect(Firework_1.getTextureRect());

                if(FireworkDelay >=30) FireworkDelay =0, Firework_1.setPosition(rand() %(1920 -256), rand() %(1080 -256)), Firework_2.setPosition(rand() %(1920 -256), rand() %(1080 -256)), Firework_3.setPosition(rand() %(1920 -256), rand() %(1080 -256)), Firework_4.setPosition(rand() %(1920 -256), rand() %(1080 -256));
                else FireworkDelay ++;

            Window->draw(Firework_1);
            Window->draw(Firework_2);
            Window->draw(Firework_3);
            Window->draw(Firework_4);
            }

            if(PauseState ==0) sens =-1, Transition =0;

            if(PauseState ==1 && Pause.getScale().x ==1.0) PauseState =2;
            if(PauseState ==1) {
            float realSize =Pause.getScale().x *960;
            Pause.setScale((realSize +32) /960.0, (realSize +32) /960.0);
            Pause.setPosition(960 -480.0 *Pause.getScale().x, 540.0 -270.0 *Pause.getScale().x);
            }

            if(PauseState ==3 && Pause.getScale().x ==0.0) PauseState =0;
            if(PauseState ==3) {
            float realSize =Pause.getScale().x *960;
            Pause.setScale((realSize -32) /960.0, (realSize -32) /960.0);
            Pause.setPosition(960 -480.0 *Pause.getScale().x, 540.0 -270.0 *Pause.getScale().x);
            }

            if(Transition ==-2 && sens ==-1) Transition =0;

            if(sens ==1 && Transition !=270) Transition +=16;
            if(sens ==-1 && Transition !=0) Transition -=16;

        Window->draw(Pause);

        PauseChoice.setScale(Pause.getScale());
        PauseChoice.setPosition(960 -240.0 *Pause.getScale().x, 540 -67.5 *Pause.getScale().x +(Transition -135) *PauseChoice.getScale().x);
        Window->draw(PauseChoice);

        TextMenu.setString(Language ? "Continuer" : "Continue");
        TextMenu.setScale(Pause.getScale());
        TextMenu.setPosition(960 -TextMenu.getGlobalBounds().width /2, 540 -180 *TextMenu.getScale().x);
        Window->draw(TextMenu);

        TextMenu.setString(Language ? "Quitter" : "Exit");
        TextMenu.setPosition(960 -TextMenu.getGlobalBounds().width /2, 540 +95 *TextMenu.getScale().x);
        Window->draw(TextMenu);

            if(Shape_1.getRadius() && Shape_1.getRadius() && Shape_1.getRadius() !=1104) Shape_1.setRadius(Shape_1.getRadius() +((int)Shape_1.getRadius() %2 ? -16 : 16)), Shape_1.setPosition(960 -Shape_1.getRadius(), 540 -Shape_1.getRadius()), Window->draw(Shape_1);

        Window->draw(Shape_2);
        Window->display();

        }

    // END OF FUNCTION

    unsigned long TotalPoints =PointsAbs +PointsRel;

        if(TotalPoints >4294967295) TotalPoints =4294967295;

        if(!isRecordSended && !valueExit && Window->isOpen()) {
        std::thread ThreadRecord(SendRecords, LevelID, PointsRel, Mode, Window->isOpen() && Heart >=120 ? 1 : -1, List, (unsigned int)std::stoul(ReadSave(0, 8, false)), ReadUsername());
        ThreadRecord.detach();
        }

    c =false;

        while(networkIsRunning || networkIsRunning2) sf::sleep(sf::milliseconds(125));

        if(valueExit ==true) return 0;

        if(Window->isOpen()) {
            if(Mode ==0) WriteSave(28, 8, (unsigned int)TotalPoints);
            else (*Scoring) =(unsigned int)TotalPoints;
        }

        // 0 => Fermé || 1 => Jamais terminé || 2 => Fini avec drapeau trouvé || 3 => Fini avec drapeau trouvé, toutes les PC
        if(Window->isOpen() && Heart >=120 && Mode ==0) {
            if(!End && PokeCoins.size() ==0) return 3;
            else if(!End) return 2;
            else return 1;
        }

    return Window->isOpen() && Heart >=120;
    }
