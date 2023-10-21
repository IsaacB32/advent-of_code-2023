#include "FileReader.h"

namespace FileReader{
    int GetLineCount(const string& filePath)
    {
        int lineCount = 0;
        string temp; //unused variable
        ifstream file(filePath);
        while(getline(file, temp))
        {
            lineCount++;
        }
        file.close();
        return lineCount;
    }
    string* ReadFile(const string& filePath) {
        string *allLines = new string[GetLineCount(filePath)];

        ifstream file(filePath);

        string line;
        int index = 0;
        while (getline(file, line)) {
            allLines[index] = line;
            index++;
        }
        file.close();
        return allLines;
    }

    string* ParseLineByCharacter(string line, char character)
    {
        //substr
        //find
        for (int i = 0; i < line.length(); ++i) {
            string currentSubString = line.substr(i, line.length());
        }
        return nullptr;
    }
}