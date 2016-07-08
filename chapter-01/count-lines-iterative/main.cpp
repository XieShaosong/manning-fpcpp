
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

std::vector<int>
count_lines_in_files(const std::vector<std::string> &files)
{
    std::vector<int> results;
    char c = 0;

    for (const auto &file: files) {
        int line_count = 0;

        std::ifstream in(file);

        while (in.get(c)) {
            if (c == '\n') {
                line_count++;
            }
        }

        results.push_back(line_count);
    }

    return results;
}

int main(int argc, char *argv[])
{
    auto results = count_lines_in_files({"main.cpp"});

    for (auto result: results) {
        std::cout << result << " line(s)\n";
    }

    return 0;
}

