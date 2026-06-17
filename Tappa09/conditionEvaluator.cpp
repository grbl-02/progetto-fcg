#include "conditionEvaluator.hpp"
#include "flags.hpp"

ConditionEvaluator::ConditionEvaluator(const Flags& flags)
{
    conditionMap["no_chests_opened"] = [&flags]() {
        return !flags.chest_3_opened && !flags.chest_5_opened;
    };
    conditionMap["chest_3_only"] = [&flags]() {
        return flags.chest_3_opened && !flags.chest_5_opened;
    };
    conditionMap["chest_5_opened"] = [&flags]() {
        return flags.chest_5_opened;
    };
    conditionMap["!chest_3_opened"] = [&flags]() {
        return !flags.chest_3_opened;
    };
    conditionMap["chest_3_opened"] = [&flags]() {
        return flags.chest_3_opened;
    };
    conditionMap["room_3_gauntlet_cleared"] = [&flags]() {
        return flags.room_3_gauntlet_cleared;
    };
    conditionMap["room_3_gauntlet_triggered_not_cleared"] = [&flags]() {
        return flags.room_3_gauntlet_triggered && !flags.room_3_gauntlet_cleared;
    };
    conditionMap["room_5_gauntlet_cleared"] = [&flags]() {
        return flags.room_5_gauntlet_cleared;
    };
    conditionMap["room_5_gauntlet_triggered_not_cleared"] = [&flags]() {
        return flags.room_5_gauntlet_triggered && !flags.room_5_gauntlet_cleared;
    };
    conditionMap["room_6_gauntlet_cleared"] = [&flags]() {
        return flags.room_6_gauntlet_cleared;
    };
    conditionMap["room_6_gauntlet_triggered_not_cleared"] = [&flags]() {
        return flags.room_6_gauntlet_triggered && !flags.room_6_gauntlet_cleared;
    };
}

bool ConditionEvaluator::evaluate(const std::string& conditionKey)
{
    auto it = conditionMap.find(conditionKey);
    if (it != conditionMap.end())
    {
        return it->second();
    }
    return false;
}