//============================================================================
// Name        : Yellow_Week1_Case4.cpp
// Author      : grec88
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <tuple>
#include <map>
#include <string>
using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
	NEW,          // новая
	IN_PROGRESS,  // в разработке
	TESTING,      // на тестировании
	DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

void clean_up(TasksInfo &a) {
	for (auto& [key, value] : a) {
		if (value == 0) {
			a.erase(key);
		}
	}
}

class TeamTasks {
public:
	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const string &person) const {
		return tasks_base.at(person);
	}

	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const string &person) {
		++tasks_base[person][TaskStatus::NEW];
	}

	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string &person,
			int task_count) {
		TasksInfo new_one;
		TasksInfo old_one;
		int q = tasks_base[person][TaskStatus::NEW] - task_count;
		if (q <= 0) {
			int a1 = tasks_base[person][TaskStatus::IN_PROGRESS] + q;
			tasks_base[person][TaskStatus::IN_PROGRESS] +=
					tasks_base[person][TaskStatus::NEW];
			tasks_base[person][TaskStatus::NEW] = 0;
			new_one[TaskStatus::IN_PROGRESS] =
					tasks_base[person][TaskStatus::NEW];
			if (a1 <= 0) {
				int a2 = tasks_base[person][TaskStatus::TESTING] + a1;
				tasks_base[person][TaskStatus::TESTING] += a1 - q;
				tasks_base[person][TaskStatus::IN_PROGRESS] -= a1 - q;
				old_one[TaskStatus::TESTING] =
						tasks_base[person][TaskStatus::TESTING];
				new_one[TaskStatus::TESTING] = q;
				if (a2 <= 0) {
					tasks_base[person][TaskStatus::DONE] += a2 - a1;
					tasks_base[person][TaskStatus::TESTING] += a2 - a1;
				} else {
					old_one[TaskStatus::IN_PROGRESS] = a1 - q;
					old_one[TaskStatus::TESTING] =
							tasks_base[person][TaskStatus::TESTING];
					new_one[TaskStatus::TESTING] = q;
					tasks_base[person][TaskStatus::TESTING] += q;
					tasks_base[person][TaskStatus::NEW] = a1;
				}
			} else {
				old_one[TaskStatus::IN_PROGRESS] = a1 - q;
				old_one[TaskStatus::TESTING] =
						tasks_base[person][TaskStatus::TESTING];
				new_one[TaskStatus::TESTING] = q;
				tasks_base[person][TaskStatus::TESTING] -= q;
				tasks_base[person][TaskStatus::IN_PROGRESS] += q;
			}
		} else {
			new_one[TaskStatus::IN_PROGRESS] = task_count;
			old_one[TaskStatus::NEW] = q;
			old_one[TaskStatus::IN_PROGRESS] =
					tasks_base[person][TaskStatus::IN_PROGRESS];
			old_one[TaskStatus::TESTING] =
					tasks_base[person][TaskStatus::TESTING];
			tasks_base[person][TaskStatus::IN_PROGRESS] += task_count;
			tasks_base[person][TaskStatus::NEW] = q;
		}
		clean_up(new_one);
		clean_up(old_one);
		return tie(new_one, old_one);
	}
private:
	map<string, TasksInfo> tasks_base;
};

int main() {
	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
	return 0;
}
