## Ideje

### Funkcije
```
Type Complex
    Real r=0,i=0

# implicit Construcors
Complex(i=0), Complex(1, 2), Complex(i=0, r=2) ...

+ (c1, c2 : Complex) -> Complex
    return c1.r + c2.r, c1.i + c2.i

norm (c1, c2 : Complex) -> Int
    return Math::sqrt(c1.r + c2.r)

main () -> int
    c = Complex(1, 2)
    d = Complex(4, 5)
    print(|c|) # funkcija |c|
    d = c + d # Konstruise nov objekat i dodeljuje ga d-u
    c.+ d     # Menja vrednost objekta c
    c +.d     # Menja vrednost objekta d
    norma = norm(c1, c2)
    norma = c1 `norm` c2
```

### Tipovi
```
Int, Real, Char, Bool
"Zdravo" == ['Z', 'd', 'r', 'a', 'v', 'o'] 
[], ()
{}
list = [x | x : 1..10, x < 5]
sublist = list[-5..-1, odd]
list = [] : 'a'..'z', < 5 # Populacija liste
Tuple kao lista, samo imutabilan, ne moze populacija
```


### Petlje
```
i = 1
while i < 10, filter2 ..
    i.+ 1

for i : 1..10, filter1, filter2
    print(i)

for i, j : 1..10, i <= j
    print(i, j)
```
