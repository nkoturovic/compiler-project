/* Deklarisemo eksterne funkcije */
void exit(int status);
int printf(char * s, double num);
int puts(char *s);

int faktorijel(int n) 
{
    if (n == 1 || n == 0)
        return 1;
    else if (n > 1)
        return n * faktorijel(n-1);
    else {
        printf("Faktorijel nije definisan za: %.2lf\n", n);
        exit(1);
    }
}

/* main funkcija */
int main() 
{
    double sum = 0;
    for (int i=0; i <= 10; i=i+1) { 
        for (int j = 0; j < 10; j=j+1)
            sum = sum + i / 100.0;
        printf("%.0lf. iteracija\n", i+1);
    }

    int n;
    /* Blok ima svoj sopstveni doseg
     * za promenljive */
    {
        char sum = 's'; // Tip char
        double PI = 3.1415; // Tip double
        n = 3;
    }

    printf("Suma iznosi %.2lf\n", sum);

    if (sum > 5)
        puts("Suma je veca od 5");
    else if (sum < 5)
        puts("Suma je manja od 5");
    else
        puts("Vrednost sume je tacno 5");

    printf("Faktorijel sume je %.0lf\n", faktorijel(sum));

    return sum;
}

