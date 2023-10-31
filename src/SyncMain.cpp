#include <iostream>
#include <pthread.h>
#include <random>
#include <vector>
#include <mutex>
#include <chrono>

//Функция вычисления вероятности однопоточно
double Probability(int rounds_count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 51);

    int count_same_rank = 0;

    for (int i = 0; i < rounds_count; ++i) {
        std::vector<std::pair<int, int>> card_values(13);
        int index;
        for (int j = 0; j < 13; ++j) {
            card_values[j].first = j;
            card_values[j].second = 0;
        }
        int deck[52];
        for (int j = 0; j < 52; ++j) {
            deck[j] = j;
        }

        for (int j = 0; j < 52; ++j) {
            index = dis(gen);
            if (card_values[index % 13].second < 4) {
                card_values[index % 13].second += 1;
                std::swap(deck[j], deck[index]);
                continue;
            }
            index = dis(gen);
            card_values[index % 13].second += 1;
            std::swap(deck[j], deck[index]);
        }

        if (deck[0] % 13 == deck[1] % 13) {
            ++count_same_rank;
        }
    }
    double probability = static_cast<double>(count_same_rank) / rounds_count;

    return probability;
}

int main(int argc, char* argv[]) {
    auto start = std::chrono::steady_clock::now();
    double prob = Probability(atoi(argv[1]));
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << prob << '\n' << elapsed.count() << '\n';
}