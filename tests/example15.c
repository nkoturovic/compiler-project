int printf(char * s, double a);

int main()
{
    int a = 0.0;
    a = a + (+2.3);
    a = a + (-2.3);
    a = a + 2.3 - 2.8 + 3 - 1;
    a = a + 1.1;
    a = a + (-4) + 2.2;
    a = a + !5;
    a = a + ~3 + ~4;
    a = a + ~0b11111;
    a = a + !5 + !3;
    printf("%lf\n", a);
    return a;
}
