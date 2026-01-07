@echo off

g++ -c main.cpp -I"C:\biblioteki\SFML-3.0.2\include"
g++ main.o -o main -L"C:\biblioteki\SFML-3.0.2\lib"  -lsfml-graphics -lsfml-window -lsfml-system

echo Hahaha
