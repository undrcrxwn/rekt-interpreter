#pragma once
#include <exception>
#include <string>

namespace rekt
{
    class Exception : virtual public std::exception
    {
    public:
        Exception(std::string s = "") : message(s) {}
        virtual std::string What()              { return Name() + " ocurred: " + Description() + (message.empty() ? "" : " " + message); }
        virtual std::string Name()              { return "Unspecified rekt exception"; }
        virtual std::string Description()       { return "Something went wrong. Try again or contact the support."; }

    protected:
        std::string message;
    };

    class WrongSignatureException : public Exception
    {
    public:
        WrongSignatureException(std::string s = "") : Exception(s) {}
        std::string Name()        override      { return "Wrong signature exception"; }
        std::string Description() override      { return "The given parameters are not of the expected types – unsupported signature."; }
    };

    class MaximumRecursionDepthException : public Exception
    {
    public:
        MaximumRecursionDepthException(std::string s = "") : Exception(s) {}
        std::string Name()        override      { return "Maximum recursion depth exception"; }
        std::string Description() override      { return "Maximum recursion depth exceeded – there may be an infinite loop."; }
    };

    class InvalidConversionException : public Exception
    {
    public:
        InvalidConversionException(std::string s = "") : Exception(s) {}
        std::string Name()        override      { return "Invalid conversion exception"; }
        std::string Description() override      { return "Incompatible types cannot be converted. Try to use explicit conversion or ensure the data is correct."; }
    };
}
