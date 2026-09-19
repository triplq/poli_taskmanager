#pragma once

#include <string>

class Task {
protected:
	std::string text;

public:
	explicit Task(const std::string& n_text) : text(n_text) { };

	virtual std::string to_string() const = 0;

	const std::string& get_text() const {
		return text;
	}

	virtual std::unique_ptr<Task> clone() const = 0;

	virtual bool is_expired() const = 0;

	virtual ~Task() = default;
};