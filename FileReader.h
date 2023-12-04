#ifndef ADVENTOFCODE2023_FILEREADER_H
#define ADVENTOFCODE2023_FILEREADER_H

#include <iostream>

#include <fstream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

class FileReader{
public:
    static inline vector<string> alphabetList = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
    static inline vector<string> symbolsList = {"~", "`", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "-", "+", "=", "{", "}", "[", "]", "|", "\\", ":", ";", "\"", "'", "<", ">", ",", "?", "/", "."};
    static inline vector<string> symbolsListNoPeriod = {"~", "`", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "-", "+", "=", "{", "}", "[", "]", "|", "\\", ":", ";", "\"", "'", "<", ">", ",", "?", "/"};
    static inline vector<string> numbersList = {"0","1","2","3","4","5","6","7","8","9"};

    static vector<string> ReadFileRows(const string& filePath);
    static vector<vector<string>> ReadFileRowsByKey(const string& line, const string& key);
    static vector<vector<string>> ReadFileColumnsByKey(const string& filePath, const string& key);
    static inline vector<vector<string>> ReadFileColumns(const string& filePath) {return ReadFileColumnsByKey(filePath, "");}

    static vector<string> SplitByKey(const string& line, const string& key);
    static vector<string> SplitByKey(const string& line, const vector<string>& keys);
    static vector<string> SplitByCharacter(const string& line);
    static inline vector<string>SplitBySpace(const string& line){return SplitByKey(line, " ");}
    static inline vector<string>SplitByDoubleSpace(const string& line){return SplitByKey(line, "  ");}
    static inline vector<string>SplitByPeriod(const string& line){return SplitByKey(line, ".");}

    static string ReplaceLine(string& line, const string& key, const string& toReplace);
    static void ReplaceAllAbsolute(vector<string>& file, const string& key, const string& toReplace);
    static vector<string> ReplaceAll(vector<string> file, const string& key, const string& toReplace);
    static vector<string> ReplaceAll(vector<string> file, const vector<string>& keys, const string& toReplace);

    static int GetLineCount(const string& filePath);
    static int GetSizeOfLineWithoutKey(const string& line, string key);

    static bool Contains(const string& line, const string& key);
    static bool Contains(const string& line, const vector<string>& values, bool containsAll);

    static inline bool CheckForNeighbor(vector<vector<string>> grid, const string& key, int rowIndex, int columnIndex, int range)
    {
        vector<string> keys;
        keys.push_back(key);
        return CheckForNeighbor(std::move(grid), keys, rowIndex, columnIndex, range);
    };
    static bool CheckForNeighbor(vector<vector<string>> grid, const vector<string>& keys, int rowIndex, int columnIndex, int range);
    static inline vector<int> GetNeighbor(vector<vector<string>> grid, const string& key, int rowIndex, int columnIndex, int range)
    {
        vector<string> keys;
        keys.push_back(key);
        return GetNeighbor(std::move(grid), keys, rowIndex, columnIndex, range);
    };
    static vector<int> GetNeighbor(vector<vector<string>> grid, const vector<string>& keys, int rowIndex, int columnIndex, int range);

    static void PrintVector(const vector<string>& v, const string& title);
    static void PrintVector(const vector<vector<string>>& v, const string& title);
    static inline void PrintVector(const vector<string>& v) {return PrintVector(v, "");}
    static inline void PrintVector(const vector<vector<string>>& v) {return PrintVector(v, "");}
};
#endif //ADVENTOFCODE2023_FILEREADER_H
