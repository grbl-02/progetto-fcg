#pragma once
#include "config.hpp"

struct Menu
{
    sf::Text start_game;
    sf::Text quit;
    int option;

    Menu(sf::Font& font);
    void draw(sf::RenderWindow& window);
};