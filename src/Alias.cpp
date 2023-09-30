#include "Alias.hpp"

bool Alias::isAlias(const std::string &key) const {
  if (aliases.find(key) == aliases.end()) {
    return false;
  } else
    return true;
}

bool Alias::create(std::vector<char *> &tokens) {
  if (tokens[1] == nullptr || tokens[2] == nullptr)
    return false;

  std::string fullCommnand{};
  for (int i = 2; i < tokens.size() - 1; i++) {
    fullCommnand = fullCommnand + " " + tokens[i];
  }

  // Erase quotes if the are present
  fullCommnand.erase(std::remove(fullCommnand.begin(), fullCommnand.end(), '\"'), fullCommnand.end());

  if (fullCommnand.back() == ';')
    fullCommnand.pop_back(); // Remove the ; from the string

  aliases[tokens[1]] = fullCommnand;
  return true;
}

bool Alias::destroy(std::vector<char *> &tokens) {
  if (tokens[1] == nullptr)
    return false;

  auto it = aliases.find(tokens[1]);
  if (it != aliases.end())
    aliases.erase(it);

  return true;
}