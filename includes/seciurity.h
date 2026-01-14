#ifndef SECIURITY_H//pilnuje aby nie includować wielokrotnie tego pliku
#define SECIURITY_H
#include <SFML/Graphics.hpp>
#include "seciurity.cpp"

//Funkcje szyfrująca
void encrypt(
    std::string& text,//Tekst do zaszyfrowania
    int n//Przesunięcie
);

//Funkcje deszyfrująca
void decrypt(
    std::string& text//Tekst do zdeszyfrowania
    ,int n//Przesunięcie
);


#endif
