# 資料結構 HW1
## Code Explanation
#### Code: https://github.com/antyee87/DataStructureHW1
* ### Class
    * HomewprkRequirement
        - An interface with:
            * void insert(int id, int score);
            * std::vector\<int> search(int id)

    * DataStructureA  
        - Dyanamic Array with sort when insert.

    * DataStructureB
        - Static Array, id as index, link

    * DataStructureC
        - linked list, sorted by memory address

* ### Experiment
    * Experiment 1
        - Insertion time

    * Experiment 2
        - Search time

    * Experiment 3
        - Traversal time

    * Experiment Extra

## Experiment 
* ### Environment
    - CPU: 12th Gen Intel(R) Core(TM) i7-12700 (2.10 GHz)
    - RAM: 24GB
    - OS: Windows 11 家用版 22H2 (Build 26100.6584)
    - Compiler: Visual Studio 2022 (MSVC)

* ### Experiment Result
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

    * ##### Insertion time
    ![Insertion time](https://github.com/antyee87/DataStructureHW1/blob/main/experiment1.png?raw=true)

    設n為資料量
    Data structure A 的插入效率: O(n)
    Data structure B 的插入效率: O(1)
    Data structure C 的插入效率: O(n)
    圖中折線特徵符合預測(k=11~20)
    Data structure A每k+1時，需要插入量為2倍，遍歷時長與鏈表長度成正相關，從k=20所花時間 取 2 * [k-1 -> k的長度期望值倍率] * [k-1資料點所花時間]

    * ##### Search time
    ![Search time](https://github.com/antyee87/DataStructureHW1/blob/main/experiment2.png?raw=true)

    設n為長度
    Data structure A 的搜尋效率: O(log n)
    Data structure B 的搜尋效率: O(1)
    Data structure C 的搜尋效率: O(n)
    圖中折線特徵大致符合預測，除了Data structure C在k=24時時間突然衝髙，有可能是其他程式佔用電腦資源的影響


    * ##### Traversal time
    ![Traversa time](https://github.com/antyee87/DataStructureHW1/blob/main/experiment3.png?raw=true)

    遍歷速度基本上相同，只差在CPU cache命中率
    Data structure A會稍慢，因為為了避免插入效率過低，用指標儲存資料，指標解引用耗費了額外時間