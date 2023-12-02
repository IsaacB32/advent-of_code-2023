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

bool FileReader::Contains(const string &line, const string &key) {
    vector<string> split = FileReader::SplitByKey(line, key);
    return split.size() != 1;
}

bool FileReader::Contains(const string &line, const vector<string>& values) {
    int containsCount = 0;
    for (const auto & value : values) {
        if(FileReader::Contains(line, value)) containsCount++;
    }
    return (containsCount == values.size());
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


