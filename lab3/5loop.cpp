#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

struct Point3D {
    double x, y, z;
};

int main(int argc, char* argv[]) {
    int num_threads = std::stoi(argv[1]);
    std::string filename = argv[2];

    std::ifstream file(filename);
    std::vector<Point3D> points;
    Point3D p;
    while (file >> p.x >> p.y >> p.z) {
        points.push_back(p);
    }
    file.close();

    size_t N = points.size();
    double total_x = 0.0, total_y = 0.0, total_z = 0.0;

    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<double> local_x(num_threads, 0.0);
    std::vector<double> local_y(num_threads, 0.0);
    std::vector<double> local_z(num_threads, 0.0);
    std::vector<std::thread> threads;

    auto worker = [&](int thread_id) {
        size_t items_per_thread = (N + num_threads - 1) / num_threads;
        size_t start_idx = thread_id * items_per_thread;
        size_t end_idx = std::min(start_idx + items_per_thread, N);

        double sum_x = 0, sum_y = 0, sum_z = 0;
        for (size_t i = start_idx; i < end_idx; ++i) {
            sum_x += points[i].x;
            sum_y += points[i].y;
            sum_z += points[i].z;
        }
        local_x[thread_id] = sum_x;
        local_y[thread_id] = sum_y;
        local_z[thread_id] = sum_z;
        };

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    for (int i = 0; i < num_threads; ++i) {
        total_x += local_x[i];
        total_y += local_y[i];
        total_z += local_z[i];
    }

    double center_x = total_x / N;
    double center_y = total_y / N;
    double center_z = total_z / N;

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    std::cout << "=== Loop Parallelization ===" << std::endl;
    std::cout << "Threads: " << num_threads << ", Points: " << N << std::endl;
    std::cout << "Center: (" << center_x << ", " << center_y << ", " << center_z << ")" << std::endl;
    std::cout << "Time: " << duration.count() << " sec" << std::endl;

    return 0;
}
