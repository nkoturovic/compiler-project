int main() {

    int i = 99;
    int n = 50;

    for (int i=0; i<10; i = i+1) {
        n = n + 1;
    }

    if (n > i)
        return n;

    return i;
}
