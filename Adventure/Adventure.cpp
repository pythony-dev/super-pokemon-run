
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Save.hpp"
#include "Adventure.hpp"
#include "Play.hpp"

extern std::string Path;

    std::wstring LevelName(int Level, bool Language) {
        if(!Language) {
            switch(Level) {
                case 0 :
                return L"A New Beginning";

                case 1 :
                return L"The Secret Of Bricks";

                case 2 :
                return L"Pokémon Under Water";

                case 3 :
                return L"The Great Jump";

                case 4 :
                return L"Warm Front";

                case 5 :
                return L"Danger Magma";

                case 6 :
                return L"Long Distance Attack";

                case 7 :
                return L"Obstacle Path";

                case 8 :
                return L"Through The Elements";

                case 9 :
                return L"Deadly Labyrinths";

                case 10 :
                return L"Block Destruction";

                case 11 :
                return L"Secret Passages";

                case 12 :
                return L"The East Loop Loop";

                case 13 :
                return L"But Where To Go ?";

                case 14 :
                return L"A Great Labyrinth";

                case 15 :
                return L"Swim Or Sink ?";

                case 16 :
                return L"Under The Sea";

                case 17 :
                return L"Sharp Pimples";

                case 18 :
                return L"Winning Changes";

                default :
                return L"Passage To Risks";

                /*case 20 :
                return L"A Dark Mansion";

                case 21 :
                return L"The Dark Corridor";

                case 22 :
                return L"The Dark Gate";

                case 23 :
                return L"The Three Passages";

                case 24 :
                return L"Lost In The Night";

                case 25 :
                return L"A Great Flight";

                case 26 :
                return L"Nocturnal Bathing";

                case 27 :
                return L"Scare Switch";

                case 28 :
                return L"Dark Detours";

                case 29 :
                return L"Labyrinth To Spirits";

               case 30 :
                return L"Ghost Blocks";

                case 31 :
                return L"Midnight Bath";

                case 32 :
                return L"Multiple Paths";

                case 33 :
                return L"Secondary Road";

                case 34 :
                return L"Bastion Bill Ball";

                case 35 :
                return L"Frozen Road";

                case 36 :
                return L"Experiencing Course";

                case 37 :
                return L"The Time Of A Block";

                case 38 :
                return L"Victory Road";

                default :
                return L"Last Jump";*/
            }
        } else {
            switch(Level) {
                case 0 :
                return L"Un Nouveau Départ";

                case 1 :
                return L"Le Secret Des Briques";

                case 2 :
                return L"Pokémon À L'Eau";

                case 3 :
                return L"Le Grand Saut";

                case 4 :
                return L"Chaud Devant";

                case 5 :
                return L"Danger Magma";

                case 6 :
                return L"Attaque Longue Distance";

                case 7 :
                return L"Chemin D'Obstacles";

                case 8 :
                return L"À Travers Les Elements";

                case 9 :
                return L"Labyrinthes Mortel";

                case 10 :
                return L"Destruction De Blocs";

                case 11 :
                return L"Passages Secerets";

                case 12 :
                return L"La Boucle Est Bouclée";

                case 13 :
                return L"Mais Où Aller ?";

                case 14 :
                return L"Un Grand Labyrinthe";

                case 15 :
                return L"Nager Ou Couler ?";

                case 16 :
                return L"Sous La Mer";

                case 17 :
                return L"Des Picots Piquants";

                case 18 :
                return L"Changements Gagnants";

                default :
                return L"Passage À Risques";

                /*case 20 :
                return L"Une Sombre Demeure";

                case 21 :
                return L"Le Couloir Obscure";

                case 22 :
                return L"La Porte Des Ténèbres";

                case 23 :
                return L"Les Trois Passages";

                case 24 :
                return L"Perdu Dans La Nuit";

                case 25 :
                return L"Un Grand Envol";

                case 26 :
                return L"Baignade Nocturne";

                case 27 :
                return L"Interrupteur Frayeur";

                case 28 :
                return L"Détours Sombres";

                case 29 :
                return L"Labyrinthe Aux Esprits";

                case 30 :
                return L"Blocs Fantômes";

                case 31 :
                return L"Bain De Minuit";

                case 32 :
                return L"Chemins Multiples";

                case 33 :
                return L"Chemin Secondaire";

                case 34 :
                return L"Bastion Bill Balle";

                case 35 :
                return L"Chemin Gelé";

                case 36 :
                return L"Parcours Éprouvant";

                case 37 :
                return L"Le Temps D'Un Bloc";

                case 38 :
                return L"Route Victoire";

                default :
                return L"Dernier Saut";*/
            }
        }
    }

    void Send(int User, int Level, int Status) {
    sf::Http Http("http://www.pythony.dev");
    sf::Http::Request Request("/SuperPokemonRun/Application/Finish.php?User=" +std::to_string(User) +"&Level=" +std::to_string(Level) +"&Status=" +std::to_string(Status));
    Http.sendRequest(Request);
    }

    void Adventure(sf::RenderWindow *Window) {
    sf::Clock Clock, clock2;

    sf::Event Event;

    sf::Font Font;
    Font.loadFromFile(Path +"Font_1.ttf");

    sf::Text Text("", Font, 32);
    Text.setColor(sf::Color::Black);

    sf::CircleShape Shape_1(1089, 60);
    Shape_1.setFillColor(sf::Color::Black);
    sf::RectangleShape Shape_2(sf::Vector2f(0, 0));
    Shape_2.setFillColor(sf::Color::Black);

    sf::Texture Texture[3];
    Texture[0].loadFromFile(Path +"Adventure.jpg");
    Texture[1].loadFromFile(Path +"Medal.png");
    Texture[2].loadFromFile(Path +"Choice_2.png");

    sf::Sprite Background(Texture[0]), Items(Texture[1]), Choice(Texture[2]);

    sf::Music Music;
    Music.openFromFile(Path +"Adventure.ogg");
    Music.setLoop(true);

    sf::SoundBuffer SoundBuffer[4];
    SoundBuffer[0].loadFromFile(Path +"Pipe_1.wav");
    SoundBuffer[1].loadFromFile(Path +"Switch_4.wav");
    SoundBuffer[2].loadFromFile(Path +"Error_1.wav");
    SoundBuffer[3].loadFromFile(Path +"Start_2.wav");

    sf::Sound Pipe(SoundBuffer[0]), Switch(SoundBuffer[1]), Close(SoundBuffer[2]), Start(SoundBuffer[3]);

    int Level[40] ={0}, sel =0, o =-1, t =0, Add =0;
    bool Language =(std::stoi(ReadSave(156, 1, false)) >>2) %2;
    bool AutoStart =false;

    std::string Content =ReadSave(0 +8, 20);

        for(int Z =0; Z !=20; Z ++) {
        int Current =Content[Z];
        Current =Current >='0' && Current <='9' ? Current -'0' : Current >='A' && Current <='F' ? Current -'A' +10 : 0;

        Level[Z *2 +1] =Current %4;
        Level[Z *2] =Current %16 /4;
        }

        if(Level[0] ==1) AutoStart =true, Shape_1.setRadius(0);

    Music.play();

        while(Window->isOpen() && Shape_1.getRadius() !=1104) {
            if(Window->pollEvent(Event) && Event.type ==sf::Event::Closed) Window->close();

            else if(Shape_2.getSize().x ==1920 || AutoStart) {
            Music.stop();

            int Status =Play(Window, Add !=15 ? sel +1 : 0);

            std::thread MyThread(Send, 1, Add !=15 ? sel +1 : 0, Status);
            MyThread.detach();
            Music.play();
            Shape_2.setSize(sf::Vector2f(1920 -1, 1080));

                if(Status >Level[sel] && Add !=15) {
                Level[sel] =Status;

                    if(sel %2 ==0) WriteSave(sel /2 +8, 1, Level[sel] *4 +Level[sel +1]);
                    else WriteSave(sel /2 +8, 1, Level[sel] +Level[sel -1] *4);

                }

                if(Status >1 && Level[sel +1] ==0 && Add !=15) {
                Level[sel +1] =1;

                    if(sel %2 ==0) WriteSave(sel /2 +8, 1, Level[sel] *4 +1);
                    else WriteSave(sel /2 +1 +8, 1, Level[sel +2] +4);
                }

            Add =0;

                if(AutoStart) {
                AutoStart =false;

                    if(Level[0] ==1) return;
                }
            }

            else if(Event.type ==sf::Event::KeyPressed && !Shape_1.getRadius() && !Shape_2.getSize().x) {
                if(Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) Shape_2.setSize(sf::Vector2f(32, 18)), Music.stop(), Start.play();
                else if(Event.key.code ==sf::Keyboard::Escape || Event.key.code ==sf::Keyboard::BackSpace) Shape_1.setRadius(16), Pipe.play();
                else if(Event.key.code ==sf::Keyboard::Right && sel <15 && Level[sel +5] !=0 && o ==-1) Switch.play(), o =sel +5;
                else if(Event.key.code ==sf::Keyboard::Right && sel <15 && Level[sel +5] ==0 && o ==-1 && clock2.getElapsedTime().asMilliseconds() >500) Close.play(), clock2.restart();
                else if(Event.key.code ==sf::Keyboard::Left && sel >4 && Level[sel -5] ==0 && o ==-1 && clock2.getElapsedTime().asMilliseconds() >500) Close.play(), clock2.restart();
                else if(Event.key.code ==sf::Keyboard::Left && sel >4 && Level[sel -5] !=0 && o ==-1) Switch.play(), o =sel -5;
                else if(Event.key.code ==sf::Keyboard::Up && sel %5 <4 && Level[sel +1] !=0 && o ==-1) Switch.play(), o =sel +1;
                else if(Event.key.code ==sf::Keyboard::Up && sel %5 <4 && Level[sel +1] ==0 && o ==-1 && clock2.getElapsedTime().asMilliseconds() >500) Close.play(), clock2.restart();
                else if(Event.key.code ==sf::Keyboard::Down && sel %5 >0 && Level[sel -1] !=0 && o ==-1) Switch.play(), o =sel -1;
                else if(Event.key.code ==sf::Keyboard::Down && sel %5 >0 && Level[sel -1] ==0 && o ==-1 && clock2.getElapsedTime().asMilliseconds() >500) Close.play(), clock2.restart();

                else if(clock2.getElapsedTime().asMilliseconds() >500 && o ==-1) {
                    if(Event.key.code ==sf::Keyboard::Right && sel >=35) Close.play(), clock2.restart();
                    else if(Event.key.code ==sf::Keyboard::Left && sel <=4) Close.play(), clock2.restart();
                    else if(Event.key.code ==sf::Keyboard::Up && sel %5 ==4 && clock2.getElapsedTime().asMilliseconds() >500) Close.play(), clock2.restart();
                    else if(Event.key.code ==sf::Keyboard::Down && sel %5 ==0 && clock2.getElapsedTime().asMilliseconds() >500) Close.play(), clock2.restart();
                }

                if(Add ==0 && Event.key.code ==sf::Keyboard::S) Add ++;
                else if(Add ==1 && Event.key.code ==sf::Keyboard::U) Add ++;
                else if(Add ==2 && Event.key.code ==sf::Keyboard::P) Add ++;
                else if(Add ==3 && Event.key.code ==sf::Keyboard::E) Add ++;
                else if(Add ==4 && Event.key.code ==sf::Keyboard::R) Add ++;
                else if(Add ==5 && Event.key.code ==sf::Keyboard::P) Add ++;
                else if(Add ==6 && Event.key.code ==sf::Keyboard::O) Add ++;
                else if(Add ==7 && Event.key.code ==sf::Keyboard::K) Add ++;
                else if(Add ==8 && Event.key.code ==sf::Keyboard::E) Add ++;
                else if(Add ==9 && Event.key.code ==sf::Keyboard::M) Add ++;
                else if(Add ==10 && Event.key.code ==sf::Keyboard::O) Add ++;
                else if(Add ==11 && Event.key.code ==sf::Keyboard::N) Add ++;
                else if(Add ==12 && Event.key.code ==sf::Keyboard::R) Add ++;
                else if(Add ==13 && Event.key.code ==sf::Keyboard::U) Add ++;
                //else if(Add ==14 && Event.key.code ==sf::Keyboard::N) Add ++, Shape_2.setSize(sf::Vector2f(32, 18)), Music.stop(), Start.play(); // ONLY FOR 1.0 VERSION

            }

            else if(Shape_1.getRadius() ==1) Shape_1.setRadius(0);
            else if(Shape_2.getSize().x ==31) Shape_2.setSize(sf::Vector2f(0, 0));

        Window->clear(sf::Color::Black);
        Window->draw(Background);

        Text.setString(LevelName(sel, Language));
        Text.setCharacterSize(32);
        Text.setPosition(960 -Text.getGlobalBounds().width /2, 16);
        Window->draw(Text);
        
            if(!Language) Text.setString(Level[sel] ==1 ? L"UNFINISHED" : Level[sel] ==2 ? L"FINISHED" : L"PERFECT");
            else Text.setString(Level[sel] ==1 ? L"NON TERMINÉ" : Level[sel] ==2 ? L"TERMINÉ" : L"PARFAIT");

        Text.setCharacterSize(24);
        Text.setPosition(960 -Text.getGlobalBounds().width /2, 80);
        Window->draw(Text);

        Choice.setPosition(int(sel /5) *(88 +108) +int(sel /10) *88 +88 -27, sel /5 <3 || sel /5 >4 ? 864 -sel %5 *216 +27 : 761.6 -sel %5 *(108 +82.4) +128 +82.4 /2 -27);

            if(t ==15) sel =o;
            if(t ==30) t =0, o =-1;
            else if(o !=-1 && t !=30) {
            t ++;
            Choice.setScale((double)abs(t -15) /15, (double)abs(t -15) /15);
            Choice.move(81 -81 *Choice.getScale().x, 81 -81 *Choice.getScale().x);
            } else Choice.setScale(1, 1);

        Window->draw(Choice);

            for(int Y =0; Y <5; Y ++) {
                for(int X =0; X <4; X ++) {
                Items.setTextureRect(sf::IntRect(Level[X *5 -Y +4] *108 -108, X *5 -Y +4 !=sel ? 0 : int(Clock.getElapsedTime().asMilliseconds() %500 /125) *108, 108, 108));
                Items.setPosition(X *(88 +108) +int(X /2) *88 +88,
                X <3 || X >4 ? Y *216 +54 : Y *(108 +82.4) +128 +82.4 /2);
                Window->draw(Items);
                }
            }

            if(Shape_1.getRadius() && Shape_1.getRadius() !=1104) Shape_1.setRadius(Shape_1.getRadius() +((int)Shape_1.getRadius() %2 ? -16 : 16)), Shape_1.setPosition(960 -Shape_1.getRadius(), 540 -Shape_1.getRadius()), Window->draw(Shape_1);
            else if(Shape_2.getSize().x) Shape_2.setSize(sf::Vector2f(Shape_2.getSize().x +(int(Shape_2.getSize().x) %2 ==0 ? 32 : -32), Shape_2.getSize().y +(int(Shape_2.getSize().x) %2 ==0 ? 18 : -18))), Shape_2.setPosition(960 -Shape_2.getSize().x /2, 540 -Shape_2.getSize().y /2), Window->draw(Shape_2);

        Window->display();
        }
    }
