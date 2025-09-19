#pragma once
#include "HomeworkRequirement.h"
#include <memory>
#include <vector>

const int MAX_ID = 1 << 20;
class StudentProfileB
{
public:
    std::vector<int> scores;
    int next_student = -1;
};

class DataStructureB : public HomeworkRequirement
{
public:
    DataStructureB();
    void insert(int id, int score) override;
    std::vector<int> search(int id) override;
    size_t total() override;
    StudentProfileB *data;
    int begin;
};