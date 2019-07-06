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

Ovaj dokument je zamišljen kao sažeti prikaz našeg puta u procesu konstrukcije kompilatora. Kompilator koji ćemo konstruisati podržava interaktivni mod u kome se ponaša kao interpreter, a naravno podržava i prevođenje na mašinski kod željene arhitekture.


Projekat koji je integralni deo ovog dokumenta, i ovaj dokument, predstavljaju predispitnu obavezu na kursu *Konstrukcija kompilatora*, koji se održava na 3. godini studija Informatike na Matematičkom fakultetu kod profesora dr. Filipa Marića. Ovim projektom pokušaćemo da uradimo nešto što će pre svega nama u budućnosti, a nadamo se i još nekome, biti korisno kao putokaz koji može pratiti u procesu konstrukcije prevodioca za neki (željeni) programski jezik.


Dakle, više ćemo se fokusirati na praktičnu primenu, tako da čitalac može da preuzme određene delove koda i prilagodi ih svojim potrebama, tj. da ne gubi vreme na koracima koji predstavljaju nužni a ne suštnski deo ovog procesa.


Ovo nije udžbenik iz koga možete naučiti osnove o konstrukciji prevodioca, kao ni priručnik za alate `Lex`, `Bison` ili `LLVM`. U ovom dokumentu, kao što je već rečeno, trudićemo se da praktično primenimo znanje stečeno na kursu i istraživanju ove oblasti. Svakako ćemo koristiti navedene alate i dati osnovni uvid o tome šta oni suštinski rade i kako funkcionišu. Za ostatak možete posetiti referencirane materijale, ukoliko želite da produbiti znanja o temi i navedenim alatima. 


 Ovaj dokument nije namenjem samo nama, negovim autorima. Ovaj dokument namenjen je za svakog ko želi da se upusti u proces implementacije programskog prevodioca.


Ukoliko vam bilo šta nije jasno dobrodošli ste da pitate, ukoliko smatrate da je nešto moglo biti odrađeno na jednostavniji i bolji način, dobrodošli ste da unapredite. Kako kod, tako i ovaj dokument koji je javno dostupan i svako može učestvovati u procesu njegovog unapređenja.


Projekat i ovaj dokument su dostupni na sledećoj adresi:  
[www.github.com/nkoturovic/compiler-project](www.github.com/nkoturovic/compiler-project).


**Autori:**  
Nebojša Koturović (<necer95@gmail.com>)  
Blagoje Mirković (<blagoje.mirkovic@gmail.com>)


\newpage \ \thispagestyle{empty} \ \newpage

# Predstavljanje jezika

Jezik za koji konstruišemo kompilator nije već postojeći programski jezik, u ovaj jezik potrudićemo se da uključimo stilove programiranja i konstrukcije koje su nam se učinile korisnim, zanimljivim a pre svega jednostavnim.

Jeziku ćemo nadenuti ime: `Ime Jezika` 

## Osnovne konstrukcije jezika

### Zdravo svete

Pre svega, napišimo verovatno najpoznatiji program koji postoji.
```
print('Zdravo, svete!')
```
Program se prevodi komandom `./compiler zdravo.prog -o zdravo`

Pokrenimo naš program: `./zdravo`
```
Zdravo svete!
```

### Izrazi

### Operatori

### Naredbe kontrole toka
