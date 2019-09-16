/* function declarations */
int getchar();
int printf(char * s, int num);
int puts(char *s);
int getpid();
int rand();
void srand(int seed);

int main()
{

    srand(getpid()); // postavljamo seed
    int num = rand() % 10; // gen. random u [0, 9]
    puts("Zamisljen je broj u segmentu [0, 9].");
    puts("Pogodi ga!");

    { // blok ogranicava zivotni vek
        int guess = -1;
        while (guess != num) {
            puts("Unesite broj: ");
            guess = getchar();
            getchar(); // kupi '\n'
            guess = guess - '0'; // ASCII to INT
            if (guess != num)
                puts("Pogresili ste!");
        }
    }

    printf("Cestitamo, zamisljeni broj je %d\n", num);

    return 0;
}
