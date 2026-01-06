#ifndef BUTTON_H//pilnuje aby nie includować wielokrotnie tego pliku
#define BUTTON_H
#include "make_button.cpp"
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


#endif // BUTTON_H
