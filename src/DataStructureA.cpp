#include "DataStructureA.h"

DataStructureA::DataStructureA() : size(0), capacity(1), data(nullptr)
{
    data = AllocTraits::allocate(alloc, capacity);
}

void DataStructureA::insert(int id, int score)
{
    int left = 0, right = size - 1;
    size_t mid = -1;
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
    {
        data[mid].scores.push_back(score);
    }
    else
    {
        size_t new_capacity = capacity;
        if (size == capacity)
            new_capacity = capacity * 10;

        bool inserted = false;
        StudentProfileA *new_data = AllocTraits::allocate(alloc, new_capacity);
        for (StudentProfileA *p = data, *q = new_data; p < data + size; ++q)
        {
            if (!inserted && id < p->id)
            {
                AllocTraits::construct(alloc, q, id);
                q->scores.push_back(score);
                inserted = true;
            }
            else
            {
                AllocTraits::construct(alloc, q, std::move(*p));
                ++p;
            }
        }
        if (!inserted)
        {
            AllocTraits::construct(alloc, new_data + size, id);
            (new_data + size)->scores.push_back(score);
        }
        for (size_t i = 0; i < size; ++i)
        {
            AllocTraits::destroy(alloc, data + i);
        }
        AllocTraits::deallocate(alloc, data, capacity);
        data = new_data;
        capacity = new_capacity;

        ++size;
    }
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
size_t DataStructureA::total()
{
    size_t result = 0;
    for (size_t i = 0; i < size; ++i)
    {
        for (auto score : data[i].scores)
            result += score;
    }
    return result;
}