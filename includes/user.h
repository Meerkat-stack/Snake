#ifndef USER_H//pilnuje aby nie includować wielokrotnie tego pliku
#define USER_H
#include <SFML/Graphics.hpp>
#include "user.cpp"

//Tworzy rakme na tekst, prz wpisywaniu
sf::RectangleShape text_frame(
    float a,//Szerokość ramki 
    float b,//Wysokość ramki
    float x,//Położenie, składowa x-owa
    float y,//Położenie, składowa y-owa
    unsigned int border_color//Kolor ramki
);

//Tworzy tło za ramką na tekst
sf::RectangleShape text_frame_bg(
    float a,//Szerokość ramki 
    float b,//Wysokość ramki
    float x,//Położenie, składowa x-owa
    float y,//Położenie, składowa y-owa
    unsigned int bg_color,//Kolor tła ramki
    unsigned int border_color//Kolor ramki ramki
);


#endif
