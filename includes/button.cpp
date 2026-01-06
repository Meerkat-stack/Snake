#include <iostream>
#include <SFML/Graphics.hpp>

//akcja guzika, zwraca 1 gdzy kliknięty
int button_action(const sf::RectangleShape &button, const sf::Event &ev, sf::RenderWindow &window){
    if(ev.is<sf::Event::MouseButtonPressed>()){
        const auto* mousePress = ev.getIf<sf::Event::MouseButtonPressed>();
        if(mousePress->button == sf::Mouse::Button::Left){
            sf::Vector2i pixelPos = mousePress->position;
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            if(button.getGlobalBounds().contains(worldPos)) return 1;
        }
    }
    return 0;
}

//animacja guzika
void button_animation(sf::RectangleShape &button, sf::RenderWindow &window)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
    bool hovered = button.getGlobalBounds().contains(worldPos);
    if(hovered) button.setScale({1.15f, 1.15f});
    else button.setScale({1.0f, 1.0f});
}
