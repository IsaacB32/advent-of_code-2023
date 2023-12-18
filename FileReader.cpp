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

vector<string> FileReader::GetColumnsFromRows2D(vector<vector<string>> rows2D) {
    vector<string> columns;
    for (int i = 0; i < rows2D[0].size(); ++i) {
        string c;
        for (int j = 0; j < rows2D.size(); ++j) {
            c += rows2D[j][i];
        }
        columns.push_back(c);
    }
    return columns;
}
vector<string> FileReader::GetRowsFromRows2D(vector<vector<string>> rows2D) {
    vector<string> rows;
    for (int i = 0; i < rows2D[0].size(); ++i) {
        string c;
        for (int j = 0; j < rows2D.size(); ++j) {
            c += rows2D[i][j];
        }
        rows.push_back(c);
    }
    return rows;
}

vector<string> FileReader::FlipRowsToColumns(vector<string> rows) {
    vector<string> columns;
    int columnSize = (int)rows[0].size();
    if(columnSize != rows.size()){
        cout << "Columns and Rows must be same size!!" << endl;
        return rows;
    }

    for (int i = 0; i < rows.size(); ++i) {
        string column;
        for (int j = 0; j < columnSize; ++j) {
            column += rows[j][i];
        }
        columns.push_back(column);
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

//cuts into 2D vector by row-key
vector<vector<string>> FileReader::CutRowsByKey(const vector<string>& lines, const string &key) {
    vector<vector<string>> output;
    vector<string> validLines;
    for (int i = 0; i < lines.size(); ++i) {
        if(lines[i] != key){
            validLines.push_back(lines[i]);
        }
        else {
            output.push_back(validLines);
            validLines.clear();
        }
    }
    if(!validLines.empty()) output.push_back(validLines);
    return output;
}
//cuts into 2D vector every value
vector<vector<string>> FileReader::CutRowsByNumber(const vector<string>& lines, const int &value) {
    vector<vector<string>> output;
    vector<string> validLines;
    for (int i = 0; i < lines.size(); ++i) {
        if(i % value == 0){
            validLines.push_back(lines[i]);
        }
        else {
            output.push_back(validLines);
            validLines.clear();
        }
    }
    if(!validLines.empty()) output.push_back(validLines);
    return output;
}


string FileReader::ReplaceLine(string &line, const string &key, const string &toReplace) {
    if(!Contains(line, key)) return line;

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
    for (string & line : file) {
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

vector<string> FileReader::RemoveEmpty(vector<string> list) {
    vector<string> fill;
    for (int i = 0; i < list.size(); ++i) {
        if(!list[i].empty()) fill.push_back(list[i]);
    }
    return fill;
}

vector<int> FileReader::StringToInt(vector<string> integerList) {
    vector<string> removedEmpty = RemoveEmpty(integerList);
    vector<int> integer;
    for (int i = 0; i < integerList.size(); ++i) {
        integer.push_back(stoi(integerList[i]));
    }
    return integer;
}
vector<long long> FileReader::StringToLong(vector<string> integerList) {
    vector<string> removedEmpty = RemoveEmpty(integerList);
    vector<long long> integer;
    for (int i = 0; i < integerList.size(); ++i) {
        integer.push_back(stoll(integerList[i]));
    }
    return integer;
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

int FileReader::SumVector(vector<int> values) {
    int size = 0;
    for (int i = 0; i < values.size(); ++i) {
        size += values[i];
    }
    return size;
}

int FileReader::CountOccurances(vector<vector<char>> rows, const char &key) {
    int count = 0;
    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < rows[i].size(); ++j) {
            if(rows[i][j] == key) count++;
        }
    }
    return count;
}

int FileReader::CountUniqueOccurances(const string &line, const char& key) {
    int count = 0;
    bool inGroup;
    for (int i = 0; i < line.length(); ++i) {
        if (line[i] == key) {
            if (!inGroup) {
                inGroup = true;
                ++count;
            }
        } else inGroup = false;
    }
    return count;
}

vector<int> FileReader::CountUniqueOccurancesSize(const string &line, const char &key) {
    vector<int> sizes;
    int count = 0;
    bool inGroup;
    for (int i = 0; i < line.length(); ++i) {
        if (line[i] == key) {
            count++;
            if (!inGroup) {
                inGroup = true;
            }
        }
        else {
            if(inGroup) {
                sizes.push_back(count);
                count = 0;
            }
            inGroup = false;
        }
    }
    if(count > 0) sizes.push_back(count);
    return sizes;
}

int FileReader::IndexOf(const string &str, int startIndex, char key) {
    auto it = find(str.begin() + startIndex+1, str.end(), key);

    // Check if the character was found
    if (it != str.end()) {
        // Calculate the index using std::distance
        return std::distance(str.begin(), it);
    } else {
        // Return a special value (e.g., -1) to indicate that the character was not found
        return -1;
    }
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

bool FileReader::CheckForNeighbor(vector<vector<string>> grid, const vector<string>& keys, int rowIndex, int columnIndex, int range) {
    vector<int> position = GetNeighbor(std::move(grid), keys, rowIndex, columnIndex, range);
    return (position.size() == 2);
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


