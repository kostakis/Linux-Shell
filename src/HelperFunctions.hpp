#pragma once

#include <string>
#include <vector>

void makeTokens(std::vector<char *> &lineTokens, std::string &line);
void printTokens(const std::vector<char *> &lineTokens);

void parseEnviromentalVariables(std::string &line);

std::vector<std::string> expandWildcards(const std::string &token);

void execute(std::vector<char *> &tokens);