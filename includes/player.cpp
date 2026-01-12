#include <iostream>
#include <fstream>
#include "seciurity.h"

// Struktura gracza
struct Player {
	std::string name = "Unknown";

	// Statystyki podstawowe
	float total_pixels_traveled = 0.0f;
	int total_play_time_sec = 0;
	int snake_max_length = 0;
	int snake_min_length = 0;

	// Statystyki rozszerzone (Przyczyny zgonów)
	int deaths_wall = 0;
	int deaths_self = 0;

	// Postępy
	int unlocked_maps_count = 8;
	int unlocked_skins_count = 8;
};

extern Player player;

void save_player(){
    if (player.name == "Guest" || player.name == "Unknown") return;//Dla gości albo niezalogowanych nie zapisuje nic
    else{
        std::ifstream original("userdata/users_data.txt");//Otwiera plik do odczytu
        std::ofstream temp("userdata/users_data_temp.txt");//Otwiera plik do zapisu

        std::string s_name, s_dist, s_time, s_maxL, s_minL, s_wall, s_self, s_maps, s_skins;

        std::string tmp_name = player.name;
        encrypt(tmp_name,16);

        while(original>>s_name>>s_dist>>s_time>>s_maxL>>s_minL>>s_wall>>s_self>>s_maps>>s_skins) {

            // decrypt(s_name,16);//Deszyfruje odczytaną nazwę gracza

            if(s_name==tmp_name){
                //Modyfikuje dane gracza przed zapisaniem        
                // std::cout<<"    |"<<s_name<<std::endl;//Debug
                // std::cout<<player.name<<std::endl;//Debug

                // s_name = player.name;
                // encrypt(s_name,16);//Szyfruje przed zapisem
                s_dist = std::to_string(player.total_pixels_traveled);//Zamienia int na string
                s_time = std::to_string(player.total_play_time_sec);
                s_maxL = std::to_string(player.snake_max_length);
                s_minL = std::to_string(player.snake_min_length);
                s_wall = std::to_string(player.deaths_wall);
                s_self = std::to_string(player.deaths_self);
                s_maps = std::to_string(player.unlocked_maps_count);
                s_skins = std::to_string(player.unlocked_skins_count);
            }

            temp<<s_name<<" "<<s_dist<<" "<<s_time<<" "<<s_maxL<<" "<<s_minL<<" "<<s_wall<<" "<<s_self<<" "<<s_maps<<" "<<s_skins<<"\n";
        }
        original.close();
        temp.close();

        //Usuwa stary plik, 0 oznacza powodzenie 
        if (std::remove("userdata/users_data.txt") != 0) {
            // std::cout<<"Old file canceled\n";//Debug
        }

        //Zmienia nazwę nowego pliku
        if (std::rename("userdata/users_data_temp.txt", "userdata/users_data.txt") != 0) {
            // std::cout<<"Error: zmiana nazwy nie powiodła się\n"//Debug
        }

    }
}

//Odczytuje dane gracza
void read_player() {
    std::ifstream file("userdata/users_data.txt");

    if (!file.is_open()) {
        std::cerr << "Błąd: Nie można otworzyć pliku users_data.txt do odczytu!\n";
        return;
    }

    std::string s_name, s_dist, s_time, s_maxL, s_minL, s_wall, s_self, s_maps, s_skins;

    std::string target_cipher = player.name;
    encrypt(target_cipher, 16); 

    while (file >> s_name >> s_dist >> s_time >> s_maxL >> s_minL >> s_wall >> s_self >> s_maps >> s_skins) {
        
        // Jeśli zaszyfrowane imię z pliku pasuje do naszego celu
        if (s_name == target_cipher) {
            // 4. Konwertujemy stringi z pliku na liczby i zapisujemy w strukturze player
            try {
                player.total_pixels_traveled = std::stof(s_dist);
                player.total_play_time_sec = std::stoi(s_time);
                player.snake_max_length = std::stoi(s_maxL);
                player.snake_min_length = std::stoi(s_minL);
                player.deaths_wall = std::stoi(s_wall);
                player.deaths_self = std::stoi(s_self);
                player.unlocked_maps_count = std::stoi(s_maps);
                player.unlocked_skins_count = std::stoi(s_skins);
            } catch (...) {
                std::cerr << "Bład konwersji danych dla gracza: " << player.name << "\n";
            }
            break; // Znaleźliśmy właściwego gracza, wychodzimy z pętli
        }
    }

    file.close();
}

//Funkcja budująca wizualizację statystyk
void build_player_stat(sf::Text* statistic, sf::Font& font, sf::Font& math_font,float width, float height, unsigned int color){
    int size = (int)(1.5*(0.5f*0.037f*height+3));//Polepszyć jakość tak jak przy innych rozmiarach
    
    std::string labels[9];
    labels[0] = "Player:\t" + player.name;
    labels[1] = "Total play time: " + std::to_string(player.total_play_time_sec) + " s";
    labels[2] = "Distance traveled: " + std::to_string((int)player.total_pixels_traveled) + " px";
    labels[3] = "Max snake length: " + std::to_string(player.snake_max_length);
    labels[4] = "Min snake length: " + std::to_string(player.snake_min_length);
    labels[5] = "Deaths by wall: " + std::to_string(player.deaths_wall);
    labels[6] = "Deaths by tail: " + std::to_string(player.deaths_self);
    labels[7] = "Unlocked maps:\t";
    sf::String math_symbols[] = { 
        L"\u03C0", L"e", L"\u03B3", L"\u03B6(3)", L"\u03D5", L"\u221A2", L"\u221A3", L"\u221A5" 
    };    
    for(int i=0;i<player.unlocked_maps_count;i++){
        labels[8] += math_symbols[i];
        labels[8] += " ";
    }

// Jeśli chcesz użyć 9. pozycji dla skinów, musiałbyś zwiększyć tablicę do [9]
// labels[8] = "Unlocked skins: " + std::to_string(player.unlocked_skins_count);

    //Ustawia kolor wszyskic na jednakowy, ustawia punkt odniesienia na lewo i ewentualnie grubośc krawędzi, pozycje
    for(int i=0;i<9;i++){
        if(i==8){
            statistic[i] = sf::Text(math_font,labels[i],size);
            statistic[i].setStyle(sf::Text::Bold);//Pogrubia symbole
            statistic[i].setCharacterSize(4*size);//Ustawia większą czcionkę
            statistic[i].setScale({0.25f,0.25f});//Skaluje symbole
            // statistic[i].setOutlineThickness(0.25);
            // statistic[i].setOutlineColor(sf::Color(color));
        }
        else{
            statistic[i] = sf::Text(font,labels[i],size);//Tekst pola
        }
        statistic[i].setFillColor(sf::Color(color));
        //Ewentualnie grubość krawędzi
        sf::FloatRect bounds = statistic[i].getLocalBounds();
        statistic[i].setOrigin({bounds.position.x, 0.f});
        

        //Ustawienie pozycji
        if(i==0)//Nazwa
            statistic[i].setPosition({width/8.0f,height/12.0f + i*0.07f*height});//Ustawia odstep między linijkami
        else if(i==1||i==2||i==3)//Pierwsza kolumna
            statistic[i].setPosition({width/8.0f+0.02f*width,height/12.0f + i*0.07f*height});
        else if(i==4||i==5||i==6)//Druga kolumna
            statistic[i].setPosition({width/8.0f+0.02f*width+0.4f*width,height/12.0f + (i-3)*0.07f*height});
        else if(i==7)//Etykieta dla stałych
            statistic[i].setPosition({width/8.0f,height/12.0f + (i-3)*0.075f*height});
        else if(i==8)//Stałe
            statistic[i].setPosition({width/8.0f+0.2f*width,height/12.0f + (i-4)*0.075f*height});

    }
}

//Funkcja aktualizująca statystyki za pomoca sf::setString("");
void update_player_stat(sf::Text* statistic){
    
    statistic[0].setString("Player:\t" + player.name);
    statistic[1].setString("Total play time: " + std::to_string(player.total_play_time_sec) + " s");
    statistic[2].setString("Distance traveled: " + std::to_string((int)player.total_pixels_traveled) + " px");
    statistic[3].setString("Max snake length: " + std::to_string(player.snake_max_length));
    statistic[4].setString("Min snake length: " + std::to_string(player.snake_min_length));
    statistic[5].setString("Deaths by wall: " + std::to_string(player.deaths_wall));
    statistic[6].setString("Deaths by tail: " + std::to_string(player.deaths_self));
    statistic[7].setString("Unlocked maps:");

    sf::String tmp_label = "";
    sf::String math_symbols[] = { 
        L"\u03C0", L"e", L"\u03B3", L"\u03B6(3)", L"\u03D5", L"\u221A2", L"\u221A3", L"\u221A5" 
    };    
    for(int i=0;i<player.unlocked_maps_count;i++){
        tmp_label += math_symbols[i];
        tmp_label += "\t";
    }

    statistic[8].setString(tmp_label);//Wyświetla stałe jakie osiągnieto

    for(int i = 0; i < 9; i++) {
        sf::FloatRect bounds = statistic[i].getLocalBounds();
        statistic[i].setOrigin({bounds.position.x, 0.f}); // Tylko jeśli chcesz wyrównanie do lewej po zmianie tekstu
    }

}