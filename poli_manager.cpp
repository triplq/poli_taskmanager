#include <iostream>
#include <ctime>
#include <chrono>
#include <stdexcept>

class Task {
private:
	std::string text;

public:
	explicit Task(const std::string& n_text) : text(n_text) { }

	const std::string& get_text() const {
		return text;
	}
};

class DeadlineTask : private Task {
private:
	std::tm deadline;

public:
	DeadlineTask(const std::string& n_text, const std::tm& n_deadline) : Task(n_text), deadline(n_deadline) { };

	const std::tm& get_deadline() const {
		return deadline;
	}
};

class ReccuringTask : private Task {
private:
	unsigned int days;
	std::tm last_complete_day;

public:
	ReccuringTask(const std::string& n_text, unsigned int n_days, const std::tm& n_last_complete_day) : Task(n_text) {
		if(n_days == 0){
			throw std::invalid_argument("Quantity of days can't be zero in reccuring task");
		}
		days = n_days;
		last_complete_day = n_last_complete_day;
	}

	unsigned int get_days() const {
		return days;
	}

	const std::tm& get_last_complete_day() const {
		return last_complete_day;
	}
};