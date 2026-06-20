#pragma once
#include "config.hpp"

struct DialogueManager {
    std::unordered_map<std::string, std::vector<std::string>> dialogueMap;

    DialogueManager();
};