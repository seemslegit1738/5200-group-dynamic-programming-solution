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
	// get total time of dependencies + self
	tasks_and_time = time_helper(tasks_and_time);

	if (std::get<1>(tasks_and_time) > deadline)
		return {{}, -1};
	
	return tasks_and_time;
}

std::tuple<int, int, std::vector<Task>> Task::maximize_dependency_reward(int time, int reward, std::vector<Task> task_list) {
	// determine intersection between task's own dependencies and tasks not already executed
	std::vector<Task> task_list_intersect = {};
	for (auto task : task_list) {
		if (std::find(dependencies.begin(), dependencies.end(), task) != dependencies.end())
			task_list_intersect.push_back(task);
	}
	if (task_list_intersect.size() > 1) {
		int total_time = time;
		std::vector<std::tuple<int, int, std::vector<Task>>> results = {};
		int i = 0;
		for (auto dep : task_list_intersect) {
			// calculate value of current dependency when put first
			std::vector<Task> task_list_copy = task_list;
			task_list_copy.erase(std::find(task_list_copy.begin(), task_list_copy.end(), dep));
			std::tuple<int, int, std::vector<Task>> result;
			bool doigetreward = std::get<1>(dep.is_option(time, task_list)) != -1;
			if (doigetreward) 
				result = dep.maximize_dependency_reward(time + dep.duration, reward + dep.taskreward, task_list_copy);
			else
				result = dep.maximize_dependency_reward(time + dep.duration, reward, task_list_copy);
			results.at(i) = result;
			// calculate value of other dependencies
			std::vector<Task> task_list_copy2 = task_list_intersect;
			task_list_copy.erase(std::find(task_list_copy.begin(), task_list_copy.end(), dep));
			std::tuple<int, int, std::vector<Task>> result2;
			std::vector<Task> *result_list = &std::get<2>(result);
			// create difference between dependency list and result list
			for (Task task : task_list_intersect) {
				if (std::find(result_list->begin(), result_list->end(), task) == result_list->end())
					task_list_copy2.erase(std::find(task_list_copy2.begin(), task_list_copy2.end(), task));
			}
			result2 = maximize_dependency_reward(std::get<0>(result), std::get<1>(result), task_list_copy2);
			// combine values
			std::get<0>(results.at(i)) += std::get<0>(result2);
			std::get<1>(results.at(i)) += std::get<1>(result2);
			// TODO: Figure out how to conduct list difference between results.at(i) and results2
			i++;
		}
		// return maximum reward
		std::sort(results.begin(), results.end(), [](const auto &a, const auto &b) { return std::get<1>(a) < std::get<1>(b); });
		int total_reward = std::get<1>(results.at(results.size() - 1));
		// time will be the same no matter the order
		total_time += std::get<0>(results.at(0));
		// create tuple of passed-in function args
		// all elements of results should contain the same list, as all solutions will execute all dependencies.
		std::vector<Task> omitted_chosen = task_list;
		std::vector<Task> *winning_list = &std::get<2>(results.at(results.size() - 1));
		for (auto task : task_list_intersect) {
			if ((std::find(task_list_intersect.begin(), task_list_intersect.end(), task) != task_list_intersect.end()) && (std::find(winning_list->begin(), winning_list->end(), task) == winning_list->end()))
				omitted_chosen.erase(std::find(omitted_chosen.begin(), omitted_chosen.end(), task));
		}
		std::tuple<int, int, std::vector<Task>> args = {time, reward, std::get<2>(results.at(0))};
		return {total_time, total_reward, std::get<2>(results.at(0))};

	} else if (dependencies.size() == 1) {
		Task dep = dependencies.at(0);
		if (std::find(task_list_intersect.begin(), task_list_intersect.end(), dep) != task_list_intersect.end()) {
			// remove singular dependency from task list
			std::vector<Task> task_list_copy = task_list;
			task_list_copy.erase(std::find(task_list_copy.begin(), task_list_copy.end(), dep));
			if (std::get<1>(dep.is_option(time, task_list)) != -1) {
				return {time + dep.duration, reward + dep.taskreward, task_list_copy};
			} else {
				return {time + dep.duration, reward, task_list_copy};
			}
		}
	}
	return {time, reward, task_list};
}

int dynamic_schedule(std::vector<Task> const & task_list, int time) {
	// TODO: incorporate memo table

	std::vector<int> rewards = {};
	std::vector<Task> task_list_copy = {};
	// iterate through tasks, determining which can be chosen
	for (int i = 0; i < task_list.size(); i++) {
		auto tasks_and_time = task_list.at(i).is_option(time, task_list);
		if (std::get<1>(tasks_and_time) == -1) {
			// do not remove ineligible task from the list, in case it is a dependency.
			rewards.at(i) = -1;
			continue;
		}
		// create copy of task list, excluding options
		// copying from task_list is necessary because is_option only returns a list of ids, not Task objects.
		for (std::string id : std::get<0>(tasks_and_time)) {
			for (int j = 0; j < task_list.size(); j++) {
				if (task_list.at(j).taskid == id)
					task_list_copy.push_back(task_list.at(j));
			}
		}
		rewards.at(i) = dynamic_schedule(task_list_copy, std::get<1>(tasks_and_time));
		// TODO: incorporate total rewards of dependencies (in optimal order) using helper function
		rewards.at(i) += task_list.at(i).reward;
		
	}
	// return greatest reward
	std::sort(rewards.begin(), rewards.end());
	return rewards.at(rewards.size() - 1);
}
