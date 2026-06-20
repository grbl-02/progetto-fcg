#pragma once
#include "config.hpp"

struct Flags;

struct ConditionEvaluator {
    std::unordered_map<std::string, std::function<bool()>> condition_map;

    ConditionEvaluator(const Flags& flags);
    bool evaluate(const std::string& conditionKey);
};