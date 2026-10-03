#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <string>
#include <omp.h>

int main(int argc, char* argv[])
{
    if (argc < 4) {
        std::cerr << "Ошибка: Неверное количество аргументов.\n";
        std::cerr << "Использование: " << argv[0] << " <потоки> <размер_массива> <тип_schedule: static|dynamic|guided>\n";
        return 1;
    }

    int threads = std::stoi(argv[1]);
    int n = std::stoi(argv[2]);
    std::string sched_type = argv[3];

    if (n < 3) {
        std::cerr << "Размер массива должен быть не менее 3.\n";
        return 1;
    }

    omp_set_num_threads(threads);

    std::vector<double> a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = i;
    }

    std::vector<double> b(n, 0.0);
    b[0] = a[0];
    b[n - 1] = a[n - 1];

    auto start_time = std::chrono::high_resolution_clock::now();

    if (sched_type == "static") {
#pragma omp parallel for schedule(static)
        for (int i = 1; i < n - 1; ++i) {
            b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
        }
    }
    else if (sched_type == "dynamic") {
#pragma omp parallel for schedule(dynamic)
        for (int i = 1; i < n - 1; ++i) {
            b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
        }
    }
    else if (sched_type == "guided") {
#pragma omp parallel for schedule(guided)
        for (int i = 1; i < n - 1; ++i) {
            b[i] = (a[i - 1] + a[i] + a[i + 1]) / 3.0;
        }
    }
    else {
        std::cerr << "Неизвестный тип распределения! Используйте: static, dynamic или guided.\n";
        return 1;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end_time - start_time;

    
    std::cout << "Mode: " << sched_type << " | Threads: " << threads << " | N = " << n << "\n";
    std::cout << "Time: " << duration.count() << " ms.\n";
    std::cout << "Control element b[10]: " << b[10] << " (Expected 10)\n\n";

    return 0;
}


    