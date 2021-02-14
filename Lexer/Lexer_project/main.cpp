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
     
     if (argc > 1) // this can pass even when the filepath is not a good one (which is fine but not ideal)
     {
          file_path.assign(argv[1]);                   // get CL arg and assign it to file_path
          std::cout << "\n\nRetrieved CL arg: \n"      // report file path retrieved
                    << file_path 
                    << std::endl;

          
          /* A bitmask value of member type iostate formed by a combination of error 
          state flag bits to be set (badbit, eofbit and/or failbit), or set to goodbit (or zero).*/
          in_stream.exceptions(std::ifstream::failbit);

          // ** Try Loading File from path provided by argv **
          try
          {
               in_stream.open(file_path, std::ifstream::in);              // opening the source file
                              
          }
          catch (const std::ifstream::failure& e)                         // catch paramter same as bitmask
          {
               std::ostringstream msg;
               msg << "\n\n**ERROR** \n\nOpening file '" << file_path
                    << "' failed, it either doesn't exist or is not accessible.\n\n";
               std::cout << msg.str();
               return 1;
          }
          // *************End of Try/Catch ****************


         
         // string(InputIterator first, InputIterator last);          // Overloaded std::String ctor
          std::string file_text((                                     
               std::istreambuf_iterator<char>(in_stream)),            // First
               std::istreambuf_iterator<char>());                     // Last




          std::cout << "\n\nFile Contains: \n\t" << file_text << std::endl; // File Content Report (for debug only)

          
     }
     
     std::cin.get();
     return 0;
}
