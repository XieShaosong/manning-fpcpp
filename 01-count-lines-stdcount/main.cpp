
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <algorithm>
#include <iterator>

int count_lines(const std::string &filename)
{
    std::ifstream in(filename);
    in.unsetf(std::ios_base::skipws);

    return std::count(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        '\n');
}

std::vector<int>
count_lines_in_files(const std::vector<std::string> &files)
{
    std::vector<int> results;

    for (const auto &file: files) {
        results.push_back(count_lines(file));
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

