#include <iostream>
#include "CustomString.hpp"

int main(int argc, char const *argv[])
{
    char data[6] = "Hello";

    CustomString *string1 = new CustomString(data);    // Test with variable
    CustomString *string2 = new CustomString("World"); // Test with inline string

    // Testing whether or not any changes to data appear inside of string1
    data[1] = 'I';

    std::cout << "Str1: " << string1->c_str() << std::endl;
    std::cout << "Str2: " << string2->c_str() << std::endl;

    CustomString *string3 = string1->Concatenate(string2);

    std::cout << "Str3: " << string3->c_str() << std::endl;

    // This should not be done as the previous string3 is no more accessible afterwards which leads to a memory leak
    //string3 = string3->Concatenate(string1);
    CustomString *string4 = string3->Concatenate(string1);

    std::cout << "Str3: " << string4->c_str() << std::endl;

    delete string1;
    delete string2;
    delete string3;
    delete string4;

    return 0;
}
