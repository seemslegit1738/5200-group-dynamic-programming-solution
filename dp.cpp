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
		for (std::string id : *task_id_list) {
			auto it = std::find(temp_id_list->begin(), temp_id_list->end(), id);
			if (it == temp_id_list->end())
				task_id_list->erase(it);
		}

		*current_time += *temp_time;
	}
	// update task list and time for current task
	auto it = std::find(task_id_list->begin(), task_id_list->end(), taskid);
	// ensure that the current task id exists in the list: if not, skip without adding its duration.
	if (it != task_id_list->end())
		task_id_list->erase(it);
	else
		return tasks_and_time;
	*current_time += duration;

	return tasks_and_time;
}

std::tuple<std::vector<std::string>, int> Task::is_option(int time, std::vector<Task> const task_list) const {
	// initialize recursive state
	std::tuple<std::vector<std::string>, int> tasks_and_time = {{}, time};
	for (int i = 0; i < task_list.size(); i++) {
		std::get<0>(tasks_and_time).push_back(task_list.at(i).taskid);
	}
	// get total time of dependencies
	tasks_and_time = time_helper(tasks_and_time);

	if (std::get<1>(tasks_and_time) > deadline)
		return {{}, -1};
	
	return tasks_and_time;
}

int dynamic_schedule(std::vector<Task> const & task_list, int time) {
	// TODO: incorporate memo table

	// iterate through tasks, determining which can be chosen
	std::vector<int> rewards = {};
	std::vector<Task> task_list_copy = {};
	for (int i = 0; i < task_list.size(); i++) {
		auto tasks_and_time = task_list.at(i).is_option(time, task_list);
		if (std::get<1>(tasks_and_time) != -1) {
			// create copy of task list, excluding options
			for (std::string id : std::get<0>(tasks_and_time)) {
				for (int j = 0; j < task_list.size(); j++) {
					if (task_list.at(j).taskid == id)
						task_list_copy.push_back(task_list.at(j));
				}
			}
			rewards.at(i) = dynamic_schedule(task_list_copy, std::get<1>(tasks_and_time));
		}
	}
	std::sort(rewards.begin(), rewards.end());
	// return greatest reward
	return rewards.at(rewards.size() - 1);
}
