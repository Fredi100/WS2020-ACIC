#include <iostream>
#include "CustomString.hpp"

int main(int argc, char const *argv[])
{
    CustomString *string1 = new CustomString("Hello");
    CustomString *string2 = new CustomString("World");

    std::cout << "Str1: " << string1->c_str() << std::endl;
    std::cout << "Str2: " << string2->c_str() << std::endl;

    CustomString *string3 = string1->Concatenate(string2);

    std::cout << "Str3: " << string3->c_str() << std::endl;

    string3 = string3->Concatenate(string1);

    std::cout << "Str3: " << string3->c_str() << std::endl;

    delete string1;
    delete string2;

    return 0;
}
