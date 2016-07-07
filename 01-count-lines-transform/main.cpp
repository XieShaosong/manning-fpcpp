
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

    return std::count(
        std::istream_iterator<char>(in >> std::noskipws),
        std::istream_iterator<char>(),
        '\n');
}

std::vector<int>
count_lines_in_files(const std::vector<std::string> &files)
{
    std::vector<int> results(files.size());

    std::transform(files.cbegin(), files.cend(),
                   results.begin(),
                   count_lines);

    return results;
}

int main(int argc, char *argv[])
{
    auto results = count_lines_in_files({"main.cpp"});

    for (const auto &result: results) {
        std::cout << result << " line(s)\n";
    }

    return 0;
}

