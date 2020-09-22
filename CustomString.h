#ifndef CUSTOM_STRING
#define CUSTOM_STRING

#include <string>

class CustomString
{
private:
    size_t length;
    char* data;

public:
    CustomString(){};
    CustomString(const char *);
    CustomString(const std::string *);
    ~CustomString();

    concatenate(const char *);
    concatenate(const CustomString *);
    concatenate(const std::string *);
}

#endif