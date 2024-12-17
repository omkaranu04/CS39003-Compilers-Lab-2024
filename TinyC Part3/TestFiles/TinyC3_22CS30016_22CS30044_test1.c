void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
    return;
}

void BubbleSort(int *arr, int length)
{
    int i = 0;
    int j;
    for (i = 0; i < length; i++)
    {
        for (j = i + 1; j < length; j++)
        {
            if (arr[i] > arr[j])
            {
                swap(&arr[i], &arr[j]);
            }
        }
    }
    return;
}
int main()
{
    int arr[5];
    arr[0] = (int)6.9;
    arr[1] = 4;
    arr[2] = 1;
    arr[3] = 3;
    arr[4] = -1;
    int n = 5;
    BubbleSort(arr, n);
    return 0;
}