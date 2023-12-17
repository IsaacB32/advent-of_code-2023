#include "FileReader.h"
#include "PQueue.h"
#include <unordered_map>
#include <cmath>
#include <utility>
#include <algorithm>

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
void DayEight_2(vector<string> rows);
void DayNine(vector<string> rows);
void DayNine_2(vector<string> rows);
void DayTen(vector<vector<string>> rows2D);
void DayEleven(vector<vector<string>> rows2D, vector<vector<string>> columns);
void DayTwelve(vector<string> rows);
void DayThirdteen(vector<string> rows);
void DayFourteen(vector<vector<string>> rows);

void DayFiveteen(vector<string> rows);

void DaySixteen(vector<string> rows);

int main() {
//    vector<vector<string>> rows2D = FileReader::ReadFileRowsByKey(filePath, "");
    vector<string> rows = FileReader::ReadFileRows(filePath);
//    vector<vector<string>> columns = FileReader::ReadFileColumnsByKey(filePath, " ");
//    vector<vector<string>> cutRows = FileReader::CutRowsByKey(rows, "");

    DaySixteen(rows);
}
struct component;
struct contraption;

struct contraption{
    int distance;
    component* componentType;

    contraption(){
        distance = INT32_MAX;
        componentType = nullptr;
    }
    contraption(int d, component* ct){
        distance = d;
        componentType = ct;
    }
};
struct component{
    contraption contraptions[4]; // 0 = north, 1 = east, 2 = south, 3 = west;
    char componentType;
    pair<int,int> position;

    component(){
        componentType = '.';
    }
    component(pair<int,int> pos){
        componentType = '.';
        position = pos;
    }
    component(contraption c[4], char ct, pair<int,int> pos) {
        copy(c, c + 4, contraptions);
        componentType = ct;
        position = pos;
    }

    bool samePosition(pair<int,int> p) const{
        return (p.first == position.first && p.second == position.second);
    }
    bool onlyOnePosition(pair<int,int> &p){
        pair<int,int> savedPos = {p.first, p.second};
        if(p.first == position.first) {
            p.first = -1;
        }
        else if(p.second == position.first){
            p.second = -1;
        }
        return !samePosition(savedPos) && (savedPos.first == position.first || savedPos.second == position.second);
    }

    friend ostream &operator<<(ostream& os, const component& other)  {
        os << "Pos (" << other.position.first << ", " << other.position.second << ") Type: " << other.componentType;
        return os;
    }
    bool operator==(const component& other) const{
        return this->samePosition(other.position);
    }
};
bool isComponent(char c)
{
   if(c == '.') return false;
   return true;
}
vector<component> travledComponents;
void followLight(component starting, int direction){ // 0 = up, 1 = right, 2 = down, 3 = left
    if(FileReader::Contains(travledComponents, starting)) return;
    travledComponents.push_back(starting);

    char componentType = starting.componentType;
    int nextDirection = -1;
    if(componentType == '\\'){
        if(direction == 0) nextDirection = 3;
        else if(direction == 1) nextDirection = 2;
        else if(direction == 2) nextDirection = 1;
        else nextDirection = 0;
    }
    else if(componentType == '/'){
        if(direction == 0) nextDirection = 1;
        else if(direction == 1) nextDirection = 0;
        else if(direction == 2) nextDirection = 3;
        else nextDirection = 2;
    }
    else if(componentType == '|'){
        if(direction == 0){
            nextDirection = 0;
        }
        else if(direction == 1 || direction == 3){
            nextDirection = 0;
            int splitDirection = 2;
            if(starting.contraptions[splitDirection].distance != INT32_MAX){
                component next = *starting.contraptions[splitDirection].componentType;
                followLight(next, splitDirection);
            }
        }
        else {
            nextDirection = 2;
        }
    }
    else if(componentType == '-'){
        if(direction == 1){
            nextDirection = 1;
        }
        else if(direction == 0 || direction == 2){
            nextDirection = 1;
            int splitDirection = 3;
            if(starting.contraptions[splitDirection].distance != INT32_MAX){
                component next = *starting.contraptions[splitDirection].componentType;
                followLight(next, splitDirection);
            }
        }
        else {
            nextDirection = 3;
        }
    }

    if(starting.contraptions[nextDirection].distance != INT32_MAX){
        component next = *starting.contraptions[nextDirection].componentType;
        followLight(next, nextDirection);
    }
    else { //the light ends
        return;
    }
}
void DaySixteen(vector<string> rows)
{
    vector<component> components;
    for (int i = 0; i < rows.size(); ++i) {
        for (int j = 0; j < rows[i].size(); ++j) {
            if(isComponent(rows[i][j])){
                component c({i, j});
                c.componentType = rows[i][j];
                components.push_back(c);
            }
        }
    }

    for (int i = 0; i < components.size(); ++i) {

        //reset shortest
        pair<int,component*> shortestSideDistance[4];
        for (int h = 0; h < 4; ++h) {
            shortestSideDistance[h] = {INT32_MAX, &components[0]};
        }

        component* current = &components[i];
        contraption contraptions[4];
        for (int j = 0; j < components.size(); ++j) {
            pair<int,int> lookingPos = components[j].position;
            if(current->onlyOnePosition(lookingPos)){
                int distance;
                pair<int,int> index;
                if(lookingPos.first == -1){ //x
                    distance = current->position.second - components[j].position.second;
                    index.first = 1; //less than
                    index.second = 3; //greater than
                }
                else { //y
                    distance = current->position.first - components[j].position.first;
                    index.first = 2; //less than
                    index.second = 0; //greater than
                }

                if(distance < 0){
                    int absDis = abs(distance);
                    if(absDis < shortestSideDistance[index.first].first) shortestSideDistance[index.first] = {absDis, &components[j]};
                }
                else {
                    if (distance < shortestSideDistance[index.second].first) shortestSideDistance[index.second] = {distance, &components[j]};
                }
            }
        }
        for (int j = 0; j < 4; ++j) {
            if(shortestSideDistance[j].first != INT32_MAX){
                contraption con(shortestSideDistance[j].first, shortestSideDistance[j].second);
                contraptions[j] = con;
            }
        }
        copy(contraptions, contraptions + 4, current->contraptions);
    }

    followLight(components[0], 1);
    
    int distance = 0;
    for (int i = 0; i < travledComponents.size()-1; ++i) {
        distance += abs(travledComponents[i].position.first - travledComponents[i + 1].position.first) + abs(travledComponents[i].position.second - travledComponents[i + 1].position.second);
    }
    distance += travledComponents.size();
    cout << "total distance: " << distance;
}

int doHash(char c, int currentValue){
    int value = (unsigned char)c;
    value += currentValue;
    value *= 17;
    value %= 256;

    currentValue = value;
    return currentValue;
}
struct command{
    string label;
    int focusLength;
    bool operator==(const command& other) const {
        return label == other.label;
    }
};
void DayFiveteen(vector<string> rows) {
    vector<command> hashMap[256];
    vector<string> steps = FileReader::SplitByKey(rows[0], ",");
    long long total = 0;
    for (int i = 0; i < steps.size(); ++i) {
        int hashValue = 0;
        command currentCommand;
        bool hasEqual = false;
        if (FileReader::Contains(steps[i], "=")) {
            string label = FileReader::SplitByKey(steps[i], "=")[0];
            int lenseIndex = stoi(FileReader::SplitByKey(steps[i], "=")[1]);
            currentCommand.label = label;
            currentCommand.focusLength = lenseIndex;
            hasEqual = true;
        } else {
            currentCommand.label = FileReader::SplitByKey(steps[i], "-")[0];
            currentCommand.focusLength = -1;
        }

        for (int j = 0; j < currentCommand.label.size(); ++j) {
            hashValue = doHash(currentCommand.label[j], hashValue);
        }

        vector<command> box = hashMap[hashValue];
        if (hasEqual) {
            if (!box.empty() && FileReader::Contains(box, currentCommand)) {
                auto indexBefore = find(box.begin(), box.end(), currentCommand);
                box.erase(std::remove(box.begin(), box.end(), currentCommand), box.end());
                box.insert(indexBefore, currentCommand);
            }
            else{
                box.push_back(currentCommand);
            }
        }
        else{
            if (!box.empty() && FileReader::Contains(box, currentCommand)) {
                auto indexBefore = find(box.begin(), box.end(), currentCommand);
                box.erase(std::remove(box.begin(), box.end(), currentCommand), box.end());
            }
        }
        hashMap[hashValue] = box;
    }

    for (int i = 0; i < 256; ++i) {
        vector<command> box = hashMap[i];
        if(!box.empty())
        {
            int lense = 0;
            for (int j = 0; j < box.size(); ++j) {
                lense += (1 + i) * (j+1) * box[j].focusLength;
            }
            total += lense;
        }
    }
    cout << "Total: " << total << endl;
}

struct rolledDir{
    vector<string> lines;
    int direction = 0;
    int order = 0;
    bool operator==(const rolledDir& other) const {
        return lines == other.lines && direction == other.direction;
    }
};
namespace std {
    template<>
    struct hash<rolledDir> {
        std::size_t operator()(const rolledDir &rd) const {
            // Combine the hash values of lines and direction
            std::size_t hashValue = 0;
            for (const auto &line: rd.lines) {
                hashValue ^= hash<std::string>()(line) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            }
            hashValue ^= hash<int>()(rd.direction) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            return hashValue;
        }
    };
}
string SwapRocks(string lineOfRocks, int& beginIndex, int i, int increment){
    char current = lineOfRocks[i];
    if(current == 'O'){
        swap(lineOfRocks[i], lineOfRocks[beginIndex]);
        beginIndex = beginIndex + increment;
    }
    else if(current == '#') {
        beginIndex = i + increment;
    }
    return lineOfRocks;
}
string moveRocks(string lineOfRocks, bool isLeft){
    int beginIndex;
    if(!isLeft)
    {
        beginIndex = (int)lineOfRocks.length()-1;
        for (int i = beginIndex; i >= 0; --i) {
            lineOfRocks = SwapRocks(lineOfRocks, beginIndex, i, -1);
        }
    }
    else
    {
        beginIndex = 0;
        for (int i = 0; i < lineOfRocks.length(); ++i) {
            lineOfRocks = SwapRocks(lineOfRocks, beginIndex, i, 1);
        }
    }
    return lineOfRocks;
}
long long countRocks(string item){
    long long total = 0;
    for (int i = 0; i < item.length(); ++i) {
        if(item[i] == 'O') total += (long long)item.length() - i;
    }
    return total;
}
void DayFourteen(vector<vector<string>> rows2D) {
    vector<string> columns = FileReader::GetColumnsFromRows2D(rows2D);
    vector<string> rows = FileReader::GetRowsFromRows2D(rows2D);

    vector<vector<string>> savedKeys;
    unordered_map<rolledDir, pair<vector<string>, int>> seenBefore;
    rolledDir current;
    current.lines = columns;
    current.direction = 0;
    bool direction[] = {true, true, false, false};

    long long total = 0;
    for (int bigloop = 0; bigloop < 1000000000; ++bigloop) {
        rolledDir east;
        for (int k = 0; k < 4; ++k) {
            bool dir = direction[k];
            rolledDir rolledPlatform;
            rolledPlatform.direction = k;
            for (int i = 0; i < current.lines.size(); ++i) {
                string rolledItem = moveRocks(current.lines[i], dir);
                rolledPlatform.lines.push_back(rolledItem);
            }
            if (k == 3) east = current;
            current.direction = k;
            current.lines = FileReader::FlipRowsToColumns(rolledPlatform.lines);
            current.order = bigloop;
        }
        if (seenBefore.find(east) != seenBefore.end()) { //we have found duplicate
            int loopLeft = 1000000000 - seenBefore[east].second;
            int lengthOfLoop = seenBefore.size() - seenBefore[east].second;
            int indexOfAnswer = (seenBefore[east].second - 1) + (loopLeft % lengthOfLoop);
            vector<string> flippedCurrent = FileReader::FlipRowsToColumns(savedKeys[indexOfAnswer]);
            for (const string &line: flippedCurrent) {
                total += countRocks(line);
            }
            break;
        } else
        {
            savedKeys.push_back(east.lines);
            seenBefore[east].first = east.lines; //we haven't seen before
            seenBefore[east].second = bigloop; //we haven't seen before
        }
    }
    cout << "Total: " << total << endl;
}

int reflectedIndex(int reflection, int i) {return (i + abs(reflection - i) * 2) - 1;}
int checkForSingleDifference(string a, string b) {
    int indexOfDifference = 0, diff = 0;
    for (int i = 0; i < a.length(); ++i) {
        if(a[i] != b[i]) {
            diff++;
            indexOfDifference = i;
        }
    }
    if(diff == 1) return indexOfDifference;
    return -1;
}
bool testReflection(vector<string> reflection){
    for (int i = 0; i < reflection.size() / 2; ++i) {
        string current = reflection[i];
        string reflect = reflection[reflectedIndex((int)reflection.size() / 2, i)];
        if(current != reflect) return false;
    }
    return true;
}
bool CheckReflection(vector<string> map, int& total) {
    for (int i = 0; i < map.size(); ++i) {
        for (int reflectionIndex = i + 1; reflectionIndex < map.size(); ++reflectionIndex) {
            int rIndex = reflectedIndex(reflectionIndex, i);
            if (rIndex >= map.size()) break;

            const string& current = map[i];
            const string& next = map[rIndex];
            int indexOfDifference = checkForSingleDifference(current, next);
            if(indexOfDifference != -1) //contains a single difference
            {
                map[i][indexOfDifference] = (map[i][indexOfDifference] == '.') ? '#' : '.';

                vector<string> testingReflection;
                int reflectionSpace = min(reflectionIndex, (int)map.size() - reflectionIndex);
                int startingIndex = reflectionIndex - reflectionSpace;
                int endingIndex = reflectionIndex + reflectionSpace;
                for (int j = startingIndex; j < endingIndex; ++j) {
                    testingReflection.push_back(map[j]);
                }

                if(testReflection(testingReflection)){
                    total = reflectionIndex;
                    return true;
                }

                map[i][indexOfDifference] = (map[i][indexOfDifference] == '.') ? '#' : '.';
            }
        }
    }
    return false;
}
int findReflectionIndex(vector<string> rows){
    int total = 0;
    //row reflection:
    if(CheckReflection(rows, total)) return 100*total;

    vector<string> columns;
    for (int i = 0; i < rows[0].size(); ++i) {
        string c;
        for (int j = 0; j < rows.size(); ++j) {
            c += rows[j][i];
        }
        columns.push_back(c);
    }

    //column reflection:
    total = 0;
    if(CheckReflection(columns, total)) return total;
    return -1;
}
void DayThirdteen(vector<string> rows){
    int total = 0;
    vector<vector<string>> map = FileReader::CutRowsByKey(rows, "");
    for (int i = 0; i < map.size(); ++i) {
        int f = findReflectionIndex(map[i]);
        total += f;
//        cout << "(" << i << "): " << f << endl;
    }
    cout << "Total: " << total << endl;
}

vector<int> cutVector(vector<int> v, int indexToCut){
    vector<int> cut;
    for (int i = indexToCut+1; i < v.size(); ++i) {
        cut.push_back(v[i]);
    }
    return cut;
}
int countFitsPair(int length, vector<int> groups) {
    int add = 0;
    int secondGroupAmount = (length - (groups[1] - 1));
    for (int i = 0; i < length; i++) {
        int numGroup = secondGroupAmount - (i + groups[0] + 1);
        if (numGroup <= 0) break;
        add += numGroup;
    }
    return add;
}
int countFitsHelper(int length, vector<int> groups, int& add)
{
    if(groups.empty()) return 0;
    else if(groups.size() == 1) return length - groups[0] - 1;
    else if(groups.size() == 2) return add + countFitsPair(length, groups);

    int firstGroupAmount = (length - (groups[0] - 1));
    for (int i = 0; i < length; ++i) {
        int l = firstGroupAmount - (i + groups[0] + 1);
        if(l <= groups[1] + 1) break;
        add += countFitsHelper(l, cutVector(groups, i), add);
    }
    return add;
}
int countFits(int length, vector<int> groups){
    int add = 0;
    return countFitsHelper(length, std::move(groups), add);
}
long long factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        return n * factorial(n - 1);
    }
}
long long calculateCombinations(int n, int k) {
    return factorial(n) / (factorial(k) * factorial(n - k));
}
long long choose(int length, const vector<int>& groups)
{
    int n = length - 1 - (FileReader::SumVector(groups) - (int)groups.size());
    int k = (int)groups.size();
    cout << "n: " << n << endl;
    cout << "k: " << k << endl;
    return calculateCombinations(n, k);
}
void DayTwelve(vector<string> rows){
    for (int i = 0; i < rows.size(); ++i) {
        vector<int> values = FileReader::StringToInt(FileReader::SplitByKey(FileReader::SplitBySpace(rows[i])[1], ","));

        string hotSprings = FileReader::ReplaceLine(FileReader::SplitBySpace(rows[i])[0], "#","?");
        vector<int> groupSize = FileReader::CountUniqueOccurancesSize(hotSprings, '?');
    }

    vector<int> testing = {1,3,1,6};
    cout << choose(15,testing) << endl;

    vector<int> testing2 = {1,1};
    cout << choose(5, testing2);
//    cout << countFits(9,testing2);

    /*for (int i = 0; i < rows.size(); ++i) {
        string hotSprings = FileReader::SplitBySpace(rows[i])[0];
        int currentValuesIndex = 0;
        vector<int> values = FileReader::StringToInt(FileReader::SplitByKey(FileReader::SplitBySpace(rows[i])[1], ","));
        for (int j = 1; j < hotSprings.length() - 1; ++j) {
            char prev = hotSprings[i - 1];
            char next = hotSprings[i + 1];
            if (hotSprings[i] == '#') {
                bool nextFilled = true;
                int indexOfNext = FileReader::IndexOf(hotSprings, i, '#');
                for (int k = i; k < i + indexOfNext; ++k) {
                    if (hotSprings[k] != '#' && hotSprings[k] != '?') {
                        nextFilled = false;
                        break;
                    }
                }
                if (nextFilled) {
                    prev = '.';
                    for (int k = i; k < i + indexOfNext; ++k) {
                        hotSprings[k] = '#';
                    }
                }
                //check next values[currenValueIndex] == '#'
                //true set prev and next = '.' also increase currentValue index
                //false check next values.substring(i, indexOf('#')) == values[currentValuesIndex]
                //true set that entire substring to '#'
            }
            hotSprings[i - 1] = prev;
            hotSprings[i + 1] = prev;
        }
//        cout << "Hot Springs: " << hotSprings << endl;
    }*/
}


long long findDistance(pair<long long,long long> a, pair<long long,long long> b){
    long long distance = abs(a.first - b.first) + abs(a.second - b.second);
    return distance;
}
void DayEleven(vector<vector<string>> rows2D, vector<vector<string>> columns){
    vector<pair<long long,long long>> positions;
    string galexyString = "#";
    long long xIndex =0,yIndex=0;
    for (int i = 0; i < rows2D.size(); ++i) {
        if(!FileReader::Contains(rows2D[i], galexyString)){
            xIndex+=1000000;
        }
        else xIndex++;
        yIndex = 0;
        for (int j = 0; j < rows2D[i].size(); ++j) {
            if(!FileReader::Contains(columns[j], galexyString)){
                yIndex+=1000000;
            }
            else yIndex++;
            if(rows2D[i][j] == "#") positions.emplace_back(yIndex,xIndex);
        }
    }

    long long totalDistances = 0;
    vector<pair<long long,long long>> compared;
    for (long long i = 0; i < positions.size(); ++i) {
        for (long long j = 0; j < positions.size(); ++j) {
            if (FileReader::Contains(compared, {i, j})) continue;
            if (j == i) continue;

            compared.emplace_back(j, i);
            long long distance = findDistance(positions[i], positions[j]);
            totalDistances += distance;
        }
    }
    cout << "Total Distance: " << totalDistances << endl;

}

vector<vector<string>> FindPipe(const string& pipe) {
    vector<vector<string>> possiblePipeValues;
    if (pipe == "F") {
        possiblePipeValues = {{"-","7","J"},{"|","L","J"}};
    } else if (pipe == "L") {
        possiblePipeValues = {{"|","F","7"},{"7","J","-"}};
    } else if (pipe == "7") {
        possiblePipeValues = {{"-","F","L"},{"|","L","J"}};
    } else if (pipe == "J") {
        possiblePipeValues = {{"-","F","L"},{"|","F","7"}};
    } else if (pipe == "|") {
        possiblePipeValues = {{"|","F","7"}, {"|","L","J"}};
    } else if (pipe == "-") {
        possiblePipeValues = {{"-","F","L"}, {"7","J","-"}};
    } else if (pipe == ".") {
        possiblePipeValues = {};
    } else if (pipe == "S"){
        possiblePipeValues = {{"F","L","-"},{"F","7","|"},{"J","7","-"},{"L","J","|"}};
    }
    return possiblePipeValues;
}
vector<pair<int,int>> FindCoords(const string& pipe){
    vector<pair < int, int>> possibleCoords;
    if(pipe == "F"){
        possibleCoords = {{1,0},{0,1}};
    }
    else if(pipe == "L"){
        possibleCoords = {{0,-1},{1,0}};
    }
    else if(pipe == "7"){
        possibleCoords = {{-1,0},{0,1}};
    }
    else if(pipe == "J"){
        possibleCoords = {{-1,0},{0,-1}};
    }
    else if(pipe == "|"){
        possibleCoords = {{0,-1},{0,1}};
    }
    else if(pipe == "-"){
        possibleCoords = {{-1,0},{1,0}};
    }
    else if(pipe == "."){
        possibleCoords = {};
    }
    else if(pipe == "S"){
        possibleCoords = {{-1,0},{0,-1},{1,0},{0,1}};
    }
    return possibleCoords;
}
void DayTen(vector<vector<string>> row2D){
    pair<int,int> startIndex;
    string startString = "S";
    for (int i = 0; i < row2D.size(); ++i) {
        if(!FileReader::Contains(row2D[i], startString)) continue;
        for (int j = 0; j < row2D[i].size(); ++j) {
            if(row2D[i][j] == "S") {
                startIndex.first = j;
                startIndex.second = i;
                break;
            }
        }
        break;
    }

    vector<pair<int,int>> path;
    //left = [x-1][y]
    //right = [x+1][y]
    //up =   [x][y+1]
    //down = [x][y-1]

    //F = look down , look right
    //L = look up, look right
    //J = look up, look left
    //7 = look down, look left
    //- = look left, look right
    //| = look up, look down
    pair<int,int> prevIndex = {-1,-1};
    cout << "starting index: " << startIndex.first << ", " << startIndex.second << endl;
    pair<int,int> currentIndex = startIndex;
    string currentPipe = "S";
    do {
        vector<vector<string>> possiblePipes = FindPipe(currentPipe);
        vector<pair<int,int>> coords = FindCoords(currentPipe);
        for (int i = 0; i < possiblePipes.size(); ++i) {
            pair<int,int> point = {currentIndex.first + coords[i].first, currentIndex.second + coords[i].second};
            if((point.first < 0 || point.second < 0) || point == prevIndex) continue;

            string current = row2D[point.second][point.first];
//            FileReader::PrintVector(possiblePipes[i]);
//            cout << "point: " << point.first << ", " << point.second << endl;
//            cout << "current: " << current << endl;
            if(FileReader::Contains(possiblePipes[i], current))
            {
                prevIndex = currentIndex;

                currentPipe = current;
                currentIndex = point;

                path.push_back(currentIndex);
                break;
            }
            else if(current == "S") currentPipe = "S";
        }
    } while(currentPipe != "S");

    int total = path.size();
    if(path.size() % 2 != 0) total = (total / 2) + 1;
    else total = total / 2;
    cout << "Total: " << total << endl;
}

bool allConstant(vector<int> row, int& adder){
    int firstValue = row[0];
    for (int i = 1; i < row.size(); ++i) {
        if(row[i] != firstValue) return false;
    }
    adder = firstValue;
    return true;
}
vector<int> findDifferences(vector<int> row){
    vector<int> diffList;
    for (int i = 0; i < row.size()-1; i++) {
        int diff = row[i+1] - row[i];
        diffList.push_back(diff);
    }
    return diffList;
}
void DayNine_2(vector<string> row){
    vector<vector<int>> numbers;
    vector<int> nextValues;
    for (int i = 0; i < row.size(); ++i) {
        numbers.push_back(FileReader::StringToInt(FileReader::SplitBySpace(row[i])));
    }

    for (int i = 0; i < numbers.size(); ++i) {
        vector<int> currentRow = numbers[i];
        int nextValue = 0;
        vector<int> firstValues;
        firstValues.push_back(currentRow.front());
        while(!allConstant(currentRow, nextValue)){
            currentRow = findDifferences(currentRow);
            firstValues.push_back(currentRow.front());
        }

        int value = currentRow[0];
        for (int j = firstValues.size()-2; j >= 0; --j) {
            value = firstValues[j] - value;
        }
        nextValues.push_back(value);
    }

    int total = FileReader::SumVector(nextValues);
    cout << "Total: " << total << endl;
}
void DayNine(vector<string> row) {
    vector<vector<int>> numbers;
    vector<int> nextValues;
    for (int i = 0; i < row.size(); ++i) {
        numbers.push_back(FileReader::StringToInt(FileReader::SplitBySpace(row[i])));
    }

    for (int i = 0; i < numbers.size(); ++i) {
        vector<int> currentRow = numbers[i];
        int nextValue = 0;
        vector<int> lastValues;
        lastValues.push_back(currentRow.back());
        while(!allConstant(currentRow, nextValue)){
            currentRow = findDifferences(currentRow);
            lastValues.push_back(currentRow.back());
        }
        nextValues.push_back(FileReader::SumVector(lastValues));
    }

    int total = FileReader::SumVector(nextValues);
    cout << "Total: " << total << endl;
}

//Reddit and ChatGPT assisted answer (find_lcm and prime_factorization methods ChatGPT, LCM logic Reddit)
string CutOrder(string order, char isLeft){
    int sideIndex = (isLeft == 'L') ? 0 : 1;
    string split = FileReader::SplitBySpace(order)[sideIndex];
    if(sideIndex == 0) split = split.substr(1);
    else split = split.substr(0, split.length()-1);
    return split;
}
bool isKeysEnded(vector<string> keys){
    int count = 0;
    for (int i = 0; i < keys.size(); ++i) {
        if(keys[i][2] == 'Z') count++;
        if(count == keys.size()) return true;
    }
    return false;
}
std::vector<std::pair<long long, long long>> prime_factorization(long long n) {
    std::vector<std::pair<long long, long long>> factors;
    for (int i = 2; i <= n; ++i) {
        long long power = 0;
        while (n % i == 0) {
            ++power;
            n /= i;
        }
        if (power > 0) {
            factors.emplace_back(i, power);
        }
    }
    return factors;
}
long long find_lcm(const vector<long long>& numbers) {
    vector<std::pair<long long, long long>> lcm_factors;

    for (int num : numbers) {
        auto factors = prime_factorization(num);
        for (const auto& factor : factors) {
            auto it = std::find_if(lcm_factors.begin(), lcm_factors.end(),
                                   [&](const std::pair<long long, long long>& p) {
                                       return p.first == factor.first;
                                   });

            if (it == lcm_factors.end() || factor.second > it->second) {
                if (it != lcm_factors.end()) {
                    it->second = factor.second;
                } else {
                    lcm_factors.emplace_back(factor);
                }
            }
        }
    }

    long long lcm = 1;
    for (const auto& factor : lcm_factors) {
        lcm *= static_cast<long long>(std::pow(factor.first, factor.second));
    }

    return lcm;
}
void DayEight_2(vector<string> rows){
    vector<string> instructions = FileReader::SplitByCharacter(rows[0]);

    //<key, value>
    unordered_map<string, string> map;
    for (int i = 2; i < rows.size(); ++i) {
        string key = (FileReader::SplitByKey(rows[i],"=")[0]).substr(0, 3);
        string order = FileReader::ReplaceLine(FileReader::SplitByKey(rows[i],"=")[1], ",", "").substr(1);
        map[key] = order;
    }

    vector<string> startingKeys;
    for (int i = 2; i < rows.size(); ++i) {
        string key = FileReader::SplitByKey(rows[i],"=")[0].substr(0, 3);
        if(key[2] == 'A') startingKeys.push_back(key);
    }

    int loopCounts[startingKeys.size()];
    fill(loopCounts, loopCounts + startingKeys.size(), 0);

    string loopCheck[startingKeys.size()];
    long long stepCounter = 0;
    string loopCheckString;
    while(!isKeysEnded(startingKeys)) //&& loop Check >= 0
    {
        for (int i = 0; i < startingKeys.size(); ++i) {
            if(loopCheck[i] == "Done") continue;
            else loopCounts[i]++;

            string order = map[startingKeys[i]];
            char direction = instructions[stepCounter % instructions.size()][0];
            startingKeys[i] = CutOrder(order, direction);

            if(startingKeys[i][2] == 'Z')
            {
                if(loopCheck[i] == startingKeys[i])
                {
                    //must be looping somewhere
                    loopCheck[i] = "Done";

                }
                else loopCheck[i] = startingKeys[i];
            }
        }
        stepCounter++;
    }
    cout << "Total Steps: " << stepCounter << endl;

    vector<long long> counts;
    for (int i = 0; i < startingKeys.size(); ++i) {
        counts.push_back(loopCounts[i] / 2);
    }
    long long total = find_lcm(counts);
//
//    for (int i = 0; i < startingKeys.size(); ++i) {
//        cout << "Key: " << startingKeys[i] << endl;
//    }
    cout << "Total: " << total << endl;
} //37922 to low
void DayEight(vector<string> rows){
    vector<string> instructions = FileReader::SplitByCharacter(rows[0]);

    //<key, value>
    unordered_map<string, string> map;
    for (int i = 2; i < rows.size(); ++i) {
        string key = (FileReader::SplitByKey(rows[i],"=")[0]).substr(0, 3);
        string order = FileReader::ReplaceLine(FileReader::SplitByKey(rows[i],"=")[1], ",", "").substr(1);
        map[key] = order;
    }


    int A_Index = 0;
    for (int i = 2; i < rows.size(); ++i) {
        string key = FileReader::SplitByKey(rows[i],"=")[0].substr(0, 3);
        if(key == "AAA")
        {
            A_Index = i;
            break;
        }
    }

    //int loopCheck = 66600000;
    long long stepCounter = 0;
    string loopCheckString;
    string key = (FileReader::SplitByKey(rows[A_Index],"=")[0]).substr(0, 3);
    while(key != "ZZZ" ) //&& loopCheck >= 0
    {
        string order = map[key];
        char direction = instructions[stepCounter % instructions.size()][0];
        key = CutOrder(order, direction);
        stepCounter++;
    }
    cout << "Total Steps: " << stepCounter << endl;
}

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