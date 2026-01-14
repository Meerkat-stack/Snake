#ifndef SIGNUP_PAGE_H//pilnuje aby nie includować wielokrotnie tego pliku
#define SIGNUP_PAGE_H
#include <SFML/Graphics.hpp>
#include "signup_page.cpp"
#include "seciurity.h"


int add_user(
    std::string& username,//Nazwa użytkownika 
    std::string& password//Hasło
);

void log(
    int& state,//Aktualna strona 
    int& error_log,//Czy dane nie były błędne?
    std::string& login_input,//Login
    std::string& password_input,//Hasło
    std::string& hide_password_input,//Hasło ukryte (same***)
    sf::Text& login_input_text,//Text loginu
    sf::Text& password_input_text,//Text hasła
    int& active_field
);




#endif
