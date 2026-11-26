#include <vector>
#include "dp.h"

void fixed_datasets() {
	std::vector<Task> D1_INDEPENDENT = {
		{"A", 3, 8, 20, {}},
		{"B", 2, 5, 25, {}},
		{"C", 4, 10, 30, {}},
		{"D", 1, 3, 15, {}},
		{"E", 5, 12, 40, {}}
	};
	Task d2_chain_task1 = {"A", 2, 3, 15, {}};
	Task d2_chain_task2 = {"B", 3, 7, 25, {&d2_chain_task1}};
	Task d2_chain_task3 = {"C", 2, 9, 20, {&d2_chain_task2}};
	Task d2_chain_task4 = {"D", 4, 14, 35, {&d2_chain_task3}};
	Task d2_chain_task5 = {"E", 1, 15, 10, {&d2_chain_task4}};
	std::vector<Task> D2_CHAIN = {
		d2_chain_task1,
		d2_chain_task2,
		d2_chain_task3,
		d2_chain_task4,
		d2_chain_task5
	};
	Task d3_branching_task1 = {"A", 2, 4, 20, {}};
	Task d3_branching_task2 = {"B", 3, 8, 30, {&d3_branching_task1}};
	Task d3_branching_task3 = {"C", 2, 7, 25, {&d3_branching_task1}};
	Task d3_branching_task4 = {"D", 2, 11, 35, {&d3_branching_task2, &d3_branching_task3}};
	Task d3_branching_task5 = {"E", 3, 10, 20, {}};
	Task d3_branching_task6 = {"F", 1, 13, 15, {&d3_branching_task4}};
	std::vector<Task> D3_BRANCHING = {
		d3_branching_task1,
		d3_branching_task2,
		d3_branching_task3,
		d3_branching_task4,
		d3_branching_task5,
		d3_branching_task6
	};
	std::vector<Task> D4_GREEDY_CHALLENGE = {
		{"A", 5, 5, 50, {}},
		{"B", 2, 2, 24, {}},
		{"C", 2, 4, 24, {}},
		{"D", 2, 6, 22, {}},
		{"E", 1, 7, 10, {}}
	};
	Task d5_task1 = {"A", 4, 2, 30, {}};
	Task d5_task2 = {"B", 2, 7, 50, {&d5_task1}};
	Task d5_task3 = {"C", 1, 8, 20, {&d5_task2}};
	Task d5_task4 = {"D", 2, 4, 25, {}};
	std::vector<Task> D5_LATE_PREREQUISITE = {
		d5_task1,
		d5_task2,
		d5_task3,
		d5_task4
	};
	Task d6_task1 = {"A", 2, 3, 20, {}};
	Task d6_task2 = {"B", 3, 6, 45, {&d6_task1}};
	Task d6_task3 = {"C", 1, 2, 15, {}};
	Task d6_task4 = {"D", 2, 7, 30, {}};
	Task d6_task5 = {"E", 2, 9, 35, {&d6_task2}};
	Task d6_task6 = {"F", 1, 10, 15, {&d6_task4}};
	std::vector<Task> D6_TIGHT_DEADLINES = {
		d6_task1,
		d6_task2,
		d6_task3,
		d6_task4,
		d6_task5,
		d6_task6
	};
	Task d7_task1 = {"A", 5, 2, 40, {}};
	Task d7_task2 = {"B", 3, 4, 30, {&d7_task1}};
	Task d7_task3 = {"C", 2, 3, 25, {}};
	Task d7_task4 = {"D", 1, 10, 20, {&d7_task2}};
	Task d7_task5 = {"E", 2, 12, 35, {&d7_task4}};
	std::vector<Task> D7_IMPOSSIBLE_DEADLINES = {
		d7_task1,
		d7_task2,
		d7_task3,
		d7_task4,
		d7_task5
	};
	Task d8_task1 = {"A", 2, 5, 20, {}};
	Task d8_task2 = {"B", 3, 8, 35, {&d8_task1}};
	Task d8_task3 = {"C", 1, 4, 15, {}};
	Task d8_task4 = {"D", 4, 10, 40, {}};
	Task d8_task5 = {"E", 2, 12, 30, {&d8_task2}};
	Task d8_task6 = {"F", 3, 9, 25, {&d8_task3}};
	Task d8_task7 = {"G", 2, 14, 35, {&d8_task4}};
	Task d8_task8 = {"H", 1, 13, 20, {&d8_task5, &d8_task6}};
	Task d8_task9 = {"I", 3, 16, 40, {&d8_task7}};
	Task d8_task10 = {"J", 2, 18, 30, {&d8_task8, &d8_task9}};
	std::vector<Task> D8_MIXED_LARGE = {
		d8_task1,
		d8_task2,
		d8_task3,
		d8_task4,
		d8_task5,
		d8_task6,
		d8_task7,
		d8_task8,
		d8_task9,
		d8_task10
	};
	std::vector<std::vector<Task>> FIXED_DATASETS = {
		D1_INDEPENDENT,
		D2_CHAIN,
		D3_BRANCHING,
		D4_GREEDY_CHALLENGE,
		D5_LATE_PREREQUISITE,
		D6_TIGHT_DEADLINES,
		D7_IMPOSSIBLE_DEADLINES,
		D8_MIXED_LARGE
	};
}
