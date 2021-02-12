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
#include <sstream>

int main(int argc, char** argv)
{
     std::string file_path;
     std::ifstream in_stream;
     
     if (argc > 1) // problem is this can pass even when the filepath is not a good one
     {
          file_path.assign(argv[1]);                   // get CL arg and assign it to file_path
          std::cout << "\n\nRetrieved CL arg: \n"      // report file path retrieved
                    << file_path 
                    << std::endl;

          in_stream.exceptions(std::ifstream::failbit); // exception mask for following try block
          try
          {
               in_stream.open(file_path, std::ifstream::in);              // opening the source file
                              
          }
          catch (const std::ifstream::failure& e) {
               std::ostringstream msg;
               msg << "\n\n**ERROR** \n\nOpening file '" << file_path
                    << "' failed, it either doesn't exist or is not accessible.\n\n";
               std::cout << msg.str();
               return 1;
          }

          std::string file_text(                                           // If all was good, then read the file
               (std::istreambuf_iterator<char>(in_stream)),
               std::istreambuf_iterator<char>());

          std::cout << "\n\nFile Contains: \n\t" << file_text << std::endl; // File Content Report

          
     }
     
     std::cin.get();
     return 0;
}

/*

  std::ifstream ifs("filename.txt");    
  std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    */