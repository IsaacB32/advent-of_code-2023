#include "FileReader.h"

#include <utility>

vector<string> FileReader::ReadFileRows(const string& filePath) {
    vector<string> allLines;
    ifstream file(filePath);

    string line;
    while (getline(file, line)) {
        allLines.push_back(line);
    }
    file.close();
    return allLines;
}

vector<vector<string>> FileReader::ReadFileRowsByKey(const string& filePath, const string& key) {
    vector<vector<string>> file;
    vector<string> rows = ReadFileRows(filePath);
    for (int i = 0; i < rows.size(); i++) {
        vector<string> splitLine;
        if(key.empty()) splitLine = SplitByCharacter(rows[i]);
        else splitLine = SplitByKey(rows[i], key);

        file.push_back(splitLine);
    }
    return file;
}

vector<vector<string>> FileReader::ReadFileColumnsByKey(const string& filePath, const string& key) {
    vector<vector<string>> file = ReadFileRowsByKey(filePath, key);
    vector<vector<string>> columns;
    for (int i = 0; i < file[0].size(); ++i) {
        vector<string> temp;
        for (int j = 0; j < file.size(); ++j) {
            temp.push_back(file[j][i]);
        }
        columns.push_back(temp);
    }
    return columns;
}

vector<string> FileReader::SplitByKey(const string& line, const string& key) {
    vector<string> cutLine;
    int startingIndex = 0;
    int loopGuardCount = 1000000000;
    while(startingIndex < line.length() && loopGuardCount > 0){
        int keyIndex = (int)line.find(key, startingIndex);
        if(keyIndex == -1) keyIndex = (int)line.length();

        cutLine.push_back(line.substr(startingIndex, keyIndex - startingIndex));
        startingIndex = keyIndex + (int)key.length();

        loopGuardCount--;
    }
    if(loopGuardCount <= 0) cout << "Fatale Error -- Infinite Loop" << endl;
    return cutLine;
}

//uses any of the provided vector<string> values to split the string
vector<string> FileReader::SplitByKey(const string& line, const vector<string>& keys) {
    vector<string> cutFile;
    string split;
    for (int i = 0; i < line.length(); ++i) {
        if(!FileReader::Contains(string(1, line[i]), keys, false))
        {
            split += line[i];
        }
        else {
            if(!split.empty()) cutFile.push_back(split);
            split = "";
        }
    }
    if(!split.empty()) cutFile.push_back(split);
    return cutFile;
}

vector<string> FileReader::SplitByCharacter(const string &line) {
    vector<string> cutLine;
    for (int i = 0; i < line.length(); ++i) {
        cutLine.push_back(line.substr(i, 1));
    }
    return cutLine;
}


string FileReader::ReplaceLine(string &line, const string &key, const string &toReplace) {
    vector<string> row = SplitByKey(line, key);

    string construct;
    for (int j = 0; j < row.size()-1; ++j) {
        construct += row[j] + toReplace;
    }

    if(line.find(key, line.length() - key.length()) < line.length()) construct += row[row.size()-1] + toReplace;
    else construct += row[row.size()-1];

    if(construct.length() > 0) line = construct;

    return line;
}

void FileReader::ReplaceAllAbsolute(vector<string>& file, const string& key, const string& toReplace) {
    for (auto & line : file) {
        ReplaceLine(line, key, toReplace);
    }
}

vector<string> FileReader::ReplaceAll(vector<string> file, const string &key, const string &toReplace) {
    FileReader::ReplaceAllAbsolute(file, key, toReplace);
    return file;
}

vector<string> FileReader::ReplaceAll(vector<string> file, const vector<string> &keys, const string &toReplace) {
    vector<string> replace = std::move(file);
    for (const auto & key : keys) {
        replace = FileReader::ReplaceAll(replace, key, toReplace);
    }
    return replace;
}

int FileReader::GetLineCount(const string& filePath)
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
int FileReader::GetSizeOfLineWithoutKey(const string& line, string key) {
    int totalCount = 0;

    int keyIndex = 0;
    for (char lineChar : line) {
        if(lineChar == key[keyIndex]){
            keyIndex++;
        }

        if(keyIndex == key.length()){
            totalCount++;
            keyIndex = 0;
        }
    }

    return totalCount;
}

//uses additional character '⁂' to compensate for the split method not accounting for the very last character
bool FileReader::Contains(const string &line, const string &key) {
    vector<string> split = FileReader::SplitByKey(line + "⁂", key);
    return split.size() != 1;
}

bool FileReader::Contains(const string &line, const vector<string>& values, bool containsAll) {
    int containsCount = 0;
    for (const auto & value : values) {
        if(Contains(line, value)) {
            if(!containsAll) return true;
            containsCount++;
        }
    }
    return (containsCount == values.size());
}

bool FileReader::Contains(const vector<string> &lines, const string& key) {
    for (int i = 0; i < lines.size(); ++i) {
        if(lines[i] == key) return true;
    }
    return false;
}



vector<int> FileReader::StringToInt(vector<string> values) {
    vector<int> integer;
    for (int i = 0; i < values.size(); ++i) {
        integer.push_back(stoi(values[i]));
    }
    return integer;
}

bool FileReader::CheckForNeighbor(vector<vector<string>> grid, const vector<string>& keys, int rowIndex, int columnIndex, int range) {
    vector<int> position = GetNeighbor(std::move(grid), keys, rowIndex, columnIndex, range);
    return (position.size() == 2);
}

//checks for count existing neighbors seperated by filler spaces
bool FileReader::CheckForNeighborCount(vector<vector<string>> grid, const vector<string>& keys, int rowIndex, int columnIndex, int range, int count) {
    bool lookedAtNumber = false;
    int numNeighbors = 0;
    for (int i = rowIndex-range; i < rowIndex+range+1; ++i) { //row
        if(i < 0 || i >= grid.size()) continue;
        for (int j = columnIndex-range; j < columnIndex+range+1; ++j) { //column
            if(j < 0 || j >= grid[i].size()) continue;
            if(i == rowIndex && j == columnIndex) continue;
            string neighbor = grid[i][j];
            if(FileReader::Contains(neighbor, keys, false) && !lookedAtNumber){
                lookedAtNumber = true;
                numNeighbors++;
            }
            else if(lookedAtNumber)
            {
                lookedAtNumber = false;
            }
        }
    }
    return (numNeighbors) == count;
}

//returns the first instance of a key or list of keys
vector<int> FileReader::GetNeighbor(vector<vector<string>> grid, const vector<string>& keys, int rowIndex, int columnIndex, int range){
    for (int i = rowIndex-range; i < rowIndex+range+1; ++i) { //row
        if(i < 0 || i >= grid.size()) continue;
        for (int j = columnIndex-range; j < columnIndex+range+1; ++j) { //column
            if(j < 0 || j >= grid[i].size()) continue;
            string neighbor = grid[i][j];
            if(FileReader::Contains(neighbor, keys, false)){
                vector<int> position = {i, j};
                return position;
            }
        }
    }
    vector<int> position = {-1};
    return position;
}

vector<pair<int,int>> FileReader::GetNeighborIndex(vector<vector<string>> grid, const vector<string>& keys, int rowIndex, int columnIndex){
    vector<pair<int,int>> positions;
    for (int i = rowIndex-1; i < rowIndex+1+1; ++i) { //row
        if(i < 0 || i >= grid.size()) continue;
        for (int j = columnIndex-1; j < columnIndex+1+1; ++j) { //column
            if(j < 0 || j >= grid[i].size()) continue;
            string neighbor = grid[i][j];
            if(FileReader::Contains(neighbor, keys, false)){
                pair<int,int> position = {i, j};
                positions.push_back(position);
            }
        }
    }
    return positions;
}

void FileReader::PrintVector(const vector<string>& v, const string& title) {
    cout << "-----Printing Vector: " << title << "-----" << endl;
    for (const auto & i : v) {
        cout << i << endl;
    }
    cout << endl;
}
void FileReader::PrintVector(const vector<int>& v, const string& title) {
    cout << "-----Printing Vector: " << title << "-----" << endl;
    for (const auto & i : v) {
        cout << i << endl;
    }
    cout << endl;
}
void FileReader::PrintVector(const vector<vector<string>>& v, const string& title) {
    cout << "-----Printing 2D Vector: " << title << "-----" << endl;
    for (const auto & i : v) {
        for (const auto & j : i) {
            cout << j << ", ";
        }
        cout << endl;
    }
    cout << endl;
}


