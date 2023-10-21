#include <iostream>
#include "FileReader.h"


string filePath = R"(C:\The Main File ---------\Other Stuff\Code\AdventOfCode2023\Input.txt)";
int main() {
    int lineSize = FileReader::GetLineCount(filePath);
    string* lines = FileReader::ReadFile(filePath);
    for (int i = 0; i < lineSize; ++i) {
        cout << lines[i] << endl;
    }

    delete[] lines;
    return 0;
}
