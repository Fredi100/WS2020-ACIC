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
            data = new char[other.size+1]; // Create new char array on heap for new size
            for (unsigned int i = 0; i < other.size; i++) { // Fill array with data from other object
                data[i] = other.data[i];
            }
            data[other.size + 1] = '\0'; // Don't forget to terminate the string
        }

        return *this;
    }

    CustomString& operator+=(const CustomString& other){
        // Copy content from data in a temp variable
        char temp[size + 1];
        for(int i = 0; i < size; i++){
            temp[i] = data[i];
        }
        temp[size + 1] = '\0';

        delete[] data; // Delete old data to replace with new one
        data = new char[size + other.size + 1];

        // Fill data with new content
        for(int i = 0; i < size; i++){
            data[i] = temp[i];
        }
        for(int i = 0; i < other.size; i++){
            data[size+i] = other.data[i];
        }
        data[size+other.size+1] = '\0';

        size += other.size;

        return *this;
    }

    CustomString& operator+=(const char* str){
        return *this += CustomString(str);
    }

    CustomString operator+(const CustomString& other){
        return this->Concatenate(other);
    }

    CustomString operator+(const char* str){
        return this->Concatenate(CustomString(str));
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
    CustomString Concatenate(const CustomString& other) // Can not be const as method calls would not work
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

    const char* c_str() const { return data; };

    const unsigned int GetLength() const { return size; };
};

/*#### Tests ####*/

static void testConstructor() {
    std::cout << "Testing Constructor..." << std::endl;
    CustomString string1 = CustomString("Foo");
    std::cout << "String 1: " << string1.c_str() << " | Should be Foo" << std::endl;
    char data[] = "Bar";
    CustomString string2 = CustomString(data);
    // Testing whether or not any changes to data appear inside of string1
    data[1] = 'I';
    std::cout << "String 2: " << string2.c_str() << " | Should be Bar" << std::endl;
}

static void testCStr() {
    std::cout << "Testing c_str..." << std::endl;
    CustomString string1 = CustomString("Foo");
    std::cout << "String 1: " << string1.c_str() << " | Should be Foo" << std::endl;
}

static void testConcatenate()
{
    std::cout << "Testing Concatenate..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString string2 = CustomString("Bar");
    std::cout << "String 1: " << string1.c_str() << std::endl;
    std::cout << "String 2: " << string2.c_str() << std::endl;
    CustomString stringConcat = string1.Concatenate(string2);
    std::cout << "String Concatenate: " << stringConcat.c_str() << " | Should be FooBar" << std::endl;
    // When working with pointers, following should not be done as the previous string2 is no more accessible
    // afterwards which leads to a memory leak
    // string2 = string2->Concatenate(string1);
}

static void testLength() {
    std::cout << "Testing Length..." << std::endl;
    CustomString string1 = CustomString("Foo");
    std::cout << "String 1 Length: " << string1.GetLength() << " | Should be: 3" << std::endl;
}

static void testCopyConstructor()
{
    std::cout << "Testing Copy Constructor..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString stringCopy = CustomString(string1);
    std::cout << "String 1: " << string1.c_str() << std::endl;
    std::cout << "String Copy: " << stringCopy.c_str() << " | Should be Foo" << std::endl;
}

static void testCopyAssignOperator()
{
    std::cout << "Testing Copy Assign Operator..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString string2 = CustomString("Bar");
    std::cout << "String 1 before: " << string1.c_str() << std::endl;
    std::cout << "String 2: " << string2.c_str() << std::endl;
    string1 = string2;
    std::cout << "String 1 after: " << string1.c_str() << " | Should be Bar" << std::endl;
}

static void testAddEquals1Operator(){
    std::cout << "Testing Add Equals Operator 1..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString string2 = CustomString("Bar");
    std::cout << "String 1: " << string1.c_str() << std::endl;
    std::cout << "String 2: " << string2.c_str() << std::endl;
    string1 += string2;
    std::cout << "String 1 Add: " << string1.c_str() << " | Should be FooBar" << std::endl;
}

static void testAddEquals2Operator(){
    std::cout << "Testing Add Equals Operator 2..." << std::endl;
    CustomString string1 = CustomString("Foo");
    std::cout << "String 1: " << string1.c_str() << std::endl;
    string1 += (const char*) "Bar";
    std::cout << "String 1 Add: " << string1.c_str() << " | Should be FooBar" << std::endl;
}

static void testAddOperator1(){
    std::cout << "Testing Add Operator 1..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString string2 = CustomString("Bar");
    CustomString stringAdd = string1 + string2;
    std::cout << "String 1: " << string1.c_str() << std::endl;
    std::cout << "String 2: " << string2.c_str() << std::endl;
    std::cout << "String Add: " << stringAdd.c_str() << " | Should be FooBar" << std::endl;
}

static void testAddOperator2(){
    std::cout << "Testing Add Operator 2..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString stringAdd = string1 + (const char*) "Bar";
    std::cout << "String 1: " << string1.c_str() << std::endl;
    std::cout << "String 2: Bar" << std::endl;
    std::cout << "String Add: " << stringAdd.c_str() << " | Should be FooBar" << std::endl;
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
    testAddEquals1Operator();
    testAddEquals2Operator();
    testAddOperator1();
    testAddOperator2();

    return 0;
}