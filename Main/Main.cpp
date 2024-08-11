
#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Save.hpp"
#include "Adventure.hpp"
#include "Endless.hpp"
#include "Settings.hpp"

#define VERSION ("Alpha_MacOS")

std::string Path ="";

    void Start() {
    unsigned int UserID =(unsigned int)std::stoul(ReadSave(0, 8, false));

    sf::Http Http("http://www.pythony.dev");
    sf::Http::Request Request("/SuperPokemonRun/Application/Start.php?UserID=" +std::to_string(UserID) +"&Version=" +VERSION);
    sf::Http::Response Response =Http.sendRequest(Request);
    }




    bool Intro(sf::RenderWindow *Window) {
    sf::RectangleShape Shape(sf::Vector2f(256, 256));
    Shape.setPosition(576 +512, 412);
    Shape.setFillColor(sf::Color(0, 0, 0, 191));

    sf::Texture Texture;
    Texture.loadFromFile(Path +"Background.jpg");

    sf::Sprite Sprite(Texture);

    sf::Event Event;

    int Launch =-63;
    bool Choice =false;

        while(Window->isOpen() && Launch !=64) {
            if(Window->pollEvent(Event) && Event.type ==sf::Event::Closed) Window->close();
            else if(Event.type ==sf::Event::KeyPressed && Event.key.code ==sf::Keyboard::Right && !Choice) Choice =true, Shape.move(-512, 0);
            else if(Event.type ==sf::Event::KeyPressed && Event.key.code ==sf::Keyboard::Left && Choice) Choice =false, Shape.move(512, 0);
            else if(Event.type ==sf::Event::KeyPressed && (Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return)) Launch ++;

        Window->clear(sf::Color::Black);
        Sprite.setColor(sf::Color(255, 255, 255, 255 -abs(Launch) *4));
        Window->draw(Sprite);
        Window->draw(Shape);
        Window->display();

            if(Launch !=0) Launch ++;
        }

        if(Window->isOpen()) CreateSave(Choice);

    return Window->isOpen();
    }




    int main(int argc, char *argv[]) {
    srand(int(time(NULL)));

    Path =((std::string)argv[0]).substr(0, ((std::string)argv[0]).find_last_of('/')) +"/../Resources/";

    sf::RenderWindow Window(sf::VideoMode(1920, 1080), "Super Pokemon Run");
    Window.setVerticalSyncEnabled(true);
    Window.setFramerateLimit(60);

    int SaveExist =ExistSave();

        if(!SaveExist && !Intro(&Window)) return EXIT_SUCCESS;

    std::thread ThreadStart(Start);
    ThreadStart.detach();

    sf::Event Event;

    sf::Font Font;
    Font.loadFromFile(Path +"Font_0.ttf");

    sf::Text Text("", Font, 96);
    Text.setColor(sf::Color::Black);

    sf::RectangleShape Shape_1(sf::Vector2f(1920, 1080));
    sf::CircleShape Shape(0, 60);
    Shape.setFillColor(sf::Color::Black);

    sf::Texture Texture[4];
    Texture[0].loadFromFile(Path +"Main_1.jpg");
    Texture[1].loadFromFile(Path +"Main_2.jpg");
    Texture[2].loadFromFile(Path +"Menu_1.png");
    Texture[3].loadFromFile(Path +"Choice_1.png");

    sf::Sprite Background(Texture[0]), BackgroundF(Texture[1]), Menu(Texture[2]), Choice(Texture[3]);
    Menu.setPosition(480, 1080);
    Choice.setPosition(480, 1080);

    sf::Music Music;
    Music.setLoop(true);
    Music.openFromFile(Path +"Main.ogg");

    sf::SoundBuffer SoundBuffer[4];
    SoundBuffer[0].loadFromFile(Path +"Switch_1.wav");
    SoundBuffer[1].loadFromFile(Path +"Switch_2.wav");
    SoundBuffer[2].loadFromFile(Path +"Switch_3.wav");
    SoundBuffer[3].loadFromFile(Path +"Start_1.wav");

    sf::Sound Switch_1(SoundBuffer[0]), Switch_2(SoundBuffer[1]), Switch_3(SoundBuffer[2]), Start(SoundBuffer[3]);

    int Launch =64;
    bool Language =(std::stoi(ReadSave(156, 1, false)) >>2) %2;

        if((std::stoi(ReadSave(156, 1, false)) >>3) %2 ==0) sf::Listener::setGlobalVolume(0);

    Music.play();

        while(Window.isOpen()) {
            if(Window.pollEvent(Event) && Event.type ==sf::Event::Closed) Window.close();

            //else if(!Window.hasFocus()) sf::sleep(sf::seconds(1)); // ONLY FOR DEBUG !!!

            else if(Shape.getRadius() ==1104) {
                if(Choice.getPosition().y ==108) Music.stop(), Adventure(&Window), Shape.setRadius(1089), Music.play();
                if(Choice.getPosition().y ==432) Music.stop(), Endless(&Window), Shape.setRadius(1089), Music.play();
                if(Choice.getPosition().y ==756) {
                bool Reset =false;
                Music.stop();
                Reset =Settings(&Window);

                    if(Reset) {
                        if(!Intro(&Window)) return EXIT_SUCCESS;

                    Menu.setPosition(480, 1080);
                    Choice.setPosition(480, 1080);
                    Shape.setRadius(0);
                    Launch =64;
                    sf::Listener::setGlobalVolume(100);
                    } else Shape.setRadius(1089);

                Language =(std::stoi(ReadSave(156, 1, false)) >>2) %2;
                Music.play();
                }
            }

            else if(Event.type ==sf::Event::KeyPressed && !Shape.getRadius()) {
                if((Event.key.code ==sf::Keyboard::Escape || Event.key.code ==sf::Keyboard::BackSpace) && Menu.getPosition().y ==1080) Window.close();
                else if((Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) && Menu.getPosition().y ==1080) Menu.setPosition(480, 1068), Choice.move(0, -12), Switch_1.play();
                else if((Event.key.code ==sf::Keyboard::Escape || Event.key.code ==sf::Keyboard::BackSpace) && Menu.getPosition().y ==108) Menu.setPosition(480, 119), Choice.move(0, 11), Switch_2.play();
                else if(Event.key.code ==sf::Keyboard::Down && Menu.getPosition().y ==108 && (Choice.getPosition().y ==108 || Choice.getPosition().y ==432)) Choice.move(0, 4), Switch_3.play();
                else if(Event.key.code ==sf::Keyboard::Up && Menu.getPosition().y ==108 && (Choice.getPosition().y ==432 || Choice.getPosition().y ==756)) Choice.move(0, -3), Switch_3.play();
                else if((Event.key.code ==sf::Keyboard::Space || Event.key.code ==sf::Keyboard::Return) && Menu.getPosition().y ==108 && ((int)(Choice.getPosition().y -Menu.getPosition().y) %324 ==0) && !Shape.getRadius()) Shape.setRadius(16), Start.play();
            }

            if(Menu.getPosition().y ==1079) Menu.setPosition(480, 1080), Choice.move(0, 1);
            else if(Menu.getPosition().y <1080 && Menu.getPosition().y >108) Menu.move(0, (int)Menu.getPosition().y %2 ? 16 : -16), Choice.move(0, (int)Menu.getPosition().y %2 ? 16 : -16);

            if(int(Choice.getPosition().y -Menu.getPosition().y) %324 ==1) Choice.move(0, -1);
            else if(int(Choice.getPosition().y -Menu.getPosition().y) %324 !=0) Choice.move(0, (int)(Choice.getPosition().y -Menu.getPosition().y) %2 ? -16 : 16);

            if(Shape.getRadius() ==1) Shape.setRadius(0);

        Window.clear(sf::Color::Black);
        
            if(Menu.getPosition().y !=108) Window.draw(Background);
        
        BackgroundF.setColor(sf::Color(255, 255, 255, 255 -(Menu.getPosition().y -108) /972 *255));
        Window.draw(BackgroundF);
        Window.draw(Menu);
        Window.draw(Choice);

            for(int Z =0; Z !=3; Z ++) {

                if(Language) Text.setString(Z ==0 ? "Aventure" : Z ==1 ? "Sans Fin" : "Options");
                else Text.setString(Z ==0 ? "Adventure" : Z ==1 ? "Endless" : "Options");

            Text.setPosition(960 -Text.getGlobalBounds().width /2, Menu.getPosition().y +40 +Z *324);
            Window.draw(Text);
            }

            if(Shape.getRadius()) Shape.setRadius(Shape.getRadius() +((int)Shape.getRadius() %2 ? -16 : 16)), Shape.setPosition(960 -Shape.getRadius(), 540 -Shape.getRadius()), Window.draw(Shape);
            if(Launch) Launch --;

        Shape_1.setFillColor(sf::Color(0, 0, 0, Launch *4));
        Window.draw(Shape_1);
        Window.display();
        }

    return EXIT_SUCCESS;
    }
