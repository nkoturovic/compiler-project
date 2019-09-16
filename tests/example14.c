int printf(char * s, int n);

int main()
{
    printf("%d\n", 2 && 0);
    printf("%d\n", 2 || 0);
    printf("%d\n", 0 || 2.2);
    printf("%d\n", 0 | 2 && 0);
    printf("%d\n", 0 | 2 && 3.3);
    printf("%d\n", 2 < 3);
    printf("%d\n", 2 + 2 + (2.2 <= 3.3333333333));
    printf("%d", 2 >> 3);
    printf("%d", 2 << 10);
}
