#include "taskmanager.h"
#include <iostream>

void TaskManager::increase_capacity() {
	std::unique_ptr<Task>* n_tasks = new std::unique_ptr<Task>[(capacity+1)*2];
	for (size_t i = 0; i < size; i++) {
		n_tasks[i] = std::move(tasks[i]);
	}
	capacity *= 2;
	delete[] tasks;
	tasks = n_tasks;
}

TaskManager::TaskManager() : capacity(1), size(0), tasks(new std::unique_ptr<Task>[capacity]) { }
TaskManager::TaskManager(size_t n_capacity) {
	if (n_capacity == 0) {
		throw std::invalid_argument("Capacity can't be zero");
	}
	capacity = n_capacity;
	size = 0;
	tasks = new std::unique_ptr<Task>[capacity];
}

TaskManager::TaskManager(TaskManager&& moved) noexcept : capacity(moved.capacity), size(moved.size), tasks(moved.tasks) {
	moved.tasks = nullptr;
	moved.capacity = 0;
	moved.size = 0;
}

TaskManager& TaskManager::operator=(TaskManager&& moved) noexcept {
	if (this != &moved) {
		delete[] tasks;

		tasks = moved.tasks;
		capacity = moved.capacity;
		size = moved.size;

		moved.tasks = nullptr;
		moved.capacity = 0;
		moved.size = 0;
	}

	return *this;
}

TaskManager& TaskManager::operator=(const TaskManager& other) {
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

TaskManager::TaskManager(const TaskManager& other) : capacity(other.capacity), size(other.size) {
	tasks = new std::unique_ptr<Task>[capacity];
	
	try {
		for (size_t i = 0; i < other.size; i++) {
			tasks[i] = other.tasks[i]->clone();
		}
	}
	catch (...) {
		delete[] tasks;
		tasks = nullptr;
		throw;
	}

}

TaskManager::~TaskManager() {
	delete[] tasks;
}

const Task& TaskManager::operator[](size_t index) const {
	if (index >= size) {
		throw std::invalid_argument("Index can't be more or equal size");
	}

	return *tasks[index];
}

Task& TaskManager::operator[](size_t index) {
	if (index >= size) {
		throw std::invalid_argument("Index can't be more or equal size");
	}

	return *tasks[index];
}

void TaskManager::push_back(const Task& n_task) {
	if (size == capacity) {
		increase_capacity();
	}
	tasks[size] = n_task.clone();
	size++;
}

void TaskManager::println() const {
	for (size_t i = 0; i < size; i++) {
		std::cout << i+1 << ')' << tasks[i]->to_string() << '\n';
	}
	std::cout << '\n';
}

void TaskManager::whats_expired() const {
	for (size_t i = 0; i < size; i++) {
		if (tasks[i]->is_expired()) {
			std::cout << tasks[i]->to_string() << ' ';
		}
	}
	std::cout << '\n';
}

size_t TaskManager::get_size() const {
	return size;
}

size_t TaskManager::get_capacity() const {
	return capacity;
}
