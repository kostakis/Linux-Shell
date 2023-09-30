#pragma once

#include <vector>

// Too simple to be class
void handleOutputRedirection(std::vector<char *> &lineTokens);
void handleInputRedirection(std::vector<char *> &lineTokens);
