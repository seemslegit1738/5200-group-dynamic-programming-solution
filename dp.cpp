#include "dp.h"
#include <algorithm>

bool Task::operator==(const Task & n) { return taskid == n.taskid; }

bool Task::is_option(int time) {
	if (time + duration > deadline)
		return false;
	
	return true;
}

std::tuple<std::vector<std::string>, int> Task::track_dependencies(int time) {
	// tuple index 0: id of chosen task
	// tuple index 1: total value
	// TODO: verify initialization of vector on line 18 is valid
	std::tuple<std::vector<std::string>, int> final{{taskid}, 0};
	std::tuple<std::vector<std::string>, int> temp = {{}, 0};
	for (auto dependency : dependencies) {
		temp = dependency.track_dependencies(time);
		// append ids of dependencies to chosen task list
		std::get<0>(final).insert(std::get<0>(final).end(), std::get<0>(temp).begin(), std::get<0>(temp).end());
		std::get<1>(final) += std::get<1>(temp);
	}
	if (is_option(time)) {
		std::get<1>(final) += reward;
	}
	return final;
}

// make sure that task_list is not altered! It needs to not be const in order to access member functions
int dynamic_schedule(std::vector<Task> & task_list, int time) {
	// incorporate memo table
	// iterate through tasks, determining which can be chosen
	std::vector<int> rewards = {};
	std::vector<Task> task_list_copy = {};
	for (int i = 0; i < task_list.size(); i++) {
		if (task_list.at(i).is_option(time)) {
			// create copy of task list, excluding options
			// TODO: incorporate dependencies into choice, reward and task_list removal
			// TODO: in a test file, ensure that vector assignment performs deep copy. CRUCIAL!
			task_list_copy = task_list;
			// TODO: insert loop to remove chosen tasks this path from task list copy
			rewards.at(i) = dynamic_schedule(task_list_copy, time++);
		}
	}
	std::sort(rewards.begin(), rewards.end());
	return rewards.at(rewards.size() - 1);
}
