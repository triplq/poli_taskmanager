#include <iostream>
#include <ctime>
#include <chrono>
#include <locale>
#include <stdexcept>
#include <string>

class Task {
private:
	std::string text;

public:
	explicit Task(const std::string& n_text) : text(n_text) { }

	Task(const Task& other) : text(other.text) { }

	const std::string& get_text() const {
		return text;
	}

	std::string to_string() const {
		return "Task: " + text;
	}
};

class DeadlineTask : private Task {
private:
	std::time_t deadline;

public:
	DeadlineTask(const std::string& n_text, const std::time_t& n_deadline) : Task(n_text), deadline(n_deadline) { };

	DeadlineTask(const DeadlineTask& other) : Task(other.get_text()), deadline(other.deadline) { }

	const std::time_t& get_deadline() const {
		return deadline;
	}

	std::string to_string() const {
		std::time_t local_deadline = deadline;
		std::tm tm_deadline = *std::localtime(&local_deadline);

		char buffer[32];
		std::strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M", &tm_deadline);

		return Task::to_string() + " Expires at: " + std::string(buffer);
	}

	bool is_expired() const {
		return (std::time(nullptr) >= deadline);
	}
};

class ReccuringTask : private Task {
private:
	std::time_t recure;
	std::time_t last_complete_day;

public:
	ReccuringTask(const std::string& n_text, const time_t& n_recure, const std::time_t& n_last_complete_day) : Task(n_text) {
		if(n_recure < 86400){
			throw std::invalid_argument("Quantity of days can't be zero in reccuring task");
		}
		recure = n_recure;
		last_complete_day = n_last_complete_day;
	}

	ReccuringTask(const ReccuringTask& other) : Task(other.get_text()), recure(other.recure), last_complete_day(other.last_complete_day) { }

	time_t get_recure() const {
		return recure;
	}

	const std::time_t& get_last_complete_day() const {
		return last_complete_day;
	}

	std::string to_string() const {
		std::time_t local_lcd = last_complete_day;
		std::tm tm_lcd = *std::localtime(&local_lcd);

		std::time_t local_recure = recure;
		std::tm tm_recure = *std::localtime(&local_recure);

		char buffer_lcd[32];
		std::strftime(buffer_lcd, sizeof(buffer_lcd), "%d.%m.%Y %H:%M", &tm_lcd);

		char buffer_recure[32];
		std::strftime(buffer_recure, sizeof(buffer_recure), "%d.%m.%Y %H:%M", &tm_recure);

		return Task::to_string() + " Reccuring every " + std::string(buffer_recure) + " And were last complete: " + std::string(buffer_lcd);
	}

	bool is_expired() const {
		std::time_t now = std::time(nullptr);
		return (now > last_complete_day + recure)
	}
};

int main() {
	Task task("Work");

	std::time_t deadline = std::time(nullptr) + (86400 * 5);
	DeadlineTask task_with_deadline("Call",deadline);



	return 0;
}