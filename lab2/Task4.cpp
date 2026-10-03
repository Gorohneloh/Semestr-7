#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <omp.h>

void matrix_multiply_serial(const std::vector<double>& A,
    const std::vector<double>& B,
    std::vector<double>& C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < N; ++k) {
            for (int j = 0; j < N; ++j) {
                C[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}

void matrix_multiply_parallel(const std::vector<double>& A,
    const std::vector<double>& B,
    std::vector<double>& C, int N, int threads) {
    omp_set_num_threads(threads);

#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < N; ++k) {
            for (int j = 0; j < N; ++j) {
                C[i * N + j] += A[i * N + k] * B[k * N + j];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <threads> <matrix_size_N>\n";
        return 1;
    }

    int threads = std::atoi(argv[1]);
    int N = std::atoi(argv[2]);

    std::vector<double> A(N * N, 1.5);
    std::vector<double> B(N * N, 2.0);
    std::vector<double> C_serial(N * N, 0.0);
    std::vector<double> C_parallel(N * N, 0.0);

    auto start = std::chrono::high_resolution_clock::now();
    matrix_multiply_serial(A, B, C_serial, N);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> serial_dur = end - start;
    std::cout << "Serial time: " << serial_dur.count() << " sec.\n";

    start = std::chrono::high_resolution_clock::now();
    matrix_multiply_parallel(A, B, C_parallel, N, threads);
    end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> parallel_dur = end - start;
    std::cout << "Parallel time (" << threads << " threads): " << parallel_dur.count() << " sec.\n";

    std::cout << "Speedup: " << serial_dur.count() / parallel_dur.count() << "x\n\n";

    return 0;
}
