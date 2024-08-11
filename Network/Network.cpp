
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Player.hpp"

    void GetLevel(bool *NetworkIsRunning2, bool *c, bool *needSwitch, int Session, int Level) {
        while(*c ==true) {
        sf::Http Http("http://www.pythony.dev");
        sf::Http::Request Request("/SuperPokemonRun/Application/GetLevel.php?Session=" +std::to_string(Session));
        sf::Http::Response Response =Http.sendRequest(Request);
        std::string ResponseText =Response.getBody();

            if(Response.getStatus() ==sf::Http::Response::Ok && std::stoi(ResponseText) !=Level) *needSwitch =true;

        sf::sleep(sf::milliseconds(250));
        }

    *NetworkIsRunning2 =false;
    }

    void SetPosition(std::vector<Player> *Players, int *ID, int *Mutli, int Session, int PlayerID, bool *c, bool *networkisRunning) {
        while(true) {
        sf::Http Http("http://www.pythony.dev");
        sf::Http::Request Request("/SuperPokemonRun/Application/SetPosition.php?Session=" +std::to_string(Session) +"&Player=" +std::to_string(PlayerID) +"&X=" +std::to_string((*Players)[*ID].Get_X()) +"&Y=" +std::to_string((*Players)[*ID].Get_Y()) +"&PlayerID=" +std::to_string((*Players)[*ID].Get_ID()) +"&Direction=" +std::to_string((*Players)[*ID].Get_Direction()));
        sf::Http::Response Response =Http.sendRequest(Request);
        std::string ResponseText =Response.getBody();

            while(Response.getStatus() ==sf::Http::Response::Ok && ResponseText.size() !=0) {
            int i =ResponseText[0] -'0';
            i --;
            int x =0;
            int y =0;
            int id =0;
            int dir =0;
            ResponseText.erase(ResponseText.begin(), ResponseText.begin() +2);
            
            int j =0;

                while(ResponseText[j] !=' ') j ++;

            x =std::atoi(ResponseText.substr(0, j).c_str());
            ResponseText.erase(ResponseText.begin(), ResponseText.begin() +j +1);
            
            Mutli[i *4] =x;

            j =0;

                while(ResponseText[j] !=' ') j ++;

            y =std::atoi(ResponseText.substr(0, j).c_str());
            ResponseText.erase(ResponseText.begin(), ResponseText.begin() +j +1);
            Mutli[i *4 +1] =y;

            j =0;

                while(ResponseText[j] !=' ') j ++;

            id =std::atoi(ResponseText.substr(0, j).c_str());
            ResponseText.erase(ResponseText.begin(), ResponseText.begin() +j +1);
            Mutli[i *4 +2] =id;

            j =0;

                while(ResponseText[j] !=' ') j ++;

            dir =std::atoi(ResponseText.substr(0, j).c_str());
            ResponseText.erase(ResponseText.begin(), ResponseText.begin() +j +1);
            Mutli[i *4 +3] =dir;
            //std::cout << "Set Player " << i << " at (" << x << "; " << y << "; " << id << ";" << dir << ")" << std::endl;
            }

        sf::sleep(sf::milliseconds(250));

            if(*c ==false) { *networkisRunning =false; return; }
        }
    }

    void SendRecords(int Level, int Score, int Mode, int Medal, std::string List[], unsigned int UserID, std::string UserName) {
    // NE PAS LIRE D'INFORMATIONS EN SAUVEGARDE SOUS PEINE DE PLANTAGE
    //std::cout << "Sending Record - " << "UserID : " << UserID << "; Level : " << Level << "; Status : " << Medal << " !" << std::endl;

        if(UserName !="") {
        sf::Http Http("http://www.pythony.dev");
        sf::Http::Request Request("/SuperPokemonRun/Application/UpdateScore.php?UserID=" +std::to_string(UserID) +"&Username=" +UserName +"&Level=" +std::to_string(Level) +"&Score=" +std::to_string(Score) +"&Mode=" +std::to_string(Mode) +"&Status=" +std::to_string(Medal));
        sf::Http::Response Response =Http.sendRequest(Request);

            if(Response.getStatus() !=sf::Http::Response::Ok/* || Response.getBody()[0] !='X'*/) return;

        sf::Http Http2("http://www.pythony.dev");
        sf::Http::Request Request2("/SuperPokemonRun/Application/CheckScore.php?UserID=" +std::to_string(UserID) +"&Level=" +std::to_string(Level));
        sf::Http::Response Response2 =Http2.sendRequest(Request2);
        std::string ResponseText =Response2.getBody();

        int comp =0;

            while(Response2.getStatus() ==sf::Http::Response::Ok && ResponseText.size() !=0) {
            std::string element;
            int j =0;

                while(ResponseText[j] !=' ') j ++;

            List[comp] =(ResponseText.substr(0, j).c_str());
            comp ++;
            ResponseText.erase(ResponseText.begin(), ResponseText.begin() +j +1);
            }
        }

        return;
    }
