int puts(char *s);


char * get_hello() {
    return "Hello world";
}

int main() {
    puts(get_hello());
    return 0;
}
