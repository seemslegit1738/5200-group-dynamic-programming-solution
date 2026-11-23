#pragma once
#include <vector>
#include <string>
#include <tuple>

class Task {
	int duration;
	int deadline;
	int reward;
	// Choice: either store dependencies by ID or by pointer to memory location
	// Currently stores stock dependency objects, but may require vector of pointers. Would need adjustment to method access in functions
	std::vector<Task> dependencies;

	public:
		// ID unique to each task
		std::string taskid;
		// TODO: create constructor definition
		Task();
		// == Helper function: determines when two tasks are equivalent (via unique ID)
		bool operator==(const Task & n);
		// determines whether task can be chosen (including dependencies)
		bool is_option(int time) const;
		// helper function for is_option
		std::tuple<std::vector<std::string>, int> time_helper(std::tuple<std::vector<std::string>, int> tasks_and_time) const;
		// TODO: Document function once finished
		std::tuple<std::vector<std::string>, int> track_dependencies(std::vector<Task> const & task_list, int time) const;
};

// recursive, main dynamically programmed function
int dynamic_schedule(std::vector<Task> const & task_list, int time);
