#include "textbox.hpp"

Textbox::Textbox(sf::Font& font) : text(font, "", 8) {
    box = sf::RectangleShape({(float)window_width / 3.f, (float)window_height / 15.f});
    box.setPosition({0.f, 8 * 32.f});
    box.setFillColor(sf::Color::Black);
    text.setPosition({box.getPosition().x + 10.f, box.getPosition().y + 10.f});
    isActive = false;
}

void Textbox::showNextLine() {
    if (isActive) {
        if (!dialogue_queue.empty()) {
            text.setString(dialogue_queue.front());
            dialogue_queue.pop();
        } else isActive = false;
    }
}

void Textbox::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(box);
        window.draw(text);
    }
}