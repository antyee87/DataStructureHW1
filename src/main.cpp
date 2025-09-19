#include <chrono>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <random>
#include <nlohmann/json.hpp>
#include "DataStructureA.h"
#include "DataStructureB.h"
#include "DataStructureC.h"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution random_id(1, 1 << 20);
std::uniform_int_distribution random_score(0, 100);

using json = nlohmann::json;
std::string name_mapping[] = {"Data structure A", "Data structure B", "Data structure C"};

void generate_data(std::vector<std::pair<int, int>> &data, size_t count)
{
    data.clear();
    data.resize(count + 5);
    for (size_t i = 0; i < count; ++i)
        data.emplace_back(random_id(gen), random_score(gen));
}

// int main()
// {
//     std::vector<std::pair<int, int>> data;
//     data.emplace_back(2, 50);
//     data.emplace_back(2, 100);
//     data.emplace_back(1, 20);
//     data.emplace_back(5, 40);
//     data.emplace_back(6, 60);
//     data.emplace_back(10, 50);

//     DataStructureA a;
//     DataStructureB b;
//     DataStructureC c;
//     HomeworkRequirement *data_structure[] = {&a, &b, &c};

//     for (size_t i = 0; i < 3; ++i)
//     {
//         for (auto d : data)
//             data_structure[i]->insert(d.first, d.second);
//     }
//     std::cout << "Search :\n";
//     std::cout << "Search 2 :\n";
//     for (size_t i = 0; i < 3; ++i)
//     {
//         auto result = data_structure[i]->search(2);
//         std::cout << name_mapping[i] << " :\n";
//         for (auto r : result)
//             std::cout << r << " ";
//         std::cout << "\n";
//     }
//     std::cout << "Search 7 :\n";
//     for (size_t i = 0; i < 3; ++i)
//     {
//         auto result = data_structure[i]->search(7);
//         std::cout << name_mapping[i] << " :\n";
//         for (auto r : result)
//             std::cout << r << " ";
//         std::cout << "\n";
//     }
//     std::cout << "Total :\n";
//     for (size_t i = 0; i < 3; ++i)
//     {
//         std::cout << name_mapping[i] << " :\n";
//         auto result = data_structure[i]->total();
//         std::cout << result << "\n";
//     }

//     return 0;
// }

int main()
{
    std::vector<int> set_k;
    for (int i = 11; i <= 25; ++i)
        set_k.push_back(i);
    // for (int i = 1; i <= 15; ++i)
    //     set_k.push_back(i);

    std::vector<std::pair<int, int>> data;
    std::vector<int> search_ids;
    search_ids.reserve(100000);
    /*
        result[i][j][k]
            i -> data structure type
            j -> experiment type
            k -> sum of run time
     */
    std::vector<std::vector<std::vector<std::chrono::milliseconds>>> result(
        3, std::vector<std::vector<std::chrono::milliseconds>>(
               3, std::vector<std::chrono::milliseconds>(
                      15, std::chrono::milliseconds(0))));

    DataStructureA a;
    DataStructureB b;
    DataStructureC c;
    HomeworkRequirement *data_structure[] = {&a, &b, &c};

    int data_rank = 0;
    for (auto k : set_k)
    {
        std::cout << "\rTesting : k =" << k;
        size_t n = 1 << k;
        for (int i = 0; i < 10; ++i)
        {
            generate_data(data, n);
            search_ids.clear();
            for (size_t i = 0; i < 100000; ++i)
                search_ids.push_back(random_id(gen));

            for (int data_structure_type = 0; i < 3; ++i)
            {
                auto start_time = std::chrono::high_resolution_clock::now();
                for (auto d : data)
                    data_structure[data_structure_type]->insert(d.first, d.second);
                auto end_time = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                result[data_structure_type][0][data_rank] += duration;
            }
            for (int data_structure_type = 0; i < 3; ++i)
            {
                auto start_time = std::chrono::high_resolution_clock::now();
                for (auto search_id : search_ids)
                    data_structure[data_structure_type]->search(search_id);
                auto end_time = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                result[data_structure_type][1][data_rank] += duration;
            }
            for (int data_structure_type = 0; i < 3; ++i)
            {
                auto start_time = std::chrono::high_resolution_clock::now();
                data_structure[data_structure_type]->total();
                auto end_time = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                result[data_structure_type][2][data_rank] += duration;
            }
        }
        ++data_rank;
    }
    std::cout << "\nData proccessing...";
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            for (size_t k = 0; k < 15; ++k)
                result[i][j][k] /= 10;
        }
    }

    json result_json;
    for (size_t i = 0; i < 3; ++i)
    {
        for (size_t j = 0; j < 3; ++j)
        {
            for (size_t k = 0; k < 15; ++k)
                result_json[name_mapping[i]]["experiment_" + std::to_string(j)]["data_rank_" + std::to_string(k)] = result[i][j][k].count();
        }
    }

    std::string filename = "result.json";
    if (!std::filesystem::exists(filename))
        std::ofstream(filename);
    std::fstream fs(filename);
    fs << result_json.dump(4);
    fs.close();

    return 0;
}