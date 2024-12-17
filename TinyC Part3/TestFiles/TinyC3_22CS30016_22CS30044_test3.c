float pi = 3.141592365;
int mod = 1000000007;
int *ptrGbl, temp;
char arr[1][9];
float ums[10];
int ans1, ans2, ans3, ans4, ans5, ans6, ans7, ans8, ans9, ans10;

float addiTion(int a, float b)
{
    float t = (float)(a) + b;
    return t;
}

int main()
{
    char name1 = 'o', name2 = 't';
    int o = 96;
    int t = 69;
    ans1 = o + t;
    ans2 = o - t;
    ans3 = o * t;
    ans4 = o % t;
    ans5 = t / o;
    ans6 = o & t;
    ans7 = o | t;
    ans8 = o ^ t;
    ans9 = o << 2;
    ans10 = t >> 3;

    float k = 9.99;
    float ans11 = addiTion(96, k);

    return 0;
}