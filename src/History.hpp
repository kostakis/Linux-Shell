#pragma once

#include "HelperFunctions.hpp"
#include "Alias.hpp"

#include <vector>
#include <string>
#include <cstring>
#include <iostream>
#include <array>
#include <memory>

class History {
public:
	History() = default;
	~History() = default;

	void setAllias(const std::shared_ptr<Alias>& alias);

	void printHistory() const;
	void addToHistory(const std::string& line);
	void handleHistory(std::vector<char*>& lineTokens);

private:
	std::array< std::string, 100> history;
	std::shared_ptr<Alias> m_alias;
	int maxSize = 20;
	bool startedAgain = false;
	int currentIndex = 0;
};
