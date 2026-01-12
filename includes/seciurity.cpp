#include <iostream>
#include <SFML/Graphics.hpp>


// //Funkcja szyfrująca
// void encrypt(std::string& text,int n){
//     std::string tmp_text="";
//     int i=0;
//     while(text[i]!='\0'){
//         tmp_text += (char)(text[i]+n);
//         i++;
//     }
//     text = tmp_text;
// }


// //Funkcja deszyfrująca
// void decrypt(std::string& text, int n){
//     std::string tmp_text="";
//     int i=0;
//     while(text[i]!='\0'){
//         tmp_text += (char)(text[i]-n);
//         i++;
//     }
//     text = tmp_text;

// }


// Funkcja szyfrująca - używamy referencji, by zmieniać oryginał
void encrypt(std::string& text, int n) {
    for (char& c : text) {
        c = c + n; // Przesunięcie o n pozycji w tabeli ASCII
    }
}

// Funkcja deszyfrująca
void decrypt(std::string& text, int n) {
    for (char& c : text) {
        c = c - n;
    }
}