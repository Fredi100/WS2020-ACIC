#include <iostream>
#include "CustomString.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    CustomString *string1 = new CustomString("Hello");
    CustomString *string2 = new CustomString("World");

    cout << "Str1: " << string1->c_str() << endl;
    cout << "Str2: " << string2->c_str() << endl;

    string1->Concatenate(string2);

    cout << "Str1: " << string1->c_str() << endl;

    string1->Concatenate(string1);

    cout << "Str1: " << string1->c_str() << "test" << endl;

    delete string1;
    delete string2;

    return 0;
}
