int maxMod(int a, int b)
{
    if (a < 0)
    {
        a = -a;
    }
    if (b < 0)
    {
        b = -b;
    }
    int result = a > b ? a : b;
    return result;
}

int minNeg(int a, int b)
{
    if (a > 0)
    {
        a = -a;
    }
    if (b > 0)
    {
        b = -b;
    }
    int result = a < b ? a : b;
    return result;
}
void cmp(int a, int b)
{
    int c = a > b ? 1 : 0;
    return;
}
int sub(int a, int b)
{
    int c = maxMod(a, b);
    int d = minNeg(a, b);
    int e = c - d;
    return e;
}

int main()
{
    int a = -9, b = 7;
    int c = maxMod(a, b);
    int d = minNeg(a, b);

    char name1[] = "omkar";
    char name2[] = "tanishka";

    int e = sub(c, d);
    return 0;
}