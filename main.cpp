#include <utility>
#include "FileReader.h"

static string filePath = R"(C:\The Main File ---------\Other Stuff\Code\AdventOfCode2023\Input.txt)";

void DayOne(vector<string> rows);
void DayTwo(vector<string> rows);
void DayTwo_2(vector<string> rows);

void DayThree(vector<vector<string>> rows2D, vector<string> rows);
void DayThree_2(vector<vector<string>> rows2D);

int main() {
    vector<vector<string>> rows2D = FileReader::ReadFileRowsByKey(filePath, "");
    vector<string> rows = FileReader::ReadFileRows(filePath);
    vector<vector<string>> columns = FileReader::ReadFileColumns(filePath);

    DayThree_2(rows2D);
}

void DayThree_2(vector<vector<string>> rows2D) {
    long long total = 0;
    vector<string> things = {".","*"};
    for (int i = 0; i < rows2D.size(); ++i) {
        for (int j = 0; j < rows2D[i].size(); ++j) {
            if (rows2D[i][j] == "*" && FileReader::CheckForNeighborCount(rows2D, FileReader::numbersList, i, j, 1, 2)) {
                vector<string> number;
                vector<int> gearValues;
                vector<pair<int, int>> neighborPositions = FileReader::GetNeighborIndex(rows2D, FileReader::numbersList,
                                                                                        i, j);
                for (int k = 0; k < neighborPositions.size(); ++k) {
                    pair<int, int> p = neighborPositions[k];
                    vector<string> r = rows2D[p.first];
                    if (p.first != i) //not on same row
                    {
                        string n;
                        int start = p.second-2, end = p.second+2;
                        for (int l = p.second - 2; l <= p.second + 2; ++l) {
                            if(l > p.second && r[l] == ".") {
                                end = l-1;
                                break;
                            }
                            if(r[l] != "." && start == p.second-2)
                            {
                                start = l-1;
                            }
                            else if(r[l] == "." && start != p.second-2)
                            {
                                start = p.second-2;
                            }
                        }

                        for (int l = start; l <= end; ++l) {
                            n += r[l];
                        }
                        vector<string> ratio = FileReader::RemoveEmpty(FileReader::SplitByKey(n, "."));
                        string replaceN = FileReader::ReplaceLine(n, ".", "");
                        if (!FileReader::Contains(number, replaceN)) number.push_back(replaceN);
                    } else //on the '*' row
                    {
                        string n;
                        if(p.second < j)
                        {
                            for (int l = p.second-2; l <= p.second; ++l) {
                                n += r[l];
                            }
                        }
                        else if(p.second > j){
                            for (int l = p.second; l <= p.second+2; ++l) {
                                n += r[l];
                            }
                        }
                        vector<string> ratio = FileReader::RemoveEmpty(FileReader::SplitByKey(n, things));
                        string replaceRatio = FileReader::ReplaceLine(ratio[0], ".", "");
                        if (!FileReader::Contains(number, replaceRatio)) number.push_back(replaceRatio);
                    }
                }
                vector<int> numInt = FileReader::StringToInt(number);
                FileReader::PrintVector(number);
                if(number.size() == 1) total += (numInt[0] * numInt[0]);
                else if (number.size() == 2) total += (numInt[0] * numInt[1]);
            }
        }// dumb: 74991909 wrong: 75671832 correct?: 903122
    }
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