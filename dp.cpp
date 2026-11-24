#include "dp.h"
#include <algorithm>

bool Task::operator==(const Task & n) { return taskid == n.taskid; }

std::tuple<std::vector<std::string>, int> Task::time_helper(std::tuple<std::vector<std::string>, int> tasks_and_time) const {
	// create pointers to task id list and current time value for better readability
	std::vector<std::string>* task_id_list = &std::get<0>(tasks_and_time);
	int* current_time = &std::get<1>(tasks_and_time);

	// if dependency not in task list (already accounted for), do not add its duration to total time
	if (std::find(task_id_list->begin(), task_id_list->end(), taskid) == task_id_list->end())
		return tasks_and_time;

	std::tuple<std::vector<std::string>, int> temp_tasks_and_time;
	std::vector<std::string>* temp_id_list = &std::get<0>(temp_tasks_and_time);
	int* temp_time = &std::get<1>(temp_tasks_and_time);

	// recursively iterate through all dependencies of current task.
	for (auto dependency : dependencies) {
		temp_tasks_and_time = dependency.time_helper(tasks_and_time);

		// sync current task list with temporary task list from recursive call
		if (!temp_id_list->empty()) {
			for (std::string id : *temp_id_list) {
				auto it = std::find(task_id_list->begin(), task_id_list->end(), id);
				if (it == task_id_list->end())
					task_id_list->erase(it);
			}

		}
		*current_time += *temp_time;
	}
	// update task list and time for current task
	task_id_list->erase(std::find(task_id_list->begin(), task_id_list->end(), taskid));
	*current_time += duration;

	return tasks_and_time;
}

int Task::is_option(int time, std::vector<Task> const task_list) const {
	// initialize recursive state
	std::tuple<std::vector<std::string>, int> tasks_and_time = {{}, time};
	for (int i = 0; i < task_list.size(); i++) {
		std::get<0>(tasks_and_time).push_back(task_list.at(i).taskid);
	}
	// get total time of dependencies
	tasks_and_time = time_helper(tasks_and_time);
	time = std::get<1>(tasks_and_time);

	if (time > deadline)
		return -1;
	
	return time;
}

std::tuple<std::vector<std::string>, int> Task::track_dependencies(std::vector<Task> const & task_list, int time) const {
	// tuple index 0: id of chosen tasks
	// tuple index 1: total reward
	std::tuple<std::vector<std::string>, int> final{{taskid}, 0};
	std::tuple<std::vector<std::string>, int> temp = {{}, 0};
	// TODO: create deep copy of and modify task list to prevent the same dependency being chosen twice. Ensure change is made before recursive call is made.
	if (is_option(time, task_list)) {
		std::get<1>(final) += reward;
	}
	for (auto dependency : dependencies) {
		// if dependency not found in current task list, it must already have been chosen.
		if (std::find(task_list.begin(), task_list.end(), dependency) == task_list.end())
			continue;

		temp = dependency.track_dependencies(task_list, time);
		// append dependencies chosen from recursive call to final task list
		if (!std::get<0>(temp).empty()) {
			std::get<0>(final).insert(std::get<0>(final).end(), std::get<0>(temp).begin(), std::get<0>(temp).end());
		}
		std::get<1>(final) += std::get<1>(temp);
	}
	return final;
}

int dynamic_schedule(std::vector<Task> const & task_list, int time) {
	// incorporate memo table
	// iterate through tasks, determining which can be chosen
	std::vector<int> rewards = {};
	std::vector<Task> task_list_copy = {};
	for (int i = 0; i < task_list.size(); i++) {
		// TODO: Possibly rethink is_option usage due to updated recursion of dependencies
		if (task_list.at(i).is_option(time, task_list)) {
			// create copy of task list, excluding options
			// TODO: incorporate dependencies into choice, reward and task_list removal
			task_list_copy = task_list;
			// TODO: insert loop to remove chosen tasks this path from task list copy
			// TODO: ensure that time is correctly incremented based on chosen task(s) for each choice
			rewards.at(i) = dynamic_schedule(task_list_copy, time++);
		}
	}
	std::sort(rewards.begin(), rewards.end());
	return rewards.at(rewards.size() - 1);
}
