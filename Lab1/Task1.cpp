#include <iostream>
#include <omp.h>
#include <string>

int main(int argc, char* argv[])
{
    int threads = 8;

    if (argc > 1)
        threads = std::stoi(argv[1]);

    omp_set_num_threads(threads);

#pragma omp parallel
    {
        int id = omp_get_thread_num();
        int count = omp_get_num_threads();

        std::cout << "Thread " << id
            << " of " << count
            << ": Hello World" << std::endl;
    }

    return 0;
}