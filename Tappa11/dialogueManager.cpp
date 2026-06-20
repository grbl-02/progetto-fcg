#include "dialogueManager.hpp"

DialogueManager::DialogueManager() {
    std::vector<std::string> chest_opening3;
    chest_opening3.push_back("You opened the chest.");
    chest_opening3.push_back("Inside the chest there was a button.");
    chest_opening3.push_back("You pressed the button.");
    chest_opening3.push_back("...");
    chest_opening3.push_back("There was a tremor.");
    dialogueMap["chest_opening3"] = chest_opening3;

    std::vector<std::string> chest_opened;
    chest_opened.push_back("The chest was opened.");
    dialogueMap["chest_opened"] = chest_opened;

    std::vector<std::string> chest_opening5;
    chest_opening5.push_back("You opened the chest.");
    chest_opening5.push_back("Inside the chest there was another button.");
    chest_opening5.push_back("You pressed it.");
    chest_opening5.push_back("...");
    chest_opening5.push_back("There was another tremor.");
    dialogueMap["chest_opening5"] = chest_opening5;

    std::vector<std::string> chest_opening6;
    chest_opening6.push_back("You opened the chest.");
    chest_opening6.push_back("Inside the chest there was a victory screen.");
    dialogueMap["chest_opening6"] = chest_opening6;
}