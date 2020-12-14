#include <iostream>

class CustomString {
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
    class Iterator {
    private:
        const char* pos;

    public:
        Iterator(const char* pos) {
            this->pos = pos;
        }

        Iterator& operator++() {
            this->pos++;
            return *this;
        }

        /*
         * Post Increment returns the old value before incrementing it.
         * Therefore copying the old one is necessary before incrementing this
         */
        Iterator operator++(int) {
            Iterator old = Iterator(this->pos);
            this->pos++;
            return old;
        }

        char operator*() {
            return *this->pos; // Incrementing iterator just means incrementing the position of the pointer is has
        }

        bool operator==(const Iterator& other) const{
            return this->pos == other.pos; // Comparing both pointers in order to determing if both iterators are the same
        }

        bool operator!=(const Iterator& other) const{
            return !(*this == other); // Inverse of == operator
        }
    };


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
            data = new char[other.size + 1]; // Create new char array on heap for new size
            for (unsigned int i = 0; i < other.size; i++) { // Fill array with data from other object
                data[i] = other.data[i];
            }
            data[other.size + 1] = '\0'; // Don't forget to terminate the string
        }

        return *this;
    }

    CustomString& operator+=(const CustomString& other) {
        // Copy content from data in a temp variable
        char temp[size + 1];
        for (int i = 0; i < size; i++) {
            temp[i] = data[i];
        }
        temp[size + 1] = '\0';

        delete[] data; // Delete old data to replace with new one
        data = new char[size + other.size + 1];

        // Fill data with new content
        for (int i = 0; i < size; i++) {
            data[i] = temp[i];
        }
        for (int i = 0; i < other.size; i++) {
            data[size + i] = other.data[i];
        }
        data[size + other.size + 1] = '\0';

        size += other.size;

        return *this;
    }

    CustomString& operator+=(const char* str) {  // Just calling the above implementation for a const char
        return *this += CustomString(str);      // Not really necessary as the compiler is able to implicitly convert const char* into CustomString
    }

    CustomString operator+(const CustomString& other) { // Using Concatenate to get the extended string and just return it
        return this->Concatenate(other);
    }

    CustomString operator+(const char* str) { // Same with the above one just for const char*
        return this->Concatenate(CustomString(str)); // This overload is not really necessary as the compiler is able to implicitly cast every const char* in a CustomString
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

    operator const char* () const { // Just providing a way to cast CustomString into const char*
        return data;
    }

    operator int() const { // Same with the above one but for ints
        return size;
    }

    const char* c_str() const { return data; };

    const unsigned int GetLength() const { return size; };



    Iterator begin() {
        return Iterator(&data[0]); // TODO Check if new is necessary

    }

    Iterator end() {
        return Iterator(&data[size]); // TODO Check if size is the correct value. Might need to be one higher
    }
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

static void testAddEquals1Operator() {
    std::cout << "Testing Add Equals Operator 1..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString string2 = CustomString("Bar");
    std::cout << "String 1: " << string1.c_str() << std::endl;
    std::cout << "String 2: " << string2.c_str() << std::endl;
    string1 += string2;
    std::cout << "String 1 Add: " << string1.c_str() << " | Should be FooBar" << std::endl;
}

static void testAddEquals2Operator() {
    std::cout << "Testing Add Equals Operator 2..." << std::endl;
    CustomString string1 = CustomString("Foo");
    std::cout << "String 1: " << string1.c_str() << std::endl;
    string1 += (const char*)"Bar";
    std::cout << "String 1 Add: " << string1.c_str() << " | Should be FooBar" << std::endl;
}

static void testAddOperator1() {
    std::cout << "Testing Add Operator 1..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString string2 = CustomString("Bar");
    CustomString stringAdd = string1 + string2;
    std::cout << "String 1: " << string1.c_str() << std::endl;
    std::cout << "String 2: " << string2.c_str() << std::endl;
    std::cout << "String Add: " << stringAdd.c_str() << " | Should be FooBar" << std::endl;
}

static void testAddOperator2() {
    std::cout << "Testing Add Operator 2..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString stringAdd = string1 + (const char*)"Bar";
    std::cout << "String 1: " << string1.c_str() << std::endl;
    std::cout << "String 2: Bar" << std::endl;
    std::cout << "String Add: " << stringAdd.c_str() << " | Should be FooBar" << std::endl;
}

static void testConversionFunctionString() {
    std::cout << "Testing Conversion Function String..." << std::endl;
    CustomString string = CustomString("Foo");
    puts(string);
}

static void testConversionFunctionInt() {
    std::cout << "Testing Conversion Function Int..." << std::endl;
    CustomString string = CustomString("Foo");
    std::cout << "String Size: " << (int)string << " | Should be 3" << std::endl;
}

static void testIteratorPost() {
    CustomString string = CustomString("Post Iterator!");

    for (CustomString::Iterator it = string.begin(); it != string.end(); ++it){
        std::cout << *it << std::endl;
    }
}

static void testIteratorPre() {
    CustomString string = CustomString("Pre Iterator!");

    for (CustomString::Iterator it = string.begin(); it != string.end(); it++){
        std::cout << *it << std::endl;
    }
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
    testConversionFunctionString();
    testConversionFunctionInt();
    testIteratorPost();
    testIteratorPre();

    return 0;
}