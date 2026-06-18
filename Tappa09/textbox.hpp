#pragma once
#include "config.hpp"

struct Textbox
{
    sf::RectangleShape box;
    sf::Text text;
    bool isActive;
    std::queue<std::string> dialogue_queue;

    Textbox(sf::Font& font);
    void showNextLine();
    void draw(sf::RenderWindow& window);
};