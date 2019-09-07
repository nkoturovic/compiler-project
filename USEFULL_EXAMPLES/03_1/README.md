# Hello world LLVM primer

Kada ukucamo `make` dešava se sledeće:
1) `main.cpp` se prevodi u `01_ir_generator.out`
2) pokreće se `./01_ir_generator > main.ll` koji generiše llvm međukod `main.ll`
3) Zatim se pokrece `clang++ main.ll -o main.out` čime se pravi izvršni fajl `main.out`


.ll fajl sadrži generisani međukod.


**Napomena:** Program se pokreće sa ./main.out
