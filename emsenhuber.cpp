#pragma once

#include <iostream>

class CustomString
{
private:
    char *data;

    // Size of CustomString must be a positiv integer
    unsigned int size = 0;

    /**
     * Iterates through the given char array until a termination character is found.
     * Increases the counter by one for each iteration.
     */
    const unsigned int arrLen(const char *string)
    {
        unsigned int size = 0;
        while (string[size] != '\0')
            size++;
        return size;
    }

public:
    /**
     * Creates a new CustomString object by creating a new char array on the heap and copying the content of the given initString.
     */
    CustomString(const char *initString) : size(arrLen(initString))
    {
        /*
        data needs to be created on the heap,
        otherweise the array is not accessible outside of this function's scope.
        */
        data = new char[size + 1];

        /*
        Copying content from initString into data value by value instead of just passing a reference to the array.
        This is to prevent any changes from outside to change the value of data.
        */
        for (unsigned int i = 0; i < size; i++)
            data[i] = initString[i];

        data[size] = '\0';
    } // Only takes const char array to prevent changing the given data

    /**
     * Because data was created on the heap, it needs to be manually deleted. Otherwise we would get a memory leak
     */
    ~CustomString()
    {
        delete[] data;
    }

    /**
     * Creates a new CustomString object by first taking the content of the old String,
     * copying it into a new char array and then appending the content of the other CustomString.
     */
    CustomString *Concatenate(CustomString *other)
    {
        /*
        Creating concatString on the stack, as it is only relevant for the scope of this function.
        */
        char concatString[this->GetLength() + other->GetLength() + 1];

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

        return new CustomString(concatString);
    }

    const char *c_str() { return data; };

    const unsigned int GetLength() { return size; };
};

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
