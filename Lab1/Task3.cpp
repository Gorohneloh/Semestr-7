#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <omp.h>

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <threads> <method: 1|2|3|4|5>\n";
        return 1;
    }

    int num_threads = std::stoi(argv[1]);
    int method = std::stoi(argv[2]);
    omp_set_num_threads(num_threads);

    std::cout << "=== Method " << method << " | Threads: " << num_threads << " ===\n";
    // 1 Ordered clause in reverse loop
    if (method == 1) {
#pragma omp parallel for ordered schedule(static, 1)
        for (int i = 0; i < num_threads; ++i) {
#pragma omp ordered
            {
                int target_id = num_threads - 1 - i;
               
                std::printf("Thread %d of %d saying Hello!\n", target_id, num_threads);
            }
        }
    }

    // 2 Shared turn flag with flush
    else if (method == 2) {
        int current_turn = num_threads - 1;

#pragma omp parallel shared(current_turn)
        {
            int id = omp_get_thread_num();
            bool done = false;

            while (!done) {
#pragma omp flush(current_turn)
                if (current_turn == id) {
                    std::printf("Thread %d of %d saying Hello!\n", id, omp_get_num_threads());

                    current_turn--;
#pragma omp flush(current_turn)
                    done = true;
                }
            }
        }
    }
    //3 String buffer array
    else if (method == 3) {
        std::vector<std::string> results(num_threads);

#pragma omp parallel shared(results)
        {
            int id = omp_get_thread_num();
            int total = omp_get_num_threads();
            std::string msg = "Thread " + std::to_string(id) + " of " + std::to_string(total) + " saying Hello!\n";
            results[id] = msg;
        }

        for (int i = num_threads - 1; i >= 0; --i) {
            std::cout << results[i];
        }
    }
    // 4 Critical section 
    else if (method == 4) {
        int target_id = num_threads - 1;

#pragma omp parallel shared(target_id)
        {
            int id = omp_get_thread_num();
            bool MyTurn = false;

            while (!MyTurn) {
#pragma omp critical
                {
                    if (target_id == id) {
                        std::printf("Thread %d of %d saying Hello!\n", id, omp_get_num_threads());
                        target_id--;
                        MyTurn = true;
                    }
                }
            }
        }
    }
    // 5 OpenMP locks (omp_lock_t)
    else if (method == 5) {
        int target_id = num_threads - 1;

#pragma omp parallel shared(target_id)
        {
            int id = omp_get_thread_num();
            bool done = false;
            while (!done) {
                int current;
#pragma omp critical(method5_read)
                {
                    current = target_id;
                }

                if (current == id) {
                    std::printf("Thread %d of %d saying Hello!\n", id, omp_get_num_threads());
#pragma omp critical(method5_write)
                    {
                        target_id--;
                    }
                    done = true;
                }
            }
        }
    }
    else {
        std::cerr << "Unknown method! Use 1, 2, 3, 4 or 5.\n";
        return 1;
    }

    return 0;
}
