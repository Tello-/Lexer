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

void printFileContents(std::ifstream& in)
{     
     std::string file_text((
          std::istreambuf_iterator<char>(in)),                   // Starting Position
          std::istreambuf_iterator<char>());                     // Ending Position

     std::cout << "\n\nFile Contains: \n\t" << file_text << std::endl;
}

int main(int argc, char** argv)
{
     std::string file_path;                            // A string to hold the file path of argv[1]
     std::ifstream in_stream;                          // A stream for loading contents from file argv[1]
     
     if (argc > 1)                                     // this can pass even when the filepath 
                                                       //is not a good one (which is fine but not ideal)
     {
          file_path.assign(argv[1]);                   // get CL arg and assign it to file_path
          std::cout << "\n\nRetrieved CL arg: \n"      // report file path retrieved
                    << file_path 
                    << std::endl;

          
                                                       
          in_stream.exceptions(std::ifstream::failbit);          /* A bitmask value of member type iostate 
                                                                 formed by a combination of error state flag 
                                                                 bits to be set (badbit, eofbit and/or failbit), 
                                                                 or set to goodbit (or zero).*/

               
          try                                                             // ** Try Loading File from 
          {                                                               // path provided by argv **
               in_stream.open(file_path, std::ifstream::in);              
                              
          }
          catch (const std::ifstream::failure& e)                         // catch paramter same as bitmask
          {
               std::ostringstream msg;
               msg << "\n\n**ERROR** \n\nOpening file '" << file_path
                    << "' failed, it either doesn't exist or is not accessible.\n\n";
               std::cout << msg.str();
               return 1;
          }
          

          printFileContents(in_stream);
         
     }
     
     std::cin.get();                                                  // Keeps program from terminating (debug)
     return 0;
}
