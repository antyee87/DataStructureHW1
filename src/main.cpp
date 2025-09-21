#include <chrono>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <nlohmann/json.hpp>
#include "DataStructureA.h"
#include "DataStructureB.h"
#include "DataStructureC.h"

#define BLOCK_2

using json = nlohmann::ordered_json;
const std::pair<int, int> k_range(11, 25); // Include end
const int NUMBER_OF_EXPERIMENTS = 3;
const int EXPERIMENT_ROUNDS = 10;
const int NUMBER_OF_SEARCHS = 100000;

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
const std::string name_mapping[] = {"Data structure A", "Data structure B", "Data structure C"};
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
json result_json;
const std::string filename = "result_k_" + std::to_string(k_range.first) + "_" + std::to_string(k_range.second) + ".json";
const std::filesystem::path file_path = std::filesystem::current_path() / filename;
std::vector<int> progress(3, -1);

void save_and_exit(int signal)
{
    std::ofstream ofs(file_path);
    ofs << result_json.dump(4);
    ofs.close();
    std::_Exit(0);
}

int main() 
{
    auto experiment_start_time = std::chrono::high_resolution_clock::now();
    std::cout << "\033[2J" << "\033[s";
    std::signal(SIGINT, save_and_exit);
    // load experiment result
    if (!std::filesystem::exists(file_path))
    {
        std::ofstream ofs(file_path);
        ofs.close();
    }
    std::ifstream ifs(file_path);
    try
    {
        result_json = nlohmann::json::parse(ifs);
    }
    catch (...)
    {
    }
    
    ifs.close();

    if (result_json.contains("progress"))
        progress = result_json["progress"].get<std::vector<int>>();
    else
        result_json["progress"] = progress;

    std::vector<std::pair<int, int>> data;
    std::vector<int> search_ids;
    search_ids.reserve(NUMBER_OF_SEARCHS);

    for (int k = k_range.first; k <= k_range.second; ++k)
    {
        if (k < progress[0])
            continue;

        size_t n = 1 << k;
        for (int i = 0; i < EXPERIMENT_ROUNDS; ++i)
        {
            if (k == progress[0] && i < progress[1])
                continue;

            auto round_start_time = std::chrono::high_resolution_clock::now();

            std::cout << "\033[2K" << "Testing : k = " << k << ", Round : " << i + 1 << "\n";
            DataStructureA *a = new DataStructureA();
            DataStructureB *b = new DataStructureB();
            DataStructureC *c = new DataStructureC();
            std::vector<std::pair<HomeworkRequirement *, std::string>> data_structures = {
                {a, "Data structure A"},
                {b, "Data structure B"},
                {c, "Data structure A"}
            };
            
            generate_data(data, n);
            search_ids.clear();
            for (size_t i = 0; i < NUMBER_OF_SEARCHS; ++i)
                search_ids.push_back(random_id(gen));

            std::cout << "\033[2K" << "Previous test info :\n";
            for (int data_structure_type = 0; data_structure_type < data_structures.size(); ++data_structure_type) // experiment 1
            {
                if (k == progress[0] && i == progress[1] && data_structure_type <= progress[2])
                    continue;

                std::cout << "\033[2K" << data_structures[data_structure_type].second << ":\n";
                for (int experiment_number = 0; experiment_number < NUMBER_OF_EXPERIMENTS; ++experiment_number)
                {
                    std::cout << "\tExperiment " << experiment_number + 1;
                    auto start_time = std::chrono::high_resolution_clock::now();
                    if (experiment_number == 0)
                    {
                        for (auto d : data)
                            data_structures[data_structure_type].first->insert(d.first, d.second);
                    }
                    else if (experiment_number == 1)
                    {
                        for (auto search_id : search_ids)
                            data_structures[data_structure_type].first->search(search_id);
                    }
                    else if (experiment_number == 2)
                    {
                        data_structures[data_structure_type].first->total();
                    }
                    auto end_time = std::chrono::high_resolution_clock::now();
                    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                    result_json[data_structures[data_structure_type].second][std::to_string(experiment_number + 1)][std::to_string(k)][std::to_string(i)] = duration.count();

                    std::cout << "\033[K" << " time : " << duration.count() << "(ms)\n";
                }
                result_json["progress"] = std::vector<int>{k, i, data_structure_type};
            }

            auto round_end_time = std::chrono::high_resolution_clock::now();
            auto round_duration = std::chrono::duration_cast<std::chrono::milliseconds>(round_end_time - round_start_time);

            std::cout << "\033[2K" << "Total time : " << round_duration.count() << "(ms)\n";
            std::cout << "\033[u";
        }      
    }
    std::cout << "\033[10B";

    auto experiment_end_time = std::chrono::high_resolution_clock::now();
    auto experiment_duration = std::chrono::duration_cast<std::chrono::milliseconds>(experiment_end_time - experiment_start_time);
    std::cout << "\nExperiment total time : " << experiment_duration.count() << "(ms)";

    std::ofstream ofs(file_path);
    ofs << result_json.dump(4);
    ofs.close();
    return 0;
}
#endif

#ifdef BLOCK_3
// Extra test
// 混合工作負載?
int main()
{
    return 0;
}
#endif