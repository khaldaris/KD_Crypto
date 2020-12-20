#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>
class ReadFileError : public std::exception
{
    const char * what() const throw()
    {
        return "Read file error.";
    }
};
class WriteFileError : public std::exception
{
    const char * what() const throw()
    {
        return "WriteFileError";
    }
};

class EncryptoError : public std::exception
{
    const char * what() const throw()
    {
        return "EncryptoError";
    }
};
class DecryptoError : public std::exception
{
    const char * what() const throw()
    {
        return "DecryptoError";
    }
};
class VerifyEncryptoError : public std::exception
{
    const char * what() const throw()
    {
        return "VerifyEncryptoError";
    }
};
class VerifyPlainError : public std::exception
{
    const char * what() const throw()
    {
        return "VerifyPlainError";
    }
};
#endif // EXCEPTIONS_H
