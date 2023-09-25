#pragma once

#include <vector>
#include <string>

void showPromt();

bool handleCd(const std::vector<char*>& tokens);

void makeTokens(std::vector<char*>& lineTokens, std::string& line);
void printTokens(const std::vector<char*>& lineTokens);

std::vector<std::string> splitLine(const std::string& line, char delim);
void parseEnviromentalVariables(std::string& line);

std::vector<std::string> expandWildcards(const std::string& token);

void execute(std::vector<char*>& tokens);