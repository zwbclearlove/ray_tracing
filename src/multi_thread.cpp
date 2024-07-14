//
// Created by atwbzhang on 2024/7/14.
//
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

int main() {
    unsigned concurrent_count = std::thread::hardware_concurrency();
    std::cout << "This machine supports concurrent_count threads" << concurrent_count << std::endl;
    std::vector<std::thread> threads;

    for (int i = 0; i < concurrent_count; i++) {
        threads.push_back(std::thread([i] {
            std::cout << "Thread " << i << " is running" << std::endl;
        }));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
