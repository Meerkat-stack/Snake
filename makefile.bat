@echo off
g++ -c main.cpp -I"C:\C++lib\SFML-3.0.2\include"
g++ main.o -o main -L"C:\C++lib\SFML-3.0.2\lib" -lsfml-graphics -lsfml-window -lsfml-system -lsfml-system -mwindows
echo Succes!
