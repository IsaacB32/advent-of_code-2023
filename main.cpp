#include <utility>
#include "FileReader.h"
#include "PQueue.h"
#include "algorithm"
#include <unordered_map>

static string filePath = R"(C:\The Main File ---------\Other Stuff\Code\AdventOfCode2023\Input.txt)";

void DayOne(vector<string> rows);
void DayTwo(vector<string> rows);
void DayTwo_2(vector<string> rows);
void DayThree(vector<vector<string>> rows2D, vector<string> rows);
void DayThree_2(vector<vector<string>> rows2D, vector<string> rows);
void DayFour(vector<string> rows);
void DayFour_2(vector<string> rows);

void DayFive(vector<vector<string>> rows);
void DayFive_2(vector<vector<string>> rows);
void DaySix(vector<string> rows);
void DaySeven(vector<string> rows);

void DayEight(vector<string> rows);

int main() {
//    vector<vector<string>> rows2D = FileReader::ReadFileRowsByKey(filePath, "");
    vector<string> rows = FileReader::ReadFileRows(filePath);
//    vector<vector<string>> columns = FileReader::ReadFileColumns(filePath);
//    vector<vector<string>> cutRows = FileReader::CutRowsByKey(rows, "");

    DayEight(rows);
}

string CutOrder(string order, char isLeft){
    int sideIndex = (isLeft == 'L') ? 0 : 1;
    string split = FileReader::SplitBySpace(order)[sideIndex];
    if(sideIndex == 0) split = split.substr(1);
    else split = split.substr(0, split.length()-1);
    return split;
}
void DayEight(vector<string> rows){
    vector<string> instructions = FileReader::SplitByCharacter(rows[0]);

    //<key, value>
    unordered_map<string, string> map;
    for (int i = 2; i < rows.size(); ++i) {
        string key = (FileReader::SplitByKey(rows[i],"=")[0]).substr(0, 3);
        string order = FileReader::ReplaceLine(FileReader::SplitByKey(rows[i],"=")[1], ",", "").substr(1);
        map[key] = order;
    }
    

//    vector<string> debugKeys;
//    for (int i = 2; i < rows.size(); ++i) {
//        debugKeys.push_back((FileReader::SplitByKey(rows[i],"=")[0]).substr(0, 3));
//    }
//
//    for (int i = 0; i < debugKeys.size(); ++i) {
//        string order = map[debugKeys[i]];
//        cout << "key: " << debugKeys[i] <<  " | order: " << order << endl;
//        cout << "------" << endl;
//    }

//    //int loopCheck = 66600000;
//    long long stepCounter = 0;
//    string loopCheckString;
//    string key = (FileReader::SplitByKey(rows[2],"=")[0]).substr(0, 3);
//    while(key != "ZZZ" ) //&& loopCheck >= 0
//    {
////        loopCheck-=1;
//        string order = map[key];
//        char direction = instructions[stepCounter % instructions.size()][0];
//        key = CutOrder(order, direction);
////        cout << "direcetion: " << direction << endl;
//        stepCounter++;
//    }
//    cout << "Total Steps: " << stepCounter << endl;
} //not: 666

int CardStringToInt(const string& card)
{
    vector<string> faceCards = {"A", "J", "Q", "K", "T"};
    vector<int> faceCardNums = {14, 11, 12, 13, 10};
    for (int i = 0; i < faceCards.size(); ++i) {
        if(card == faceCards[i])
        {
            return faceCardNums[i];
        }
    }
    return stoi(card);
}
void DaySeven(vector<string> rows){

    struct Row{
        // Overload the '==' operator
        bool operator==(const Row& other) const {
            return this->cardHand == other.cardHand;
        }
        string cardHand;
        int number;
    };

    enum HandType{
        FiveK = 0,
        FourK = 1,
        FH = 2,
        ThreeK = 3,
        TP = 4,
        OP = 5,
        HC = 6,
        NA = 100
    };
    vector<Row> cardHands;
    vector<HandType> handType;
    for (int i = 0; i < rows.size(); ++i) {
        PQueue<string> hand;

        Row currentRow;
        currentRow.number = stoi(FileReader::SplitBySpace(rows[i])[1]);
        currentRow.cardHand = FileReader::SplitBySpace(rows[i])[0];
        cardHands.push_back(currentRow);

        vector<string> characterSplit = FileReader::SplitByCharacter(FileReader::SplitBySpace(rows[i])[0]);
        for (int j = 0; j < characterSplit.size(); ++j) {
            hand.AddDuplicates(characterSplit[j], CardStringToInt(characterSplit[j]));
        }

        int seenBeforeSize, amountOfDuplication;
        hand.GetProperties(seenBeforeSize, amountOfDuplication);

        if(amountOfDuplication == 5 && seenBeforeSize == 1) handType.push_back(FiveK);
        else if(amountOfDuplication == 4 && seenBeforeSize == 2) handType.push_back(FourK);
        else if(amountOfDuplication == 5 && seenBeforeSize == 2) handType.push_back(FH);
        else if(amountOfDuplication == 3) handType.push_back(ThreeK);
        else if(amountOfDuplication == 4 && seenBeforeSize == 3) handType.push_back(TP);
        else if(amountOfDuplication == 2) handType.push_back(OP);
        else if(amountOfDuplication == 0) handType.push_back(HC);
        else handType.push_back(NA);
    }

    vector<PQueue<string>> groups;

    //sort the cards
    PQueue<Row> categories;
    for (int i = 0; i < cardHands.size(); ++i) {
        HandType type = handType[i];
        int typeInt = static_cast<int>(type);
        categories.AddMax(cardHands[i], typeInt);
    }

//    categories.Print();

    constexpr std::array<HandType, 8> allHandTypes = {
            FiveK, FourK, FH, ThreeK, TP, OP, HC, NA
    };

    //create groups of cards by priority
    PQueue<PQueue<Row>> group;
    for (int i = 0; i < 8; ++i) {
        PQueue<Row> same;
        for (int j = 0; j < categories.size(); ++j) {
            if (categories.getPriority(j) == allHandTypes[i]) {

                string current = categories.get(j).cardHand;
                int cardValue = CardStringToInt(string(1, categories.get(j).cardHand[0]));
                if (same.size() != 0) {
                    for (int k = 1; k < categories.get(j).cardHand.length(); ++k) {
                        cardValue = CardStringToInt(string(1, categories.get(j).cardHand[k]));
                        if (!same.ContainsPriority(cardValue)) break;
                    }
                }
                same.AddMax(categories.get(j), cardValue);
            }

            if(same.size() != 0) {
                Row firstElement = same.get(0);
                same.RemoveIndex(0);
                for (int k = 0; k < same.size(); ++k) {
                    int cardValue = CardStringToInt(string(1, firstElement.cardHand[k]));
                    if (!same.ContainsPriority(cardValue)) break;
                }
            }
        }

        cout << same.size() << endl;

        if (same.size() != 0) group.AddMin(same, i);
        if (same.size() != 0)
        {
            cout << "------Printing PQueue------" << endl;
            for (int h = 0; h < same.size(); ++h) {
                cout << "Priority " << same.getPriority(h) << ": " << same.get(h).cardHand << endl;
            }
        }
    }

    vector<Row> finalList;
    for (int i = 0; i < group.size(); ++i) {
        for (int j = 0; j < group.get(i).size(); ++j) {
            finalList.push_back(group.get(i).get(j));
        }
    }

    int total = 0;
    for (int i = 0; i < finalList.size(); ++i) {
        total += finalList[i].number * (finalList.size() - i);
    }
    cout << "Total: " << total << endl;
}

void DaySix(vector<string> rows){
    string timesS = FileReader::ReplaceAll(FileReader::SplitByKey(rows[0], ":"), " ", "")[1];
    string distancesS = FileReader::ReplaceAll(FileReader::SplitByKey(rows[1], ":"), " ", "")[1];
    vector<int> wins;

    cout << "Times: " << timesS << endl;
    cout << "Distances: " << distancesS << endl;

    long long times = stoll(timesS);
    long long distances = stoll(distancesS);

    int timeWin = 0;
    for (int j = 0; j < times; ++j) {
        if(j * (times - j) > distances) timeWin++;
    }
    cout << "Total: " << timeWin << endl; //29891250
}

long long ProcessSeed(long long seed, vector<vector<long long*>> ranges)
{
    long long seedValue = seed;
    for (int i = 0; i < ranges.size(); ++i) { //map
//        cout << "Current Map: " << i << endl;
        for (int j = 0; j < ranges[i].size(); j+=2) { //ranges
            //j = source
            //j+1  = destination
            long long sourceMin = ranges[i][j][0];
            long long sourceMax = ranges[i][j][1];
            long long destinationMax = ranges[i][j+1][1];
//            cout << "Source Min: " << sourceMin << endl;
//            cout << "Source Max: " << sourceMax << endl;
//            cout << "Destination Max: " << destinationMax << endl;
            if((seedValue >= sourceMin && seedValue <= sourceMax) || seedValue == sourceMin || seedValue == sourceMax)
            {
                long long difference = sourceMax - seedValue;
                seedValue = destinationMax - difference;
//                cout << "Fit in Range | New Value: " << seedValue << endl;
                break;
            }
//            cout << "--------" << endl;
        }
//        cout << "Current Seed Value: " << seedValue << endl;
//        cout << endl;
    }
    return seedValue;
}
void DayFive_2(vector<vector<string>> rows){
    //3 values deep
    //[i][j][k]
    //i = mapIndex
    //j = rowIndex
    //k = ranges (0 = source, 1 = destination)
    vector<vector<long long*>> ranges;

    for (int i = 1; i < rows.size(); ++i) { //loop rows -- skip seeds
        vector<long long*> r;
        for (int j = 1; j < rows[i].size(); ++j) { //loop map rows -- skip title
            vector<long long> mapNumbers = FileReader::StringToLong(FileReader::SplitBySpace(rows[i][j]));
            long long range = mapNumbers[2];
//            cout << "Range: " << range  << endl;
            long long *sourceRange = new long long[2];
            sourceRange[0] = mapNumbers[1];
            sourceRange[1] = mapNumbers[1] + range;

            long long *destinationRange = new long long[2];
            destinationRange[0] = mapNumbers[0];
            destinationRange[1] = mapNumbers[0] + range;

            r.push_back(sourceRange);
            r.push_back(destinationRange);
        }
        ranges.push_back(r);
    }

    vector<string> s = FileReader::RemoveEmpty(FileReader::SplitBySpace(FileReader::SplitByKey(rows[0][0], ":")[1]));
    vector<long long> seeds = FileReader::StringToLong(s);
    vector<long long*> pairs;
    for (int i = 0; i < seeds.size(); i+=2) {
        long long* pair = new long long[2];
        pair[0] = seeds[i]; //start
        pair[1] = seeds[i] + seeds[i+1]; //range
        pairs.push_back(pair);
    }

    long long lowest = LONG_LONG_MAX;
    for (int i = 0; i < pairs.size(); ++i) {
        for (long long  j = pairs[i][0]; j < pairs[i][1]; ++j) {
            long long value = ProcessSeed(j, ranges);
            if(value < lowest) lowest = value;
        }
    }
    cout << "Lowest: " << lowest << endl;

//    for (int i = 0; i < ranges.size(); ++i) {
//        for (int j = 0; j < ranges[i].size(); ++j) {
//            for (int k = 0; k < 2; ++k) {
//                cout << ranges[i][j][k] << " ";
//            }
//            cout << endl;
//        }
//        cout << "----------" << endl;
//    }
}
void DayFive(vector<vector<string>> rows){
    //Part 1 input
    vector<string> s = FileReader::RemoveEmpty(FileReader::SplitBySpace(FileReader::SplitByKey(rows[0][0], ":")[1]));
    vector<long long> seeds = FileReader::StringToLong(s);

    for (int i = 1; i < rows.size(); ++i) { //loop rows -- skip seeds
        vector<int> alreadyMappedSeedIndex;
        for (int j = 1; j < rows[i].size(); ++j) { //loop map rows -- skip title
            vector<long long> mapNumbers = FileReader::StringToLong(FileReader::SplitBySpace(rows[i][j]));
            long long range = mapNumbers[2];
            long long sourceRange[] = {mapNumbers[1], mapNumbers[1] + range};
            long long destinationRange[] = {mapNumbers[0], mapNumbers[0] + range};
            for (int k = 0; k < seeds.size(); ++k) {
                if(!FileReader::Contains(alreadyMappedSeedIndex, k)) {
                    if ((seeds[k] >= sourceRange[0] && seeds[k] <= sourceRange[1]) || seeds[k] == sourceRange[0] || seeds[k] == sourceRange[1])
                    {
                        long long difference = sourceRange[1] - seeds[k];
                        seeds[k] = destinationRange[1] - difference;
                        alreadyMappedSeedIndex.push_back(k);
                    }
                }
            }
        }
    }

    long long minElement = *min_element(seeds.begin(), seeds.end());
    cout << "Min: " << minElement << endl;
}

void DayFour_2(vector<string> rows){
    vector<int> copyMatches;
    vector<vector<string>> copyCardRows;
    for (int i = 0; i < rows.size(); ++i) {
        vector<string> winningNumbers = FileReader::RemoveEmpty(
                FileReader::SplitBySpace(FileReader::SplitByKey(FileReader::SplitByKey(rows[i], "|")[0], ":")[1]));
        vector<string> numbers = FileReader::RemoveEmpty(
                FileReader::SplitBySpace(FileReader::SplitByKey(rows[i], "|")[1]));
        int matches = 0;
        for (int j = 0; j < numbers.size(); ++j) {
            for (int k = 0; k < winningNumbers.size(); ++k) {
                if(numbers[j] == winningNumbers[k])
                {
                    matches++;
                }
            }
        }
        copyMatches.push_back(matches);

        vector<string> copyCard;
        for (int j = i+1; j < min(i+matches+1, static_cast<int>(rows.size())); ++j) {
            copyCard.push_back(rows[j]);
        }
        copyCardRows.push_back(copyCard);
    }
    vector<int> totalCards;
    for (int i = 0; i < copyMatches.size(); ++i) {
        totalCards.push_back(1);
    }

    for (int i = 0; i < copyMatches.size(); ++i) {
        for (int k  = 0; k < totalCards[i]; ++k) {
            for (int j = i + 1; j < i + copyMatches[i] + 1; ++j) {
                totalCards[j]++;
            }
        }
    }

    int total = 0;
    for (int i = 0; i < totalCards.size(); ++i) {
        total += totalCards[i];
    }
    cout << "Total: " << total << endl;
}
void DayFour(vector<string> rows){
    int total = 0;
    for (int i = 0; i < rows.size(); ++i) {
        vector<string> winningNumbers = FileReader::RemoveEmpty(
                FileReader::SplitBySpace(FileReader::SplitByKey(FileReader::SplitByKey(rows[i], "|")[0], ":")[1]));
        vector<string> numbers = FileReader::RemoveEmpty(
                FileReader::SplitBySpace(FileReader::SplitByKey(rows[i], "|")[1]));
        int points = 0;
        for (int j = 0; j < numbers.size(); ++j) {
            for (int k = 0; k < winningNumbers.size(); ++k) {
                if(numbers[j] == winningNumbers[k])
                {
                    points *= 2;
                    if(points == 0) points = 1;
                }
            }
        }
        total += points;
    }
    cout << "Total: " << total << endl;
}

void DayThree_2(vector<vector<string>> rows2D, vector<string> rows){
    int total = 0;
    for (int i = 0; i < rows2D.size(); ++i) {
        vector<string> row = FileReader::SplitByKey(rows[i], FileReader::symbolsList);
        if(row.empty()) continue;

        int numberIndex = 0;
        bool lastWasNumber = false;
        bool checkedNumber = false;

        for (int j = 0; j < rows2D[i].size(); ++j) {
            if(!FileReader::Contains(string(1, rows[i][j]), FileReader::symbolsList, false))
            {
                if(FileReader::CheckForNeighbor(rows2D, "*", i, j, 1)){
                    vector<int> pos = FileReader::GetNeighbor(rows2D, "*", i, j, 1);
                    if(FileReader::CheckForNeighbor(rows2D, FileReader::numbersList, pos[0], pos[1], 1)) {
                    
                    }
                }
            }
            else if(lastWasNumber)
            {

            }
        }
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