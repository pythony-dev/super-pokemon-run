
#ifndef Save_hpp
#define Save_hpp

    std::string ReadSave(int, int, bool =true);
    void WriteSave(int, int, unsigned int);
    void FormatSave(bool);
    void CreateSave(bool);
    bool ExistSave();
    void RemoveSave(bool);
    std::string ReadUsername();

#endif
