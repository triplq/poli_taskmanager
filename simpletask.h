#pragma once

#include "task.h"

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