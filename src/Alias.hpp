#pragma once

#include <algorithm>
#include <iostream>
#include <map>
#include <string.h>
#include <string>
#include <vector>

class Alias {
public:
  Alias() = default;
  ~Alias() = default;

  bool isAlias(const std::string &key) const;
  bool handle(std::vector<char *> &tokens);

  bool create(std::vector<char *> &tokens);
  bool destroy(std::vector<char *> &tokens);

  const auto &getAliasMap() const { return aliases; }

private:
  std::map<std::string, std::string> aliases;
};
