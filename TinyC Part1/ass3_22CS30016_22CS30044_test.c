/* Test file to exhaustively check all token rules */

// checking some keywords
enum gender
{
    male = 0,
    female
};
_Bool isTrue = 1;
float _Complex isComplex = 11.0 - 5.6 * I;
double _Imaginary isImaginary = 2.0 * I;
extern int val = 90;
static const double PI = 3.14;
auto x = 6.9;
volatile double w = 2.5647;
inline int sq(int x)
{
    return x * x;
}
void goto()
{
    int max = 10;
    int i = 0;
start:
    if (i < max)
    {
        printf("%d ", i);
        i++;
        goto start;
    }
}
struct point
{
    float x;
    float y;
};
typedef struct point Point;
union acads
{
    char *rollNo;
    float cg;
};
int main()
{
    // float and numbers
    short signed int num = 34;
    float f1 = 56.78;
    float f2 = 12.e-3;
    float f3 = 12.34E+5;
    float f4 = .12e3;
    float f5 = 12345E6;
    // chars
    char c1 = 'o';
    char c2 = '\b';
    // strings
    char s1[] = "";
    char s2[] = "This is a string with escape sequences \a\b\f\r\v\t\b\n \\//\\//\\//";
    // punctuators
    int a = 1, b = 1;
    a = a++;
    a = a--;
    a = a & b;
    a = a * b;
    a = a + b;
    a = a - b;
    a = ~a;
    a = !a;
    a = a / b;
    a = a % b;
    a = a << b;
    a = a >> b;
    a = a ^ b;
    a = a | b;
    a = b;
    a *= b;
    a /= b;
    a %= b;
    a += b;
    a -= b;
    a <<= b;
    a >>= b;
    a &= b;
    a ^= b;
    a |= b;
    // remaining some things
    switch (a)
    {
    case 1:
        break;
    default:
        break;
    }
    int x = 2;
    do
    {
        printf("%d", x--);
    } while (x >= 0);
    for (int i = 0; i < 6; i++)
    {
        if (((a > b) && (i >= 0)) || ((a < b) && (i != 0)) || ((a == b) && i <= 6))
            continue;
        else
            break;
    }
    // this is a single line comment

    /*
    this is a
    multi line comment
    */
    return 0;
}