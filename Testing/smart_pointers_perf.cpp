#include "../Classes/SmartPointers/SmartPointers.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <cstdlib>

struct TestData {
    int value;
    double data[10];
    TestData(int v) : value(v) {
        for (int i = 0; i < 10; i++) data[i] = v * i;
    }
};

template<class T>
double measureTime(std::function<void()> func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1000.0;
}

void testSmrtPtrPerformance(size_t count) {
    std::cout << "\n=== SmrtPtr Performance Test (count: " << count << ") ===\n";

    double createTime = measureTime<TestData>([count]() {
        std::vector<SmrtPtr<TestData>> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(makeSmrt<TestData>(i));
        }
    });

    double copyTime = measureTime<TestData>([count]() {
        std::vector<SmrtPtr<TestData>> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(makeSmrt<TestData>(i));
        }
        std::vector<SmrtPtr<TestData>> copies;
        copies.reserve(count);
        for (size_t i = 0; i < count; i++) {
            copies.push_back(ptrs[i]);
        }
    });

    double accessTime = measureTime<TestData>([count]() {
        std::vector<SmrtPtr<TestData>> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(makeSmrt<TestData>(i));
        }
        long sum = 0;
        for (size_t i = 0; i < count; i++) {
            sum += ptrs[i]->value;
        }
    });

    std::cout << "Create: " << std::fixed << std::setprecision(3) << createTime << " ms\n";
    std::cout << "Copy: " << copyTime << " ms\n";
    std::cout << "Access: " << accessTime << " ms\n";
}

void testRawPtrPerformance(size_t count) {
    std::cout << "\n=== Raw Pointer Performance Test (count: " << count << ") ===\n";

    double createTime = measureTime<TestData>([count]() {
        std::vector<TestData *> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(new TestData(i));
        }
        for (size_t i = 0; i < count; i++) {
            delete ptrs[i];
        }
    });

    double copyTime = measureTime<TestData>([count]() {
        std::vector<TestData *> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(new TestData(i));
        }
        std::vector<TestData *> copies;
        copies.reserve(count);
        for (size_t i = 0; i < count; i++) {
            copies.push_back(ptrs[i]);
        }
        for (size_t i = 0; i < count; i++) {
            delete ptrs[i];
        }
    });

    double accessTime = measureTime<TestData>([count]() {
        std::vector<TestData *> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(new TestData(i));
        }
        long sum = 0;
        for (size_t i = 0; i < count; i++) {
            sum += ptrs[i]->value;
        }
        for (size_t i = 0; i < count; i++) {
            delete ptrs[i];
        }
    });

    std::cout << "Create: " << std::fixed << std::setprecision(3) << createTime << " ms\n";
    std::cout << "Copy: " << copyTime << " ms\n";
    std::cout << "Access: " << accessTime << " ms\n";
}

void testSTLSharedPtrPerformance(size_t count) {
    std::cout << "\n=== std::shared_ptr Performance Test (count: " << count << ") ===\n";

    double createTime = measureTime<TestData>([count]() {
        std::vector<std::shared_ptr<TestData>> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(std::make_shared<TestData>(i));
        }
    });

    double copyTime = measureTime<TestData>([count]() {
        std::vector<std::shared_ptr<TestData>> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(std::make_shared<TestData>(i));
        }
        std::vector<std::shared_ptr<TestData>> copies;
        copies.reserve(count);
        for (size_t i = 0; i < count; i++) {
            copies.push_back(ptrs[i]);
        }
    });

    double accessTime = measureTime<TestData>([count]() {
        std::vector<std::shared_ptr<TestData>> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(std::make_shared<TestData>(i));
        }
        long sum = 0;
        for (size_t i = 0; i < count; i++) {
            sum += ptrs[i]->value;
        }
    });

    std::cout << "Create: " << std::fixed << std::setprecision(3) << createTime << " ms\n";
    std::cout << "Copy: " << copyTime << " ms\n";
    std::cout << "Access: " << accessTime << " ms\n";
}

void testUnqPtrPerformance(size_t count) {
    std::cout << "\n=== UnqPtr Performance Test (count: " << count << ") ===\n";

    double createTime = measureTime<TestData>([count]() {
        std::vector<UnqPtr<TestData>> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(makeUnq<TestData>(i));
        }
    });

    double moveTime = measureTime<TestData>([count]() {
        std::vector<UnqPtr<TestData>> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(makeUnq<TestData>(i));
        }
        std::vector<UnqPtr<TestData>> moved;
        moved.reserve(count);
        for (size_t i = 0; i < count; i++) {
            moved.push_back(std::move(ptrs[i]));
        }
    });

    double accessTime = measureTime<TestData>([count]() {
        std::vector<UnqPtr<TestData>> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(makeUnq<TestData>(i));
        }
        long sum = 0;
        for (size_t i = 0; i < count; i++) {
            sum += ptrs[i]->value;
        }
    });

    std::cout << "Create: " << std::fixed << std::setprecision(3) << createTime << " ms\n";
    std::cout << "Move: " << moveTime << " ms\n";
    std::cout << "Access: " << accessTime << " ms\n";
}

void testShrdPtrPerformance(size_t count) {
    std::cout << "\n=== ShrdPtr Performance Test (count: " << count << ") ===\n";

    double createTime = measureTime<TestData>([count]() {
        std::vector<ShrdPtr<TestData>> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(makeShrd<TestData>(i));
        }
    });

    double copyTime = measureTime<TestData>([count]() {
        std::vector<ShrdPtr<TestData>> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(makeShrd<TestData>(i));
        }
        std::vector<ShrdPtr<TestData>> copies;
        copies.reserve(count);
        for (size_t i = 0; i < count; i++) {
            copies.push_back(ptrs[i]);
        }
    });

    double accessTime = measureTime<TestData>([count]() {
        std::vector<ShrdPtr<TestData>> ptrs;
        ptrs.reserve(count);
        for (size_t i = 0; i < count; i++) {
            ptrs.push_back(makeShrd<TestData>(i));
        }
        long sum = 0;
        for (size_t i = 0; i < count; i++) {
            sum += ptrs[i]->value;
        }
    });

    std::cout << "Create: " << std::fixed << std::setprecision(3) << createTime << " ms\n";
    std::cout << "Copy: " << copyTime << " ms\n";
    std::cout << "Access: " << accessTime << " ms\n";
}

void printComparisonTable(const std::vector<size_t> &sizes) {
    std::cout << "\n=== Performance Comparison Table ===\n";
    std::cout << std::left << std::setw(12) << "Count";
    std::cout << std::setw(20) << "SmrtPtr (ms)";
    std::cout << std::setw(20) << "Raw Ptr (ms)";
    std::cout << std::setw(20) << "shared_ptr (ms)";
    std::cout << std::setw(20) << "UnqPtr (ms)";
    std::cout << std::setw(20) << "ShrdPtr (ms)" << "\n";
    std::cout << std::string(112, '-') << "\n";

    for (size_t count : sizes) {
        std::cout << std::setw(12) << count;

        double smrtTime = measureTime<TestData>([count]() {
            std::vector<SmrtPtr<TestData>> ptrs;
            ptrs.reserve(count);
            for (size_t i = 0; i < count; i++) {
                ptrs.push_back(makeSmrt<TestData>(i));
            }
        });

        double rawTime = measureTime<TestData>([count]() {
            std::vector<TestData *> ptrs;
            ptrs.reserve(count);
            for (size_t i = 0; i < count; i++) {
                ptrs.push_back(new TestData(i));
            }
            for (size_t i = 0; i < count; i++) {
                delete ptrs[i];
            }
        });

        double stlTime = measureTime<TestData>([count]() {
            std::vector<std::shared_ptr<TestData>> ptrs;
            ptrs.reserve(count);
            for (size_t i = 0; i < count; i++) {
                ptrs.push_back(std::make_shared<TestData>(i));
            }
        });

        double unqTime = measureTime<TestData>([count]() {
            std::vector<UnqPtr<TestData>> ptrs;
            ptrs.reserve(count);
            for (size_t i = 0; i < count; i++) {
                ptrs.push_back(makeUnq<TestData>(i));
            }
        });

        double shrdTime = measureTime<TestData>([count]() {
            std::vector<ShrdPtr<TestData>> ptrs;
            ptrs.reserve(count);
            for (size_t i = 0; i < count; i++) {
                ptrs.push_back(makeShrd<TestData>(i));
            }
        });

        std::cout << std::fixed << std::setprecision(3);
        std::cout << std::setw(20) << smrtTime;
        std::cout << std::setw(20) << rawTime;
        std::cout << std::setw(20) << stlTime;
        std::cout << std::setw(20) << unqTime;
        std::cout << std::setw(20) << shrdTime << "\n";
    }
}

void printGraph(const std::vector<size_t> &sizes, const std::vector<double> &times, const std::string &title) {
    std::cout << "\n=== " << title << " ===\n";
    if (times.empty()) return;

    double maxTime = *std::max_element(times.begin(), times.end());
    const int graphWidth = 60;

    for (size_t i = 0; i < sizes.size(); i++) {
        std::cout << std::setw(10) << sizes[i] << " |";
        int barLength = static_cast<int>((times[i] / maxTime) * graphWidth);
        for (int j = 0; j < barLength; j++) {
            std::cout << "█";
        }
        std::cout << " " << std::fixed << std::setprecision(2) << times[i] << " ms\n";
    }
}

inline void runPerformanceTests() {
    std::cout << "\n========================================\n";
    std::cout << "  Smart Pointers Performance Tests\n";
    std::cout << "========================================\n";

    std::vector<size_t> smallSizes = {10, 100, 1000};
    std::vector<size_t> largeSizes = {10000, 100000, 1000000};

    std::cout << "\n--- Small Scale Tests ---\n";
    for (size_t size : smallSizes) {
        testSmrtPtrPerformance(size);
        testRawPtrPerformance(size);
        testSTLSharedPtrPerformance(size);
        testUnqPtrPerformance(size);
        testShrdPtrPerformance(size);
    }

    std::cout << "\n--- Large Scale Tests ---\n";
    for (size_t size : largeSizes) {
        if (size <= 100000) {
            testSmrtPtrPerformance(size);
            testRawPtrPerformance(size);
            testSTLSharedPtrPerformance(size);
            testUnqPtrPerformance(size);
            testShrdPtrPerformance(size);
        }
    }

    std::cout << "\n--- Comparison Table (Small Scale) ---\n";
    printComparisonTable(smallSizes);

    std::cout << "\n--- Memory Span Performance ---\n";
    for (size_t size : {100, 1000, 10000}) {
        double time = measureTime<TestData>([size]() {
            MemorySpan<TestData> span(size);
            for (size_t i = 0; i < size; i++) {
                span.emplace(i, i);
            }
            for (size_t i = 0; i < size; i++) {
                auto ptr = span.Locate(i);
                volatile int val = ptr->value;
                (void)val;
            }
        });
        std::cout << "MemorySpan (size " << size << "): " << std::fixed << std::setprecision(3) << time << " ms\n";
    }

    std::cout << "\n========================================\n";
    std::cout << "  Performance Tests Completed\n";
    std::cout << "========================================\n";
}

