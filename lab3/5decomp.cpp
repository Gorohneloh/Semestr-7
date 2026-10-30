#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

int main(int argc, char* argv[]) {
    std::string filename = argv[1];

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
    double avg_x = 0.0, avg_y = 0.0, avg_z = 0.0;

    auto start_time = std::chrono::high_resolution_clock::now();

    auto compute_axis_avg = [](const std::vector<double>& axis_data, double& result) {
        double sum = 0.0;
        for (double val : axis_data) {
            sum += val;
        }
        result = sum / axis_data.size();
        };

    std::thread thread_X(compute_axis_avg, std::ref(X), std::ref(avg_x));
    std::thread thread_Y(compute_axis_avg, std::ref(Y), std::ref(avg_y));
    std::thread thread_Z(compute_axis_avg, std::ref(Z), std::ref(avg_z));

    thread_X.join();
    thread_Y.join();
    thread_Z.join();

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "=== Functional Decomposition ===" << std::endl;
    std::cout << "Points: " << N << std::endl;
    std::cout << "Center: (" << avg_x << ", " << avg_y << ", " << avg_z << ")" << std::endl;
    std::cout << "Time: " << duration.count() << " sec" << std::endl;

    return 0;
}
