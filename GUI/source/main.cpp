#include "windows/mainWindow.hpp"
#include <iostream>

int main (){
   Fl::scheme("gleam");
   std::cout << Fl::version() << '\n';
   SimpleWindow win(1500,900,"SimpleWindow");
   win.show();
   return Fl::run();
}
