---
title: Konstrukcija kompilatora - Seminarski rad
subtitle: "[www.github.com/nkoturovic/compiler-project](www.github.com/nkoturovic/compiler-project)"
author: 
    - Nebojša Koturović  
    - Blagoje Mirković

date: \today
---

\thispagestyle{empty}

# Predgovor {-}

Ovaj dokument je zamišljen kao apstrahovan pregled na proces konstrukcije kompilatora. Kompilator koji ćemo se potruditi da konstruišeo podržava interaktivni mod u kome se ponaša kao interpreter, a naravno podržava i prevođenje na mašinski kod željene arhitekture uz pomoć `llvm` alata.


Projekat na koji se ovaj dokument odnosi predstavlja predispitnu obavezu na kursu *Konstrukcija kompilatora*, koji se održava na 3. godini studija Matematičkog fakulteta kod profesora dr. Filipa Marića. Ovim projektom pokušaćemo da uradimo nešto što će pre svega nama u budućnosti, a nadamo se i još nekome biti korisno kao putokaz koji može pratiti u procesu konstrukcije prevodioca za neki (željeni) programski jezik. Dakle, više ćemo se fokusirati na praktičnu primenu, tako da čitalac može da preuzme određene delove koda i prilagodi ih svojim potrebama, tj. da ne gubi vreme na koracima koji predstavljaju nužni a ne suštnski deo ovog procesa.


Prilikom pisanja ovog dokumenta potrudićemo se da vas, koji čitate, ne bombardujemo nebitnim stvarima. Pokušaćemo da istaknemo suštinu, šta je to što vama može biti korisno i kakva mu je praktična primena.


Ukoliko vam bilo šta nije jasno dobrodošli ste da pitate, ukoliko smatrate da je nešto moglo biti odrađeno na jednostavniji i bolji način, dobrodošli ste da unapredite. Kako kod, tako i ovaj dokument.


Projekat je dostupan na [www.github.com/nkoturovic/compiler-project](www.github.com/nkoturovic/compiler-project).


**Autori:**  
Nebojša Koturović (<necer95@gmail.com>)  
Blagoje Mirković (<blagoje.mirkovic@gmail.com>)


\newpage \ \thispagestyle{empty} \ \newpage

# Uvod
