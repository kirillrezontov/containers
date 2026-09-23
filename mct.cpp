//
// Created by kirillr on 18.09.2026.
//
#include "mct_string.h"
#include "mct_unordered_set.h"

int main() {
    mct::unordered_set<int> s;
    for (int i = 0; i < 100; i++) {
        std::cout << i << std::endl;
        s.insert(i);
        std::cout << i << std::endl;
        s.insert(i);
        std::cout << i << std::endl;
    }
    for (const auto& i : s) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    return 0;
}
