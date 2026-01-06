#ifndef BUTTON_H//pilnuje aby nie includować wielokrotnie tego pliku
#define BUTTON_H
#include "button.cpp"
#include <SFML/Graphics.hpp>

//Funkcja tworząca przycisk
sf::RectangleShape build_button(
    float a,//Szerokość
    float b,//Wysokość
    float x,//Położenie, skłądowa x-owa
    float y, //Położenie, składowa y-owa
    unsigned int color,//Kolor przycisku
    unsigned int border_color//Kolr obramówki
);

//Funkcja tworząca napis do przycisku
sf::Text label(
    int size,//Rozmiar tekstu
    float x, //Położenie, składowa x-owa
    float y, //Położenie, składowa y-owa
    std::string label_text,//Napis
    unsigned int color,//Kolor napisu
    sf::Font& font//Czcionka
);


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
