
#include <iostream>
#include <numeric>
#include <vector>

#if 0
// Imperative version
double average_score(const std::vector<int> &scores)
{
    int sum = 0;

    for (int score: scores) {
        sum += score;
    }

    return sum / (double) scores.size();
}
#endif

double average_score(const std::vector<int> &scores)
{
    return std::accumulate(
               scores.cbegin(), scores.cend(),
               0
            ) / (double) scores.size();
}

double scores_product(const std::vector<int> &scores)
{
    return std::accumulate(
               scores.cbegin(), scores.cend(),
               1,
               std::multiplies<int>()
            );
}

int main(int argc, char *argv[])
{
    std::cout << average_score({1, 2, 3, 4}) << '\n';
    std::cout << scores_product({1, 2, 3, 4}) << '\n';
}

