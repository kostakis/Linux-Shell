#pragma once

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>

class Alias {
public:
    Alias() = default;
    ~Alias() = default;

    bool isAlias(const std::string& key) const;
    bool handle(std::vector<char*>& tokens);
    
    const auto& getAliasMap() const {
        return aliases;
    }

private:
    std::map<std::string, std::string> aliases;
};
