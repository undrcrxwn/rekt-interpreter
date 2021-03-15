#pragma once
#include <string>

namespace rekt
{
    // An Error, Warning or Info issue stored by an Element as a tip to be passed back to the View layer after the input is tokenized.
    class Issue
    {
    public:
        enum class Type
        {
            Error,
            Warning,
            Info
        };

        struct Content
        {
            std::string title;
            std::string description;
            std::string tip;
        };

        virtual Issue::Type GetIssueType() const = 0;
        virtual Content Get() const = 0;
    };

    class Error : public Issue
    {
    public:
        enum class Type
        {
            UnexpectedError,
            UndefinedToken,
            UndeclaredVariable,
            OutOfRange
        };

        Error::Type type;

        Error(Error::Type t) : type(t) {}
        Issue::Type GetIssueType() const { return Issue::Type::Error; }

        Issue::Content Get() const override
        {
            Issue::Content res;
            switch (type)
            {
            case Error::Type::UndefinedToken:
                return { "Undefined token",
                         "There is no such token binded." };
            case Error::Type::UndeclaredVariable:
                return { "Undeclared variable",
                         "There is no such variable binded.",
                         "Bind a variable first by declaring it or binding it externally before using." };
            case Error::Type::OutOfRange:
                return { "Out of range",
                         "The value exceeds the provided limits." };
            default:
                return { "Unexpected error" };
            }
        }
    };
}
