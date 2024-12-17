int binexp(int a, int b)
{
    if (b < 0)
        return 0;
    if (b == 0)
        return 1;
    else if (b > 0)
        return a * binexp(a, b - 1);
}

int main()
{
    int a = 2;
    int b = 3;
    int c = binexp(a, b);
    int d = -3;
    int e = binexp(a, d);

    int t = 7;

    if (t % 2)
    {
        if (t > 0)
            t = -t;
        else if (t < 0)
            t++;
        else
            t--;
    }
    else
    {
        if (e < 0)
            t += 2;
        else
            t--;
    }

    return 0;
}