#include <iostream>
#include <string>
#include "Shannon_Fano.h"

int main()
{
    std::string input;                              // User input

    //std::cout << "Please enter the string -- ";
    //getline(std::cin, input);

    input = "There are only two ways to live your life. One is as though nothing is a miracle. The other is as though everything is a miracle.";

    Shannon_Fano* coder = nullptr;

    try
    {
        coder = new Shannon_Fano(input);

        std::cout << std::endl;
        coder->print_all();

        std::cout << "Let's take the code of the letter 'L' -- ";
        std::cout << coder->get_the_code('L');

        std::cout << "\nAll codes:" << std::endl;
        std::string* all_codes = coder->get_all_codes();
        for (size_t i = 0; i < coder->get_number_def_letter(); ++i)
            std::cout << all_codes[i] << std::endl;

        delete[] all_codes;
        delete coder;
    }
    catch (std::bad_alloc error)
    {
        std::cout << error.what() << std::endl;
        if (coder != nullptr)
            delete coder;
    }

    return 0;
}
