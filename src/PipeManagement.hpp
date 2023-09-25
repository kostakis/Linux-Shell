#pragma once

#ifdef __linux__ 
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

#include <cstring>
#include <vector>
#include <string>
#include <iostream>

// To simple for now to be a class
int hasPipe(const std::vector<char*>& tokens);
bool handlePipe(std::vector<char*>& tokens);