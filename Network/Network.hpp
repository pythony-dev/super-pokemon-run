
#ifndef NetworkFunctions_hpp
#define NetworkFunctions_hpp

    void GetLevel(bool *, bool *, bool *, int, int);
    void SetPosition(std::vector<Player> *, int *, int *, int, int, bool *, bool *);
    bool SendRecords(int Level, int Score, int Mode, int Medal, std::string List[], unsigned int, std::string);

#endif
