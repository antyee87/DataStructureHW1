#pragma once
#include "HomeworkRequirement.h"
#include <memory>
#include <vector>

struct StudentProfilesA
{
    StudentProfilesA(int id) : id(id) {}
    int id;
    std::vector<int> scores;
};

class DataStructureA : public HomeworkRequirement
{
public:
    DataStructureA();
    ~DataStructureA();
    void insert(int id, int score) override;
    std::vector<int> search(int id) override;
    size_t total() override;
    size_t size;
    size_t capacity;
    StudentProfilesA **data;
};