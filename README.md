# Compiler project

- Project for university course on compilers

# Projekat je struktuiran na sledeći način:

- `include` folder sadrži *.hpp* fajlove, source` folder sadrži *.cpp* fajlove.
- Svaki folder/fajl unutar **include/src** direktno odgovcara odgovarajućem namespace-u.

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

## Done - Urađeno

- Lexer je završen 99%, možda fale sitne izmene
- Način izgrađivanja konstrukcija jezika u parseru je ok, može se još malo razmotriti možda.

## TODO - Uraditi

- Generisanje koda je velika stavka koju treba odraditi, u suštini to je preslikavanje: AST -> `LLVM IR` posredstvom virtuelnog (tj. apstraktnog) metoda `codegen` iz bazne klase ast::Statement.
- Treba proširiti Parser pravilima za definicije/deklaracije funkcija C-a, a naravno potom i mnogim konstrukcijama kasnije: grananje,petlje,itd i to uvezati sa AST-om...
- AST pre svega treba: Implementirati `codegen` metode za postojeće klase, i proširiti asortiman klasa.


**ZA POČETAK:** Pogledati folder `USEFULL_EXAMPLES`!!


**NAPOMENA 1:** Prilikom generacije koda, generisani `llvm IR` kod se mora nalaziti u funkcijama jer će u suprotnom optimizator da eliminiše sve što je generisano za dati `modul` kada uradimio `module->print(llvm::outs(), nullptr)` ničeg neće imati!! Zato možda prvo treba proširiti parser i uvezati sa `ast`-om tako da podržava definicije/deklaracije funkcija.. Nije dobro raditi za svaki dobijeni Value*: Value->print.. Ova rečenica će verovatno postati logična u jednom trenutku. 


**NAPOMENA 2:** Dakle za početak treba ići ka tome da može da se ispravno konstruiše IR za `example12.txt` program dat u primeru: `tests/example12.txt`.

- Fix Grammar in parser
- Maybe implement some kind of printAst() or str()
- Implement codegen methods in ast
- LLVM integration
- Semantic checkers are fine
- Many fixes required
- PARSER & AST EXTENDING!!
- CODE GENERATION!!

# Korisni materijali

- Folder `USEFULL_EXAMPLES`
- Odštampani materijali koje sam ti dao. Tu ima isto kao sa vežbi primer `Kaleidoscope` na osnovu koga smo mi radili na vežbama.
- Kaleidoscope tutorijal (isto rađeno na vežbama, imaš i odštampano): [link](https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html)
- Vežbe 2019, snimci su sa vežbi - bilo koja grupa: [link](http://poincare.matf.bg.ac.rs/~mirko/kk/2019/)
- Lik koji je napravio llvm priča o LLVM-u (mnogo kul): [link](https://www.youtube.com/watch?v=yCd3CzGSte8)
- Audio snimci sa vežbi [link](https://drive.google.com/drive/folders/1uX3_zvsyczCqeeODYlYDpg63bHovYrya?fbclid=IwAR02e0iu23jZ5jAsGxVlcNWMqOIXlZWk02fpC85DMMWTot_J2RmAZwgL_hs)
- Dodatno možda neki lik o parsiranju/llvm/ast: [link](https://www.youtube.com/watch?v=E6i8jmiy8MY&t=3487s)
- Dodatno neki lik, možda zanimljivo priča: [link](https://www.youtube.com/watch?v=E6i8jmiy8MY&t=3487s)
- Ovo nema veze sa temom ali je mnogo zanimljivo, počinje od 6min tek: [link](https://www.youtube.com/watch?v=EY6q5dv_B-o&t=1543s)
- Google Search :) !!
