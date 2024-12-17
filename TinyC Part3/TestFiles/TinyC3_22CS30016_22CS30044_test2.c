int size = 3;
void incrementByk(int arr[size][size], int k)
{
    int i = 0, j = 0;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            arr[i][j] += k;
        }
    }
    return;
}

int main()
{
    int a[5];
    int i, j;
    i = 0;
    while (i < 5)
    {
        a[i] = i + 1;
        i++;
    }
    i = 0;
    do
    {
        a[i] *= a[i];
    } while (i < 5);

    int arr[size][size];
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            arr[i][j] = (i + 1) * (j + 1);
        }
    }
    int k = 101;
    incrementByk(arr, k);

    return 0;
}