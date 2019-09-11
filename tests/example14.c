// Deklarisemo eksterne funkcije
int printf(char * s, int num);
int puts(char *s);

/* main funkcija */
int main() 
{
    int sum = 0;
    for (int i = 0; i < 10; i=i+1) { 
        for (int j = 0; j < 10; j=j+1)
        sum = sum + 1;
        printf("%d\n", i);
    }

    int n;
    /* Blok ima svoj sopstveni doseg
     * za promenljive */
    {
        int sum = 22;
        n = 3;
    }

    printf("Suma iznosi %d\n", sum);

    if (sum > 100)
        puts("Suma je veca od 100");
    else if (sum < 100)
        puts("Suma je manja od 100");
    else
        puts("Suma je tacno 100");

    return sum;
}
