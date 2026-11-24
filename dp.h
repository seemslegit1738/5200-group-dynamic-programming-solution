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
		/*
 			Determines whether or not a task can be chosen
				- If the total duration of the task and its dependencies + the current time exceeds the deadline, is_option returns -1.
				- Otherwise, it returns the total duration of the task and its dependencies + the time that was passed in.
		*/
		int is_option(int time, std::vector<Task> const task_list) const;
		// helper function for is_option
		std::tuple<std::vector<std::string>, int> time_helper(std::tuple<std::vector<std::string>, int> tasks_and_time) const;
		// TODO: Document function once finished
		std::tuple<std::vector<std::string>, int> track_dependencies(std::vector<Task> const & task_list, int time) const;
};

// recursive, main dynamically programmed function
int dynamic_schedule(std::vector<Task> const & task_list, int time);
