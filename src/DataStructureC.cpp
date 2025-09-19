#include "DataStructureC.h"

DataStructureC::DataStructureC() : begin(nullptr) {}

void DataStructureC::insert(int id, int score)
{
    Node *cur_node = begin;
    while (cur_node != nullptr)
    {
        if (cur_node->value.id == id)
        {
            cur_node->value.scores.push_back(score);
            return;
        }
        cur_node = cur_node->next;
    }

    Node *new_node = new Node(id);
    new_node->value.scores.push_back(score);

    cur_node = begin;
    Node *prev_node = nullptr;
    while (cur_node != nullptr)
    {
        if (new_node < cur_node)
            break;
        prev_node = cur_node;
        cur_node = cur_node->next;
    }

    if (prev_node == nullptr)
    {
        new_node->next = begin;
        begin = new_node;
    }
    else {
        new_node->next = prev_node->next;
        prev_node->next = new_node;
    }
}

std::vector<int> DataStructureC::search(int id) {
    Node *cur_node = begin;
    while (cur_node != nullptr)
    {
        if (cur_node->value.id == id)
            return cur_node->value.scores;

        cur_node = cur_node->next;
    }
    return std::vector<int>{-1};
}

size_t DataStructureC::total()
{
    size_t result = 0;
    Node *cur_node = begin;
    while (cur_node != nullptr)
    {
        for (auto score : cur_node->value.scores)
            result += score;

        cur_node = cur_node->next;
    }
    return result;
}