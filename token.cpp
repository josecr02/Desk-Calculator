#include <string>

using namespace std;

enum class Kind : char
{
    name, number, end, 
    plus = '+', minus = '-', mul = '*', div = '/', print=';', assign= '=', lp='(', rp=')'
};

struct Token
{
    Kind kind;
    string string_value;
    double number_value;
};

class Token_stream
{
public:
    Token get();            // read and return next token
    const Token &current(); // most recently read token
    // ...
};

// expr() handles addition and subtraction
double expr(bool get) // add and subtract
{
    double left = term(get);
    for (;;)
    { // ‘‘forever’’
        switch (ts.current().kind)
        {
        case Kind::plus:
            left += term(true);
            break;
        case Kind::minus:
            left −= term(true);
            break;
        default:
            return left;
        }
    }
}

// term() handles multiplication and division in the same way expr()
double term(bool get) // multiply and divide
{
    double left = prim(get);
    for (;;)
    {
        switch (ts.current().kind)
        {
        case Kind::mul:
            left ∗= prim(true);
            break;
        case Kind::div:
            if (auto d = prim(true))
            {
                left /= d;
                break;
            }
            return error("divide by 0");
        default:
            return left;
        }
    }
}