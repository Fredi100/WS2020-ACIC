#pragma once

class CustomString
{
private:
    char *data;

    // Size of CustomString must be a positiv integer
    unsigned int size = 0;

public:
    CustomString(const char *); // Only takes const char array to prevent changing the given data
    ~CustomString();

    CustomString *Concatenate(CustomString *);

    const char *c_str() { return data; };

    const unsigned int GetLength() { return size; };
};