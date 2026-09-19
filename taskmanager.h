#pragma once

#include "task.h"
#include <memory>

class TaskManager {
private:
	size_t capacity;
	size_t size;
	std::unique_ptr<Task>* tasks;

	void increase_capacity();

public:
	TaskManager();
	TaskManager(size_t n_capacity);
	TaskManager(TaskManager&& moved) noexcept;
	TaskManager& operator=(TaskManager&& moved) noexcept;
	TaskManager& operator=(const TaskManager& other);
	TaskManager(const TaskManager& other);
	~TaskManager();

	const Task& operator[](size_t index) const;
	Task& operator[](size_t index);

	void push_back(const Task& n_task);

	void println() const;
	void whats_expired() const;
	size_t get_size() const;
	size_t get_capacity() const;
};