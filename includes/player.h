#ifndef PLAYER_H
#define PLAYER_H
#include "player.cpp"


// Deklaracja globalnej instancji `player` (definicja w player.cpp)
extern Player player;

void save_player();

void read_player();

//Funkcja tworząca napisy ze statystykami
void build_player_stat(
    sf::Text* statistic,//Tablica przechowująca napisy ze statystykami
    sf::Font& font, //Czcionka
    sf::Font& math_font,//Czcionka dla symboli
    float width, //Szerokość okna
    float height,//Wysokość okna
    unsigned int color//Kolor napisów
);

//Funkcja aktualizująca statystyki
void update_player_stat(
    sf::Text* statistic
);

void build_stat_bg(
    sf::RectangleShape* bg//Tablica przechowująca kształty wokoło statystyk
);

void builda_gamemode_buttons(
    sf::RectangleShape* buttons,//tablica przechowująca przyciski do trybu gry
    unsigned int width,//Szerokośc okna
    unsigned int height,//Wysokość okna
    unsigned int button_color,//Kolor przycisków
    unsigned int border_color//Kolor obramówki
);

//Funkcja tworząca etykiety dla przycisków
void build_gamemode_buttons_labels(
    sf::Text* buttons_labels,//Tablica przechowująca etykiety napisów
    sf::Font& font,//Czcionka, matematyczna
    unsigned int width,//Szerokość okna
    unsigned int height,//Wysokość  okna
    unsigned int color//Kolor etykiet
);




#endif