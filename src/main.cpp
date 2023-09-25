#include "HelperFunctions.hpp"
#include "Redirection.hpp"
#include "Alias.hpp"
#include "History.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <memory>

int main(int argc, char** argv) {
	// Clear the currrent terminal
	system("clear");

	auto history = std::make_shared<History>();
	auto alias = std::make_shared<Alias>();

	history->setAllias(alias);

	while (true) {
		showPromt();

		std::string line;
		std::getline(std::cin, line);

		history->addToHistory(line);

		// Simple useful stuff
		if (line == "exit") break;
		if (line == "clear") system("clear");
		if (line == "history") {
			history->printHistory();
			continue;
		}

		auto linesSeperatedBySemiColon = splitLine(line, ';');
		for (auto& line : linesSeperatedBySemiColon) {
			// Replace any ${..} with their actual value
			parseEnviromentalVariables(line);

			// Split the line into tokens
			std::vector<char*> lineTokens;

			makeTokens(lineTokens, line);
			if (lineTokens.size() == 1) continue; // It is just the nullptr

			if (alias->isAlias(lineTokens[0]) && lineTokens.size() == 2) { // Alias can only be the first token
				auto& al = alias->getAliasMap();
				line = al.at(lineTokens[0]); // Can not throw
				lineTokens.clear();
				makeTokens(lineTokens, line); // Remake the tokens with the actual value
			}

			if (alias->handle(lineTokens)) continue; // Create/Destroy allias

			// History + number command
			history->handleHistory(lineTokens);

			// Cd command
			if (handleCd(lineTokens)) continue;

			execute(lineTokens);
		}
	}
	return 0;
}
