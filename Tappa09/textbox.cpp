#include "textbox.hpp"

Textbox::Textbox(sf::Font& font) : text(font, "", 8)
{
    box = sf::RectangleShape({(float)window_width / 3.f, (float)window_height / 6.f});
    box.setPosition({0.f, 6 * 32.f});
    box.setFillColor(sf::Color::Black);
    text.setPosition({0.f, 6 * 32.f});
    isActive = false;
}

void Textbox::showNextLine()
{
    if (isActive)
    {
        if (!dialogue_queue.empty())
        {
            text.setString(dialogue_queue.front());
            dialogue_queue.pop();
        }
        else
        {
            isActive = false;
        }
    }
}

void Textbox::draw(sf::RenderWindow& window)
{
    if (isActive)
    {
        window.draw(box);
        window.draw(text);
    }
}