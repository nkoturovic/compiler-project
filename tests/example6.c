int printf(char *s, double n);

int main()
{
    int a = 3, b;
    double c = 3.2, d = 2.3;
    int e = 0;

    b = 1.1;

    printf("%lf", a + b + c + d);

    return a * b - c + d;
}
