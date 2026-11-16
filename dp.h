#pragma once
#include <vector>
#include <string>
#include <tuple>

class Task {
	int duration;
	int deadline;
	int reward;
	// Choice: either store dependencies by ID or by pointer to memory location
	std::vector<Task> dependencies;

	public:
		std::string taskid;
		// TODO: create constructor definition
		Task();
		bool operator==(const Task & n);
		// determines whether task can be chosen
		bool is_option(int time);
		std::tuple<std::vector<std::string>, int> track_dependencies(int time);
};

// recursive, main dynamically programmed function
int dynamic_schedule(std::vector<Task> & task_list, int time);
