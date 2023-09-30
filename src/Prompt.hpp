#pragma once

#include "History.hpp"

#include <limits.h> // PATH_MAX
#include <string.h>
#include <unistd.h> // getcwd

#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct cmp_str {
  bool operator()(char const *a, char const *b) const { return std::strcmp(a, b) < 0; }
};

class Prompt {
public:
  Prompt();
  ~Prompt() = default;

  void show() const;
  void readLine();

  int main();

private:
  std::shared_ptr<History> m_history;
  std::shared_ptr<Alias> m_alias;

  std::string m_currentLine;
  std::map<const char *, std::function<bool(std::vector<char *> &)>, cmp_str> m_dispatchTable;

  std::vector<std::string> splitLine(char delim);
};