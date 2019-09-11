int f(int a) {
    return 2*a;
}

double g(double a) {
    if (a > 10) 
        return a + a + a;
    else if (a <= 3)
        return 2*a;

    return 0;
}

int main()
{
    double a = 5;
    double b = 3;
    double c;

    if (a > b) 
        c = f(a) + f(b);
    else if (a < b) {
        c = f(b) - g(b);
    } else
        c = 0;

    return c;
}
