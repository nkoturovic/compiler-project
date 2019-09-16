int printf(char * s, int n);
void exit(int status);

int faktorijel(int n) {
    if (n == 1 || n == 0)
        return 1;
    else if (n > 0)
        return n * faktorijel(n-1);
    else {
        printf("Faktorijel nije definisan za: %d\n", n);
        exit(1);
    }
}

int main()
{
    printf("faktorijel 0: %d\n", faktorijel(0));
    printf("faktorijel 1: %d\n", faktorijel(1));
    printf("faktorijel 10: %d\n", faktorijel(10));
    printf("faktorijel -1: %d\n", faktorijel(-1));
    return 0;
}
