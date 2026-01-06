#ifndef WINDOW_LOOK_H//pilnuje aby nie includować wielokrotnie tego pliku
#define WINDOW_LOOK_H
#include <SFML/Graphics.hpp>
#include "window_look.cpp"

//Tworzy ramkę wokoło okna
sf::RectangleShape build_border(
    unsigned int width,//Szerokość okna 
    unsigned int height,//Wyskokość okna
    unsigned int color//Kolor ramki
);

#endif