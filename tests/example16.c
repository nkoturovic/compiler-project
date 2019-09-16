int printf(char * s, int n);

int main()
{
    printf("%d\n", !1);
    printf("%d\n", !7);
    printf("%d\n", !-222);
    printf("%d\n", ~0x0);
    printf("%d\n", ~0xA);
    printf("%d\n", ~0xA >> 3);
    printf("%d\n", ~0xA << 3);
    printf("%d\n", 0 && 0);
    printf("%d\n", 0 || 1);
    printf("%d\n", 0 || !1);
    printf("%d\n", 1 && !1);
    printf("%d\n", 1 && 1);
    printf("%d\n", 3 && 2);
    printf("%d\n", 0 && 0);
    printf("%d\n", 0 && 1);

    return !!(2 + 3.2 * 7 || 5) && !0;
}
