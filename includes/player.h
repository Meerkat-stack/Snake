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



#endif