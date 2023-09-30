#pragma once

#ifdef __linux__
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

// To simple for now to be a class
int hasPipe(const std::vector<char *> &tokens);
bool handlePipe(std::vector<char *> &tokens);