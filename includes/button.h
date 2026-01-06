#ifndef BUTTON_H//pilnuje aby nie includować wielokrotnie tego pliku
#define BUTTON_H
#include "button.cpp"
#include <SFML/Graphics.hpp>

// Akcja guzika, zwraca 1 gdy kliknięty
int button_action(
    const sf::RectangleShape &button, //prostokąt który reprezentuje guzik
    const sf::Event &ev, //zdażenie w pętli
    sf::RenderWindow &window//okno w którym dzieje się akcja
);

// Animacja guzika
void button_animation(
    sf::RectangleShape &button,//animowany guzik 
    sf::RenderWindow &window//oko w którym akcja się rozgrywa
);


#endif // BUTTON_H
