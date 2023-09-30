#pragma once

#include <limits.h> // PATH_MAX
#include <unistd.h> // getcwd
#include <iostream>
#include <string.h>
#include <string>

class Prompt
{
public:
    Prompt() = default;
    ~Prompt() = default;

    void show() const;
    std::string readLine();

private:
    std::string m_currentLine;
};