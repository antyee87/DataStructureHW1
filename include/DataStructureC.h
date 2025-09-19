#pragma once
#include "HomeworkRequirement.h"
#include <memory>
#include <vector>

class StudentProfileC
{
public:
    StudentProfileC(int id) : id(id) {}
    int id;
    std::vector<int> scores;
};

class DataStructureC : public HomeworkRequirement
{
public:
    struct Node {
        Node(int id) : next(nullptr), value(StudentProfileC(id)) {}
        Node *next;
        StudentProfileC value;
    };

    DataStructureC();
    void insert(int id, int score) override;
    std::vector<int> search(int id) override;
    size_t total() override;
    Node *begin;
};