#include <chrono>
#include <fstream>
#include <filesystem>
#include <iostream>
#include <random>
#include <nlohmann/json.hpp>
#include "DataStructureA.h"
#include "DataStructureB.h"
#include "DataStructureC.h"

#define BLOCK_2

using json = nlohmann::ordered_json;
const std::string name_mapping[] = {"Data structure A", "Data structure B", "Data structure C"};
const std::pair<int, int> k_range(18, 25); // Include end
const int NUMBER_OF_EXPERIMENTS = 3;
const int EXPERIMENT_ROUNDS = 10;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution random_id(1, MAX_ID);
std::uniform_int_distribution random_score(0, 100);

void generate_data(std::vector<std::pair<int, int>> &data, size_t count)
{
    data.clear();
    data.reserve(count + 5);
    for (size_t i = 0; i < count; ++i)
        data.emplace_back(random_id(gen), random_score(gen));
}

#ifdef BLOCK_1
// Test
int main()
{
    std::vector<std::pair<int, int>> data;
    data.emplace_back(2, 50);
    data.emplace_back(2, 100);
    data.emplace_back(1, 20);
    data.emplace_back(5, 40);
    data.emplace_back(6, 60);
    data.emplace_back(10, 50);

    DataStructureA a;
    DataStructureB b;
    DataStructureC c;
    HomeworkRequirement *data_structure[] = {&a, &b, &c};

    for (size_t i = 0; i < 3; ++i)
    {
        for (auto d : data)
            data_structure[i]->insert(d.first, d.second);
    }
    std::cout << "Search :\n";
    std::cout << "Search 2 :\n";
    for (size_t i = 0; i < 3; ++i)
    {
        auto result = data_structure[i]->search(2);
        std::cout << name_mapping[i] << " :\n";
        for (auto r : result)
            std::cout << r << " ";
        std::cout << "\n";
    }
    std::cout << "Search 7 :\n";
    for (size_t i = 0; i < 3; ++i)
    {
        auto result = data_structure[i]->search(7);
        std::cout << name_mapping[i] << " :\n";
        for (auto r : result)
            std::cout << r << " ";
        std::cout << "\n";
    }
    std::cout << "Total :\n";
    for (size_t i = 0; i < 3; ++i)
    {
        std::cout << name_mapping[i] << " :\n";
        auto result = data_structure[i]->total();
        std::cout << result << "\n";
    }

    return 0;
}
#endif

#ifdef BLOCK_2
// Homework test
int main() 
{
    if (!std::filesystem::exists(std::filesystem::current_path() / "result"))
        std::filesystem::create_directories(std::filesystem::current_path() / "result");
    std::cout << "\033[2J" << "\033[s";
    auto experiment_start_time = std::chrono::high_resolution_clock::now();

    std::vector<int> set_k;
    for (int i = k_range.first; i <= k_range.second; ++i)
        set_k.push_back(i);

    std::vector<std::pair<int, int>> data;
    std::vector<int> search_ids;
    search_ids.reserve(100000);

    //  result[i][j][k]
    //      i -> data structure type
    //      j -> experiment type
    //      k -> sum of run time

    std::vector<std::vector<std::vector<std::chrono::milliseconds>>> result(
        3, std::vector<std::vector<std::chrono::milliseconds>>(
               NUMBER_OF_EXPERIMENTS, std::vector<std::chrono::milliseconds>(
                                          set_k.size(), std::chrono::milliseconds(0))));

    int data_rank = 0;
    for (auto k : set_k)
    {
        size_t n = 1 << k;
        for (int i = 0; i < EXPERIMENT_ROUNDS; ++i)
        {
            auto round_start_time = std::chrono::high_resolution_clock::now();

            std::cout << "\033[2K" << "Testing : k = " << k << ", Round : " << i + 1 << "\n";
            DataStructureA *a = new DataStructureA();
            DataStructureB *b = new DataStructureB();
            DataStructureC *c = new DataStructureC();
            HomeworkRequirement *data_structure[] = {a, b, c};
            // HomeworkRequirement *data_structure[] = {a, b};
            
            generate_data(data, n);
            search_ids.clear();
            for (size_t i = 0; i < 100000; ++i)
                search_ids.push_back(random_id(gen));

            std::cout << "Previous test info :\n";
            for (int experiment_number = 0; experiment_number < NUMBER_OF_EXPERIMENTS; ++experiment_number)
            {
                auto experiment_start_time = std::chrono::high_resolution_clock::now();
                for (int data_structure_type = 0; data_structure_type < sizeof(data_structure) / sizeof(HomeworkRequirement *); ++data_structure_type) // experiment 1
                {
                    auto start_time = std::chrono::high_resolution_clock::now();
                    if (experiment_number == 0)
                    {
                        for (auto d : data)
                            data_structure[data_structure_type]->insert(d.first, d.second);
                    }
                    else if (experiment_number == 1)
                    {
                        for (auto search_id : search_ids)
                            data_structure[data_structure_type]->search(search_id);
                    }
                    else if (experiment_number == 2)
                    {
                        data_structure[data_structure_type]->total();
                    }

                    auto end_time = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                    result[data_structure_type][experiment_number][data_rank] += duration;
                }

                auto experiment_end_time = std::chrono::high_resolution_clock::now();
                auto experiment_duration = std::chrono::duration_cast<std::chrono::milliseconds>(experiment_end_time - experiment_start_time);
                std::cout << "\033[2K" << "Experiment " << experiment_number + 1 << " time : " << experiment_duration.count() << "(ms)\n";
            }

            auto round_end_time = std::chrono::high_resolution_clock::now();
            auto round_duration = std::chrono::duration_cast<std::chrono::milliseconds>(round_end_time - round_start_time);

            std::cout << "\033[2K" << "Total time : " << round_duration.count() << "(ms)\n";
            std::cout << "\033[u";
        }

        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < NUMBER_OF_EXPERIMENTS; ++j)
            {
                    result[i][j][data_rank] /= EXPERIMENT_ROUNDS;
            }
        }

        json result_json;
        result_json["k"] = k;
        for (size_t i = 0; i < 3; ++i)
        {
            for (size_t j = 0; j < NUMBER_OF_EXPERIMENTS; ++j)
                result_json[name_mapping[i]]["experiment_" + std::to_string(j + 1)] = result[i][j][data_rank].count();
        }

        std::string filename = "result_k_" + std::to_string(k) + ".json";
        std::filesystem::path file_path = std::filesystem::current_path() / "result" / filename;
        if (!std::filesystem::exists(file_path))
            std::ofstream(file_path);
        std::ofstream fs(file_path, std::ios::out | std::ios::trunc);
        if (!fs.is_open())
            std::cerr << "Cannot open file: " << file_path << std::endl;

        fs << result_json.dump(4);
        fs.close();

        ++data_rank;
    }
    std::cout << "\033[10B";

    auto experiment_end_time = std::chrono::high_resolution_clock::now();
    auto experiment_duration = std::chrono::duration_cast<std::chrono::milliseconds>(experiment_end_time - experiment_start_time);
    std::cout << "\nExperiment total time : " << experiment_duration.count() << "(ms)";
    return 0;
}
#endif

#ifdef BLOCK_3
// Extra test
int main()
{
    return 0;
}
#endif