#include "FileReader.h"

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
    vector<string> rows = FileReader::ReadFileRows(filePath);
    for (const auto & row : rows) {
        vector<string> splitLine = FileReader::SplitByKey(row, key);
        file.push_back(splitLine);
    }
    return file;
}

vector<vector<string>> FileReader::ReadFileColumnsByKey(const string& filePath, const string& key) {
    vector<vector<string>> file = FileReader::ReadFileRowsByKey(filePath, key);
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
    while(startingIndex < line.length()){
        int keyIndex = (int)line.find(key, startingIndex);
        if(keyIndex == -1) keyIndex = (int)line.length();

        cutLine.push_back(line.substr(startingIndex, keyIndex - startingIndex));
        startingIndex = keyIndex + (int)key.length();
    }
    return cutLine;
}

void FileReader::ReplaceAllAbsolute(vector<string>& file, const string& key, const string& toReplace) {
    for (auto & i : file) {
        vector<string> row = FileReader::SplitByKey(i, key);

        string construct;
        for (int j = 0; j < row.size()-1; ++j) {
            construct += row[j] + toReplace;
        }

        if(i.find(key, i.length() - key.length()) < i.length()) construct += row[row.size()-1] + toReplace;
        else construct += row[row.size()-1];

        if(construct.length() > 0) i = construct;
    }
}
vector<string> FileReader::ReplaceAll(vector<string> file, const string &key, const string &toReplace) {
    FileReader::ReplaceAllAbsolute(file, key, toReplace);
    return file;
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

void FileReader::PrintVector(const vector<string>& v, const string& title) {
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


