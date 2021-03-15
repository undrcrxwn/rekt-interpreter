#pragma once
#include <exception>
#include <string>

namespace rekt
{
    class Exception : virtual public std::exception
    {
    public:
        Exception(std::string msg = "") : message(msg) {}
        virtual std::string What()        { return Name() + " ocurred: " + Description() + (message.empty() ? "" : " " + message); }
        virtual std::string Name()        { return "Unspecified rekt exception"; }
        virtual std::string Description() { return "Something went wrong."; }

    protected:
        std::string message;
    };

    class WrongSignatureException : public Exception
    {
    public:
        WrongSignatureException(std::string msg = "") : Exception(msg) {}
        std::string Name()        override { return "Wrong signature exception"; }
        std::string Description() override { return "The given parameters are not of the expected types – unsupported signature."; }
    };

    class MaximumRecursionDepthException : public Exception
    {
    public:
        MaximumRecursionDepthException(std::string msg = "") : Exception(msg) {}
        std::string Name()        override { return "Maximum recursion depth exception"; }
        std::string Description() override { return "Maximum recursion depth exceeded – there may be an infinite loop."; }
    };

    class InvalidConversionException : public Exception
    {
    public:
        InvalidConversionException(std::string msg = "") : Exception(msg) {}
        std::string Name()        override { return "Invalid conversion exception"; }
        std::string Description() override { return "Incompatible types cannot be converted. Try to use explicit conversion or ensure the data is correct."; }
    };

    class UndefinedTokenException : public Exception
    {
    public:
        std::string target;

        UndefinedTokenException(std::string token = "", std::string msg = "")
            : Exception(msg), target(token) {}
        std::string Name()        override { return "Undefined token exception"; }
        std::string Description() override
        {
            return "There is no "
                + (target.empty() ? "such token" : "token \"" + target + "\"")
                + " binded.";
        }
    };

    class UndeclaredVariableException : public Exception
    {
    public:
        std::string target;

        UndeclaredVariableException(std::string variable = "",
            std::string msg = "Declare a variable first before using it.")
            : Exception(msg), target(variable) {}
        std::string Name()        override { return "Undeclared variable exception"; }
        std::string Description() override
        {
            return "There is no "
                + (target.empty() ? "such variable" : "variable \"" + target + "\"")
                + " declared.";
        }
    };

    class OutOfRangeException : public Exception
    {
    public:
        std::string target;

        OutOfRangeException(std::string value = "",
            std::string msg = "Make sure the value does not exceed the provided limits.")
            : Exception(msg), target(value) {}
        std::string Name()        override { return "Out of range exception"; }
        std::string Description() override
        {
            return (target.empty() ? "The given value" : target)
                + " is out of range.";
        }
    };
}
