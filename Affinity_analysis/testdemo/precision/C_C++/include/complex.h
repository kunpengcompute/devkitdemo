
typedef struct complex_t {
    double re;
    double im;
} complex;

complex conv_from_polar(double r, double radians);
complex add(complex left, complex right);
complex multiply(complex left, complex right);

static inline double abs2(complex left)
{
    double res = left.im * left.im + left.re * left.re;
    return res;
}