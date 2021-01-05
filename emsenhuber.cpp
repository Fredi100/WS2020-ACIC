#include <assert.h>
#include <iostream>
#include <memory>
#include <functional>


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

    struct Deleter {
        void operator()(CustomString* ptr) {
            std::cout << "Deleting CustomString" << std::endl;
            delete ptr;
        }
    };

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

        bool operator==(const Iterator& other) const {
            return this->pos == other.pos; // Comparing both pointers in order to determing if both iterators are the same
        }

        bool operator!=(const Iterator& other) const {
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
        // Create new array for combined strings
        char* newData = new char[size + other.size + 1];

        // Copy content from old data into newData
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }

        // Copy content from other data into newData
        for (int i = 0; i < other.size; i++) {
            newData[size + i] = other.data[i];
        }

        // Terminate newData
        newData[size + other.size + 1] = '\0';

        // Increase size
        size += other.size;

        // Delete old data to replace with new one
        delete[] data;
        data = newData;

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

    bool operator==(const CustomString& other) const {
        if (size != other.size) return false;
        for (int i = 0; i < size; i++) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }

    bool operator==(const char* str) const {
        return this->operator==(CustomString(str));
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
        Would prefer creating concatString on the stack, as it is only relevant for the scope of
        this function. But MSVC does not allow creating arrays with variable length on the stack.
        Therefore it is necessary to create the char array on the heap
        */
        char* concatString = new char[size + other.size + 1];

        /*
        Iterate through the old array and copy the content into the new array
        */
        for (unsigned int i = 0; i < size; i++)
            concatString[i] = data[i];

        /*
        Iterate through the other array and copy the content into the new array
        shifted by the size of the old content
        */
        for (unsigned int i = 0; i < other.size; i++)
            concatString[size + i] = other.data[i];

        // Lastly insert a null terminator to signify the end of this string
        concatString[size + other.size] = '\0';

        CustomString newCustomString = CustomString(concatString);
        delete[] concatString;

        return newCustomString;
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

template <class T, class D = std::default_delete<T>>
class UniquePtr {
private:
    T* obj;
    D del;
public:

    UniquePtr() { };

    UniquePtr(T* object) : obj(object) { }

    UniquePtr(T* object, const D& deleter) : obj(object), del(deleter) { }

    // Move Constructor
    // Implemented (6) as defined in http://www.cplusplus.com/reference/memory/unique_ptr/unique_ptr/
    UniquePtr(UniquePtr&& other) : UniquePtr(other.Release()) { }

    // Copy Constructor marked as a deleted function in order to prevent copying
    // Deleted (9) as defined in http://www.cplusplus.com/reference/memory/unique_ptr/unique_ptr/
    UniquePtr(const UniquePtr& other) = delete;

    ~UniquePtr() {
        Reset();
    }

    // Move Assignment operator
    // Implemented as defined in http://www.cplusplus.com/reference/memory/unique_ptr/operator=/
    UniquePtr& operator=(UniquePtr&& other) {
        Reset(other.Release());
        return *this;
    }

    // Copy Assignment operator marked as deleted function in order to prevent copying
    // Deleted (4) as defined in http://www.cplusplus.com/reference/memory/unique_ptr/operator=/
    UniquePtr& operator=(const UniquePtr&) = delete;

    T* operator->() const {
        return obj;
    }

    T operator*() const {
        return *obj;
    }

    // Returns true if the UniquePtr is currently managing an object and false if not
    explicit operator bool() const {
        return (this->obj != nullptr);
    }

    // Returns the currently managed object and stops managing it by setting the pointer to null
    T* Release() {
        T* temp = Get();
        this->obj = nullptr;
        return temp;
    }

    // Deletes the currently managed object and replaces it with other
    void Reset(T* other = nullptr) {
        std::swap(other, obj); // Swapping both pointers so no temp pointer is needed
        if(other != nullptr)
            del(other);
    }

    void Swap(UniquePtr& other) {
        T* temp = obj;
        obj = other.obj;
        other.obj = temp;
    }

    T* Get() {
        return obj;
    }
};

/*#### Tests ####*/
/* String Tests */
static void testConstructor1() { // Tests the regular CustomString Constructor
    std::cout << "Testing CustomString Constructor1..." << std::endl;
    CustomString string1 = CustomString("Foo");
    assert(string1 == CustomString("Foo"));
}

static void testConstructor2() { // Test whether or not it is possible to edit the data inside a CustromString when keeping the reference to the original char[]
    std::cout << "Testing CustomString Constructor2..." << std::endl;
    char data[] = "Bar";
    CustomString string1 = CustomString(data);
    // Testing whether or not any changes to data appear inside of string1
    data[1] = 'I';
    assert(string1 == CustomString("Bar"));
}

static void testConcatenate() {
    std::cout << "Testing CustomString Concetenate..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString string2 = CustomString("Bar");
    CustomString stringConcat = string1.Concatenate(string2);
    assert(stringConcat == CustomString("FooBar"));
    // When working with pointers, following should not be done as the previous string2 is no more accessible
    // afterwards which leads to a memory leak
    // string2 = string2->Concatenate(string1);
}

static void testLength() {
    std::cout << "Testing CustomString GetLength..." << std::endl;
    CustomString string1 = CustomString("Foo");
    assert(string1.GetLength() == 3);
}

static void testCopyConstructor() {
    std::cout << "Testing CustomString Copy Constructor..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString stringCopy = CustomString(string1);
    assert(string1 == stringCopy);
}

static void testCopyAssignOperator() {
    std::cout << "Testing CustomString Copy Assign Operator..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString string2 = CustomString("Bar");
    string1 = string2;
    assert(string1 == string2);
}

static void testAddEquals1Operator() {
    std::cout << "Testing CustomString Add Equals Operator with two CustomStrings..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString string2 = CustomString("Bar");
    string1 += string2;
    assert(string1 == CustomString("FooBar"));
}

static void testAddEquals2Operator() {
    std::cout << "Testing CustomString Add Equals Operator with one CustomString and one char array..." << std::endl;
    CustomString string1 = CustomString("Foo");
    string1 += (const char*)"Bar";
    assert(string1 == CustomString("FooBar"));
}

static void testAddOperator1() {
    std::cout << "Testing CustomString Add Operator with two CustomStrings..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString string2 = CustomString("Bar");
    CustomString stringAdd = string1 + string2;
    assert(stringAdd == CustomString("FooBar"));
}

static void testAddOperator2() {
    std::cout << "Testing CustomString Add Operator with one CustomString and one char array..." << std::endl;
    CustomString string1 = CustomString("Foo");
    CustomString stringAdd = string1 + (const char*)"Bar";
    assert(stringAdd == CustomString("FooBar"));
}

static void testConversionFunctionString() {
    std::cout << "Testing CustomString Char Array Conversion..." << std::endl;
    CustomString string = CustomString("Foo");
    const char* test = string;
    assert(string == CustomString(test));
}

static void testConversionFunctionInt() {
    std::cout << "Testing CustomString Int Conversion..." << std::endl;
    CustomString string = CustomString("Foo");
    assert((int)string == 3);
}

/* Iterator Tests */
static void testIteratorPost() {
    std::cout << "Testing CustomString Post Iterator..." << std::endl;
    CustomString string = CustomString("Post Iterator!");
    const char* data = string.c_str();
    int i = 0;

    for (CustomString::Iterator it = string.begin(); it != string.end(); ++it) {
        assert(*it == data[i]);
        i++;
    }
}

static void testIteratorPre() {
    std::cout << "Testing CustomString Pre Iterator..." << std::endl;
    CustomString string = CustomString("Post Iterator!");
    const char* data = string.c_str();
    int i = 0;

    for (CustomString::Iterator it = string.begin(); it != string.end(); it++) {
        assert(*it == data[i]);
        i++;
    }
}

/* Unique Pointer Tests */
static void testPointerAsterisk() {
    std::cout << "Testing CustomPointer Asterisk Operator..." << std::endl;
    UniquePtr<CustomString> pointer(new CustomString("Foo"));
    assert((*pointer) == "Foo");
}

static void testPointerArrow() {
    std::cout << "Testing CustomPointer Arrow Operator..." << std::endl;
    UniquePtr<CustomString> pointer(new CustomString("Foo"));
    assert(pointer->GetLength() == 3);
}

static void testPointerBool() {
    std::cout << "Testing CustomPointer Bool Conversion..." << std::endl;
    UniquePtr<CustomString> correct(new CustomString("Foo"));
    assert(correct);
    UniquePtr<CustomString> wrong;
    assert(!wrong);
}

static void testPointerMoveConstructor() {
    std::cout << "Testing CustomPointer Move Constructor..." << std::endl;
    UniquePtr<CustomString> pointer(new CustomString("Foo"));
    UniquePtr<CustomString> movePointer(std::move(pointer));

    assert(!pointer);
    assert((*movePointer) == "Foo");
}

static void testPointerMoverAssignOperator(){
    std::cout << "Testing CustomPointer Move Assign Operator..." << std::endl;
    UniquePtr<CustomString> pointer(new CustomString("Foo"));
    UniquePtr<CustomString> movePointer = std::move(pointer);

    assert(!pointer);
    assert((*movePointer) == "Foo");
}

static void testPointerResetEmpty(){
    std::cout << "Testing CustomPointer Reset with no Parameter..." << std::endl;
    UniquePtr<CustomString> pointer(new CustomString("Foo"));
    pointer.Reset();

    assert(!pointer);
}

static void testPointerResetFilled(){
    std::cout << "Testing CustomPointer Reset with Parameter..." << std::endl;
    UniquePtr<CustomString> pointer(new CustomString("Foo"));
    pointer.Reset(new CustomString("Bar"));

    assert((*pointer) == "Bar");
}

static void testPointerSwap(){
    std::cout << "Testing CustomPointer Swap..." << std::endl;
    UniquePtr<CustomString> pointer1(new CustomString("Foo"));
    UniquePtr<CustomString> pointer2(new CustomString("Bar"));

    pointer1.Swap(pointer2);

    assert((*pointer1) = "Bar");
    assert((*pointer2) = "Foo");
}

static void testPointerDeleter() {
    std::cout << "Testing CustomPointer Deleter..." << std::endl;
    UniquePtr<CustomString, CustomString::Deleter> pointer(new CustomString("Foo"));
}


/* Main */

int main(int argc, char const* argv[]) {

    // String Tests
    testConstructor1();
    testConstructor2();
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

    // Iterator Tests
    testIteratorPost();
    testIteratorPre();

    // Pointer Tests
    
    testPointerAsterisk();
    testPointerArrow();
    testPointerBool();
    testPointerMoveConstructor();
    testPointerMoverAssignOperator();
    testPointerResetEmpty();
    testPointerResetFilled();
    testPointerSwap();
    testPointerDeleter();
    return 0;
}