struct s {
    int a;
    float b[5][5];
};

struct t{
    struct s x[10];
};

float * f(int * a[10], float b[8][5]) {
    return b[0]; // Allowed as b[0] is an array of floats, i.e. b[0] is a pointer to a float
}

int main() {
    float a[10][10];
    int b[10][10];
    int * c[5];
    struct s x;
    struct t y;
    int i;
    // f(c, x.b); //Allowed. as int *c[5] and int * a[10] are compatible. Also, float b[5][5] and float b[8][5] are compatible.
    // f(c, a); // Error. float b[8][5] and float a[10][10] are incompatible.
    y.x[i].b[2] = x.b[1]; // Error. Assigning whole arrays.
}