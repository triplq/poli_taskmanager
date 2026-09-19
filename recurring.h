#pragma once

#include "task.h"
#include <ctime>

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