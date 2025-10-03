#include "DataStructureC.h"
#include <algorithm>
#include <unordered_map>

DataStructureC::DataStructureC() : head(nullptr) {}

DataStructureC::DataStructureC(const std::vector<std::pair<int, int>> &data) : head(nullptr)
{
    std::unordered_map<int, Node *> node_map;
    for (const auto& d : data)
    {
        if (!node_map.contains(d.first))
        {
            Node *new_node = new Node(d.first);
            node_map[d.first] = new_node;
        }
        node_map[d.first]->value.scores.push_back(d.second);
    }
    std::vector<Node *> node_vector;
    node_vector.reserve(node_map.size());
    for (const auto& [key, value] : node_map)
        node_vector.push_back(value);
    std::sort(node_vector.begin(), node_vector.end());
    for (auto it = node_vector.rbegin(); it != node_vector.rend(); ++it)
    {
        (*it)->next = head;
        head = *it;
    }
}

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
    // Linear search if id exist, if not exist then add node
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
    // Insert node
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
    return std::vector<int>();
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