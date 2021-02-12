// Author: Josh Lollis
// Created 2/11/2021
// Last Edited: 2/11/2021


/* This program is a bare bones main entry point that takes command line arguments

The purpose of this piece of code is for collecting the file name of 
     a source file. Future programs can then use this template to make
     a main entry that loads files after getting the file name.*/


#include <iostream>
#include <string>

int main(int argc, char** argv)
{
     if (argc)
     {
          std::string file_string{ argv[1] };
          std::cout << file_string << std::endl;
     }
     
     std::cin.get(); // temp fix to keep the console window up before termination of program
     return 0;
}