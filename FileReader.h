#ifndef ADVENTOFCODE2023_FILEREADER_H
#define ADVENTOFCODE2023_FILEREADER_H

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
using namespace std;

class FileReader{
public:
    static inline vector<string> alphabetList = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};

    static vector<string> ReadFileRows(const string& filePath);
    static vector<vector<string>> ReadFileRowsByKey(const string& line, const string& key);
    static vector<vector<string>> ReadFileColumnsByKey(const string& filePath, const string& key);
    static inline vector<vector<string>> ReadFileColumns(const string& filePath) {return ReadFileColumnsByKey(filePath, "");}

    static vector<string> SplitByKey(const string& line, const string& key);
    static inline vector<string>SplitBySpace(const string& line){return SplitByKey(line, " ");}
    static inline vector<string>SplitByDoubleSpace(const string& line){return SplitByKey(line, "  ");}
    static inline vector<string>SplitByComma(const string& line){return SplitByKey(line, ",");}

    static string ReplaceLine(string& line, const string& key, const string& toReplace);
    static void ReplaceAllAbsolute(vector<string>& file, const string& key, const string& toReplace);
    static vector<string> ReplaceAll(vector<string> file, const string& key, const string& toReplace);
    static vector<string> ReplaceAll(vector<string> file, const vector<string>& keys, const string& toReplace);

    static int GetLineCount(const string& filePath);
    static int GetSizeOfLineWithoutKey(const string& line, string key);

    static bool Contains(const string& line, const string& key);
    static bool Contains(const string& line, const vector<string>& values); //returns true if it contains all values

    static void PrintVector(const vector<string>& v, const string& title);
    static void PrintVector(const vector<vector<string>>& v, const string& title);
    static inline void PrintVector(const vector<string>& v) {return PrintVector(v, "");}
    static inline void PrintVector(const vector<vector<string>>& v) {return PrintVector(v, "");}
};
#endif //ADVENTOFCODE2023_FILEREADER_H
