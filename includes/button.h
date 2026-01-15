#ifndef BUTTON_H//pilnuje aby nie includować wielokrotnie tego pliku
#define BUTTON_H
#include <SFML/Graphics.hpp>
#include "button.cpp"

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
inline void button_animation(
    sf::RectangleShape &button,//animowany guzik 
    sf::Text &text,//animowana etykieta guzika
    sf::RenderWindow &window//oko w którym akcja się rozgrywa
);

// inline void button_shake_animation(
//     sf::RectangleShape &button, 
//     sf::Text &text,
//     int frames
// );


inline void build_start_bttons(
    unsigned int width,//Szerokość ekranu
    unsigned int height,//Wysokość ekranu
     unsigned int button_color,//Kolor przycisku
     unsigned int text_color,//Kolor tekstu
     sf::RectangleShape* buttons,//Tablica z przyciskami starowymi
     sf::Text* buttons_labels,//Tablica z napisami do nich
     sf::Font& font//Czcionka
);




#endif // BUTTON_H
