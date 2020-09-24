#pragma once

class CustomString
{
private:
    char *data;

    // Size of CustomString must be a positiv integer
    unsigned int size = 0;

public:
    CustomString(const char *); // Only takes const char array to prevent changing the given data
    /*
    Custom destructor is currently not necessary
    as all member variables get automatically destroyed by the default destructor.
    */

    CustomString* Concatenate(CustomString *);

    const char *c_str() { return data; };

    const unsigned int GetLength() { return size; };
};