#include "DataStructureA.h"
#include <algorithm>
#include <cstring>

DataStructureA::DataStructureA() : size(0), capacity(1), data(nullptr)
{
    data = new StudentProfilesA*[capacity];
}

DataStructureA::~DataStructureA()
{
    for (size_t i = 0; i < size; ++i)
        delete data[i];
    delete [] data;
}

void DataStructureA::insert(int id, int score)
{
    // Find if id exist, binary search
    int left = 0, right = size - 1;
    size_t mid = -1;
    bool found = false;
    while (left <= right)
    {
        mid = (left + right) / 2;
        if (data[mid]->id < id)
            left = mid + 1;
        else if (data[mid]->id > id)
            right = mid - 1;
        else
        {
            found = true;
            break;
        }
    }

    // If found id, push score into
    if (found)
    {
        data[mid]->scores.push_back(score);
    }
    else
    {
        // If not array big enough, expand it
        if (size == capacity) {
            size_t new_capacity = capacity * 10;
            StudentProfilesA **new_data = new StudentProfilesA *[new_capacity];
            StudentProfilesA **q = new_data;
            std::memcpy(new_data, data, size * sizeof(StudentProfilesA *));
            delete[] data;
            data = new_data;
            capacity = new_capacity;
        }

        // Insert student profiles
        std::move_backward(data + left, data + size, data + size + 1);
        data[left] = new StudentProfilesA(id);
        data[left]->scores.push_back(score);
        ++size;
    }
}

std::vector<int> DataStructureA::search(int id)
{
    // Find if id exist, binary search
    int left = 0, right = size - 1;
    size_t mid;
    bool found = false;
    while (left <= right)
    {
        mid = (left + right) / 2;
        if (data[mid]->id < id)
            left = mid + 1;
        else if (data[mid]->id > id)
            right = mid - 1;
        else
        {
            found = true;
            break;
        }
    }

    if (found)
        return data[mid]->scores;
    else
        return std::vector<int>();
}
size_t DataStructureA::total()
{
    size_t result = 0;
    for (size_t i = 0; i < size; ++i)
    {
        for (const auto& score : data[i]->scores)
            result += score;
    }
    return result;
}