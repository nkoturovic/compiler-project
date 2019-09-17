# Compiler project (C -> LLVM)

## Prevođenje

Prevođenje se vrši komandom:
```
./ctollvm.out putanja_do_prog.c
```
Prevodilac će generisati fajlove `imeprograma.o` i `imeprograma.ll`.

## Primer

Sledeći primer otprilike ilustruje trenutne mogućnosti kompilatora, dosta stvari je u fazi razvoja.

**Primer programa:**
```c
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

```
1. iteracija
2. iteracija
3. iteracija
4. iteracija
5. iteracija
6. iteracija
7. iteracija
8. iteracija
9. iteracija
10. iteracija
11. iteracija
Suma iznosi 5.50
Suma je veca od 5
Faktorijel sume je 120
```

# Projekat je struktuiran na sledeći način:

- `include` folder sadrži *.hpp* fajlove, source` folder sadrži *.cpp* fajlove.
- Svaki folder/fajl unutar **include/src** direktno odgovcara odgovarajućem namespace-u.


## Done - Urađeno

- Lexer je završen 99%, možda fale sitne izmene
- Način izgrađivanja konstrukcija jezika u parseru je ok, može se još malo razmotriti možda.
- Code generation

## TODO - Uraditi

- Napraviti bolji TODO list
- Spisak podržanih i nepodržanih jezičkih konstrukata.
- Otkloniti potencijalne bug-ove.
- Nastaviti razvoj kompilatora

## Namespace

- Namespace odgovara fizičkoj lokaciji *foldera/fajla* u include/src direktorijumu. 

- `compiler` - glavni namespace, odnosi se na sve što ima veze sa kompajlerom.
- `compiler::ast` - namespace koji sadrži definiciju *stabla apstraktne sintakse* koji predstavlja međureprezentaciju sintaksno ispravnog programa stablo se kreira od strane parsera tj. (Bison-a/yacc-a), pa na njemu dalje treba vršiti `codegen` koja preslikava drvo u `LLVM IR`.
- `compiler::lang` - namespace koji sadrži sve ono što se odnosi na definisanje jezika
- `compiler::lang::types` - fajl/namespace koji sadrži definicije tipova: `char`, `int`, `double`, `pokazivacki tipovi` ...
- `compiler::lang::operators` - sadrži definicije operacija koje postoje UnOp i BinOp
- `compiler::lang::semantic` - sadrži semantička pravila (npr. kako se BinOp +,-,*,/) ponašaju. pr => BinOpCheck(+(int, double)) -> double. Semantička pravila najviše se odnose na **proveru tipova** npr. operatora itd. Sva pravila koja su do sada definisana su dobra (provereno putem interpretacije).
- `compiler::codegen` - Sadrži globalne definicije objekata potrebnih za generisanje međukoda (llvm::Context,Module,IRBuilder), i možda neke dodatne `llvm`.stvari, čisto da budu lepo na jednom mestu a ne globalne promenljive, njima se može pristupiti sa (compiler::codegen::context/module/builder) jer su statičke promenljive i žive od početka do kraja programa.
- `compiler::structs` - Bilo koje pomoćne strukture koje ne znaš gde da staviš.


Valja pogledati svaki od ovih foldera/fajlova da bi se razumela struktura projekta i kako su stvari implementirane


## Korisni materijali

- Folder `USEFULL_EXAMPLES` (Pogledati neki od starih commit-ova).
- Odštampani materijali koje sam ti dao. Tu ima isto kao sa vežbi primer `Kaleidoscope` na osnovu koga smo mi radili na vežbama.
- Kaleidoscope tutorijal (isto rađeno na vežbama, imaš i odštampano): [link](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html)
- Vežbe 2019, snimci su sa vežbi - bilo koja grupa: [link](http://poincare.matf.bg.ac.rs/~mirko/kk/2019/)
- Lik koji je napravio llvm priča o LLVM-u (mnogo kul): [link](https://www.youtube.com/watch?v=yCd3CzGSte8)
- Audio snimci sa vežbi [link](https://drive.google.com/drive/folders/1uX3_zvsyczCqeeODYlYDpg63bHovYrya?fbclid=IwAR02e0iu23jZ5jAsGxVlcNWMqOIXlZWk02fpC85DMMWTot_J2RmAZwgL_hs)
- Dodatno možda neki lik o parsiranju/llvm/ast: [link](https://www.youtube.com/watch?v=E6i8jmiy8MY&t=3487s)
- Dodatno neki lik, možda zanimljivo priča: [link](https://www.youtube.com/watch?v=E6i8jmiy8MY&t=3487s)
- Ovo nema veze sa temom ali je mnogo zanimljivo, počinje od 6min tek: [link](https://www.youtube.com/watch?v=EY6q5dv_B-o&t=1543s)
- Google Search :) !!
