#include "words.h"

int randomNumber(int start, int end) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(start, end);
    return dist(mt);
}

std::string getLine() {
    std::string line;
    int random = 0;
    int numOfLines = 0;
    std::ifstream file(WORD_PATH);
    std::vector<std::string> lines(200);

    while (file.is_open())  {
        for (auto i = 0; i < lines.size(); i++)
            std::getline(file, lines[i]);
        file.close();
    }

    return lines[randomNumber(0, 200)];
}
