int printf(char * s, int n);

int main()
{
    int a = 0;
    printf("%d\n", a);
    a = a + 2 + 3 - 2.2;
    printf("%d\n", a);
    a = a + 2 + 3 * 7;
    printf("%d\n", a);
    a = a + 2 + 3/3 - 3;
    printf("%d\n", a);
    a = a + 15 % 7;
    printf("%d\n", a);
    a = a + 2 + 3 % 3;
    printf("%d\n", a);

    return a;
}
