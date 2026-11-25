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

std::tuple<int, int, std::vector<Task>> Task::maximize_dependency_reward(int time, int reward, std::vector<Task> choosable_dependencies) {
	// create tuple of passed-in function args
	std::tuple<int, int, std::vector<Task>> args = {time, reward, choosable_dependencies};
	if (choosable_dependencies.size() > 1) {
		// determines which dependencies are still before their deadline. Dependencies that are not are indiscriminately executed last.
		std::vector<Task> depchoices = {};
		std::vector<Task> chooselast = {};
		int total_time = time;
		// TODO: Fix this to account for the calling Task's own dependencies, excluding any missing from the list (already executed)
		for (auto dependency : choosable_dependencies) {
			if (std::get<1>(dependency.is_option(time, choosable_dependencies)) != -1)
				depchoices.push_back(dependency);
			else
				chooselast.push_back(dependency);
		}
		std::vector<std::tuple<int, int, std::vector<Task>>> results = {};
		int i = 0;
		for (auto dep : depchoices) {
			std::vector<Task> depchoices_copy = depchoices;
			depchoices_copy.erase(std::find(depchoices_copy.begin(), depchoices_copy.end(), dep));
			auto result = maximize_dependency_reward(time + dep.duration, reward + dep.reward, depchoices_copy);
			results.at(i) = result;
			i++;
		}
		// return maximum reward
		std::vector<int> rewards;
		for (auto res : results) {
			rewards.push_back(std::get<1>(res));
		}
		std::sort(rewards.begin(), rewards.end());
		int total_reward = rewards.at(rewards.size() - 1);
		// time will be the same no matter the order
		total_time += std::get<0>(results.at(0));
		for (auto dep : chooselast)
			total_time += dep.duration;
		// TODO: remove from choosable_dependencies all dependencies that have been chosen.
		return {total_time, total_reward, choosable_dependencies};

	} else if (dependencies.size() == 1) {
		// TODO: Perhaps at the beginning of the function, create a vector that is the intersection between the choosable dependencies and the dependencies of the current Task. Reworking this function with that should make things much easier.
		// Then, make sure to fix how choosable_dependencies is passed into the return statement.
		
		if (std::get<1>(dependencies.at(0).is_option(time, choosable_dependencies)) != -1)
			return {time + dependencies.at(0).duration, reward + dependencies.at(0).reward, choosable_dependencies};
		return {time + dependencies.at(0).duration, reward, choosable_dependencies};
	}
	return args;
}

int dynamic_schedule(std::vector<Task> const & task_list, int time) {
	// TODO: incorporate memo table

	std::vector<int> rewards = {};
	std::vector<Task> task_list_copy = {};
	// iterate through tasks, determining which can be chosen
	for (int i = 0; i < task_list.size(); i++) {
		auto tasks_and_time = task_list.at(i).is_option(time, task_list);
		if (std::get<1>(tasks_and_time) == -1) {
			continue;
		}
		// create copy of task list, excluding options
		for (std::string id : std::get<0>(tasks_and_time)) {
			for (int j = 0; j < task_list.size(); j++) {
				if (task_list.at(j).taskid == id)
					task_list_copy.push_back(task_list.at(j));
			}
		}
		rewards.at(i) += dynamic_schedule(task_list_copy, std::get<1>(tasks_and_time));
		
	}
	std::sort(rewards.begin(), rewards.end());
	// return greatest reward
	return rewards.at(rewards.size() - 1);
}
