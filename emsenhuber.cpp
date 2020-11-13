#pragma once

#include <iostream>

class CustomString
{
private:
    char* data;

    // Size of CustomString must be a positiv integer
    unsigned int size = 0;

    /**
     * Iterates through the given char array until a termination character is
     * found. Increases the counter by one for each iteration.
     */
    const unsigned int arrLen(const char* string)
    {
        unsigned int size = 0;
        while (string[size] != '\0')
            size++;
        return size;
    }

public:
    /**
     * Creates a new CustomString object by creating a new char array on the heap
     * and copying the content of the given initString.
     */
    CustomString(const char* initString) : size(arrLen(initString)) {
        /*
        data needs to be created on the heap,
        otherweise the array is not accessible outside of this function's scope.
        */
        data = new char[size + 1];

        /*
        Copying content from initString into data value by value instead of just
        passing a reference to the array. This is to prevent any changes from
        outside to change the value of data.
        */
        for (unsigned int i = 0; i < size; i++)
            data[i] = initString[i];

        data[size] = '\0';
    } // Only takes const char array to prevent changing the given data

    /**
     * Copy constructor creates a new object from the given one being identical to
     * the content but still being two completely seperate objects in the end
     */
    CustomString(const CustomString& obj) : CustomString(obj.data) {} // just taking the content of the given object and calling a fitting constructor for it

    /*
     * Copy assign operator changes the data of the given object instead of
     * replacing it
     */
    CustomString& operator=(const CustomString other)
    {
        if (&other != this) // Checking for self-assignment, skipping unnecessary calls if possible
        {
            delete[] data;               // Delete old data array from heap
            data = new char[other.size]; // Create new char array on heap for new size
            for (unsigned int i = 0; i < other.size; i++) { // Fill array with data from other object
                data[i] = other.data[i];
            }
            data[other.size + 1] = '\0'; // Don't forget to terminate the string
        }

        return *this;
    }

    /**
     * Because data was created on the heap, it needs to be manually deleted.
     * Otherwise we would get a memory leak
     */
    ~CustomString() { delete[] data; }

    /**
     * Creates a new CustomString object by first taking the content of the old
     * String, copying it into a new char array and then appending the content of
     * the other CustomString.
     */
    CustomString Concatenate(CustomString& other) // Can not be const as method calls would not work
    {
        /*
        Creating concatString on the stack, as it is only relevant for the scope of
        this function.
        */
        unsigned int otherLength = other.GetLength();

        char concatString[this->GetLength() + other.GetLength() + 1];

        /*
        Iterate through the old array and copy the content into the new array
        */
        for (unsigned int i = 0; i < this->GetLength(); i++)
            concatString[i] = this->data[i];

        /*
        Iterate through the other array and copy the content into the new array
        shifted by the size of the old content
        */
        for (unsigned int i = 0; i < other.GetLength(); i++)
            concatString[this->GetLength() + i] = other.c_str()[i];

        // Lastly insert a null terminator to signify the end of this string
        concatString[this->GetLength() + other.GetLength()] = '\0';

        return CustomString(concatString);
    }

    const char* c_str() { return data; };

    const unsigned int GetLength() { return size; };
};

/*#### Tests ####*/

static void testConstructor() {
    std::cout << "Testing Constructor..." << std::endl;
    CustomString string1 = CustomString("Foo");
    std::cout << "String 1: " << string1.c_str() << std::endl;
    char data[] = "Bar";
    CustomString string2 = CustomString(data);
    // Testing whether or not any changes to data appear inside of string1
    data[1] = 'I';
    std::cout << "String 2: " << string2.c_str() << std::endl;
}

static void testCStr() {
    std::cout << "Testing c_str..." << std::endl;
    CustomString string1 = CustomString("Foo");
    std::cout << "String 1: " << string1.c_str() << std::endl;
}

static void testConcatenate()
{
    std::cout << "Testing Concetenate..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString string2 = CustomString("Bar");
    std::cout << "String 1: " << string1.c_str() << std::endl;
    std::cout << "String 2: " << string2.c_str() << std::endl;
    CustomString stringConcat = string1.Concatenate(string2);
    std::cout << "String Concatenate: " << stringConcat.c_str() << std::endl;
    // When working with pointers, following should not be done as the previous string2 is no more accessible
    // afterwards which leads to a memory leak
    // string2 = string2->Concatenate(string1);
}

static void testLength() {
    std::cout << "Testing Length..." << std::endl;
    CustomString string1 = CustomString("Foo");
    std::cout << "String 1 Lenght: " << string1.GetLength() << " Should be: 3" << std::endl;
}

static void testCopyConstructor()
{
    std::cout << "Testing Copy Constructor..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString stringCopy = CustomString(string1);
    std::cout << "String 1: " << string1.c_str() << std::endl;
    std::cout << "String Copy: " << stringCopy.c_str() << std::endl;
}

static void testCopyAssignOperator()
{
    std::cout << "Testing Copy Assign Operator..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString string2 = CustomString("Bar");
    std::cout << "String 1 before: " << string1.c_str() << std::endl;
    std::cout << "String 2: " << string2.c_str() << std::endl;
    string1 = string2;
    std::cout << "String 1 after: " << string1.c_str() << std::endl;
}

/* Main */

int main(int argc, char const* argv[])
{
    testConstructor();
    testCStr();
    testConcatenate();
    testLength();
    testCopyConstructor();
    testCopyAssignOperator();

    return 0;
}