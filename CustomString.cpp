#include "CustomString.h"

CustomString::CustomString(const char *initString)
{
    this->length = getSizeOfCharArray(initString);
    this->data = new char[length];
    this->data = initString;
}

int getSizeOfCharArray(const char *string)
{
    size_t size = 0;
    for (size = 0; string[size] != '\0'; size++);
    return size;
}

int main()
{
    CustomString* string = new CustomString("Hallo Liebe Leute");

}