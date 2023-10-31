#include <iostream>
#include <pthread.h>
#include <random>
#include <vector>
#include <mutex>
#include <chrono>

int rounds_count;
int count_same_rank;
int thread_count;

struct arg_t {
    int id;
    int thread_rounds_count;
};

//Один раунд метода
void Round() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 51);

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

//Подсчёт вероятности многопоточно
void* ThreadFunction(void* argument) {
    int* count = static_cast<int*>(argument);
    std::mutex mutex_count;
    for (int i = 0; i < *(count); ++i) {
        mutex_count.lock();
        Round();
        mutex_count.unlock();
    }
    pthread_exit(0);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::logic_error("Указано неполное число ключей");
    }

    count_same_rank = 0;
    rounds_count = atoi(argv[1]);
    thread_count = atoi(argv[2]);
    if (rounds_count < thread_count) {
        thread_count = rounds_count;
    }
    if (thread_count <= 0) {
        throw std::logic_error("Количество потоков не может быть меньше 1");
    }

    arg_t args[thread_count];
    if (rounds_count % thread_count == 0) {
        for (int i = 0; i < thread_count; ++i) {
            args[i].id = i;
            args[i].thread_rounds_count = rounds_count / thread_count;
        }
    } else {
        for (int i = 0; i < thread_count - 1; ++i) {
            args[i].id = i;
            args[i].thread_rounds_count = rounds_count / thread_count;
        }
        args[thread_count - 1].id = thread_count - 1;
        args[thread_count - 1].thread_rounds_count = rounds_count / thread_count + rounds_count % thread_count; 
    }

    auto start = std::chrono::steady_clock::now();

    pthread_t tid[thread_count];
    for (int i = 0; i < thread_count; ++i) {
        pthread_create(&tid[i], nullptr, ThreadFunction, &args[i].thread_rounds_count);
    }
    for (int i = 0; i < thread_count; ++i) {
        pthread_join(tid[i], nullptr);
    }
    double probability = static_cast<double>(count_same_rank) / rounds_count;
    std::cout << probability << '\n';

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << elapsed.count() << '\n';
}