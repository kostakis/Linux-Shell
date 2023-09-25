#include "Alias.hpp"

bool Alias::isAlias(const std::string& key) const {
    if (aliases.find(key) == aliases.end()) {
        return false;
    }
    else return true;
}

bool Alias::handle(std::vector<char*>& tokens) {
    if (strcmp(tokens[0], "createalias") == 0) {
        if (tokens[1] == nullptr || tokens[2] == nullptr) return false;

        std::string fullCommnand{};
        for (int i = 2; i < tokens.size() - 1; i++) {
            fullCommnand = fullCommnand + " " + tokens[i];
        }

        // Erase quotes if the are present
        fullCommnand.erase(std::remove(fullCommnand.begin(), fullCommnand.end(), '\"'), fullCommnand.end());

        if (fullCommnand.back() == ';')  fullCommnand.pop_back(); // Remove the ; from the string

        aliases[tokens[1]] = fullCommnand;
        return true;
    }
    else if (strcmp(tokens[0], "destroyalias") == 0) {
        if (tokens[1] == nullptr) return false;

        // We dont check if ; is present at the last.
        // Dunno if it is mandatory

        auto it = aliases.find(tokens[1]);
        if (it != aliases.end()) aliases.erase(it);

        return true;
    }
    else return false;
}
