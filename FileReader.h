#ifndef ADVENTOFCODE2023_FILEREADER_H
#define ADVENTOFCODE2023_FILEREADER_H

#include <fstream>
#include <utility>
using namespace std;
namespace FileReader{
    int GetLineCount(const string& filePath);
    string* ReadFile(const string& filePath);

    string* ParseLineByCharacter(string line, char character);
    inline string* ParseLineBySpace(string line){return FileReader::ParseLineByCharacter(std::move(line),' ');}
    inline string* ParseLineByComma(string line){return FileReader::ParseLineByCharacter(std::move(line),',');}
};
#endif //ADVENTOFCODE2023_FILEREADER_H
