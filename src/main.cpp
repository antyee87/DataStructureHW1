#include <iostream>
#include <random>
#include "DataStructureA.h"
#include "DataStructureB.h"
#include "DataStructureC.h"


int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution random_id(1, 1 << 20);
    std::uniform_int_distribution random_score(0, 100);

    std::vector<std::pair<int, int>> data;

    DataStructureA a;
    for (int i = 0; i < 10; ++i)
        a.insert(random_id(gen), random_score(gen));

    for (int i = 0; i < a.size; ++i)
        std::cout << a.data[i].id << " ";

    return 0;
}