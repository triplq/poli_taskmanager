#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <stdexcept>
#include <string>
#include <memory>

class Task {
protected:
	std::string text;

public:
	Task() { }
	explicit Task(const std::string& n_text) : text(n_text) { }

	virtual std::string to_string() const = 0;

	const std::string& get_text() const {
		return text;
	}

	virtual std::unique_ptr<Task> clone() const = 0;

	virtual bool is_expired() const = 0;

	virtual ~Task() = default;
};

class SimpleTask : public Task{
public:
	SimpleTask(const std::string& n_text) : Task(n_text) { }

	std::string to_string() const override {
		return "Task: " + text;
	}

	std::unique_ptr<Task> clone() const override {
		return std::make_unique<SimpleTask>(*this);
	}

	bool is_expired() const override {
		return false;
	}
};

class DeadlineTask : public Task {
private:
	std::time_t deadline;

public:
	DeadlineTask(const std::string& n_text, time_t n_deadline) : Task(n_text), deadline(n_deadline) { }

	time_t get_deadline() const {
		return deadline;
	}

	std::string to_string() const override {
		std::time_t local_deadline = deadline;
		std::tm tm_deadline = *std::localtime(&local_deadline);

		char buffer[32];
		std::strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M", &tm_deadline);

		return "Task: " + text + " Expires at: " + std::string(buffer);
	}

	std::unique_ptr<Task> clone() const override {
		return std::make_unique<DeadlineTask>(*this);
	}

	bool is_expired() const override {
		return (std::time(nullptr) >= deadline);
	}
};

class ReccuringTask : public Task {
private:
	std::time_t recure;
	std::time_t last_complete_day;

public:
	ReccuringTask(const std::string& n_text, time_t n_recure, time_t n_last_complete_day) : Task(n_text) {
		if (n_recure < 86400) {
			throw std::invalid_argument("Quantity of days can't be zero in reccuring task");
		}
		recure = n_recure;
		last_complete_day = n_last_complete_day;
	}

	time_t get_recure() const {
		return recure;
	}

	time_t get_last_complete_day() const {
		return last_complete_day;
	}

	std::string to_string() const override {
		std::time_t local_lcd = last_complete_day;
		std::tm tm_lcd = *std::localtime(&local_lcd);

		char buffer_lcd[32];
		std::strftime(buffer_lcd, sizeof(buffer_lcd), "%d.%m.%Y %H:%M", &tm_lcd);

		return "Task: " + text + " Reccuring every " + std::to_string(recure / 86400) + " days And were last complete: " + std::string(buffer_lcd);
	}

	std::unique_ptr<Task> clone() const override {
		return std::make_unique<ReccuringTask>(*this);
	}

	bool is_expired() const override {
		std::time_t now = std::time(nullptr);

		return (now >= last_complete_day + recure);
	}
};

class TaskManager {
private:
	size_t capacity;
	size_t size;
	std::unique_ptr<Task>* tasks;

	void increase_capacity() {
		std::unique_ptr<Task>* n_tasks = new std::unique_ptr<Task>[capacity*2];
		for (size_t i = 0; i < size; i++) {
			n_tasks[i] = std::move(tasks[i]);
		}
		capacity *= 2;
		delete[] tasks;
		tasks = n_tasks;
	}

public:
	TaskManager() : capacity(1), size(0), tasks(new std::unique_ptr<Task>[capacity]) { }
	TaskManager(size_t n_capacity) {
		if (n_capacity == 0) {
			throw std::invalid_argument("Capacity can't be zero");
		}
		capacity = n_capacity;
		size = 0;
		tasks = new std::unique_ptr<Task>[capacity];
	}

	TaskManager& operator=(const TaskManager& other) {
		if (this != &other) {
			std::unique_ptr<Task>* n_tasks = new std::unique_ptr<Task>[other.capacity];
			
			try {
				for (size_t i = 0; i < other.size; i++) {
					n_tasks[i] = other.tasks[i]->clone();
				}
			}
			catch (...) {
				delete[] n_tasks;
				throw;
			}
						
			size = other.size;
			capacity = other.capacity;
			delete[] tasks;
			tasks = n_tasks;
		}

		return *this;
	}

	TaskManager(const TaskManager& other) : capacity(other.capacity), size(other.size) {
		tasks = new std::unique_ptr<Task>[capacity];
		
		for (size_t i = 0; i < size; i++) {
			tasks[i] = other.tasks[i]->clone();
		}
	}

	~TaskManager() {
		delete[] tasks;
	}

	const Task& operator[](size_t index) const {
		if (index >= size) {
			throw std::invalid_argument("Index can't be more or equal size");
		}

		return *tasks[index];
	}

	Task& operator[](size_t index) {
		if (index >= size) {
			throw std::invalid_argument("Index can't be more or equal size");
		}

		return *tasks[index];
	}

	void push_back(const Task& n_task) {
		if (size == capacity) {
			increase_capacity();
		}
		tasks[size] = n_task.clone();
		size++;
	}

	void println() const {
		for (size_t i = 0; i < size; i++) {
			std::cout << i+1 << ')' << tasks[i]->to_string() << '\n';
		}
		std::cout << '\n';
	}

	void whats_expired() const {
		for (size_t i = 0; i < size; i++) {
			if (tasks[i]->is_expired()) {
				std::cout << tasks[i]->to_string() << ' ';
			}
		}
		std::cout << '\n';
	}

	size_t get_size() const {
		return size;
	}

	size_t get_capacity() const {
		return capacity;
	}
};

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