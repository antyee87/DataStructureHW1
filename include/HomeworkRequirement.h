#pragma once
#include <vector>

using size_t = unsigned long long;

class HomeworkRequirement {
    virtual void insert(int id, int score) = 0;
    virtual std::vector<int> search(int id) = 0;
};