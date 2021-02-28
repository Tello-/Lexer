// Author: Josh Lollis, Victor Georgescu, Leonel Vazquez
// Created 2/11/2021
// Last Edited: 2/22/2021
// Course: Compilers 323
// Professor: Anthony Le


/* The purpose of this piece of code is to provide a working iteration. We have accounted
for most situations in our RE, but there are two fail cases, which are included in the
fail_case.txt. The issue here involves writing invalid characters to the output file.
For our final draft we will have an FSM incorporated into the design as well as correct
current bugs. Proper documentation will also be provided. */


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
using namespace std;


void lexer_analysis(int& curr_Position, string file_String, ofstream& write_to_File);

int main(int argc, char** argv)
{
     int cp = 0;    // intialze current position 

     /* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        :::::::::::::::::::::::::::::::::::::::::FILE LOADING::::::::::::::::::::::::::::::::::::::::::::::::::::::
        :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
     std::string   file_path;
     std::ifstream in_stream;
     std::ofstream of_stream;

     if (argc > 1) // this can pass even when the filepath is not a good one (which is fine but not ideal)
     {
          file_path.assign(argv[1]);                   // get CL arg and assign it to file_path

          /* A bitmask value of member type iostate formed by a combination of error
          state flag bits to be set (badbit, eofbit and/or failbit), or set to goodbit (or zero).*/
          in_stream.exceptions(std::ifstream::failbit);


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
          // *************End of Try/Catch ****************



         // string(InputIterator first, InputIterator last);          // Overloaded std::String ctor
          std::string file_text((
               std::istreambuf_iterator<char>(in_stream)),            // First
               std::istreambuf_iterator<char>());                     // Last

          of_stream.open("output.txt");       // open a file to write in

         // check for error
          if (!of_stream.is_open())
          {
               std::cout << "Error opening file";
               exit(1);
          }

          /* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
             :::::::::::::::::::::::::::::::::::::::::END OF FILE LOADING:::::::::::::::::::::::::::::::::::::::::::::::
             :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/


          of_stream << "TOKEN\t\t\tLEXEMES" << std::endl << std::endl;                // write to file

          // loop through entire string of text   
          while (cp < file_text.length())
          {
               lexer_analysis(cp, file_text, of_stream);
          }
     }

     of_stream.close();                 // close file
     std::cin.get();                    // Keeps program from terminating (debug)
     return 0;
}

void lexer_analysis(int& curr_Position, string file_String, ofstream& write_to_File)
{
     /* Finite State Machine Table Representation

     curr state |  alpha  |  digit  |    .    |    _    |    $    | delimit |gen input|  Backup?
     ___->0_____|____1____|____3____|____7____|____7____|____7____|____7____|____7____|
     _____1_____|____1____|____1____|____2____|____1____|____1____|____2____|____7____|
     ____(2)____|____0____|____0____|____0____|____0____|____0____|____0____|____0____|    Y
     _____3_____|____4____|____3____|____5____|____7____|____7____|____4____|____7____|
     ____(4)____|____0____|____0____|____0____|____0____|____0____|____0____|____0____|    Y
     _____5_____|____6____|____5____|____6____|____7____|____7____|____6____|____7____|
     ____(6)____|____0____|____0____|____0____|____0____|____0____|____0____|____0____|    Y
     ____(7)____|____0____|____0____|____0____|____0____|____0____|____0____|____0____|    Y

               0     Starting state          4     End integer
               1     In identifier           5     In real/float
               2     End identifier          6     End real/float
               3     In integer              7     General punctuation
     */
     int state[8][7] = {
             {1, 3, 7, 7, 7, 7, 7},
             {1, 1, 2, 1, 1, 2, 7},
             {0, 0, 0, 0, 0, 0, 0},
             {4, 3, 5, 7, 7, 4, 7},
             {0, 0, 0, 0, 0, 0, 0},
             {6, 5, 6, 7, 7, 6, 7},
             {0, 0, 0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0, 0, 0}
     };              //initialize state table
     int intstate, realstate;                      // character pointer
     static std::string token, prevtoken;                            // character string recognized token and previous token (if necessary)


     std::regex id("[a-zA-Z][a-zA-Z0-9_$]*");
     std::regex keyword("int|float|bool|True|False|if|else|then|endif|endelse|while|whileend|do|enddo|for|endfor|STDinput|STDoutput|and|or|not", std::regex::icase);
     std::regex intnum("[\\+|\\-]?[0-9]+");
     std::regex realnum("[\\+|\\-]?([0-9]+([.][0-9]*)?|[.][0-9]+)");
     std::regex sep("[\\(|\\)|{|}|\\[|\\]|,|.|:|;]");
     std::regex oper("([*|\\+|\\-|=|/|>|<|%]=*)|\\+\\+|\\-\\-");
     std::regex operillegal("([*|\\+|\\-|=|/|>|<|%][*|/|>|<|%])|([*|\\+|=|/|>|<|%][\\-|*|/|>|<|%])|([*|\\-|=|/|>|<|%][\\+|*|/|>|<|%])");
     std::regex delimiter("[\\s|\\0|\\(|\\)|{|}|\\[|\\]|,|.|:|;|*|\\+|\\-|=|/|>|<|%]");

     // move/append current string into token
     token += file_String[curr_Position];

     // check to see if the current token is a operator
     if (regex_match(token, oper))
     {
          // loop if it is a operator and the file is not at the end
          while (regex_match(token, oper) && curr_Position < file_String.length())
          {
               curr_Position++;   // increment current position 
               token += file_String[curr_Position]; // update/append token to new character
          }
          // check to see if current token is a integer and the previous is a seperator or whether the previous token was a space or operator
          if (regex_match(token, intnum) && (regex_match(prevtoken, sep) || prevtoken == "" || regex_match(prevtoken, oper)))
          {
               realstate = 0; // set real state to false

               // loop while token is a integer and we are not at the end of the file
               while (regex_match(token, intnum) && curr_Position < file_String.length())
               {
                    curr_Position++;   // increment current position
                    token += file_String[curr_Position]; // update/append current character to token
               }

               // check to see if current position is a period
               if (file_String[curr_Position] == '.')
               {
                    realstate = 1;  // set realstate to true

                    // loop while token is a real number and it is not at the end of the file
                    while (regex_match(token, realnum) && curr_Position < file_String.length())
                    {
                         curr_Position++;   // increment current position
                         token += file_String[curr_Position]; // update/append token to next character
                    }
               }
               token.pop_back();       // move back a spot in our token string
               std::string temp;       // create new string
               temp += file_String[curr_Position];  // move the current string into a temporary string 
               curr_Position--;                   // decrement current location

               // check to see if the temporary string has a delimeter
               if (regex_match(temp, delimiter))
               {
                    // check to see if the realstate flag was set off
                    if (realstate == 1)
                    {
                         // write to file
                         write_to_File << "REAL\t\t=\t" << token << std::endl;
                    }
                    else
                    {
                         // write to file
                         write_to_File << "INTEGER\t\t=\t" << token << std::endl;
                    }
               }
          }

          // check to see if token is a integer and its previous token is also an integer or if previous number is a real number
          else if ((regex_match(token, intnum) && regex_match(prevtoken, intnum)) || (regex_match(prevtoken, realnum)))
          {
               token.pop_back();                                            // pop the current character from token
               curr_Position--;                                            // decrement location
               write_to_File << "OPERATOR\t=\t" << token << std::endl;     // write to file   
          }

          // check to see that there are no illegal operators
          else if ((!regex_match(token, operillegal)) && (isalpha(prevtoken[0])) && (isalpha(prevtoken[1])) || ((prevtoken != "") && (prevtoken.length() < 2))) // CHANGE
          {
               token.pop_back();                                           // pop current token position
               curr_Position--;                                            // decrement location
               write_to_File << "OPERATOR\t=\t" << token << std::endl;     // write to file
          }
          prevtoken = token;  // place current token as a previous token
     }

     // check to see if it is a separator
     else if (regex_match(token, sep))
     {
          write_to_File << "SEPARATOR\t=\t" << token << std::endl;        // write to file
          prevtoken = token;                                          // place current token as a previous token
     }

     // check to see if it is an identifier, integer, or real
     else if (regex_match(token, id) || regex_match(token, intnum))
     {
          int col, lexState = 0, exitFlag = 0;                                      // initialize column, initial lexer state, and exit flag

          while (curr_Position <= file_String.length())
          {
               if (isalpha(file_String[curr_Position])) {
                    col = 0;
               }
               else if (isdigit(file_String[curr_Position])) {
                    col = 1;
               }
               else if (file_String[curr_Position] == '.') {
                    col = 2;
               }
               else if (file_String[curr_Position] == '_') {
                    col = 3;
               }
               else if (file_String[curr_Position] == '$') {
                    col = 4;
               }
               else {
                    std::string temp;
                    temp += file_String[curr_Position];
                    if (regex_match(temp, delimiter)) {
                         col = 5;
                    }
                    else {
                         col = 6;
                    }
               }
               lexState = state[lexState][col];    // record Lexer state

               // determine the next course of action based on table
               switch (lexState)
               {
               case 2:
                    token.pop_back();          // pop the current token
                    // check to see if the token is a keyword
                    if (regex_match(token, keyword))
                    {
                         write_to_File << "KEYWORD\t\t=\t" << token << std::endl;    // write to file
                    }
                    else
                    {
                         write_to_File << "IDENTIFIER\t=\t" << token << std::endl;   // write to file
                    }
                    exitFlag = 1;
                    break;

               case 4:
                    token.pop_back();          // pop the current token
                    write_to_File << "INTEGER\t\t=\t" << token << std::endl; // write to file
                    exitFlag = 1;
                    break;

               case 6:
                    token.pop_back();          // pop the current token
                    write_to_File << "REAL\t\t=\t" << token << std::endl;   // write to file
                    exitFlag = 1;
                    break;

               case 7:
                    exitFlag = 2;
                    break;
               }
               if (exitFlag == 1) {
                    curr_Position--;                     // decrement location
                    prevtoken = token;                   // move the current position into the previous string variable
                    break;
               }
               else if (exitFlag == 2) {
                    curr_Position--;
                    break;
               }
               curr_Position++;                       // increment positon
               token += file_String[curr_Position];     // update/append token
          }
     }

     // check to see if it is a comment block
     else if (token == "!")
     {
          int temp = curr_Position;     // create temporary location

          // loop while we don't reach the end of the string
          while (curr_Position < file_String.length())
          {
               curr_Position++;   // increment position

               // check to see if character is present
               if (file_String[curr_Position] == '!')
               {
                    break;
               }
          }

          // check to see if end of file is reached
          if (curr_Position > file_String.length())
          {
               curr_Position = temp;  // adjust position
               write_to_File << "SPECIAL CHAR\t=\t!" << std::endl; // write to file
          }
     }

     token = ""; // make current character space

     curr_Position++;       // increment location
}
