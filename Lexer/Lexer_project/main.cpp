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
     std::string file_path;
     std::ifstream in_stream;
     
     if (argc)
     {
          file_path.assign(argv[1]);                   // get CL arg and assign it to file_path
          std::cout << "Retrieved CL arg: \n" 
                    << file_path 
                    << std::endl;
          int iValue;
          try
          {
               in_stream.open(file_path);              // opening the source file
          }
          catch (std::exception exc)                   // TODO: figure out best way to handle exceptions here
          {
               std::cerr << "Caught Exception: " << exc.what();
               
               return 1;
          }

          std::string file_text((std::istreambuf_iterator<char>(in_stream)), std::istreambuf_iterator<char>());
          std::cout << "\nText from file: \n\t" << file_text << std::endl;
     }
     
     std::cin.get();
     return 0;
}

/*

  std::ifstream ifs("filename.txt");    
  std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    */