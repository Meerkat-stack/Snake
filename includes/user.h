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

//Przyjmuje tekst z klawiatury i przypisuje go do zmiennej aby go potem wyświetlić
void input_text(
    std::string& target_string,//Zmienna przechowująca napis 
    const sf::Event &ev,//Zdarzenie
    int limit,//Maksymalna liczba znaków
    int active_field,//Które pole jest aktywne, bo jak hasła to nie wyświetlaj hasła
    int char_count//Zlicza znaki aby przesuwać kursor
);



#endif
