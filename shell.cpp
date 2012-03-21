/**
Copyright (C) 2012, Edward Chernysh
All rights reserved.

This file is part of Zenon. 
Independent Interactive Zenon Shell for Windows.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the author nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <EDWARD CHERNYSH> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**/

/* NOTICE: The binary will be installed by Zenon_Windows_Installer.*/

// <include headers="required"> :P
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <fstream>
#include <new>
#include <stdio.h> // for FILENAME_MAX
#include <vector>

#ifdef _WIN32
       #include <windows.h>
       #include <direct.h>
#elif __linux__ || __APPLE__
       #include <unistd.h>
#endif
// </include>

#define GetCurrentDir _getcwd // define  external function

char cpath[FILENAME_MAX];

using namespace std;     // std namespace

typedef unsigned int universal;

int main(int argc, char *argv[])
{
     string str, holder;        // initialize strings
     int iln ,np, *ip;          // initialize integers (one pointer)   
     static bool iAlloc = false;// static boolean, I want it only ONCE.
     vector<string> lstring(1);
     
     string::size_type index,cindex, sindex; // special integers...
     
     std::cout << "Independent Interactive Zenon Shell (IIZShell) : [Version 1.0.0.0]\nCopyright (C) 2012\n" << endl;
     std::cout << "For more info use: 'zenon_info:'" << endl;
     for (;;)    // Forever ; keep the shell running
     {
         std::cout << ">>>";
         getline(cin, holder);  // Store input in holder
         
         // <check_for_tokens>
         index = holder.find("int[]:", 0 );
         cindex = holder.find("char[]:",0);
         sindex = holder.find("string:", 0);
         // </check_for_tokens>
         
         if (holder == ";" || holder == "exit") 
            break;  // Close the Shell.
         if (holder.size() < 2) // check for a valid command 
            continue;      // BACK TO GO! :D
         else if (holder.find("script:",0) != (-1))
         {
              // <externscript>
              holder.replace(holder.find("script:",0), 7,"");
              string filename;
              for (int buffer=1; buffer<holder.size();buffer++)
                  filename+=holder[buffer];
              string _path;
              #ifdef _WIN32
                     _path = "zenon.exe -f ";              
              #endif
              
              _path += filename;
              system(_path.c_str());
              // </externscript>
         }
         else if (sindex != (-1))
         {
              if (holder.find("input", 0) != (-1))
                 getline(cin, lstring[0]);  
              else   
              {                    
                   char pchar[holder.size() + 1];
                   std::copy(holder.begin() + strlen("string:") + 1, holder.end(), pchar);
                   pchar[holder.size()] = '\0'; 
                   
                   string _data;
                   
                   for (int i = 0; i < sizeof(pchar); i++)
                             _data+=pchar[i];
                   lstring[0] = _data; 
              }
         }
         else if (holder.find("[string]", 0) != (-1))         
              holder.replace(holder.find("[string]", 0), 8 , lstring[0]);        
         else
         {
            /**
               Execute line,
               this line may have been modified by the Shell
               (if tokens were present).
            **/ 
            if (!GetCurrentDir(cpath, sizeof(cpath) / sizeof(TCHAR)))           
               return errno;   // check...
            cpath[sizeof(cpath) - 1] = '\0'; /* not 'needed' */ 
            str = cpath;
            #ifdef _WIN32
                   str += "/zenon.exe ";
            #elif __linux__
                   str += "./zenon.out ";
            #elif __APPLE__
                   str += "./zenon.out ";
            #endif
            str += holder;        
                         // Move characters from str -> *c[]         
            system(str.c_str());            
            str.clear();        
         
         }
     }     
     return 0;
}
