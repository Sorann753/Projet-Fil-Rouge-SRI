#include "mainWindow.hpp"
#include <iostream>

int main (){
   Fl::scheme("gleam");
   std::cout << Fl::version() << '\n';
   SimpleWindow win(300,200,"SimpleWindow");
   return Fl::run();
}