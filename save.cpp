#include "save.h"

int save(float wpm, float time)
{

    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::fstream file;
    if (!(std::filesystem::exists(SAVE_PATH))){
        file.open(SAVE_PATH, std::ios::out);
        file.close();
    }
    file.open(SAVE_PATH, std::ios::out | std::ios::in);

    std::string line = std::to_string(wpm) + "," + std::to_string(time) + "," + std::ctime(&end_time);

    file << line << std::endl;
    file.close();

    return 0;
}
