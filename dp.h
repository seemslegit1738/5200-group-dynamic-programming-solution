#pragma once
#include <vector>
#include <string>
#include <tuple>

class Task {
	int duration;
	int deadline;
	// Choice: either store dependencies by ID or by pointer to memory location
	// Currently stores stock dependency objects, but may require vector of pointers. Would need adjustment to method access in functions
	std::vector<Task*> dependencies;

	public:
		// ID unique to each task
		std::string taskid;
		int taskreward;
		Task(std::string taskid, int duration, int deadline, int reward);
		// == Helper function: determines when two tasks are equivalent (via unique ID)
		bool operator==(const Task & n) const;
		bool operator<(const Task & n) const;
		void add_dependency(Task* dependency);
		/*
 			Determines whether or not a task can be chosen
				- If the total duration of the task and its dependencies + the current time exceeds the deadline, is_option returns -1 and an empty task list.
				- Otherwise, it returns the total duration of the task and its dependencies + the time that was passed in.
		*/
		std::tuple<std::vector<std::string>, int> is_option(int time, std::vector<Task> const task_list) const;
		// helper function for is_option
		std::tuple<std::vector<std::string>, int> time_helper(std::tuple<std::vector<std::string>, int> tasks_and_time) const;
		// calculates order to execute dependencies in such that their total reward is maximized; different from dynamic_schedule in that dependencies both eligible and ineligible for reward must all be executed.
		std::tuple<int, int, std::vector<Task>> maximize_dependency_reward(int time, int reward, std::vector<Task> task_list) const;
};

// recursive, main dynamically programmed function
int dynamic_schedule(std::vector<Task> const & task_list, int time);
