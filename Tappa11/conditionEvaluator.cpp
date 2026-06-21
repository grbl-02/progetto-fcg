#include "conditionEvaluator.hpp"
#include "flags.hpp"

ConditionEvaluator::ConditionEvaluator(const Flags& flags) {
    condition_map["no_chests_opened"] = [&flags]() {
        return !flags.chest_3_opened && !flags.chest_5_opened;
    };
    condition_map["chest_3_only"] = [&flags]() {
        return flags.chest_3_opened && !flags.chest_5_opened;
    };
    condition_map["chest_5_opened"] = [&flags]() {
        return flags.chest_5_opened;
    };
    condition_map["!chest_3_opened"] = [&flags]() {
        return !flags.chest_3_opened;
    };
    condition_map["chest_3_opened"] = [&flags]() {
        return flags.chest_3_opened;
    };
    condition_map["room_3_gauntlet_cleared_but_chest_not_fallen"] = [&flags]() {
        return flags.room_3_gauntlet_cleared && !flags.chest_3_fell;
    };
    condition_map["room_3_gauntlet_triggered_not_cleared"] = [&flags]() {
        return flags.room_3_gauntlet_triggered && !flags.room_3_gauntlet_cleared;
    };
    condition_map["room_5_gauntlet_cleared_but_chest_not_fallen"] = [&flags]() {
        return flags.room_5_gauntlet_cleared && !flags.chest_5_fell;
    };
    condition_map["room_5_gauntlet_triggered_not_cleared"] = [&flags]() {
        return flags.room_5_gauntlet_triggered && !flags.room_5_gauntlet_cleared;
    };
    condition_map["room_6_gauntlet_cleared_but_chest_not_fallen"] = [&flags]() {
        return flags.room_6_gauntlet_cleared && !flags.chest_6_fell;
    };
    condition_map["room_6_gauntlet_triggered_not_cleared"] = [&flags]() {
        return flags.room_6_gauntlet_triggered && !flags.room_6_gauntlet_cleared;
    };
    condition_map["chest_3_fell"] = [&flags]() {
        return flags.chest_3_fell && !flags.chest_3_opened;
    };
    condition_map["chest_5_fell"] = [&flags]() {
        return flags.chest_5_fell && !flags.chest_5_opened;
    };
    condition_map["chest_6_fell"] = [&flags]() {
        return flags.chest_6_fell && !flags.chest_6_opened;
    };
    condition_map["chest_6_opened"] = [&flags]() {
        return flags.chest_6_opened;
    };
}

bool ConditionEvaluator::evaluate(const std::string& condition_key) {
    auto it = condition_map.find(condition_key);
    if (it != condition_map.end())
    {
        return it->second();
    }
    return false;
}