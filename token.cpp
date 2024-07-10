#include <string>
#include <map>

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
    Token_stream(istream &s) : ip{&s}, owns{false} {}
    Token_stream(istream* p) : ip{p}, owns{true} {}
    ~Token_stream() { close(); }
    Token get();      // read and return next token
    Token &current(); // most recently read token
    void set_input(istream &s)
    {
        close();
        ip = &s;
        owns = false;
    }
    void set_input(istream* p)
    {
        close();
        ip = p;
        owns = true;
    }

private:
    void close()
    {
        if (owns)
            delete ip;
    }
    istream* ip;          // pointer to an input stream
    bool owns;           // does the Token_stream own the istream?
    Token ct{Kind::end}; // current token

    Token Token_stream::get()
    {
        char ch = 0;
        * ip >> ch;
        switch (ch)
        {
        case 0:
            return ct = {Kind::end}; // assign and return
        case ';':                    // end of expression; print
        case '*':
        case '/':
        case '+':
        case '-':
        case '(':
        case ')':
        case '=':
            return ct == {static_cast<Kind>(ch)};
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
            ip-> putback(ch);      // put the first digit (or .) back into the input stream
            *ip >> ct.number_value; // read number into ct
            ct.kind = Kind::number;
            return ct;
        default: // name, name =, or error
            if (isalpha(ch))
            {
                ip-> putback(ch);      // put the first character back into the input stream
                *ip >> ct.string_value; // read string into ct
                ct.kind = Kind::name;
                return ct;
            }
            error("bad token");
            return ct = {Kind::print};
        }
    }
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

map<string, double> table;
double expr(bool);

// prim() goes lower in the hierarchy
double prim(bool get) // handle primaries
{
    if (get)
        ts.get(); // read next token
    switch (ts.current().kind)
    {
        case Kind::number: // floating-point constant
        {
            double v = ts.current().number_value;
            ts.get();
            return v;
        }
        case Kind::name:
        {
            double &v = table[ts.current().string_value]; // find the corresponding
            if (ts.get().kind == Kind::assign)
                v = expr(true); // ’=’ seen: assignment
            return v;
        }
        case Kind::minus: // unar y minus
            return -prim(true);
        case Kind::lp:
        {
            auto e = expr(true);
            if (ts.current().kind != Kind::rp)
                return error("')' expected");
            ts.get(); // eat ’)’
            return e;
        }
        default:
            return error("primar y expected");
    }
}