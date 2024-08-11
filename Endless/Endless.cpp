
#include <cmath>
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Save.hpp"
#include "Endless.hpp"
#include "Play.hpp"

extern std::string Path;

    static int GetDecimal(char Hexa) {
        return Hexa >='0' && Hexa <='9' ? Hexa -'0' : Hexa >='A' && Hexa <='F' ? Hexa -'A' +10 : 0;
    }

    std::string WriteScore(unsigned int Score, int Count =8) {
    std::string Final =std::to_string(Score);

        while(Final.size() <Count) Final ="0" +Final;

    return Final;
    }

    void GetPlay(bool *Start, int *Session, int *Players, int IDPlayer) {
        while(*Session !=-2) {
        sf::Http Http("http://www.pythony.dev");
        sf::Http::Request Request("/SuperPokemonRun/Application/GetStart.php?Session=" +std::to_string(*Session) +"&Player=" +std::to_string(IDPlayer));
        sf::Http::Response Response =Http.sendRequest(Request);

            if(Response.getStatus() ==sf::Http::Response::Ok && Response.getBody()[0] =='1') {*Start =true; return ; }
            else if(Response.getStatus() ==sf::Http::Response::Ok && Response.getBody()[0] =='0' && Response.getBody()[1] >'1' && Response.getBody()[1] <'5') (*Players) =int(Response.getBody()[1] -'0');

        sf::sleep(sf::seconds(1));
        }
    }

    void CreateSession(bool *Start, int *Session, int *Players, int Level, bool Random) {
    sf::Http Http("http://www.pythony.dev");
    sf::Http::Request Request("/SuperPokemonRun/Application/CreateSession.php?Level=" +std::to_string(Level) +(Random ? "&Random=Random" : ""));
    sf::Http::Response Response =Http.sendRequest(Request);

        if(Response.getStatus() ==sf::Http::Response::Ok && Response.getBody() !="0") *Session =std::stoi(Response.getBody());

    GetPlay(Start, Session, Players, 1);
    }

    void StartSession(int Session) {
    sf::Http Http("http://www.pythony.dev");
    sf::Http::Request Request("/SuperPokemonRun/Application/StartSession.php?Session=" +std::to_string(Session));
    Http.sendRequest(Request);
    }

    void RandomSession(bool *Start, int *Session, int *Players, int *PlayerID, int Level) {
    sf::Http Http("http://www.pythony.dev");
    sf::Http::Request Request("/SuperPokemonRun/Application/CheckRandom.php?Level=" +std::to_string(Level));
    sf::Http::Response Response =Http.sendRequest(Request);

        if(Response.getStatus() ==sf::Http::Response::Ok) {
        *Session =std::stoi(Response.getBody());
        *PlayerID =Response.getBody()[Response.getBody().size() -1] -'0';
        *Players =*PlayerID;
        GetPlay(Start, Session, Players, *PlayerID);
        }
    }

    void CreatePlayer(bool *Start, int *Session, int *Players, int *PlayerID) {
    sf::Http Http("http://www.pythony.dev");
    sf::Http::Request Request("/SuperPokemonRun/Application/CreatePlayer.php?Session=" +std::to_string(*Session));
    sf::Http::Response Response =Http.sendRequest(Request);

        if(Response.getStatus() ==sf::Http::Response::Ok && std::atoi(Response.getBody().c_str()) >1 && std::atoi(Response.getBody().c_str()) <5) {
        *PlayerID =std::atoi(Response.getBody().c_str());
        *Players =*PlayerID;
        GetPlay(Start, Session, Players, *PlayerID);
        }
    }

    int randLevel(int categorie, int last) {
    int level =last;
        // 0 - 1 - 2

        while(level ==last) {
            if(categorie ==0) level =rand() %20 +1; // 1 TO 20
            else if(categorie ==1) level =rand() %15 +21; // 21 TO 35
            else if(categorie ==2) level =rand() %5 +36; // 36 TO 40
        }

    return level;
    }

    void Endless(sf::RenderWindow *Window) {
    sf::Texture Texture[5];
    Texture[0].loadFromFile(Path +"Endless.jpg");
    Texture[1].loadFromFile(Path +"Choice_3.png");
    Texture[2].loadFromFile(Path +"Choice_4.png");
    Texture[3].loadFromFile(Path +"Item_0.png");

    sf::Sprite Background(Texture[0]), Choix_1(Texture[1]), Choix_2(Texture[2]), Item(Texture[3]);
    sf::Sprite Menu;
    Menu.move(0, 1080);

    sf::Font Font;
    Font.loadFromFile(Path +"Font_2.ttf");

    sf::Text Text("", Font, 64);
    Text.setColor(sf::Color::Black);

    sf::CircleShape Shape(1089, 60);
    Shape.setFillColor(sf::Color::Black);
    sf::RectangleShape Shape_2(sf::Vector2f(0, 0));
    Shape_2.setFillColor(sf::Color::Black);

    sf::Clock Clock_1, Clock_2, Clock_3;

    sf::Event Event;

    sf::Music Music;
    Music.openFromFile(Path +"Endless.ogg");
    Music.setLoop(true);

    sf::SoundBuffer SoundBuffer[7];
    SoundBuffer[0].loadFromFile(Path +"Switch_5.wav");
    SoundBuffer[1].loadFromFile(Path +"Switch_6.wav");
    SoundBuffer[2].loadFromFile(Path +"Switch_7.wav");
    SoundBuffer[3].loadFromFile(Path +"Switch_8.wav");
    SoundBuffer[4].loadFromFile(Path +"Start_3.wav");
    SoundBuffer[5].loadFromFile(Path +"Pipe_2.wav");
    SoundBuffer[6].loadFromFile(Path +"Error_2.wav");

    sf::Sound Switch_1(SoundBuffer[0]), Switch_2(SoundBuffer[1]), Switch_3(SoundBuffer[2]), Switch_4(SoundBuffer[3]), SStart(SoundBuffer[4]), Pipe(SoundBuffer[5]), Error(SoundBuffer[6]);

    int Choice =0, c2 =0;
    int Session =-2, Player =0;
    int s =0;
    int PlayerID =0;
    int decalage1 =128;
    int decalageSortie =0;
    int nextChoix =-1;
    int decalageMenu1 =368;
    int decalageMenu2 =0;
    int nextMenu =-1;

    unsigned int records[12] ={0};

        for(int Z =0; Z !=12; Z ++) {
        std::string Value =ReadSave(28 +Z *8 +8, 8);
        unsigned int Decimal =0;

            for(int y =0; Value.size(); y ++) Decimal +=GetDecimal(Value[0]) *std::pow(16, Value.size() -1), Value.erase(0, 1);

        records[Z] =Decimal;
        }

    bool Start =false;
    bool Language =(std::stoi(ReadSave(156, 1, false)) >>2) %2;

    Music.play();

        while(Window->isOpen() && Shape.getRadius() !=1104) {
            if(Shape.getRadius() ==1) Shape.setRadius(0);
            else if(Shape_2.getSize().x ==31) Shape_2.setSize(sf::Vector2f(0, 0));

            if(Window->pollEvent(Event)) {
                if(Event.type ==sf::Event::Closed) Window->close();
                else if(Event.type ==sf::Event::KeyPressed && !Shape.getRadius() && !Shape_2.getSize().x) {
                    if((Event.key.code ==sf::Keyboard::Escape || Event.key.code ==sf::Keyboard::BackSpace) && Menu.getPosition().y ==1080) Shape.setRadius(16), Pipe.play();

                    if(Menu.getPosition().y ==0 && c2 ==2 && decalageMenu1 ==368 && decalageMenu2 ==0) {
                    int news =s;

                        if(Event.key.code ==sf::Keyboard::Num1) news *=10, news +=1;
                        else if(Event.key.code ==sf::Keyboard::Num2) news *=10, news +=2;
                        else if(Event.key.code ==sf::Keyboard::Num3) news *=10, news +=3;
                        else if(Event.key.code ==sf::Keyboard::Num4) news *=10, news +=4;
                        else if(Event.key.code ==sf::Keyboard::Num5) news *=10, news +=5;
                        else if(Event.key.code ==sf::Keyboard::Num6) news *=10, news +=6;
                        else if(Event.key.code ==sf::Keyboard::Num7) news *=10, news +=7;
                        else if(Event.key.code ==sf::Keyboard::Num8) news *=10, news +=8;
                        else if(Event.key.code ==sf::Keyboard::Num9) news *=10, news +=9;
                        else if(Event.key.code ==sf::Keyboard::Num0) news *=10;

                        if(news <65536) s =news;
                    }

                    if((Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) && Menu.getPosition().y ==0 && c2 ==0 && decalageMenu1 ==368 && decalageMenu1 ==368 && decalageMenu2 ==0) Shape_2.setSize(sf::Vector2f(32, 18)), Music.stop(), SStart.play();

                    else if(Menu.getPosition().y ==0 && decalageMenu1 ==368 && decalageMenu2 ==0 && Session ==-2) {
                        // CREATE SESSION
                        if((Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) && c2 ==3) {
                        Player =1;
                        PlayerID =1;
                        Session =-1;
                        std::thread tCreateSession(CreateSession, &Start, &Session, &Player, randLevel(Choice /4, -1), false);
                        tCreateSession.detach();
                        }
                        //START SESSION
                        else if((Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) && c2 ==3) {
                        std::thread tStartSession(StartSession, Session);
                        tStartSession.detach();
                        }
                        // RANDOM SESSION
                        else if((Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) && c2 ==1) {
                        std::thread tRandSession(RandomSession, &Start, &Session, &Player, &PlayerID, randLevel(Choice /4, -1));
                        tRandSession.detach();
                        }
                        // CREATE PLAYER
                        else if((Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) && c2 ==2 && s !=0) {
                        Session =s;
                        std::thread tCreatePlayer(CreatePlayer, &Start, &Session, &Player, &PlayerID);
                        tCreatePlayer.detach();
                        Player =1;
                        }
                    }

                    if((Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) && Menu.getPosition().y ==1080 && decalage1 ==128 && decalageSortie ==0) Menu.move(0, -8), Switch_3.play();
                    else if((Event.key.code ==sf::Keyboard::Escape || Event.key.code ==sf::Keyboard::BackSpace) && Menu.getPosition().y ==0) Menu.move(0, -7), Switch_4.play();
                    else if(Menu.getPosition().y ==1080 && decalage1 ==128 && decalageSortie ==0) {
                        if(Event.key.code ==sf::Keyboard::Right && Choice %4 !=3) Clock_1.restart(), Switch_1.play(), nextChoix =Choice +1;
                        else if(Event.key.code ==sf::Keyboard::Left && Choice %4 !=0) Clock_1.restart(), Switch_1.play(), nextChoix =Choice -1;
                        else if(Event.key.code ==sf::Keyboard::Up && Choice <8) Clock_1.restart(), Switch_1.play(), nextChoix =Choice +4;
                        else if(Event.key.code ==sf::Keyboard::Down && Choice >3) Clock_1.restart(), Switch_1.play(), nextChoix =Choice -4;
                        else if(Clock_1.getElapsedTime().asMilliseconds() >500 && (Event.key.code ==sf::Keyboard::Right || Event.key.code ==sf::Keyboard::Left || Event.key.code ==sf::Keyboard::Up || Event.key.code ==sf::Keyboard::Down)) Error.play(), Clock_1.restart();
                    } else if(Menu.getPosition().y ==0 && decalageMenu1 ==368 && decalageMenu2 ==0) {
                        if(Event.key.code ==sf::Keyboard::Up && c2 !=0) Clock_2.restart(), Switch_2.play(), nextMenu =c2 -1;
                        else if(Event.key.code ==sf::Keyboard::Down && c2 !=3) Clock_2.restart(), Switch_2.play(), nextMenu =c2 +1;
                        else if(Clock_2.getElapsedTime().asMilliseconds() >500 && (Event.key.code ==sf::Keyboard::Up || Event.key.code ==sf::Keyboard::Down)) Error.play(), Clock_2.restart();
                    }
                }
            }

        if(Menu.getPosition().y ==1080 -15) Menu.move(0, 15), c2 =0, Session =-2, Player =0, s =0;
        else if(Menu.getPosition().y !=0 && Menu.getPosition().y !=1080) Menu.move(0, int(Menu.getPosition().y) %2 ==0 ? -16 : 16);

        if(Shape_2.getSize().x >=1920 && c2 ==0) {
        int level =randLevel(Choice /4, -1);
        int currentScore =0;

            while(Play(Window, level, Choice %4 +1, &currentScore)) level =randLevel(Choice /4, level);
            // START SOLO GAME

        Music.play();
        
            if(currentScore >records[Choice]) records[Choice] =currentScore, WriteSave(28 +Choice *8 +8, 8, records[Choice]);
        
        Shape_2.setSize(sf::Vector2f(1920 -1, 1080));
        Session =-2;
        Start =false;
        }

        if(Start && !Shape_2.getSize().x) Shape_2.setSize(sf::Vector2f(32, 18)), Music.stop(), SStart.play();

        if(Start && Shape_2.getSize().x >=1920) {
        int level =-1;
        int currentScore =0;
        Music.stop();
        bool continuer =true;

            do {
            level =0;

            sf::Http Http("http://www.pythony.dev");
            sf::Http::Request Request("/SuperPokemonRun/Application/GetLevel.php?Session=" +std::to_string(Session));
            sf::Http::Response Response =Http.sendRequest(Request);

                if(Response.getStatus() ==sf::Http::Response::Ok) {
                level =std::stoi(Response.getBody());
                int retour =Play(Window, level, Choice %4 +1, &currentScore, Session, PlayerID);

                    if(retour ==0) continuer =false;
                    else {
                    int NewLevel =randLevel(Choice /4, level);
                    sf::Http Http("http://www.pythony.dev");
                    sf::Http::Request Request("/SuperPokemonRun/Application/SwitchLevel?Session=" +std::to_string(Session) +"&Level=" +std::to_string(level) +"&NewLevel=" +std::to_string(NewLevel));
                    sf::Http::Response Response =Http.sendRequest(Request);
                    }
                } else continuer =false;
            } while(continuer);
            // START MULTIPLAYER GAME

        Shape_2.setSize(sf::Vector2f(1920 -1, 1080));
        Music.play();

            if(currentScore >records[Choice]) records[Choice] =currentScore, WriteSave(28 +Choice *8 +8, 8, records[Choice]);

        Session =-2;
        Start =false;
        s =0;
        }

            if(nextChoix !=-1 && decalageSortie !=128) decalageSortie +=8;
            if(nextChoix !=-1 && decalageSortie ==128) decalage1 =0, decalageSortie =0, Choice =nextChoix, nextChoix =-1;
            if(decalage1 !=128) decalage1 +=8;

            if(nextMenu !=-1 && decalageMenu2 !=368) decalageMenu2 +=16;
            if(nextMenu !=-1 && decalageMenu2 ==368) decalageMenu1 =0, decalageMenu2 =0, c2 =nextMenu, nextMenu =-1, Session =-2, Player =0, s =0;
            if(decalageMenu1 !=368) decalageMenu1 +=16;

        Window->clear(sf::Color::Black);
        Window->draw(Background);
        Choix_1.setScale(abs(float(decalage1 -decalageSortie)) /128, 1.0);
        Choix_1.setPosition(480 *(Choice %4) +120 +160 -int(160 *Choix_1.getScale().x), 805 -360 *int(Choice /4) +Menu.getPosition().y -1080);
        Window->draw(Choix_1);
        Choix_2.setScale(abs(float(decalageMenu2 -decalageMenu1)) /368, 1.0);
        Choix_2.setPosition(960 -Choix_2.getScale().x *480, Menu.getPosition().y +67 +c2 *270);
        Window->draw(Choix_2);

        Text.setCharacterSize(24);
        Text.setColor(sf::Color::Black);
        //Text.setString("Niveau 1\nFACILE\nRecord : 12345");

            if(!Language) Text.setString(Choice %4 ==0 ? "Stalking The\nEnemies - " : Choice %4 ==1 ? "Coin Hunt\n" : Choice %4 ==2 ? "Time Trial\n" : "Best Score\n");
            else Text.setString(Choice %4 ==0 ? "Traque Aux Ennemis\n" : Choice %4 ==1 ? "Chasse Aux Pieces\n" : Choice %4 ==2 ? "Contre La Montre\n" : "Meilleur Score\n");

            if(!Language) Text.setString(Text.getString() +(Choice <4 ? "Easy" : Choice <8 ? "Medium" : "Advanced"));
            else Text.setString(Text.getString() +(Choice <4 ? "Facile" : Choice <8 ? "Moyen" : "Difficile"));

        Text.setString(Text.getString() +"\n" +WriteScore((unsigned int)(records[Choice])));
        Text.setPosition((Choice %4) *480 +140 +64, 820 -360 *int(Choice /4) +Menu.getPosition().y -1080 +24);
        Text.setScale(abs(float(decalage1 -decalageSortie)) /128, 1.0);
        Window->draw(Text);

            for(int Z =0; Z !=12; Z ++) {
            Item.setTextureRect(sf::IntRect(int(Z /4) *192, 0, 192, 192));
            Item.setPosition((Z %4) *480 +140 -(Z ==Choice ? abs(decalage1 -decalageSortie) : 0), 800 -360 *int(Z /4) +Menu.getPosition().y -1080);
            Window->draw(Item);
            }

            if(Menu.getPosition().y !=1147) {
            Text.setCharacterSize(28);
    
                if(!Language) {
                    if(c2 ==0) Text.setString("Solo");
                    
                    else if(c2 ==1 && Session !=-2 && c2 ==1) Text.setString("Session " +WriteScore(Session, 5) +" with " +std::to_string(Player) +" player" +(Player >1 ? "s" : ""));
                    else if(c2 ==1) Text.setString("Random Multiplayer");

                    else if(c2 ==2 && (Session ==-2 || c2 !=2)) Text.setString("Join a session " +WriteScore(s, 5));
                    else if(c2 ==2 && Session ==-1) Text.setString("Login to the session " +WriteScore(Session, 5));
                    else if(c2 ==2 && Session >-1) Text.setString("Connected to the session " +WriteScore(Session, 5) +" with " +std::to_string(Player) +" player" +(Player >1 ? "s" : ""));
                    else if(c2 ==2) Text.setString("Join the session");
                    
                    else if(c2 ==3 && (Session ==-2 || c2 !=3)) Text.setString("Create a session");
                    else if(c2 ==3 && Session ==-1) Text.setString("Creation of the session in progress...");
                    else if(c2 ==3) Text.setString("Session " +WriteScore(Session, 5) +" with " +std::to_string(Player) +" player" +(Player >1 ? "s" : ""));
                } else {
                    if(c2 ==0) Text.setString("Solo");
                    
                    else if(c2 ==1 && Session !=-2 && c2 ==1) Text.setString("Session " +WriteScore(Session, 5) +" avec " +std::to_string(Player) +" joueur" +(Player >1 ? "s" : ""));
                    else if(c2 ==1) Text.setString("Multijoueur aleatoire");

                    else if(c2 ==2 && (Session ==-2 || c2 !=2)) Text.setString("Rejoindre une session " +WriteScore(s, 5));
                    else if(c2 ==2 && Session ==-1) Text.setString("Connexion a la session " +WriteScore(Session, 5));
                    else if(c2 ==2 && Session >-1) Text.setString("Connecte a la session " +WriteScore(Session, 5) +" avec " +std::to_string(Player) +" joueur" +(Player >1 ? "s" : ""));
                    else if(c2 ==2) Text.setString("Rejoindre la session");
                    
                    else if(c2 ==3 && (Session ==-2 || c2 !=3)) Text.setString("Creer une session");
                    else if(c2 ==3 && Session ==-1) Text.setString("Creation de la session en cours...");
                    else if(c2 ==3) Text.setString("Session " +WriteScore(Session, 5) +" avec " +std::to_string(Player) +" joueur" +(Player >1 ? "s" : ""));
                }

            Text.setScale(Choix_2.getScale());
            Text.setPosition(960 -Text.getGlobalBounds().width /2, Menu.getPosition().y +125 +c2 *270);
            Window->draw(Text);
            }

            for(int Z =0; Z !=4; Z ++) {
            Item.setTextureRect(sf::IntRect((Z +3) *192, 0, 192, 192));
            Item.setPosition(960 -96 -abs(decalageMenu1 -decalageMenu2), Menu.getPosition().y +50 +270 *Z);

                if(Z !=c2) Item.move(960 -96 -Item.getPosition().x, 0);

            Window->draw(Item);
            }

            if(Shape.getRadius() && Shape.getRadius() && Shape.getRadius() !=1104) Shape.setRadius(Shape.getRadius() +((int)Shape.getRadius() %2 ? -16 : 16)), Shape.setPosition(960 -Shape.getRadius(), 540 -Shape.getRadius()), Window->draw(Shape);

            else if(Shape_2.getSize().x) Shape_2.setSize(sf::Vector2f(Shape_2.getSize().x +(int(Shape_2.getSize().x) %2 ==0 ? 32 : -32), Shape_2.getSize().y +(int(Shape_2.getSize().x) %2 ==0 ? 18 : -18))), Shape_2.setPosition(960 -Shape_2.getSize().x /2, 540 -Shape_2.getSize().y /2), Window->draw(Shape_2);

        Window->display();
        }

    Session =-2, Player =0;
    }
