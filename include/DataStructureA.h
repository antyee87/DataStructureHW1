#pragma once
#include "HomeworkRequirement.h"
#include <memory>
#include <vector>

class StudentProfileA
{
public:
    StudentProfileA(int id) : id(id) {}
    int id;
    std::vector<int> scores;
};

class DataStructureA : public HomeworkRequirement
{
public:
    using AllocTraits = std::allocator_traits<std::allocator<StudentProfileA>>;

    DataStructureA();
    void insert(int id, int score) override;
    std::vector<int> search(int id) override;
    size_t total() override;
    size_t size;
    size_t capacity;
    StudentProfileA *data;

    std::allocator<StudentProfileA> alloc;
};