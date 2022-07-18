// graphemestring.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <fcntl.h>
#include <io.h>
#include <string>

#include <iostream>
#include "grapheme.h"



//*Sigh, I was forced to do my tests in a gui program because wcout stops working if you try to output any character to the console that isn't available on the default code page
//So this test doesnt work

#define DIS(n) std::cout << "variable " << #n <<" = " << n << std::endl;
//<<n<<"'\n"
//; for (auto a : n) std::wcout <<"'"<< a <<"'"<<'\n'



int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);


//    _setmode(_fileno(stdout), _O_U16TEXT);
    GraphemeString nye ( L"ñññññññ"),
        hindi ( L"अनुच्छेद"),
        emojis ( L"🌷🎁💩😜👍🏳️‍🌈"),
        diacritics ( L"Ĺo͂řȩm̅"),
        korean ( L"뎌쉐"),
        zalgo ( L"Z͑ͫ̓ͪ̂ͫ̽͏̴̙̤̞͉͚̯̞̠͍A̴̵̜̰͔ͫ͗͢L̠ͨͧͩ͘G̴̻͈͍͔̹̑͗̎̅͛́Ǫ̵̹̻̝̳͂̌̌͘!͖̬̰̙̗̿̋ͥͥ̂ͣ̐́́͜͞");

 
    DIS(emojis);
    DIS(nye);
    DIS(hindi);
   
    DIS(zalgo);
    DIS(korean);
    DIS(diacritics);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
