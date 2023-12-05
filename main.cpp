#include <utility>
#include "FileReader.h"

static string filePath = R"(C:\The Main File ---------\Other Stuff\Code\AdventOfCode2023\Input.txt)";

void DayOne(vector<string> rows);
void DayTwo(vector<string> rows);
void DayTwo_2(vector<string> rows);

void DayThree(vector<vector<string>> rows2D, vector<string> rows);
void DayThree_2(vector<vector<string>> rows2D, vector<string> rows);

int main() {
    vector<vector<string>> rows2D = FileReader::ReadFileRowsByKey(filePath, "");
    vector<string> rows = FileReader::ReadFileRows(filePath);
    vector<vector<string>> columns = FileReader::ReadFileColumns(filePath);

//    FileReader::CheckForNeighborCount(rows2D, )
    DayThree_2(rows2D, rows);
}

void DayThree_2(vector<vector<string>> rows2D, vector<string> rows){
    int total = 0;
    for (int i = 0; i < rows2D.size(); ++i) {
        for (int j = 0; j < rows2D[i].size(); ++j) {
            if(rows2D[i][j] == "*" && FileReader::CheckForNeighborCount(rows2D, FileReader::numbersList, i, j, 1, 2))
            {
                string lastRow = rows[i-1];
                string lastConstruct;
                string nextRow = rows[i+1];
                string nextConstruct;
                string currentRow = rows[i];
                string currentConstruct;
                for (int k = j-3; k <= j+3; ++k) {
                    lastConstruct += lastRow[k];
                    nextConstruct += nextRow[k];
                    currentConstruct += currentRow[k];
                }
                vector<int> currentSplit = FileReader::StringToInt(FileReader::SplitByKey(currentConstruct, FileReader::symbolsList));
                vector<int> lastSplit = FileReader::StringToInt(FileReader::SplitByKey(lastConstruct, FileReader::symbolsList));
                vector<int> nextSplit = FileReader::StringToInt(FileReader::SplitByKey(nextConstruct, FileReader::symbolsList));

                vector<vector<string>> grid;
                vector<string> last;
                vector<string> current;
                vector<string> next;
                int lastIndex = 0;
                int currentIndex = 0;
                int nextIndex = 0;
                for (int k = j-1; k < j+1; ++k) {
                    if(lastRow[k] != '.')
                    {
                          last.push_back(to_string(lastSplit[lastIndex]));
                          lastIndex++;
                    }
                    else last.emplace_back(".");
                    if(currentRow[k] != '.' && currentRow[k] != '*')
                    {
                        current.push_back(to_string(lastSplit[currentIndex]));
                        currentIndex++;
                    }
                    else current.emplace_back(".");
                    if(nextRow[k] != '.')
                    {
                        next.push_back(to_string(lastSplit[nextIndex]));
                        nextIndex++;
                    }
                    else next.emplace_back(".");
                }
                grid.push_back(last);
                grid.push_back(current);
                grid.push_back(next);

                int t = 0;
                vector<string> seen;
                for (int k = 0; k < grid.size(); ++k) {
                    for (int l = 0; l < grid[k].size(); ++l) {
                        if(grid[k][l] != "." && !FileReader::Contains(grid[k][l], seen, false))
                        {
                            int toInt = stoi(grid[k][l]);
                            seen.push_back(grid[k][l]);
                            t *= toInt;
                        }
                    }
                }
                total += t;

//                if(lastSplit.size() > 1)
//                {
//                    int longest = lastSplit[0];
//                    if(lastSplit[1] > longest) longest = lastSplit[1];
//                    lastSplit.clear();
//                    lastSplit.push_back(longest);
//                }
//                if(nextSplit.size() > 1)
//                {
//                    int longest = nextSplit[0];
//                    if(nextSplit[1] > longest) longest = nextSplit[1];
//                    nextSplit.clear();
//                    nextSplit.push_back(longest);
//                }
//                if(currentSplit.size() == 2){
//                    int both = currentSplit[0] * currentSplit[1];
//                    currentSplit.clear();
//                    currentSplit.push_back(both);
//                }
//
//                int emptyCount = 0;
//                if(currentSplit.empty())
//                {
//                    currentSplit.push_back(1);
//                    emptyCount++;
//                }
//                if(lastSplit.empty())
//                {
//                    lastSplit.push_back(1);
//                    emptyCount++;
//                }
//                if(nextSplit.empty())
//                {
//                    nextSplit.push_back(1);
//                    emptyCount++;
//                }
//                if(emptyCount >= 2) continue;
//
//                if(lastSplit[0] > currentSplit[0] && nextSplit[0] > currentSplit[0])
//                {
//                    currentSplit.clear();
//                    currentSplit.push_back(1);
//                }
//
//                cout << "Adding: " << (currentSplit[0] * lastSplit[0] * nextSplit[0]) << endl;
//                cout << "current: " << (currentSplit[0]) << endl;
//                cout << "last: " << lastSplit[0] << endl;
//                cout << "next: " << nextSplit[0] << endl;
//                cout << endl;
                //total+= (currentSplit[0] * lastSplit[0] * nextSplit[0]);
            }
        }
    }// < 92000357
    cout << "Total: " << total << endl;
}
void DayThree(vector<vector<string>> rows2D, vector<string> rows){
    int total = 0;
    for (int i = 0; i < rows2D.size(); ++i) {
        vector<string> row = FileReader::SplitByKey(rows[i], FileReader::symbolsList);
        if(row.empty()) continue;

        int numberIndex = 0;
        bool lastWasNumber = false;
        bool checkedNumber = false; //no duplicity

        for (int j = 0; j < rows2D[i].size(); ++j) {
//            cout << "Looking At: " << rows2D[i][j] << endl;
            if(!FileReader::Contains(string(1, rows[i][j]), FileReader::symbolsList, false))
            {
//                cout << "LastNumber: " << lastWasNumber << endl;
//                cout << "index: (" << i << "," << j << ")" << endl;
                if(!checkedNumber && FileReader::CheckForNeighbor(rows2D, FileReader::symbolsListNoPeriod, i, j, 1)){
//                    cout << "-----Row-------" << endl;
//                    FileReader::PrintVector(row, "Row");
//                    cout << "added number: " << row[numberIndex] << endl;
                    total += stoi(row[numberIndex]);
                    checkedNumber = true;
                }
                lastWasNumber = true;
            }
            else if(lastWasNumber)
            {
//                cout << "Reset Number Index: " << numberIndex << endl;
                checkedNumber = false;
                lastWasNumber = false;
                numberIndex++;
            }
        }
//        cout << "---------------New Line----------------" << endl;
    }
    cout << "Total: " << total << endl;
}

void DayTwo_2(vector<string>rows){
    int total = 0;
    for (int i = 0; i < rows.size(); ++i) {
        vector<string> games = FileReader::SplitByKey(FileReader::SplitByKey(rows[i], ":")[1], ";");
        int maxSizes[3] = {0,0,0}; //0 = red, 1 = green, 2 = blue
        for (int j = 0; j < games.size(); ++j) {
            vector<string> cube = FileReader::SplitByKey(games[j], ",");
            for (int k = 0; k < cube.size(); ++k) {
                int number = stoi(FileReader::SplitByKey(cube[k], " ")[1]);
                if(number > maxSizes[2] && FileReader::Contains(cube[k], "blue"))
                {
                    maxSizes[2] = number;
                }
                else if(number > maxSizes[1] && FileReader::Contains(cube[k], "green"))
                {
                    maxSizes[1] = number;
                }
                else if(number > maxSizes[0] && FileReader::Contains(cube[k], "red"))
                {
                    maxSizes[0] = number;
                }
            }
        }
        int product = maxSizes[0] * maxSizes[1] * maxSizes[2];
        total += product;
    }
    cout << "Total: " << total << endl;
}
void DayTwo(vector<string> rows) {
    int total = 0;
    for (int i = 0; i < rows.size(); ++i) {
        vector<string> games = FileReader::SplitByKey(FileReader::SplitByKey(rows[i], ":")[1], ";");
        bool hasFailed = false;
        for (int j = 0; j < games.size(); ++j) {
            vector<string> cube = FileReader::SplitByKey(games[j], ",");
            for (int k = 0; k < cube.size(); ++k) {
                int number = stoi(FileReader::SplitByKey(cube[k], " ")[1]);
                if(number > 14 && FileReader::Contains(cube[k], "blue"))
                {
                    hasFailed = true;
                    break;
                }
                else if(number > 13 && FileReader::Contains(cube[k], "green"))
                {
                    hasFailed = true;
                    break;
                }
                else if(number > 12 && FileReader::Contains(cube[k], "red"))
                {
                    hasFailed = true;
                    break;
                }
            }
            if(hasFailed) break;
        }
        if(!hasFailed) {
            total += (i+1);
        }
    }
    cout << "Total: " << total << endl;
}

void DayOne(vector<string> rows) {
    int total = 0;

    vector<string> replace = std::move(rows);
    vector<string> wordNumbers = {"one","two","three","four","five","six","seven","eight","nine"};
    for (auto & line : replace) {
        for (int i = 0; i < wordNumbers.size(); ++i) {
            string replaceValue = wordNumbers[i][0] + to_string(i+1) + wordNumbers[i][wordNumbers[i].length()-1];
            line = FileReader::ReplaceLine(line, wordNumbers[i], replaceValue);
        }
    }

    vector<string> removedLetters = FileReader::ReplaceAll(replace, FileReader::alphabetList, "");
    for (auto & i : removedLetters) {
        char first = i[0];
        char last = i[i.length()-1];
        string stringTotal = string(1, first) + string(1,last);
        total += stoi(stringTotal);
    }
    cout << "Total: " << total << endl;
}