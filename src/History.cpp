#include "History.hpp"

void History::setAllias(const std::shared_ptr<Alias> &alias) { m_alias = alias; }

void History::printHistory() const {
  if (startedAgain) {
    for (int i = 0; i < maxSize; i++) {
      std::cout << i << ": " << history[i] << std::endl;
    }
  } else {
    for (int i = 0; i < currentIndex; i++) {
      std::cout << i << ": " << history[i] << std::endl;
    }
  }

  std::cout << "\nhistory [number] to execute a specfic command\n\n";
}

void History::addToHistory(const std::string &line) {
  if (currentIndex == maxSize) {
    currentIndex = 0;
    startedAgain = true;
  }
  history[currentIndex++] = line;
}

void History::handleHistory(std::vector<char *> &lineTokens) {
  if (lineTokens[1] == nullptr)
    return;
  if (std::strcmp(lineTokens[0], "history") != 0)
    return;
  int number = 0;

  try {
    number = std::stoi(lineTokens[1]); // Can throw
    if (number < 0 || number > 19)
      return;

    auto command = history[number];
    lineTokens.clear();

    // Check if the history command is alias
    if (m_alias->isAlias(command)) {
      const auto &al = m_alias->getAliasMap();
      auto line = al.at(command); // Can not throw we check for alias
      parseEnviromentalVariables(line);
      makeTokens(lineTokens, line);
    } else {
      parseEnviromentalVariables(history[number]);
      makeTokens(lineTokens, history[number]);
    }
  } catch (const std::exception &e) {
    std::cout << "Number required for history usage\n";
    return;
  }
}