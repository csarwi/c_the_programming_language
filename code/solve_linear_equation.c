#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* ================= Lexer helpers ================= */

typedef struct
{
    const char *s;
} Input;

static void skip_ws(Input *in)
{
    while (isspace((unsigned char)*in->s))
        in->s++;
}

static int match(Input *in, char c)
{
    skip_ws(in);
    if (*in->s == c)
    {
        in->s++;
        return 1;
    }
    return 0;
}

/* ================= Linear pair a*x + b ================= */

typedef struct
{
    double a, b; /* value == a*x + b */
    int ok;      /* 1 if valid linear, 0 if error (nonlinear / bad syntax) */
} Lin;

/* Make lin */
static Lin lin(double a, double b)
{
    Lin L = {a, b, 1};
    return L;
}
static Lin lin_err(void)
{
    Lin L = {0, 0, 0};
    return L;
}

/* Add/sub */
static Lin lin_add(Lin u, Lin v, int sign)
{
    if (!u.ok || !v.ok)
        return lin_err();
    return lin(u.a + sign * v.a, u.b + sign * v.b);
}

/* Multiply: forbid (a!=0) * (a!=0) */
static Lin lin_mul(Lin u, Lin v)
{
    if (!u.ok || !v.ok)
        return lin_err();
    int ux = fabs(u.a) > 0.0, vx = fabs(v.a) > 0.0;
    if (ux && vx)
        return lin_err(); /* would be x*x or worse */
    double a = u.a * v.b + v.a * u.b;
    double b = u.b * v.b;
    return lin(a, b);
}

/* Divide: only by pure constant (a==0, b!=0) */
static Lin lin_div(Lin u, Lin v)
{
    if (!u.ok || !v.ok)
        return lin_err();
    if (fabs(v.a) > 0.0)
        return lin_err(); /* denominator depends on x -> nonlinear */
    if (fabs(v.b) == 0.0)
        return lin_err(); /* divide by zero */
    return lin(u.a / v.b, u.b / v.b);
}

/* ================= Parsing numbers and factors ================= */

static int parse_number(Input *in, double *out)
{
    skip_ws(in);
    const char *start = in->s;
    char *endp = NULL;

    /* number: [sign]? (digits)? ('.' digits)? with at least one digit total */
    if (*in->s == '+' || *in->s == '-')
        in->s++;

    int saw_digit = 0;
    while (isdigit((unsigned char)*in->s))
    {
        in->s++;
        saw_digit = 1;
    }
    if (*in->s == '.')
    {
        in->s++;
        while (isdigit((unsigned char)*in->s))
        {
            in->s++;
            saw_digit = 1;
        }
    }
    if (!saw_digit)
    {
        in->s = start;
        return 0;
    } /* no number */

    double val = strtod(start, &endp);
    if (endp == start)
    {
        in->s = start;
        return 0;
    }
    in->s = endp;
    *out = val;
    return 1;
}

/* Forward decls */
static Lin parse_expr(Input *in);

static Lin parse_factor(Input *in)
{
    skip_ws(in);

    /* unary +/- */
    int sign = +1;
    while (*in->s == '+' || *in->s == '-')
    {
        if (*in->s == '-')
            sign = -sign;
        in->s++;
        skip_ws(in);
    }

    Lin res;
    if (match(in, '('))
    {
        res = parse_expr(in);
        if (!match(in, ')'))
            return lin_err();
    }
    else if (*in->s == 'x' || *in->s == 'X')
    {
        in->s++;
        res = lin(1.0, 0.0);
    }
    else
    {
        double val;
        if (!parse_number(in, &val))
            return lin_err();
        res = lin(0.0, val);
    }

    if (sign < 0)
    {
        res.a = -res.a;
        res.b = -res.b;
    }
    return res;
}

static Lin parse_term(Input *in)
{
    Lin left = parse_factor(in);
    if (!left.ok)
        return left;

    for (;;)
    {
        skip_ws(in);
        if (*in->s == '*' || *in->s == '/')
        {
            char op = *in->s++;
            Lin right = parse_factor(in);
            if (!right.ok)
                return lin_err();
            left = (op == '*') ? lin_mul(left, right) : lin_div(left, right);
            if (!left.ok)
                return left;
        }
        else
        {
            break;
        }
    }
    return left;
}

static Lin parse_expr(Input *in)
{
    Lin left = parse_term(in);
    if (!left.ok)
        return left;

    for (;;)
    {
        skip_ws(in);
        if (*in->s == '+' || *in->s == '-')
        {
            char op = *in->s++;
            Lin right = parse_term(in);
            if (!right.ok)
                return lin_err();
            left = lin_add(left, right, op == '+' ? +1 : -1);
            if (!left.ok)
                return left;
        }
        else
        {
            break;
        }
    }
    return left;
}

/* ================= Solve a single equation ================= */

static int parse_linear_expr(const char *s, double *a, double *b)
{
    Input in = {s};
    Lin L = parse_expr(&in);
    if (!L.ok)
        return -1;
    skip_ws(&in);
    if (*in.s != '\0')
        return -1; /* leftover junk */
    *a = L.a;
    *b = L.b;
    return 0;
}

/* Solve (aL - aR)*x + (bL - bR) = 0
   Returns:
     0 -> unique solution in *x_out
     1 -> no solution
     2 -> infinite solutions
    -1 -> parse error
*/
int solve_linear_equation(const char *equation, double *x_out)
{
    const double EPS = 1e-12;

    const char *eq = strchr(equation, '=');
    if (!eq)
        return -1;

    size_t Llen = (size_t)(eq - equation);
    char *L = (char *)malloc(Llen + 1);
    char *R = strdup(eq + 1);
    if (!L || !R)
    {
        free(L);
        free(R);
        return -1;
    }
    memcpy(L, equation, Llen);
    L[Llen] = '\0';

    double aL = 0, bL = 0, aR = 0, bR = 0;
    int okL = parse_linear_expr(L, &aL, &bL);
    int okR = parse_linear_expr(R, &aR, &bR);
    free(L);
    free(R);
    if (okL != 0 || okR != 0)
        return -1;

    double a = aL - aR;
    double b = bL - bR;

    if (fabs(a) < EPS)
    {
        if (fabs(b) < EPS)
            return 2; /* infinite solutions */
        return 1;     /* no solution */
    }
    *x_out = -b / a;
    return 0; /* unique solution */
}

/* ================= Demo ================= */

int main(void)
{
    /*
    const char *tests[] = {
        "2*(x + 3) = 7",
        "3*(x - 2) = 7*x + 10",
        "(x - 1)/2 + 3 = 5",
        "5*x + 2 = 5*x + 2",
        "4*x + 1 = 4*x + 2",
        "-(x + 4) = 2 - (3 - x)",
        "1.5*(x - 0.5) = 2.0*x + 3.25",

        "(x+1)*(x+2) = 0",
        "x / (x + 1) = 2",
        NULL
    };

    for (int i = 0; tests[i]; ++i) {
        double x = 0.0;
        int rc = solve_linear_equation(tests[i], &x);
        printf("%-30s -> ", tests[i]);
        if (rc == 0)       printf("x = %.12g\n", x);
        else if (rc == 1)  printf("No solution\n");
        else if (rc == 2)  printf("Infinite solutions\n");
        else               printf("Parse error or nonlinear\n");
    }
    */

    char buf[1024];
    printf("\nEnter an equation (e.g., 2*(x+3) = 7):\n> ");
    if (fgets(buf, sizeof buf, stdin))
    {
        double x;
        int rc = solve_linear_equation(buf, &x);
        if (rc == 0)
            printf("x = %.12g\n", x);
        else if (rc == 1)
            printf("No solution\n");
        else if (rc == 2)
            printf("Infinite solutions\n");
        else
            printf("Parse error or nonlinear\n");
    }

    return 0;
}
