
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Save.hpp"
#include "Settings.hpp"

extern std::string Path;

    bool Settings(sf::RenderWindow* Window) {
    sf::Event Event;

    sf::RectangleShape Shape_1(sf::Vector2f(1920, 1080));
    Shape_1.setFillColor(sf::Color(0, 0, 0, 0));

    sf::Texture Texture[4];
    Texture[0].loadFromFile(Path +"Settings.jpg");
    Texture[1].loadFromFile(Path +"Choice_5.png");
    Texture[2].loadFromFile(Path +"Item.png");
    Texture[3].loadFromFile(Path +"Delete.png");

    sf::Sprite Background(Texture[0]), Choice(Texture[1]), Item(Texture[2]), Delete(Texture[3]);
    Choice.setPosition(500 +420 -23, 57);

    sf::CircleShape Shape(1089, 60);
    Shape.setFillColor(sf::Color::Black);

    sf::Font Font;
    Font.loadFromFile(Path +"Font_3.ttf");

    sf::Text Text("Hello", Font, 64), Text_2("", Font, 64);
    Text.setColor(sf::Color::Black);
    Text_2.setColor(sf::Color::Black);

    std::string Pseudo =ReadUsername();

    sf::Clock Clock;

    int CDelete =0;

    bool Choix[2] ={false, false};
    bool KeyEnter =false;
    bool Language =(std::stoi(ReadSave(156, 1, false)) >>2) %2;

    int ChoixSauvegarde =std::stoi(ReadSave(156, 1, false));

    Choix[0] =(ChoixSauvegarde >>3) %2;
    Choix[1] =(ChoixSauvegarde >>2) %2;

    sf::Music Music;
    Music.openFromFile(Path +"Settings.ogg");
    Music.setLoop(true);

    sf::SoundBuffer SoundBuffer[7];
    SoundBuffer[0].loadFromFile(Path +"Keyboard.wav");
    SoundBuffer[1].loadFromFile(Path +"Switch_9.wav");
    SoundBuffer[2].loadFromFile(Path +"Switch_10.wav");
    SoundBuffer[3].loadFromFile(Path +"Error_3.wav");
    SoundBuffer[4].loadFromFile(Path +"Pipe_3.wav");
    SoundBuffer[5].loadFromFile(Path +"Warning.wav");
    SoundBuffer[6].loadFromFile(Path +"Erase.wav");

    sf::Sound Keyboard(SoundBuffer[0]), Switch_1(SoundBuffer[1]), Switch_2(SoundBuffer[2]), Error(SoundBuffer[3]), Pipe(SoundBuffer[4]), Warning(SoundBuffer[5]), Erase(SoundBuffer[6]);

    Music.play();

        while(Window->isOpen() && Shape.getRadius() !=1104) {
            if(Window->pollEvent(Event) && Event.type ==sf::Event::Closed) Window->close();

            else if(!Shape.getRadius()) {
                if(Event.type ==sf::Event::TextEntered && Pseudo.size() !=16 && !KeyEnter && ((Event.text.unicode >='A' && Event.text.unicode <='Z') || (Event.text.unicode >='a' && Event.text.unicode <='z') || (Event.text.unicode >='0' && Event.text.unicode <='9'))) Pseudo +=Event.text.unicode, Clock.restart(), KeyEnter =true, Keyboard.play();
                if(Event.type ==sf::Event::TextEntered && Pseudo.size() ==16 && !KeyEnter && ((Event.text.unicode >='A' && Event.text.unicode <='Z') || (Event.text.unicode >='a' && Event.text.unicode <='z') || (Event.text.unicode >='0' && Event.text.unicode <='9'))) Error.play(), KeyEnter =true;
                else if(Event.type ==sf::Event::KeyReleased) KeyEnter =false;

                else if(Event.type ==sf::Event::KeyPressed && !Shape_1.getFillColor().a) {
                    if(Choice.getPosition().y ==57 && Event.key.code ==sf::Keyboard::BackSpace && Pseudo.size() >0) Pseudo.pop_back(), Keyboard.play();
                    else if(Choice.getPosition().y ==57 && Event.key.code ==sf::Keyboard::BackSpace && !KeyEnter) Error.play(), KeyEnter =true;
                    else if((Event.key.code ==sf::Keyboard::Escape || Event.key.code ==sf::Keyboard::BackSpace) && Pseudo.size() ==0 && !KeyEnter) Error.play(), KeyEnter =true;
                    else if((Event.key.code ==sf::Keyboard::Escape || (Event.key.code ==sf::Keyboard::BackSpace) || CDelete ==90) && Pseudo.size() >0) Shape.setRadius(16), Music.stop(), Pipe.play();

                    else if(Event.key.code ==sf::Keyboard::Down && int(Choice.getPosition().y) %270 ==57 && Choice.getPosition().y !=867) Choice.move(0, 14), Switch_1.play();
                    else if(Event.key.code ==sf::Keyboard::Up && int(Choice.getPosition().y) %270 ==57 && Choice.getPosition().y !=57) Choice.move(0, -13), Switch_1.play();
                    else if((Event.key.code ==sf::Keyboard::Up && Choice.getPosition().y ==57 && !KeyEnter) || (Event.key.code ==sf::Keyboard::Down && Choice.getPosition().y ==867 && !KeyEnter)) Error.play(), KeyEnter =true;

                    else if(Event.key.code ==sf::Keyboard::Right && (Choice.getPosition().y ==327 || Choice.getPosition().y ==597) && !Choix[int(Choice.getPosition().y /270) -1]) Choix[int(Choice.getPosition().y /270) -1] =true, sf::Listener::setGlobalVolume(Choice.getPosition().y ==327 ? 3 : sf::Listener::getGlobalVolume()), Shape_1.setFillColor(sf::Color(0, 0, 0, Choice.getPosition().y ==597 ? 2 : 0)), Switch_2.play(), KeyEnter =true;
                    else if(Event.key.code ==sf::Keyboard::Left && (Choice.getPosition().y ==327 || Choice.getPosition().y ==597) && Choix[int(Choice.getPosition().y /270) -1]) Choix[int(Choice.getPosition().y /270) -1] =false, sf::Listener::setGlobalVolume(Choice.getPosition().y ==327 ? 96 : sf::Listener::getGlobalVolume()), Shape_1.setFillColor(sf::Color(0, 0, 0, Choice.getPosition().y ==597 ? 2 : 0)), Switch_2.play(), KeyEnter =true;
                    else if((Choice.getPosition().y ==327 || Choice.getPosition().y ==597) && !Choix[int(Choice.getPosition().y /270) -1] && Event.key.code ==sf::Keyboard::Left && !KeyEnter) Error.play(), KeyEnter =true;
                    else if((Choice.getPosition().y ==327 || Choice.getPosition().y ==597) && Choix[int(Choice.getPosition().y /270) -1] && Event.key.code ==sf::Keyboard::Right && !KeyEnter) Error.play(), KeyEnter =true;
                    // Pas possible en haut a gauche car son couper => Supprimer la condition

                    else if((Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) && Choice.getPosition().y ==867 && (CDelete ==0 || (CDelete ==60 && !KeyEnter))) CDelete ++, KeyEnter =true;
                }
            }

            if(sf::Listener::getGlobalVolume() ==99) sf::Listener::setGlobalVolume(100);
            else if(sf::Listener::getGlobalVolume() !=0 && sf::Listener::getGlobalVolume() !=100) sf::Listener::setGlobalVolume(sf::Listener::getGlobalVolume() +(int(sf::Listener::getGlobalVolume()) %2 ? 4 : -4));

            if(Shape_1.getFillColor().a ==254) Shape_1.setFillColor(sf::Color(0, 0, 0, 255)), Language =!Language;
            else if(Shape_1.getFillColor().a ==3) Shape_1.setFillColor(sf::Color(0, 0, 0, 0));
            else if(Shape_1.getFillColor().a) Shape_1.setFillColor(sf::Color(0, 0, 0, Shape_1.getFillColor().a +(Shape_1.getFillColor().a %2 ? -12 : 12)));

            if(CDelete ==1) Warning.play();
            else if(CDelete ==61) Erase.play();
            else if(CDelete ==90 && !Shape.getRadius()) Shape.setRadius(16);

            if(int(Choice.getPosition().y) %270 ==58) Choice.move(0, -1);
            else if(int(Choice.getPosition().y) %270 !=57) Choice.move(0, int(Choice.getPosition().y) %2 ==1 ? 16 : -16);

            if(Shape.getRadius() ==1) Shape.setRadius(0);

        Window->clear(sf::Color::Black);
        Window->draw(Background);

            for(int Z =0; Z !=4; Z ++) {

                if(Z ==0) Text.setString(Pseudo);
                else if(Z ==1) Text.setString(!Language ? L"Mute" : L"Muet");
                else if(Z ==2) Text.setString(!Language ? L"English" : L"Anglais");
                else if(Z ==3 && CDelete <30) Text.setString(!Language ? L"Delete Backup" : L"Supprimer La Sauvegarde");
                else if(Z ==3) Text.setString(!Language ? L"Are You Sure ?" : L"Êtes-Vous Sûr ?");

            Text.setPosition(Z ==0 || Z ==3 ? 940 -Text.getGlobalBounds().width /2 +420 : 1378 -64 -Text.getGlobalBounds().width, 92 +Z *270);
            Window->draw(Text);

                if(Z ==1 || Z ==2) {

                    if(Z ==1) Text.setString(!Language ? L"Sound" : L"Son");
                    else Text.setString(!Language ? L"French" : L"Français");

                Text.setPosition(1378 +64, Text.getPosition().y), Window->draw(Text);
                }
            }

            for(int Z =0; Z !=3; Z ++) {

                if(Language) Text_2.setString(Z ==0 ? L"Entrez Un Pseudo :" : Z ==1 ? L"Sons Et Musiques :" : Z ==2 ? L"Langue Utilisée :" : L"Supprimer La Sauvegarde :");
                else Text_2.setString(Z ==0 ? L"Enter A Username :" : Z ==1 ? L"Sounds And Musics :" : Z ==2 ? L"Language Used :" : L"Delete Backup :");

            Text_2.setPosition(840 -Text_2.getGlobalBounds().width, 96 +270 *Z);
            Window->draw(Text_2);
            }

            for(int Z =0; Z !=2; Z ++) Item.setPosition(475 +(Choix[Z] *480) +420, 270 +Z *269 +57 -2), Window->draw(Item);

            if(CDelete %60 !=0 && CDelete %60 <30) Delete.setTextureRect(sf::IntRect(int(CDelete %60 /7.5) *64, 0, 64, 64)), CDelete ++;
            else if(CDelete %60 !=0) Delete.setTextureRect(sf::IntRect(448 -int(CDelete %60 /7.5) *64, 0, 64, 64)), CDelete +=(CDelete !=90 ? 1 : 0);

            if(CDelete %60 !=0 || CDelete ==90) for(int Z =2; Z !=13; Z ++) Delete.setPosition(480 +Z *64 +420, 867 +32 +12 ), Window->draw(Delete);

        Window->draw(Choice);
        Window->draw(Shape_1);

            if(Shape.getRadius() && Shape.getRadius() && Shape.getRadius() !=1104) Shape.setRadius(Shape.getRadius() +((int)Shape.getRadius() %2 ? -16 : 16)), Shape.setPosition(960 -Shape.getRadius(), 540 -Shape.getRadius()), Window->draw(Shape);

        Window->display();
        }

        if(CDelete ==90) {
        RemoveSave(false);

        return true;
        }

        if(Window->isOpen()) {
            for(int Z =0; Z !=16; Z ++) {
            int Save =0;
            int LastFrame =((std::stoi(ReadSave(124 +(Z *1.5) +8, 1, false)) /4)) ;

                if(Pseudo.size() >Z && Pseudo[Z] >='A' && Pseudo[Z] <='Z') Save +=Pseudo[Z] -'A' +1;
                else if(Pseudo.size() >Z && Pseudo[Z] >='a' && Pseudo[Z] <='z') Save +=Pseudo[Z] -'a' +26 +1;
                else if(Pseudo.size() >Z && Pseudo[Z] >='0' && Pseudo[Z] <='9') Save +=Pseudo[Z] -'0' +52 +1;

                // XXXX XX00 0000 XXXX XX00 0000 XXXX XX00 0000 XXXX XX00 0000
                if(Z %2 ==0) WriteSave(132 +int(Z *1.5), 2, Save <<2); // XXXX XX00
                if(Z %2 ==1) WriteSave(132 +int(Z *1.5), 2, LastFrame *64 +Save); // 00XX XXXX
            }

        WriteSave(156, 1, (Choix[0] ? 8 : 0) +(Choix[1] ? 4 : 0) +2);
        }

    return false;
    }
