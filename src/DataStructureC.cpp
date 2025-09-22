#include "DataStructureC.h"

DataStructureC::DataStructureC() : head(nullptr) {}
DataStructureC::~DataStructureC()
{
    Node *cur_node = head;
    while (cur_node != nullptr)
    {
        Node *tmp = cur_node;
        cur_node = cur_node->next;
        delete tmp;
    }
}

void DataStructureC::insert(int id, int score)
{
    // Find if id exist, traversal, if not exist, add node
    Node *new_node = new Node(id);
    new_node->value.scores.push_back(score);

    Node *cur_node = head;
    Node *new_node_prev_node = nullptr;

    while (cur_node != nullptr)
    {
        // If id exist, push score into
        if (cur_node->value.id == id)
        {
            cur_node->value.scores.push_back(score);
            delete new_node;
            return;
        }

        if (cur_node < new_node)
            new_node_prev_node = cur_node;
        cur_node = cur_node->next;
    }

    if (new_node_prev_node == nullptr)
    {
        new_node->next = head;
        head = new_node;
    }
    else
    {
        new_node->next = new_node_prev_node->next;
        new_node_prev_node->next = new_node;
    }
}

std::vector<int> DataStructureC::search(int id) {
    // Find if id exist, traversal
    Node *cur_node = head;
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
    Node *cur_node = head;
    while (cur_node != nullptr)
    {
        for (const auto& score : cur_node->value.scores)
            result += score;

        cur_node = cur_node->next;
    }
    return result;
}