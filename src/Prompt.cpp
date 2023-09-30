#include "Prompt.hpp"

#include <sstream>
#include <sys/stat.h> // struct stat sb

Prompt::Prompt() : m_history(std::make_shared<History>()), m_alias(std::make_shared<Alias>()) {
  m_dispatchTable = {{"exit",
                      [&](std::vector<char *> &) {
                        exit(1);
                        return true;
                      }},

                     {"clear",
                      [&](std::vector<char *> &) {
                        system("clear");
                        return true;
                      }},

                     {"history",
                      [&](std::vector<char *> &tokens) {
                        if (tokens[1] == nullptr) {
                          m_history->printHistory();
                          return true;
                        }
                        return false;
                      }},

                     {"cd",
                      [&](std::vector<char *> &tokens) {
                        if (tokens[1] != nullptr) {
                          struct stat sb;
                          if (stat(tokens[1], &sb) == 0) {
                            if (chdir(tokens[1]) == -1) { // Failed to change dir
                              return false;
                            } else {
                              return true;
                            }
                          }
                        }

                        return false;
                      }},

                     {"createalias", [&](std::vector<char *> &tokens) { return m_alias->create(tokens); }},

                     {"destoryalias", [&](std::vector<char *> &tokens) { return m_alias->destroy(tokens); }}};

  m_history->setAllias(m_alias);
}

int Prompt::main() {
  // Clear the currrent terminal
  system("clear");

  while (true) {
    show();
    readLine();
    auto lines = splitLine(';');

    for (auto &line : lines) {
      parseEnviromentalVariables(line);

      // Split the line into tokens
      std::vector<char *> lineTokens;
      makeTokens(lineTokens, line);
      if (lineTokens.size() == 1)
        continue; // It is just the nullptr

      // Check if the command is an allias
      if (m_alias->isAlias(lineTokens[0]) && lineTokens.size() == 2) { // Alias can only be the first token
        auto &al = m_alias->getAliasMap();
        line = al.at(lineTokens[0]); // Can not throw
        lineTokens.clear();
        makeTokens(lineTokens, line); // Remake the tokens with the actual value
      }

      try {
        if (m_dispatchTable[lineTokens[0]](lineTokens))
          continue;
      } catch (const std::exception &e) {
        // std::cerr << e.what() << '\n';
      }

      m_history->handleHistory(lineTokens); // history + number
      execute(lineTokens);
    }
  }

  return 0;
}

void Prompt::show() const {
  char currentPath[PATH_MAX];
  if (getcwd(currentPath, PATH_MAX) != NULL) {
    std::cout << "in-mysh-now@" << currentPath << " >> ";
  } else {
    std::cout << "getcwd error exiting shell\n";
    std::cout << "Error string: " << strerror(errno) << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Prompt::readLine() {
  m_currentLine.clear();
  std::getline(std::cin, m_currentLine);
  m_history->addToHistory(m_currentLine);
}

std::vector<std::string> Prompt::splitLine(char delim) {
  std::vector<std::string> tokens;
  std::stringstream ss(m_currentLine);
  std::string token;

  while (std::getline(ss, token, delim)) {
    tokens.push_back(token);
  }

  // We had no semicolon at the line
  if (tokens.size() == 0)
    tokens.push_back(m_currentLine);

  return tokens;
}