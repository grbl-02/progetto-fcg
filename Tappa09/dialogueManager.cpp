#include "dialogueManager.hpp"

DialogueManager::DialogueManager()
{
    std::vector<std::string> chest_opening;
    chest_opening.push_back("You opened the chest.");
    chest_opening.push_back("Inside the chest there was a button.");
    chest_opening.push_back("You pressed the button.");
    chest_opening.push_back("... There was a tremor.");
    dialogueMap["chest_opening"] = chest_opening;
}