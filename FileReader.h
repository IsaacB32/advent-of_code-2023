#ifndef ADVENTOFCODE2023_FILEREADER_H
#define ADVENTOFCODE2023_FILEREADER_H

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
using namespace std;

class FileReader{
public:
    static vector<string> ReadFileRows(const string& filePath);
    static vector<vector<string>> ReadFileRowsByKey(const string& line, const string& key);
    static vector<vector<string>> ReadFileColumnsByKey(const string& filePath, const string& key);
    static inline vector<vector<string>> ReadFileColumns(const string& filePath) {return FileReader::ReadFileColumnsByKey(filePath, "");}

    static vector<string> SplitByKey(const string& line, const string& key);
    static inline vector<string>SplitBySpace(const string& line){return FileReader::SplitByKey(line, " ");}
    static inline vector<string>SplitByDoubleSpace(const string& line){return FileReader::SplitByKey(line, "  ");}
    static inline vector<string>SplitByComma(const string& line){return FileReader::SplitByKey(line, ",");}

    static void ReplaceAllAbsolute(vector<string>& file, const string& key, const string& toReplace);
    static vector<string> ReplaceAll(vector<string> file, const string& key, const string& toReplace);

    static int GetLineCount(const string& filePath);
    static int GetSizeOfLineWithoutKey(const string& line, string key);

    static void PrintVector(const vector<string>& v, const string& title);
    static void PrintVector(const vector<vector<string>>& v, const string& title);
    static inline void PrintVector(const vector<string>& v) {return FileReader::PrintVector(v, "");}
    static inline void PrintVector(const vector<vector<string>>& v) {return FileReader::PrintVector(v, "");}
};
#endif //ADVENTOFCODE2023_FILEREADER_H
