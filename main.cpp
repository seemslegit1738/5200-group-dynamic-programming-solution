#include <iostream>
#include "dp.h"

int main() {
	std::vector<std::vector<Task>> FIXED_DATASETS = fixed_datasets();
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
	int reward = 0;

	reward = dynamic_schedule(FIXED_DATASETS.at(0), 0);
	std::cout << "D1_INDEPENDENT: " << reward << "\n";

	reward = dynamic_schedule(FIXED_DATASETS.at(0), 0);

	for (int i = 0; i < sizeof(dataset_names) / sizeof(dataset_names[0]); i++) {
		reward = dynamic_schedule(FIXED_DATASETS.at(i), 0);
		std::cout << dataset_names[i] << ": " << reward << "\n";
	}

	return 0;
}
