// Author: Josh Lollis
// Created 2/11/2021
// Last Edited: 2/11/2021


/* This program is a bare bones main entry point that takes command line arguments

The purpose of this piece of code is for collecting the file name of 
     a source file. Future programs can then use this template to make
     a main entry that loads files after getting the file name.*/


#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char** argv)
{
     std::string         file_path;
     std::string         file_text;

     std::ifstream       in_stream;
     
     if (argc)
     {
          file_path.assign(argv[1]);                   // get CL arg and assign it to file_path
          std::cout << "Retrieved CL arg: \n" 
                    << file_path 
                    << std::endl;

          //try
          //{
          //     //in_stream.open(file_path);
          //}
          //catch ()
          //{

          //}
     }
     
     std::cin.get();
     return 0;
}

/*

std::ifstream in("file.txt");
std::string contents((std::istreambuf_iterator<char>(in)),
    std::istreambuf_iterator<char>());
    
    */