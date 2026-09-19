#pragma once

#include "task.h"
#include <ctime>


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