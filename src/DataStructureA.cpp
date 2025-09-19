#include "DataStructureA.h"
#include <iostream>

DataStructureA::DataStructureA():size(0),capacity(1),data(nullptr) {
    data = AllocTraits::allocate(alloc, capacity);
}

void DataStructureA::insert(int id, int score)
{
    int left = 0, right = size - 1;
    size_t mid;
    bool found = false;
    while(left <= right) {
        mid = (left + right) / 2;
        if (data[mid].id < id)
            left = mid + 1;
        else if (data[mid].id > id)
            right = mid - 1;
        else {
            found = true;
            break;
        }
    }
    if (found) {
        data[mid].scores.push_back(score);
    } else {
        size_t new_capacity = capacity;
        if (size == capacity)
            new_capacity = capacity * 10;

        bool inserted = false;
        StudentProfile *new_data = AllocTraits::allocate(alloc, new_capacity);
        for (StudentProfile *p = data, *q = new_data; p < data + size;)
        {
            if (!inserted && id <= p->id) {
                AllocTraits::construct(alloc, q, id);
                q->scores.push_back(score);
                inserted = true;
                ++q;
            } else {
                AllocTraits::construct(alloc, q, std::move(*p));
                ++p;
                ++q;
            }
        }
        if (!inserted) {
            AllocTraits::construct(alloc, new_data + size, id);
            (new_data + size)->scores.push_back(score);
        }
        AllocTraits::deallocate(alloc, data, capacity);
        data = new_data;
        capacity = new_capacity;
    }
    ++size;
}

std::vector<int> DataStructureA::search(int id)
{
    int left = 0, right = size - 1;
    size_t mid;
    bool found = false;
    while (left <= right)
    {
        mid = (left + right) / 2;
        if (data[mid].id < id)
            left = mid + 1;
        else if (data[mid].id > id)
            right = mid - 1;
        else
        {
            found = true;
            break;
        }
    }
    if (found)
        return data[mid].scores;
    else
        return std::vector<int>{-1};
}