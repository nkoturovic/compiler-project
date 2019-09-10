int main()
{
    int a = 0;
    while (a < 10)
        a = a + 1;

    int b = 10;
    while (b > 0) {
        b = b - 1;
        a = a + 1;
    }

    return a-b;
}
