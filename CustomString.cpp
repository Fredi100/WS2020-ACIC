#include "CustomString.hpp"

/**
 * Iterates through the given char array until a termination character is found.
 * Increases the counter by one for each iteration.
 */
const unsigned int sizeOf(const char *string)
{
    unsigned int size = 0;
    while (string[size] != '\0')
        size++;
    return size;
}

/**
 * Creates a new CustomString object by creating a new char array and copying the content of the given initString.
 */
CustomString::CustomString(const char *initString) : size(sizeOf(initString))
{
    /*
    Create a new char array on the heap.
    Otherwise the array is not accessible outside of this function's scope
    */
    this->data = new char[size]; // TODO Is maybe missing the null terminator

    // Copying content from one array to the others
    for (unsigned int i = 0; i < size; i++)
        this->data[i] = initString[i];
}

/**
 * Extends the current CustomString object by first taking the old content,
 * copying it into a new char array and then appending the content of the other CustomString.
 */
void CustomString::Concatenate(CustomString *other)
{
    // concatString needs to be one char longer than both strings combined to accomodate the null terminator
    char *concatString = new char[this->GetLength() + other->GetLength() + 1];

    /*
    Iterate through the old array and copy the content into the new array
    */
    for (unsigned int i = 0; i < this->GetLength(); i++)
        concatString[i] = this->data[i];

    /*
    Iterate through the other array and copy the content into the new array
    shifted by the size of the old content
    */
    for (unsigned int i = 0; i < other->GetLength(); i++)
        concatString[this->GetLength() + i] = other->c_str()[i];

    // Lastly insert a null terminator to signify the end of this string
    concatString[this->GetLength() + other->GetLength()] = '\0';

    delete this->data; // Delete old data to free its memory
    this->data = concatString;
    this->size = sizeOf(concatString);
}