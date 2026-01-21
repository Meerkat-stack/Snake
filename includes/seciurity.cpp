#include <iostream>
#include <SFML/Graphics.hpp>

//Funkcja szyfrująca, używa referencji, by zmieniać oryginał
void encrypt(std::string& text, int n) {
    for (char& c : text) {//To samo co for (int i = 0; i < text.size(); i++)
        c = c + n; // Przesunięcie o n pozycji w tabeli ASCII
    }
}

//Funkcja deszyfrująca
void decrypt(std::string& text, int n) {
    for (char& c : text) {//To samo co for (int i = 0; i < text.size(); i++)
        c = c - n;
    }
}