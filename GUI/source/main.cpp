 #include "mainWindow.hpp"

int main (){
   Fl::scheme("gleam");
   SimpleWindow win(300,200,"SimpleWindow");
   return Fl::run();
}