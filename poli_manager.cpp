#include <cstddef>
#include <iostream>
#include <ctime>
#include <stdexcept>
#include <string>

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

	virtual Task* clone() const = 0;

	virtual bool is_expired() const = 0;

	virtual ~Task() = default;
};

class SimpleTask : public Task{
public:
	SimpleTask() { }
	SimpleTask(const std::string& n_text) : Task(n_text) { }

	std::string to_string() const override {
		return "Task: " + text;
	}

	SimpleTask* clone() const override {
		return new SimpleTask(*this);
	}

	bool is_expired() const override {
		return false;
	}
};

class DeadlineTask : public Task {
private:
	std::time_t deadline;

public:
	DeadlineTask() { }
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

	DeadlineTask* clone() const override {
		return new DeadlineTask();
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
	ReccuringTask() { }
	ReccuringTask(const std::string& n_text, time_t n_recure, time_t n_last_complete_day) : Task(n_text) {
		if(n_recure < 86400){
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

		std::time_t local_recure = recure;
		std::tm tm_recure = *std::localtime(&local_recure);

		char buffer_lcd[32];
		std::strftime(buffer_lcd, sizeof(buffer_lcd), "%d.%m.%Y %H:%M", &tm_lcd);

		char buffer_recure[32];
		std::strftime(buffer_recure, sizeof(buffer_recure), "%d.%m.%Y %H:%M", &tm_recure);

		return "Task: " + text + " Reccuring every " + std::string(buffer_recure) + " And were last complete: " + std::string(buffer_lcd);
	}

	ReccuringTask* clone() const override {
		return new ReccuringTask();
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
	Task** tasks;

	void increase_capacity() {
		Task** new_tasks = new Task*[capacity*2];
		for (size_t i = 0; i < size; i++){
			new_tasks[i] = tasks[i];
		}
		capacity *= 2;
		delete[] tasks;
		tasks = new_tasks;
	}

public:
	TaskManager() : capacity(1), size(0), tasks(new Task*[capacity]) { };
	TaskManager(size_t n_capacity) {
		if (n_capacity == 0) {
			throw std::invalid_argument("Capacity can't be zero");
		}
		capacity = n_capacity;
		size = 0;
		tasks = new Task*[capacity];
	}

	TaskManager& operator=(const TaskManager& other) {
		if (this != &other) {
			Task** n_tasks = new Task*[other.capacity];
			for (size_t i = 0; i < other.size; i++) {
				n_tasks[i] = other.tasks[i];
			}
			capacity = other.capacity;
			size = other.size;

			delete[] tasks;
			tasks = n_tasks;
			n_tasks = nullptr;
		}

		return *this;
	}

	TaskManager(const TaskManager& other) : capacity(other.capacity), size(other.size) {
		tasks = new Task*[capacity];
		for (size_t i = 0; i < size; i++) {
			tasks[i] = other.tasks[i]->clone();
		}
	}

	~TaskManager() {
		for (size_t i = 0; i < size; i++) {
			delete tasks[i];
		}

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

	void push_back(Task* n_task) {
		if (size == capacity){
			increase_capacity();
		}
		tasks[size] = n_task;
		n_task = nullptr;
		size++;
	}

	void println() {
		for (size_t i = 0; i < size; i++) {
			std::cout << tasks[i]->to_string() << ' ';
		}
		std::cout << '\n';
	}

	void whats_expired() {
		for (size_t i = 0; i < size; i++) {
			if(tasks[i]->is_expired()){
				std::cout << tasks[i]->to_string() << ' ';
			}
		}
		std::cout << '\n';
	}
};

int main() {
	SimpleTask task("Work");

	std::time_t deadline = std::time(nullptr) + (86400 * 5); //5 days
	DeadlineTask task_with_deadline("Call",deadline);

	std::time_t recure = 86400 * 7; //week
	std::time_t last_complete_day = std::time(nullptr) - (86400 * 9);
	ReccuringTask task_with_recure("Drink", recure, last_complete_day);

	std::cout << "============TESTING SIMPLETASK============\n";
	std::cout << task.get_text() << ' ' << task.to_string() << '\n';

	SimpleTask task2 = task;

	std::cout << task2.get_text() << ' ' << task2.to_string() << '\n';

	std::cout << "============TESTING DEADLINE============\n";

	std::time_t local_deadline = task_with_deadline.get_deadline();
	std::tm tm_deadline = *std::localtime(&local_deadline);
	std::cout << std::asctime(&tm_deadline) << " Is it expired? " << task_with_deadline.is_expired() << ' ' << task_with_deadline.to_string() << '\n';

	std::cout << "===============TESTING RECURE============\n";

	time_t local_recure = task_with_recure.get_recure();
	std::tm tm_recure = *std::localtime(&local_recure);
	time_t local_lcd = task_with_recure.get_last_complete_day();
	std::tm tm_lcd = *std::localtime(&local_lcd);

	std::cout << asctime(&tm_recure) << task_with_recure.to_string() << " Is it expired? " << task_with_recure.is_expired() << ' ' << asctime(&tm_lcd);


	return 0;
}