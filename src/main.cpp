#include <chrono>
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

void generate_id(std::vector<int> &data, size_t count)
{
    data.clear();
    data.reserve(count + 5);
    for (size_t i = 0; i < count; ++i)
        data.push_back(random_id(gen));
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
// Homework experiment
json result_json;
const std::string filename = "result_k_" + std::to_string(k_range.first) + "_" + std::to_string(k_range.second) + ".json";
const std::filesystem::path file_path = std::filesystem::current_path() / filename;

int main()
{
    auto experiment_start_time = std::chrono::high_resolution_clock::now();
    std::cout << "\033[2J" << "\033[s";
    // Load experiment result
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

    std::vector<std::pair<int, int>> data;
    std::vector<int> search_ids;
    search_ids.reserve(NUMBER_OF_SEARCHS);

    for (int k = k_range.first; k <= k_range.second; ++k)
    {
        size_t n = 1 << k;
        for (int i = 0; i < EXPERIMENT_ROUNDS; ++i)
        {
            auto round_start_time = std::chrono::high_resolution_clock::now();

            std::cout << "\033[u";
            std::cout << "\033[2K" << "Testing : k = " << k << ", Round : " << i + 1 << "\n";
            DataStructureA *a = new DataStructureA();
            DataStructureB *b = new DataStructureB();
            DataStructureC *c = new DataStructureC();
            std::vector<std::pair<HomeworkRequirement *, std::string>> data_structures = {
                {a, "Data structure A"},
                {b, "Data structure B"},
                {c, "Data structure C"}};

            generate_data(data, n);
            generate_id(search_ids, NUMBER_OF_SEARCHS);

            std::cout << "\033[2K" << "Previous test info :\n";
            for (int data_structure_type = 0; data_structure_type < data_structures.size(); ++data_structure_type) // experiment 1
            {
                const std::vector<std::string> keys = {"k=" + std::to_string(k), "round=" + std::to_string(i + 1), data_structures[data_structure_type].second};
                json *tmp_json = &result_json;
                bool contained_object = true;
                for (const auto &key : keys)
                {
                    if (!tmp_json->contains(key) || !(*tmp_json)[key].is_object())
                    {
                        contained_object = false;
                        break;
                    }
                    tmp_json = &((*tmp_json)[key]);
                }
                if (contained_object)
                    continue;

                std::cout << "\033[2K" << data_structures[data_structure_type].second << ":\n";
                json part_result_json;
                for (int experiment_number = 0; experiment_number < NUMBER_OF_EXPERIMENTS; ++experiment_number)
                {
                    std::cout << "\tExperiment " << experiment_number + 1;
                    auto start_time = std::chrono::high_resolution_clock::now();
                    if (experiment_number == 0)
                    {
                        // Skip data structure C insertion experiment when k > 20
                        if (k > 20 && data_structure_type == 2)
                        {
                            data_structures[data_structure_type].first = new DataStructureC(data);
                        }
                        else
                        {
                            for (const auto &d : data)
                                data_structures[data_structure_type].first->insert(d.first, d.second);
                        }
                    }
                    else if (experiment_number == 1)
                    {
                        for (const auto &search_id : search_ids)
                            data_structures[data_structure_type].first->search(search_id);
                    }
                    else if (experiment_number == 2)
                    {
                        data_structures[data_structure_type].first->total();
                    }
                    auto end_time = std::chrono::high_resolution_clock::now();

                    if (experiment_number == 0 || experiment_number == 1)
                    {
                        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                        part_result_json[std::to_string(experiment_number + 1)] = duration.count();

                        std::cout << "\033[K" << " time : " << duration.count() << "(ms)\n";
                    }
                    else if (experiment_number == 2)
                    {
                        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
                        part_result_json[std::to_string(experiment_number + 1)] = duration.count();

                        std::cout << "\033[K" << " time : " << duration.count() << "(µs)\n";
                    }
                }
                // Save experiment result
                result_json["k=" + std::to_string(k)]["round=" + std::to_string(i + 1)][data_structures[data_structure_type].second] = part_result_json;
                std::ofstream ofs(file_path);
                ofs << result_json.dump(4);
                ofs.close();

                delete data_structures[data_structure_type].first;
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

    return 0;
}
#endif

#ifdef BLOCK_3
// Extra experiment
// 混合測試?
// k = 14
//
json result_json;
const std::string filename = "extra_experiment.json";
const std::filesystem::path file_path = std::filesystem::current_path() / filename;
std::uniform_int_distribution random_int(1, 100);

int main()
{
    auto experiment_start_time = std::chrono::high_resolution_clock::now();
    std::cout << "\033[2J" << "\033[s";

    std::vector<std::pair<int, std::pair<int, int>>> data;

    for (int k = 10; k <= 90; k += 10)
    {
        size_t n = 1 << 14;
        for (int i = 0; i < EXPERIMENT_ROUNDS; ++i)
        {
            auto round_start_time = std::chrono::high_resolution_clock::now();

            std::cout << "\033[u";
            std::cout << "\033[2K" << "Testing : k = " << k << ", Round : " << i + 1 << "\n";
            DataStructureA *a = new DataStructureA();
            DataStructureB *b = new DataStructureB();
            DataStructureC *c = new DataStructureC();
            std::vector<std::pair<HomeworkRequirement *, std::string>> data_structures = {
                {a, "Data structure A"},
                {b, "Data structure B"},
                {c, "Data structure C"}};

            data.clear();
            for (int i = 0; i < n; ++i)
            {
                int tmp = random_int(gen);
                if (tmp <= k)
                    data.emplace_back(0, std::pair(random_id(gen), random_score(gen)));
                else
                    data.emplace_back(1, std::pair(random_id(gen), 0));
            }

            std::cout << "\033[2K" << "Previous test info :\n";
            for (int data_structure_type = 0; data_structure_type < data_structures.size(); ++data_structure_type) // experiment 1
            {
                std::cout << "\033[2K" << data_structures[data_structure_type].second << ":\n";
                auto start_time = std::chrono::high_resolution_clock::now();
                for (const auto &d : data)
                {
                    if (d.first == 0)
                    {
                        data_structures[data_structure_type].first->insert(d.second.first, d.second.second);
                    }
                    else
                    {
                        data_structures[data_structure_type].first->search(d.second.first);
                    }
                }
                auto end_time = std::chrono::high_resolution_clock::now();

                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
                result_json["k=" + std::to_string(k)][data_structures[data_structure_type].second]["round=" + std::to_string(i + 1)] = duration.count();

                std::cout << "\033[K" << " time : " << duration.count() << "(µs)\n";
                // Save experiment result
                std::ofstream ofs(file_path);
                ofs << result_json.dump(4);
                ofs.close();

                delete data_structures[data_structure_type].first;
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

    return 0;
}
#endif