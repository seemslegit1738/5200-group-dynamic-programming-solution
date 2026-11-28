#include "dp.h"
#include <algorithm>

Task::Task(std::string taskid, int duration, int deadline, int reward)
	: taskid(taskid), duration(duration), deadline(deadline), taskreward(reward) {}

bool Task::operator==(const Task & n) const { return taskid == n.taskid; }

void Task::add_dependency(Task* dependency) {
	dependencies.push_back(dependency);
}

std::tuple<std::vector<std::string>, int> Task::time_helper(std::tuple<std::vector<std::string>, int> tasks_and_time) const {
	// create pointers to task id list and current time value for better readability
	std::vector<std::string>& task_id_list = std::get<0>(tasks_and_time);
	int& current_time = std::get<1>(tasks_and_time);

	// if task not in task list (already accounted for), do not add its duration to total time
	if (std::find(task_id_list.begin(), task_id_list.end(), taskid) == task_id_list.end())
		return tasks_and_time;

	// recursively iterate through all dependencies of current task.
	for (auto& dependency : dependencies) {
		std::tuple<std::vector<std::string>, int> temp_tasks_and_time = dependency->time_helper(tasks_and_time);
		std::vector<std::string>& temp_id_list = std::get<0>(temp_tasks_and_time);
		int& temp_time = std::get<1>(temp_tasks_and_time);

		// sync current task list with temporary task list from recursive call
		int k = task_id_list.size();
		for (int i = 0; i < k; i++) {
			auto it = std::find(temp_id_list.begin(), temp_id_list.end(), task_id_list.at(i));
			if (it == temp_id_list.end()) {
				task_id_list.erase(task_id_list.begin() + i);
				i--;
				k--;
			}
		}

		current_time += temp_time;
	}
	// update task list and time for current task
	auto it = std::find(task_id_list.begin(), task_id_list.end(), taskid);
	// ensure that the current task id exists in the list: if not, skip without adding its duration.
	if (it != task_id_list.end())
		task_id_list.erase(it);
	else
		return tasks_and_time;
	current_time += duration;

	return tasks_and_time;
}

std::tuple<std::vector<std::string>, int> Task::is_option(int time, std::vector<Task> const task_list) const {
	// initialize recursive state
	std::tuple<std::vector<std::string>, int> tasks_and_time = {std::vector<std::string>{}, time};
	for (int i = 0; i < static_cast<int>(task_list.size()); i++) {
		std::get<0>(tasks_and_time).push_back(task_list.at(i).taskid);
	}
	// get total time of dependencies + self
	tasks_and_time = time_helper(tasks_and_time);

	if (std::get<1>(tasks_and_time) > deadline)
		return {std::vector<std::string>{}, -1};
	
	return tasks_and_time;
}

/*
	[0]: Time
	[1]: Reward
	[2]: Task List
*/
std::tuple<int, int, std::vector<Task>> Task::maximize_dependency_reward(int time, int reward, std::vector<Task> task_list) const {
	// determine intersection between task's own dependencies and tasks not already executed
	std::vector<Task> task_list_intersect = {};
	for (auto& task : task_list) {
		if (std::find_if(dependencies.begin(), dependencies.end(), [&task] (Task * dep) { return task == *dep; }) != dependencies.end())
			task_list_intersect.push_back(task);
	}
	if (!(task_list_intersect.empty())) {
		int total_time = time;
		std::vector<std::tuple<int, int, std::vector<Task>>> results = {};
		int i = 0;
		for (auto& dep : task_list_intersect) {
			// calculate value of current dependency when put first
			std::vector<Task> task_list_copy = task_list;
			task_list_copy.erase(std::find(task_list_copy.begin(), task_list_copy.end(), dep));
			std::tuple<int, int, std::vector<Task>> result;
			bool doigetreward = std::get<1>(dep.is_option(time, task_list)) != -1;
			if (doigetreward)
				result = dep.maximize_dependency_reward(time + dep.duration, reward + dep.taskreward, task_list_copy);
			else
				result = dep.maximize_dependency_reward(time + dep.duration, reward, task_list_copy);
			results.push_back(result);
			// calculate value of other dependencies
			std::vector<Task> task_list_copy2 = task_list_intersect;
			task_list_copy2.erase(std::find(task_list_copy2.begin(), task_list_copy2.end(), dep));
			std::tuple<int, int, std::vector<Task>> result2;
			std::vector<Task> &result_list = std::get<2>(result);
			// create intersection between dependency list and resulting list (to avoid processing the same task twice)
			for (Task task : task_list_intersect) {
				if (std::find(result_list.begin(), result_list.end(), task) == result_list.end()) {
					auto it = std::find(task_list_copy2.begin(), task_list_copy2.end(), task);
					if (it != task_list_copy2.end())
						task_list_copy2.erase(it);
				}
			}
			result2 = maximize_dependency_reward(std::get<0>(result), 0, task_list_copy2);
			// combine values
			// total time will be equivalent to return value of result2, since previous total time was passed in as an argument
			std::get<0>(results.at(i)) = std::get<0>(result2);
			std::get<1>(results.at(i)) += std::get<1>(result2);
			std::vector<Task> result2_choices = {};
			std::vector<Task> &result2_list = std::get<2>(result2);
			for (Task task : task_list_copy2) {
				if (std::find(result2_list.begin(), result2_list.end(), task) == result2_list.end())
					result2_choices.push_back(task);
			}
			int k = result_list.size();
			for (int j = 0; j < k; j++) {
				if (std::find(result2_choices.begin(), result2_choices.end(), result_list.at(j)) != result2_choices.end()) {
					result_list.erase(result_list.begin() + j);
					j--;
					k--;
				}
			}
			std::get<2>(results.at(i)) = result_list;
			i++;
		}
		// return maximum reward
		std::sort(results.begin(), results.end(), [](const auto &a, const auto &b) { return std::get<1>(a) < std::get<1>(b); });
		int total_reward = std::get<1>(results.at(results.size() - 1));
		// time will be the same no matter the order
		total_time += std::get<0>(results.at(0));
		// create tuple of passed-in function args
		// all elements of results should contain the same list, as all solutions will execute all dependencies.
		return {total_time, total_reward, std::get<2>(results.at(0))};
	}
	return {time, reward, task_list};
}

int dynamic_schedule(std::vector<Task> const & task_list, int time) {
	// TODO: incorporate memo table

	std::vector<int> rewards = {};
	// iterate through tasks, determining which can be chosen
	if (task_list.empty()) {
		return 0;
	}
	for (int i = 0; i < static_cast<int>(task_list.size()); i++) {
		auto tasks_and_time = task_list.at(i).is_option(time, task_list);
		if (std::get<1>(tasks_and_time) == -1) {
			// do not remove ineligible task from the list, in case it is a dependency.
			rewards.push_back(0);
			continue;
		}
		// create copy of task list, excluding options
		std::vector<Task> task_list_copy = {};
		// copying from task_list is necessary because is_option only returns a list of ids, not Task objects.
		// TODO: Ensure that list of ids removes current task (task_list.at(i)) as well as all of its dependencies.
		for (std::string id : std::get<0>(tasks_and_time)) {
			for (int j = 0; j < static_cast<int>(task_list.size()); j++) {
				if (task_list.at(j).taskid == id)
					task_list_copy.push_back(task_list.at(j));
			}
		}
		// removes self from copy of task list passed into maximize_dependency_reward()
		std::vector<Task> task_list_copy_for_deps = task_list;
		task_list_copy_for_deps.erase(task_list_copy_for_deps.begin() + i);
		rewards.push_back(dynamic_schedule(task_list_copy, std::get<1>(tasks_and_time)));
		auto dep_results = task_list.at(i).maximize_dependency_reward(time, 0, task_list_copy_for_deps);
		rewards.at(i) += task_list.at(i).taskreward + std::get<1>(dep_results);
		
	}
	// return greatest reward
	std::sort(rewards.begin(), rewards.end());
	return rewards.at(rewards.size() - 1);
}
