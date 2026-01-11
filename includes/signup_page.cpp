#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>


//Funkcja która sprawdza czy nazwa użytkownika nie jest zajęta
//Funckaj która zapisuje użytkownika w książce adresów
int add_user(std::string username, std::string password) {
    encrypt(username,16);
    encrypt(password,16);
    // zwraca 1 gdy zarejestrowano pomyślnie
    // zwraca 0 gdy użytkownik o takim loginie już istnieje
    // zwraca -1 gdy nie można otworzyć/zapisać pliku

    // 1. NAJPIERW SPRAWDZAMY CZY LOGIN JEST WOLNY
    std::ifstream file_in("userdata/users_log.txt");
    if (file_in.is_open()) {
        std::string u, p;
        while (file_in >> u >> p) {
            if (u == username) {
                file_in.close();
                return 0; // Login zajęty!
            }
        }
        file_in.close();
    }

    // 2. DOPISUJEMY NOWEGO UŻYTKOWNIKA
    // std::ios::app sprawia, że nie kasujemy pliku, tylko dopisujemy na końcu
    std::ofstream file_out("userdata/users_log.txt", std::ios::app);

    //dopisuje gracza
    std::ofstream file_player("userdata/users_data.txt",std::ios::app);
    
    if (!file_out.is_open()||!file_player.is_open()) {
        // std::cout << "Cannot open users_log.txt to add user.\n";
        return -1;
    }

    // Zapisujemy: login [spacja] hasło [nowa linia]
    file_out << username << " " << password << "\n";
    file_player<<username<<" 0 0 0 0 0 0 1 1\n";
    
    file_out.close();
    file_player.close();
    return 1; // Sukces
}

void sign_up(int& state, int& error_log, std::string& login_input, std::string& password_input, std::string& hide_password_input, sf::Text& login_input_text, sf::Text& password_input_text, int& active_field) {
    // Wywołujemy funkcję dopisującą do pliku
    int signup_status = add_user(login_input, password_input);
    
    // Debug
    // std::cout << "Rejestracja: " << login_input << " | Status: " << signup_status << std::endl;

    // Czyścimy pola tekstowe (tak jak w logowaniu)
    login_input = "";
    password_input = "";
    hide_password_input = "";
    login_input_text.setString("");
    password_input_text.setString("");

    switch (signup_status) {
        case -1:
            state = 0; // Błąd pliku - wracamy do startu
            error_log = -2; // Możesz przypisać -2 jako błąd systemowy
            break;
        case 0:
            // Użytkownik już istnieje
            state = 2; // Zostajemy na stronie rejestracji
            error_log = -2; // Kod błędu dla "Username taken"
            // std::cout << "Login zajety\n";
            break;
        case 1:
            // Sukces! Przenosimy użytkownika do logowania, żeby się sprawdził
            state = 1; 
            active_field = 0;
            error_log = 2; // Kod dla "Account created, please log in"
            // std::cout << "Zarejestrowano pomyślnie\n";
            break;
    }
}