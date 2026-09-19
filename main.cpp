#include "simpletask.h"
#include "deadline.h"
#include "recurring.h"
#include "taskmanager.h"

int main() {
	SimpleTask task("Work");

	std::time_t deadline = std::time(nullptr) + (86400 * 5); //5 days
	DeadlineTask task_with_deadline("Call",deadline);

	std::time_t recure = 86400 * 7; //week
	std::time_t last_complete_day = std::time(nullptr) - (86400 * 9);
	ReccuringTask task_with_recure("Drink", recure, last_complete_day);

	// std::cout << "============TESTING SIMPLETASK============\n";
	// std::cout << task.get_text() << ' ' << task.to_string() << '\n';

	// std::cout << "============TESTING DEADLINE============\n";

	// std::time_t local_deadline = task_with_deadline.get_deadline();
	// std::tm tm_deadline = *std::localtime(&local_deadline);
	// std::cout << std::asctime(&tm_deadline) << " Is it expired? " << task_with_deadline.is_expired() << ' ' << task_with_deadline.to_string() << '\n';

	// std::cout << "===============TESTING RECURE============\n";

	// time_t local_recure = task_with_recure.get_recure();
	// std::tm tm_recure = *std::localtime(&local_recure);
	// time_t local_lcd = task_with_recure.get_last_complete_day();
	// std::tm tm_lcd = *std::localtime(&local_lcd);

	// std::cout << asctime(&tm_recure) << task_with_recure.to_string() << " Is it expired? " << task_with_recure.is_expired() << ' ' << asctime(&tm_lcd);

	SimpleTask task2("Quit");
	
	TaskManager tasks;
	tasks.push_back(task);
	tasks.push_back(task_with_deadline);
	tasks.push_back(task_with_recure);

	TaskManager tasks2(tasks);
	tasks2.push_back(task2);

	tasks2.push_back(SimpleTask("hello"));

	tasks.println();
	tasks2.println();

	tasks2 = tasks;

	tasks.println();
	tasks2.println();


	// int* p = new int(10);
	// std::cout << p[15];

	// int* p = new int(10);
	// int* a = p;

	// delete p;

	// std::cout << *p << ' ' << *a << '\n';
	

	// SimpleTask* task = new SimpleTask("Work");
	// Task* arr = new Task[3];
	// arr[0] = *task;
	// std::cout << arr[0].to_string() << '\n';

	return 0;
}