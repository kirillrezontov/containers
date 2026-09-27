//
// Created by kirillr on 18.09.2026.
//

#include <algorithm>

#include "mct_string.h"
#include "mct_unordered_set.h"
#include <thread>
#include <fstream>
static auto thread_count = std::thread::hardware_concurrency();

#define N 100

int main() {
    // mct::unordered_set<double> s;
    // for (int k = 1; k < N; k++) {s.insert(k+static_cast<double>(k)/((k<<3)^k)); s.insert(k+static_cast<double>(k)/((k<<3)^k));}
    // mct::vector<double> v(s.begin(), s.end()); std::cout << v.size() << std::endl;
    // mct::sort(v.begin(), v.end());
    // for (const auto& i: v) std::cout << i << ' ';
    // std::cout << std::endl;
    try {
        auto func = [](int i, const int j) {
            mct::unordered_set<double> s; mct::string filename("test/test_"); filename+=mct::to_string(i);
            for (int k = i; k < j; k++) {s.insert(k+static_cast<double>(k)/((k<<3)^k)), s.insert(k+static_cast<double>(k)/((k<<3)^k));};
            if (s.size()!=N) return;
            mct::vector<double> v(s.begin(), s.end());
            for (int k = i; k < j; k++) if (s.erase(k+static_cast<double>(k)/((k<<3)^k)) != true) exit(-1);
            if (!s.empty()) exit(-1);
            mct::sort(v.begin(), v.end());
            std::ofstream outfile(filename.c_str());
            if (!outfile.is_open()) { throw mct::bad_file(filename.c_str(), "func"); return; }
            for (const auto& x : v) outfile << x << '\n';
        };
        mct::vector<std::jthread> v;
        for (int i = 1; i <= thread_count; i++) {
            v.emplace_back(func, i , i+N);
        }
    }
    catch (const mct::exception& e) { e.what(); }
    return 0;
}
