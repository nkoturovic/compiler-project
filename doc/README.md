---
title: Konstrukcija kompilatora - Seminarski rad
subtitle: "[www.github.com/nkoturovic/compiler-project](www.github.com/nkoturovic/compiler-project)"
author: Nebojša Koturović
date: \today
---

# Predgovor


Ovaj dokument je zamišljen kao apstrahovan pregled na proces konstrukcije kompilatora. Kompilator koji ću se potruditi da konstruišem podržava interaktivni mod u kome se ponaša kao interpreter, a naravno podržava i prevođenje na mašinski kod željene arhitekture uz pomoć `llvm` alata.


Projekat na koji se ovaj dokument odnosi predstavlja predispitnu obavezu na kursu `Konstrukcija kompilatora` koji pohađam, a koji se održava na 3. godini studija `Matematičkog fakulteta` kod profesora dr. Filipa Marića. Ovim projektom pokušaću da uradim nešto što će pre svega meni, a nadam se i još nekome biti korisno kao putokaz koji može pratiti u procesu konstrukcije prevodioca za neki (željeni) programski jezik. Dakle, više ću se fokusirati na praktičnu primenu, tako da čitalac može da preuzme određene delove koda i prilagodi ih svojim potrebama, tj. da ne gubi vreme na koracima koji predstavljaju nužni a ne suštnski deo ovog procesa.


Prilikom pisanja ovog dokumenta potrudiću se da vas, koji čitate, ne bombardujem nebitnim stvarima. Pokušaću da istaknem suštinu, šta je to što vama može biti korisno i kakva mu je praktična primena.


Ukoliko vam bilo šta nije jasno dobrodošli ste da pitate, ukoliko smatrate da je nešto moglo biti odrađeno na jednostavniji i bolji način, dobrodošli ste da unapredite. Kako kod, tako i ovaj dokument jer možda i ja nisam imao dovoljno znanja i energije koja bi mi omogućila da sve ovo bude kvalitetnije i bolje.


Moja ime je Nebojša Koturović, kontakt `e-mail` adresa: [](necer95@gmail.com)


# Uvod
