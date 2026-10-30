#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>

double global_sum = 0.0;
std::mutex mtx;

int main() {
    std::string filename = "points.txt";

    std::vector<double> X, Y, Z;
    std::ifstream file(filename);
    double x, y, z;
    while (file >> x >> y >> z) {
        X.push_back(x);
        Y.push_back(y);
        Z.push_back(z);
    }
    file.close();

    size_t N = X.size();

    auto start_time = std::chrono::high_resolution_clock::now();

    auto compute_axis_sum = [](const std::vector<double>& axis_data) {
        double local_sum = 0.0;
        for (double val : axis_data) {
            local_sum += val;
        }

        mtx.lock();
        global_sum += local_sum;
        mtx.unlock();
        };

    std::thread thread_X(compute_axis_sum, std::ref(X));
    std::thread thread_Y(compute_axis_sum, std::ref(Y));
    std::thread thread_Z(compute_axis_sum, std::ref(Z));

    thread_X.join();
    thread_Y.join();
    thread_Z.join();

    double final_result = global_sum / (3.0 * N);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "=== Task 6: Critical Sections ===" << std::endl;
    std::cout << "Points: " << N << std::endl;
    std::cout << "Result: " << final_result << std::endl;
    std::cout << "Time: " << duration.count() << " sec" << std::endl;

    return 0;
}
