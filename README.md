# 資料結構 HW1
---
## Contents
- [Code Explanation](#CodeExplanation)
    - [Data Structure A](#DataStructureA)
    - [Data Structure B](#DataStructureB)
    - [Data Structure C](#DataStructureC)

- [Experiment](#Experiment)
    - [Insertion time](#InsertionTime)
    - [Search time](#SearchTime)
    - [Traversal time](#TraversalTime)
    - [Mixed test time](#mixed-test-time)

- [Thoughts](#Thoughts)

## Code Explanation<a id="CodeExplanation"></a>
#### Code: https://github.com/antyee87/DataStructureHW1
* ### Class
    * HomewprkRequirement
        - An interface with:
        ```c++
        virtual void insert(int id, int score) = 0;
        virtual std::vector<int> search(int id) = 0;
        virtual size_t total() = 0;
        ```

    * DataStructureA<a id=DataStructureA></a>
        - Dyanamic Array with sort when insert.
        ```c++
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
        ```

    * DataStructureB<a id=DataStructureB></a>
        - Static Array, id as index, link
        ```c++
        void DataStructureB::insert(int id, int score)
        {
            data[id].scores.push_back(score);

            // If linked, return
            if (data[id].next_student != -1)
                return;

            // Construct link
            // Find nearest id before this id
            for (size_t i = id - 1; i >= 1; --i)
            {
                if (data[i].scores.size() > 0)
                {
                    data[id].next_student = data[i].next_student;
                    data[i].next_student = id;
                    return;
                }
            }
            // Find nearest id after this id
            begin = id;
            for (size_t i = id + 1; i <= MAX_ID; ++i)
            {
                if (data[i].scores.size() > 0)
                {
                    data[id].next_student = i;
                    return;
                }
            }
            data[id].next_student = 0;
        }

        std::vector<int> DataStructureB::search(int id)
        {
            // Ya, just get scores
            if (data[id].scores.size() > 0)
                return data[id].scores;
            else
                return std::vector<int>();
        }
        ```

    * DataStructureC<a id=DataStructureC></a>
        - linked list, sorted by memory address
        ```c++
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
            // Linear search if id exist
            Node *cur_node = head;
            while (cur_node != nullptr)
            {
                if (cur_node->value.id == id)
                    return cur_node->value.scores;

                cur_node = cur_node->next;
            }
            return std::vector<int>();
        }
        ```

## Experiment<a id="Experiment"></a>
* ### Environment
    - CPU: 12th Gen Intel(R) Core(TM) i7-12700 (2.10 GHz)
    - RAM: 24GB
    - OS: Windows 11 家用版 22H2 (Build 26100.6584)
    - Compiler: Visual Studio 2022 (MSVC)

* ### Result
    1. ##### Insertion time<a id=InsertionTime></a>
    ![Insertion time](https://github.com/antyee87/DataStructureHW1/blob/main/experiment1.png?raw=true)
    <br>
    Data structure A 插入的時間複雜度O(n)
    在本實驗中，因為id上限是$2^{20}$，儲存完所有id後，所花時間只剩下二分搜尋，但陣列長度上限為$2^{20}$，所以時間複雜度應該可以視為從O(n)->O(1)
    Data structure B 插入的時間複雜度: O(n)
    在本實驗中，建立link的時間複雜度應該算O(1)，因為陣列長度固定為$2^{20}$
    Data structure C 插入的時間複雜度: O(n)

    Data structure C每k+1時，需要插入量為2倍，遍歷時長與鏈表長度成正相關，從k=20所花時間，保守估計為 2 * [k-1 -> k的長度期望值倍率] * [k-1資料點所花時間]
        Data structure A、C的長度期望值為: $2^{20}(1-(1 - \frac{1}{2^{20}})^{2^k})$
    | k | 長度期望值 | 倍率 |
    |---|-----------|------|
    |11 |  2,046    |none  |
    |12 |  4,088    |1.998 |
    |13 |  8,160    |1.9961|
    |14 |  16,257   |1.9923|
    |15 |  32,261   |1.9844|
    |16 |  63,530   |1.9693|
    |17 |  123,211  |1.9394|
    |18 |  231,944  |1.8825|
    |19 |  412,583  |1.7788|
    |20 |  662,827  |1.6065|
    |21 |  906,667  |1.3679|
    |22 |  1,029,371|1.1353|
    |23 |  1,048,224|1.0183|
    |24 |  1,048,576|1.0003|
    |25 |  1,048,576|1.0   |

    <br>

    2. ##### Search time<a id=SearchTime></a>
    ![Search time](https://github.com/antyee87/DataStructureHW1/blob/main/experiment2.png?raw=true)
    <br>
    Data structure A 搜尋的時間複雜度: O(log n)
    Data structure B 搜尋的時間複雜度: O(1)
    Data structure C 搜尋的時間複雜度: O(n)
    本實驗中由於id大小的限制，Data structure C長度存在上限，時間複雜度會慢慢變O(1)
    圖中折線特徵大致符合預測，除了Data structure C在k=24時時間突然衝髙，有可能是其他程式佔用電腦資源的影響或是cache miss
    <br>

    3. ##### Traversal time<a id=TraversalTime></a>
    ![Traversal time](https://github.com/antyee87/DataStructureHW1/blob/main/experiment3.png?raw=true)
    <br>
    遍歷時間複雜度都是 O(n)
    遍歷速度基本上相同，但有些有趣的發現:
    Data structure A會稍慢，因為為了避免插入效率過低，用指標儲存資料，指標解引用耗費了額外時間
    Data structure B，在資料量少時資料的記憶體位置並不夠連續，容易cache miss，故速度比其他兩者慢，但随資料量增加，連續性、速度也隨之增加
    Data structure C，雖然是鏈表，但又照記憶體位置排序節點，表現起來跟動態陣列相近，但如上所說，指標解引耗費額外時間，讓他在整個遍歷實驗裡成為表現最佳的資料結構
    <br>    

    4. #### Mixed test time<a id=MixedTestTime></a>
    ![Mixed test time](https://github.com/antyee87/DataStructureHW1/blob/main/experimentExtra.png?raw=true)
    <br>
    ###### 動機:
    資料結構是要使用的，我想測試看看在不同的插入-搜尋比例下，各資料結構的表現如何
    根據網路上的資料，政大113學年度在學學生人數為16,899人，我假設學號都是連續的，選擇測試資料量為$2^{14}$
    資料生成是隨機的，插入-搜尋比例並不完全準確的
    ###### 結果:
    Data structure A 在本資料量下表現最為優秀，插入效率O(n)，搜尋效率O(log n)
    Data structure B 稍遜一籌，插入效率O(n)，搜尋效率O(1)，在本資料量下，插入太慢拖累了表現，link的設計難以說是有益的
    Data structure C 表現極差，插入效率O(n)，搜尋效率O(n)，純粹的鏈表只適合拿來當作學習圖的踏板，並非是實用的資料結構


## Thoughts<a id=Thoughts></a>
過去學習演算法或資料結構後，基本上只有少量的實作，而不會真的去比較不同演算法或資料結構之間的差異(只有高中學習歷程做sorting速度比較，黑歷史)。關於如何構建實驗用的程式，需要如何解釋圖片，或是需要在報告中寫出哪些資訊，都不是很了解，作業要求只有一頁word檔還是太難了。這次的三個資料結構，有種刁難人的感覺，總想改進，但仔細想想要是改進了就不符合作業要求了，實在不是很令人愉悅。
這次依然高強度使用ChatGPT，從應該如何設計實驗架構，到debug，到如何用json儲存檔案(我覺得json比較好讀)等等，除了有時要引誘它說出符合我預期的答案有些困難，整體而言，他很好的簡化我查閱資料與debug的困難。然後我有時會問一些比較跳脫的問題，不是很重要。
聊天紀錄:https://chatgpt.com/share/68df9b2b-55b4-8002-ad3f-3798fd1344e3