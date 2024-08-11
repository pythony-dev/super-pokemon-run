
#include <cmath>
#include <fstream>

#include "Save.hpp"

#define SIZE_SAVE 157

extern std::string Path;
//std::string RealPath ="/Users/AnthonyMew/Desktop/";

    static int GetDecimal(char Hexa) {
        return Hexa >='0' && Hexa <='9' ? Hexa -'0' : Hexa >='A' && Hexa <='F' ? Hexa -'A' +10 : 0;
    }




    static char GetHexa(int Decimal) {
        return Decimal >=0 && Decimal <=9 ? Decimal +'0' : Decimal >=10 && Decimal <=15 ? Decimal +'A' -10 : 0;
    }




    std::string ReadSave(int Position, int Size, bool Hexa) {

        if(!ExistSave()) RemoveSave(true);

    std::ifstream Stream(Path +"Super Pokemon Run.sav");
    Stream.seekg(0, std::ios::end);

        if(Stream.tellg() ==SIZE_SAVE *2) {
        Stream.seekg(0, std::ios::beg);
        std::string Password ="";
        char New =' ';
        unsigned int Count =0;

            for(int Z =0; Z !=SIZE_SAVE; Z ++) Stream.get(New), Password +=New, Count +=GetDecimal(New);

        Stream.seekg(SIZE_SAVE, std::ios::beg); // Implicite
        std::string Data ="";

            for(int Z =0; Z !=SIZE_SAVE; Z ++) Stream.get(New), Data +=New, Count +=GetDecimal(New);

        //std::cout << "Read Parity : " << Count << " %4 => " << Count %4 << std::endl;

            if(Count %4 !=0) RemoveSave(true);
            //std::cout << "PARITY ERROR !";

        std::string MyData ="";

            for(int Z =0; Z !=Size; Z ++) MyData +=GetHexa((GetDecimal(Data[Position +Z]) -GetDecimal(Password[Position +Z]) +16) %16);

            if(Hexa) return MyData;

        unsigned int Decimal =0;

            for(int Z =0; MyData.size(); Z ++) {
            Decimal +=GetDecimal(MyData[MyData.size() -1]) *std::pow(16, Z);
            MyData.pop_back();
            }

        return std::to_string(Decimal);
        }

    RemoveSave(true);
    }




    void WriteSave(int Position, int Size, unsigned int Data) {
        if(!ExistSave()) RemoveSave(true);

    std::fstream Stream(Path +"Super Pokemon Run.sav");
    Stream.seekp(0, std::ios::end);

        if(ExistSave() && Stream.tellp() ==SIZE_SAVE *2) {
        std::string Password ="";
        unsigned int Count =0;

            for(int Z =0; Z !=SIZE_SAVE; Z ++) Password +=GetHexa(rand() %16), Count +=GetDecimal(Password[Password.size() -1]);

        std::string Save =ReadSave(0, SIZE_SAVE);

        std::string NewData ="";

            for(int Z =0; Data !=0 && Z !=Size; Z ++) {
            std::string Insert(1, GetHexa(Data %16));
            NewData.insert(0, Insert);
            Data /=16;
            }

            while(NewData.size() <Size) NewData.insert(0, "0");

            for(int Z =0; Z !=Size; Z ++) Save[Position +Z] =NewData[Z];

        int LastSave =GetDecimal(Save[SIZE_SAVE -1]);

            for(int Z =0; Z !=SIZE_SAVE; Z ++) Save[Z] =GetHexa((GetDecimal(Save[Z]) +GetDecimal(Password[Z])) %16), Count +=GetDecimal(Save[Z]);

        //std::cout << "Write Parity : " << Count << " %4 => " << Count %4 << std::endl;

            if(Count %4 !=0) {
            int LastData =LastSave & 0xC;
            int LastPart =((LastSave %4) +4 -(Count %4)) %4;
            int LastPassword =GetDecimal(Password[Password.size() -1]);

            Save[SIZE_SAVE -1] =GetHexa((LastData +LastPart +LastPassword) %16);
            }

        Stream.seekp(0, std::ios::beg);

            for(int Z =0; Z !=SIZE_SAVE; Z ++) Stream.put(Password[Z]);

            for(int Z =0; Z !=SIZE_SAVE; Z ++) Stream.put(Save[Z]);
        } else RemoveSave(true);
    }




    void FormatSave(bool Choice) {

        if(!ExistSave()) RemoveSave(true);

    WriteSave(0, 4, rand() %65536); WriteSave(4, 4, rand() %65536); // UserID
    WriteSave(8, 1, 4); WriteSave(9, 19, 0); // Adventure
    WriteSave(28, 8, 0); // Score
    WriteSave(36, 96, 0); // Endless

        if(!Choice) WriteSave(132, 6, 3800144), WriteSave(138, 6, 10075359), WriteSave(144, 6, 11534336), WriteSave(150, 6, 0);
        else WriteSave(132, 6, 3841008), WriteSave(138, 6, 8240074), WriteSave(144, 6, 10942447), WriteSave(150, 6, 11534336);

    WriteSave(156, 1, Choice ? 12 : 8); // Settings
    }




    void CreateSave(bool Choice) {
    RemoveSave(false);
    std::ofstream Stream(Path +"Super Pokemon Run.sav");

        if(Stream.fail()) RemoveSave(true);

        for(int Z =0; Z !=SIZE_SAVE *2; Z ++) Stream.put('0');

    Stream.close();
    FormatSave(Choice);
    }




    bool ExistSave() {
    std::ifstream Stream(Path +"Super Pokemon Run.sav");

    return !Stream.fail();
    }




    void RemoveSave(bool Exit) {
        remove((Path +"Super Pokemon Run.sav").c_str());

        if(Exit) exit(EXIT_FAILURE);
    }




    std::string ReadUsername() {
    std::string Username ="";

        for(int Z =0; Z !=16; Z ++) {
        int Saved =0;
        int LastFrame =((std::stoi(ReadSave(124 +(Z *1.5) +8, 1, false)) /4)) ;

            if(Z %2 ==0) Saved =std::stoi(ReadSave(132 +int(Z *1.5), 2, false)), Saved >>=2;
            else if(Z %2 ==1) Saved =std::stoi(ReadSave(132 +int(Z *1.5), 2, false)), Saved -=LastFrame *64;

            if(Saved >=1 && Saved <=26) Username +=Saved +'A' -1;
            else if(Saved >=27 && Saved <=52) Username +=Saved +'a' -1 -26;
            else if(Saved >=53 && Saved <=62) Username +=Saved +'0' -52 -1;
        }

    return Username;
    }
