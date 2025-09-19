#include "DataStructureB.h"

DataStructureB::DataStructureB()
{
    data = new StudentProfileB[MAX_ID + 5];
    begin = -1;
}

void DataStructureB::insert(int id, int score)
{
    data[id].scores.push_back(score);

    for (size_t i = id - 1; i > 0; --i)
    {
        if (data[i].scores.size() > 0)
        {
            data[id].next_student = data[i].next_student;
            data[i].next_student = id;
            return;
        }
    }
    begin = id;

    for (size_t i = id + 1; i <= MAX_ID; ++i)
    {
        if (data[i].scores.size() > 0)
        {
            data[id].next_student = i;
            break;
        }
    }
}

std::vector<int> DataStructureB::search(int id)
{
    if (data[id].scores.size() > 0)
        return data[id].scores;
    else
        return std::vector<int>{-1};
}

size_t DataStructureB::total()
{
    size_t result = 0;
    size_t cur_student = begin;
    while(cur_student != -1)
    {
        for (auto score : data[cur_student].scores)
            result += score;
        cur_student = data[cur_student].next_student;
    }
    return result;
}