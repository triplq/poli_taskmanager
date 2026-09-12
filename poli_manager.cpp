#include <iostream>
#include <ctime>
#include <chrono>
#include <locale>
#include <stdexcept>
#include <string>

class Task {
protected:
	std::string text;

public:
	explicit Task(const std::string& n_text) : text(n_text) { }

	virtual std::string to_string() const = 0;

	const std::string& get_text() const {
		return text;
	}

	virtual ~Task() = default;
};

class SimpleTask : public Task{
public:
	// using Task::Task;
	SimpleTask(const std::string& n_text) : Task(n_text) { }

	std::string to_string() const override {
		return "Task: " + text;
	}
};

class DeadlineTask : public Task {
private:
	std::time_t deadline;

public:
	DeadlineTask(const std::string& n_text, time_t n_deadline) : Task(n_text), deadline(n_deadline) { };

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

	bool is_expired() const {
		return (std::time(nullptr) >= deadline);
	}
};

class ReccuringTask : public Task {
private:
	std::time_t recure;
	std::time_t last_complete_day;

public:
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

	bool is_expired() const {
		std::time_t now = std::time(nullptr);

		return (now >= last_complete_day + recure);
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