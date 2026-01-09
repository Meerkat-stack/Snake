#ifndef LOGING_PAGE_H//pilnuje aby nie includować wielokrotnie tego pliku
#define LOGING_PAGE_H
#include <SFML/Graphics.hpp>
#include "loging_page.cpp"

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
    std::string& hide_target_string,//Zmienna przechowująca ukryte hasło (same ***)
    const sf::Event &ev,//Zdarzenie
    int limit,//Maksymalna liczba znaków
    int active_field//Które pole jest aktywne, bo jak hasła to nie wyświetlaj hasła
);

//Funkcja sprawdzająca dane logowania
int verify_user(
    std::string& username,//Login
    std::string& password//Hasło
);

//Funkcja logująca użytkownika
void log(
    int& state,//Aktualna strona 
    int& error_log,//Czy dane nie były błędne?
    std::string& login_input,//Login
    std::string& password_input,//Hasło
    std::string& hide_password_input,//Hasło ukryte (same***)
    sf::Text& login_input_text,//Text loginu
    sf::Text& password_input_text//Text hasła
);



#endif
