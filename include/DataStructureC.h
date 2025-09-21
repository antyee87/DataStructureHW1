#pragma once
#include "HomeworkRequirement.h"
#include <memory>
#include <vector>

struct StudentProfilesC
{
    StudentProfilesC(int id) : id(id) {}
    int id;
    std::vector<int> scores;
};

class DataStructureC : public HomeworkRequirement
{
public:
    struct Node
    {
        Node(int id) : next(nullptr), value(StudentProfilesC(id)) {}
        Node *next;
        StudentProfilesC value;
    };

    DataStructureC();
    ~DataStructureC();
    void insert(int id, int score) override;
    std::vector<int> search(int id) override;
    size_t total() override;
    Node *head;
};