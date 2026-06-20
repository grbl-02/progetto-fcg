#pragma once
#include "config.hpp"

struct Flags;

struct ConditionEvaluator {
    std::unordered_map<std::string, std::function<bool()>> conditionMap;

    ConditionEvaluator(const Flags& flags);
    bool evaluate(const std::string& conditionKey);
};