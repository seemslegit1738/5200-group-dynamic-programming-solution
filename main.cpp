#include <iostream>
#include "dp.h"

void print_results(std::tuple<std::vector<std::string>, int> reward, std::string dataset_name) {
	std::cout << "\n" << dataset_name << ":\n";

	std::vector<std::string>& task_id_list = std::get<0>(reward);
	int& total_reward = std::get<1>(reward);

	std::cout << "\nList of tasks (in order): ";
	for (std::string id : task_id_list) {
		std::cout << id << " ";
	}

	std::cout << "\nTotal reward: " << total_reward << std::endl;
}

int main() {
	// initialize fixed datasets
	std::vector<Task> D1_INDEPENDENT = {
		{"A", 3, 8, 20},
		{"B", 2, 5, 25},
		{"C", 4, 10, 30},
		{"D", 1, 3, 15},
		{"E", 5, 12, 40}
	};

	std::vector<Task> D2_CHAIN = {
		{"A", 2, 3, 15},
		{"B", 3, 7, 25},
		{"C", 2, 9, 20},
		{"D", 4, 14, 35},
		{"E", 1, 15, 10}
	};
	D2_CHAIN.at(1).add_dependency(&D2_CHAIN.at(0));
	D2_CHAIN.at(2).add_dependency(&D2_CHAIN.at(1));
	D2_CHAIN.at(3).add_dependency(&D2_CHAIN.at(2));
	D2_CHAIN.at(4).add_dependency(&D2_CHAIN.at(3));

	std::vector<Task> D3_BRANCHING = {
		{"A", 2, 4, 20},
		{"B", 3, 8, 30},
		{"C", 2, 7, 25},
		{"D", 2, 11, 35},
		{"E", 3, 10, 20},
		{"F", 1, 13, 15}
	};
	D3_BRANCHING.at(1).add_dependency(&D3_BRANCHING.at(0));
	D3_BRANCHING.at(2).add_dependency(&D3_BRANCHING.at(0));
	D3_BRANCHING.at(3).add_dependency(&D3_BRANCHING.at(1));
	D3_BRANCHING.at(3).add_dependency(&D3_BRANCHING.at(2));
	D3_BRANCHING.at(5).add_dependency(&D3_BRANCHING.at(3));

	std::vector<Task> D4_GREEDY_CHALLENGE = {
		{"A", 5, 5, 50},
		{"B", 2, 2, 24},
		{"C", 2, 4, 24},
		{"D", 2, 6, 22},
		{"E", 1, 7, 10}
	};

	std::vector<Task> D5_LATE_PREREQUISITE = {
		{"A", 4, 2, 30},
		{"B", 2, 7, 50},
		{"C", 1, 8, 20},
		{"D", 2, 4, 25}
	};
	D5_LATE_PREREQUISITE.at(1).add_dependency(&D5_LATE_PREREQUISITE.at(0));
	D5_LATE_PREREQUISITE.at(2).add_dependency(&D5_LATE_PREREQUISITE.at(1));

	std::vector<Task> D6_TIGHT_DEADLINES = {
		{"A", 2, 3, 20},
		{"B", 3, 6, 45},
		{"C", 1, 2, 15},
		{"D", 2, 7, 30},
		{"E", 2, 9, 35},
		{"F", 1, 10, 15}
	};
	D6_TIGHT_DEADLINES.at(1).add_dependency(&D6_TIGHT_DEADLINES.at(0));
	D6_TIGHT_DEADLINES.at(4).add_dependency(&D6_TIGHT_DEADLINES.at(1));
	D6_TIGHT_DEADLINES.at(5).add_dependency(&D6_TIGHT_DEADLINES.at(3));

	std::vector<Task> D7_IMPOSSIBLE_DEADLINES = {
		{"A", 5, 2, 40},
		{"B", 3, 4, 30},
		{"C", 2, 3, 25},
		{"D", 1, 10, 20},
		{"E", 2, 12, 35}
	};
	D7_IMPOSSIBLE_DEADLINES.at(1).add_dependency(&D7_IMPOSSIBLE_DEADLINES.at(0));
	D7_IMPOSSIBLE_DEADLINES.at(3).add_dependency(&D7_IMPOSSIBLE_DEADLINES.at(1));
	D7_IMPOSSIBLE_DEADLINES.at(4).add_dependency(&D7_IMPOSSIBLE_DEADLINES.at(3));

	std::vector<Task> D8_MIXED_LARGE = {
		{"A", 2, 5, 20},
		{"B", 3, 8, 35},
		{"C", 1, 4, 15},
		{"D", 4, 10, 40},
		{"E", 2, 12, 30},
		{"F", 3, 9, 25},
		{"G", 2, 14, 35},
		{"H", 1, 13, 20},
		{"I", 3, 16, 40},
		{"J", 2, 18, 30}
	};
	D8_MIXED_LARGE.at(1).add_dependency(&D8_MIXED_LARGE.at(0));
	D8_MIXED_LARGE.at(4).add_dependency(&D8_MIXED_LARGE.at(1));
	D8_MIXED_LARGE.at(5).add_dependency(&D8_MIXED_LARGE.at(2));
	D8_MIXED_LARGE.at(6).add_dependency(&D8_MIXED_LARGE.at(3));
	D8_MIXED_LARGE.at(7).add_dependency(&D8_MIXED_LARGE.at(4));
	D8_MIXED_LARGE.at(7).add_dependency(&D8_MIXED_LARGE.at(5));
	D8_MIXED_LARGE.at(8).add_dependency(&D8_MIXED_LARGE.at(6));
	D8_MIXED_LARGE.at(9).add_dependency(&D8_MIXED_LARGE.at(7));
	D8_MIXED_LARGE.at(9).add_dependency(&D8_MIXED_LARGE.at(8));

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

	std::string dataset_names[] = {
		"D1_INDEPENDENT",
		"D2_CHAIN",
		"D3_BRANCHING",
		"D4_GREEDY_CHALLENGE",
		"D5_LATE_PREREQUISITE",
		"D6_TIGHT_DEADLINES",
		"D7_IMPOSSIBLE_DEADLINES",
		"D8_MIXED_LARGE"
	};
	std::tuple<std::vector<std::string>, int> reward = {{}, 0};

	for (int i = 0; i < static_cast<int>(sizeof(dataset_names) / sizeof(dataset_names[0])); i++) {
		reward = dynamic_schedule(FIXED_DATASETS.at(i), 0);
		print_results(reward, dataset_names[i]);
		clear_memo_table();
	}

	return 0;
}
