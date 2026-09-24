//
// Created by kirillr on 18.09.2026.
//
#include <algorithm>

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
    mct::vector<int> v(s.begin(), s.end());
    mct::sort(v.begin(), v.end());
    std::cout << v.size() << std::endl;
    for (const auto& i : v) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    mct::string str;
    str = '0';
    std::cout << str+'1' << std::endl;
    return 0;
}
