#include "Prompt.hpp"

void Prompt::show() const
{
    char currentPath[PATH_MAX];
    if (getcwd(currentPath, PATH_MAX) != NULL) {
        std::cout << "in-mysh-now@" << currentPath << " >> ";
    }
    else {
        std::cout << "getcwd error exiting shell\n";
        std::cout << "Error string: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::string Prompt::readLine() 
{
    m_currentLine.clear();
    std::getline(std::cin, m_currentLine);
    return m_currentLine;
}