#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum bases
{
    G_E0     = 0x1,  // 0000 0001
    G_E1     = 0x2,  // 0000 0010
    G_E2     = 0x4,  // 0000 0100
    G_E3     = 0x8,  // 0000 1000
    G_E1E2   = 0x10, // 0001 0000
    G_E2E3   = 0x20, // 0010 0000
    G_E3E1   = 0x40, // 0100 0000
    G_E1E2E3 = 0x80, // 1000 0000
};
char const *names[] =
{
    "(null)",
    "_e0",           // 0000 0001
    "_e1",           // 0000 0010
    "_e2",           // 0000 0100
    "_e3",           // 0000 1000
    "_e1e2",         // 0001 0000
    "_e2e3",         // 0010 0000
    "_e3e1",         // 0100 0000
    "_e1e2e3"        // 1000 0000
};

// Valid combinations:
unsigned int valid[] =
{
    0x1,  // 0000 0001 -- scalar
    0x2,  // 0000 0010 -- e1
    0x4,  // 0000 0100 -- e2
    0x8,  // 0000 1000 -- e3
    0xe,  // 0000 1110 -- vector3
    0x10, // 0001 0000 -- e1e2
    0x20, // 0010 0000 -- e2e3
    0x40, // 0100 0000 -- e3e1
    0x71, // 0111 0001 -- quaternion
    0x80, // 1000 0000 -- e1e2e3
    0xff, // 1111 1111 -- full algebra
};

bool is_valid(unsigned int value)
{
    for (int i = 0; i < 11; i++)
    {
        if (valid[i] == value) return true;
    }
    return false;
}

unsigned int minimal_uber_space(unsigned int s)
{
    for (int i = 0; i < 11; i++)
    {
        unsigned int t = valid[i];
        if (t < s) continue;
        if ((s ^ t) < t)
        {
            return t;
        }
    }
    return 0xff;
}

void print_g(FILE *output, unsigned int value)
{
    if (value == G_E0)
    {
        fprintf(output, "float32");
        return;
    }
    int index = 1;
    while (value)
    {
        if ((value & 1) > 0)
        {
            fprintf(output, "%s", names[index]);
        }
        value = (value >> 1);
        index += 1;
    }
}

bool has(unsigned int b, unsigned int c)
{
    bool result = (b & c) > 0;
    return result;
}

void output_unary_minus_operators(FILE *output)
{
    for (int i = 1; i < 11; i++)
    {
        // linear combination
        unsigned int lc = valid[i];

        print_g(output, lc);
        fprintf(output, " operator - (");
        print_g(output, lc);
        fprintf(output, " a) { ");
        print_g(output, lc);
        fprintf(output, " r; ");

        int p = 1;
        for (int k = 0; k < 8; k++)
        {
            bases b = (bases) p;
            if (has(lc, b)) fprintf(output, "r._%d = -a._%d; ", k, k);
            p = (p << 1);
        }
        fprintf(output, "return r; }\n");
    }
    fprintf(output, "\n");
}

void output_additive_operators(FILE *output, char const *op)
{
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (i == 0 && j == 0) continue;

            // linear combination
            unsigned int lhs_lc = valid[i];
            unsigned int rhs_lc = valid[j];
            unsigned int result_lc = valid[i] | valid[j];
            result_lc = minimal_uber_space(result_lc);

            print_g(output, result_lc);
            fprintf(output, " operator %s (", op);
            print_g(output, lhs_lc);
            fprintf(output, " a, ");
            print_g(output, rhs_lc);
            fprintf(output, " b) { ");
            print_g(output, result_lc);
            fprintf(output, " r;");

            int p = 1;
            for (int k = 0; k < 8; k++)
            {
                bases b = (bases) p;
                if (has(result_lc, b))
                {
                    fprintf(output, " r");
                    if (result_lc != G_E0) fprintf(output, "._%d", k);
                    fprintf(output, " = ");

                    if (has(lhs_lc, b) && has(rhs_lc, b))
                    {
                        fprintf(output, "a");
                        if (lhs_lc != G_E0) fprintf(output, "._%d", k);

                        fprintf(output, " %s b", op);
                        if (rhs_lc != G_E0) fprintf(output, "._%d", k);
                        fprintf(output, ";");
                    }
                    else if (has(lhs_lc, b) && !has(rhs_lc, b))
                    {
                        fprintf(output, "a");
                        if (lhs_lc != G_E0) fprintf(output, "._%d", k);
                        fprintf(output, ";");
                    }
                    else if (!has(lhs_lc, b) && has(rhs_lc, b))
                    {
                        fprintf(output, "b");
                        if (rhs_lc != G_E0) fprintf(output, "._%d", k);
                        fprintf(output, ";");
                    }
                    else
                    {
                        fprintf(output, "0.f;");
                    }
                }
                p = (p << 1);
            }
            fprintf(output, " return r; }\n");
        }
        fprintf(output, "\n");
    }
}

void output_inner_products(FILE *output)
{
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            // linear combination
            unsigned int lhs_lc = valid[i];
            unsigned int rhs_lc = valid[j];

            fprintf(output, "float32 inner(");
            print_g(output, lhs_lc);
            fprintf(output, " a, ");
            print_g(output, rhs_lc);
            fprintf(output, " b) { float32 r");

            bool printed_first = false;
            int p = 1;
            for (int k = 0; k < 8; k++)
            {
                bases b = (bases) p;
                if (has(lhs_lc, b) && has(rhs_lc, b))
                {
                    if (printed_first)
                    {
                        fprintf(output, " %s (a", ((k >= 4) && (k <= 6) ? "-" : "+"));
                        if (lhs_lc != G_E0) fprintf(output, "._%d", k);
                        fprintf(output, " * b");
                        if (rhs_lc != G_E0) fprintf(output, "._%d", k);
                        fprintf(output, ")");
                    }
                    else
                    {
                        fprintf(output, " = %s(a", ((k >= 4) && (k <= 6) ? "-" : "+"));
                        if (lhs_lc != G_E0) fprintf(output, "._%d", k);
                        fprintf(output, " * b");
                        if (rhs_lc != G_E0) fprintf(output, "._%d", k);
                        fprintf(output, ")");

                        printed_first = true;
                    }
                }
                else
                {
                }
                p = (p << 1);
            }
            if (!printed_first)
            {
                fprintf(output, " = 0.f");
                printed_first = true;
            }
            fprintf(output, "; return r; }\n");
        }
        fprintf(output, "\n");
    }
}

void output_outer_products(FILE *output)
{
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            // linear combination
            unsigned int lhs_lc = valid[i];
            unsigned int rhs_lc = valid[j];
            unsigned int result_lc = 0;

            {
                unsigned int p = 1;
                for (int k = 0; k < 8; k++)
                {
                    unsigned int q = 1;
                    for (int s = 0; s < 8; s++)
                    {
                        unsigned int r = 0;
                        if (has(lhs_lc, p) && has(rhs_lc, q))
                        {
                            if (p == G_E0)                result_lc = (result_lc | q);
                            if (q == G_E0)                result_lc = (result_lc | p);
                            if (p == G_E1 && q == G_E2)   result_lc = (result_lc | G_E1E2);
                            if (p == G_E1 && q == G_E3)   result_lc = (result_lc | G_E3E1);
                            if (p == G_E1 && q == G_E2E3) result_lc = (result_lc | G_E1E2E3);
                            if (p == G_E2 && q == G_E1)   result_lc = (result_lc | G_E1E2);
                            if (p == G_E2 && q == G_E3)   result_lc = (result_lc | G_E2E3);
                            if (p == G_E2 && q == G_E3E1) result_lc = (result_lc | G_E1E2E3);
                            if (p == G_E3 && q == G_E1)   result_lc = (result_lc | G_E3E1);
                            if (p == G_E3 && q == G_E2)   result_lc = (result_lc | G_E2E3);
                            if (p == G_E3 && q == G_E1E2) result_lc = (result_lc | G_E1E2E3);
                            if (p == G_E1E2 && q == G_E3) result_lc = (result_lc | G_E1E2E3);
                            if (p == G_E2E3 && q == G_E1) result_lc = (result_lc | G_E1E2E3);
                            if (p == G_E3E1 && q == G_E2) result_lc = (result_lc | G_E1E2E3);
                        }
                        q = (q << 1);
                    }
                    p = (p << 1);
                }
            }
            if (result_lc == 0) continue;
            result_lc = minimal_uber_space(result_lc);

            print_g(output, result_lc);
            fprintf(output, " outer(");
            print_g(output, lhs_lc);
            fprintf(output, " a, ");
            print_g(output, rhs_lc);
            fprintf(output, " b) { ");
            print_g(output, result_lc);
            fprintf(output, " r = %s;", result_lc == G_E0 ? "0.f" : "{}");

            {
                unsigned int p = 1;
                for (int k = 0; k < 8; k++)
                {
                    unsigned int q = 1;
                    for (int s = 0; s < 8; s++)
                    {
                        if (has(lhs_lc, p) && has(rhs_lc, q))
                        {
                            if (p == G_E0 && q == G_E0)   fprintf(output, " r%s = a%s * b%s;", result_lc == G_E0 ? "" : "._0", lhs_lc == G_E0 ? "" : "._0", rhs_lc == G_E0 ? "" : "._0");
                            if (p == q)                   goto skip_this_if;
                            if (p == G_E0)              {
                                                            fprintf(output, " r");
                                                            if (result_lc != G_E0)
                                                                fprintf(output, "._%d", s);
                                                            fprintf(output, " += b");
                                                            if (rhs_lc != G_E0)
                                                                fprintf(output, "._%d", s);
                                                            fprintf(output, ";");
                                                        }
                            if (q == G_E0)              {
                                                            fprintf(output, " r");
                                                            if (result_lc != G_E0)
                                                                fprintf(output, "._%d", k);
                                                            fprintf(output, " += a");
                                                            if (lhs_lc != G_E0)
                                                                fprintf(output, "._%d", k);
                                                            fprintf(output, ";");
                                                        }
                            if (p == G_E1 && q == G_E2)   fprintf(output, " r._4 += a._1 * b._2;");
                            if (p == G_E1 && q == G_E3)   fprintf(output, " r._6 -= a._1 * b._3;");
                            if (p == G_E1 && q == G_E2E3) fprintf(output, " r._7 += a._1 * b._5;");
                            if (p == G_E2 && q == G_E1)   fprintf(output, " r._4 -= a._2 * b._1;");
                            if (p == G_E2 && q == G_E3)   fprintf(output, " r._5 += a._2 * b._3;");
                            if (p == G_E2 && q == G_E3E1) fprintf(output, " r._7 += a._2 * b._6;");
                            if (p == G_E3 && q == G_E1)   fprintf(output, " r._6 -= a._3 * b._1;");
                            if (p == G_E3 && q == G_E2)   fprintf(output, " r._5 -= a._3 * b._2;");
                            if (p == G_E3 && q == G_E1E2) fprintf(output, " r._7 += a._3 * b._4;");
                            if (p == G_E1E2 && q == G_E3) fprintf(output, " r._7 += a._4 * b._3;");
                            if (p == G_E2E3 && q == G_E1) fprintf(output, " r._7 += a._5 * b._1;");
                            if (p == G_E3E1 && q == G_E2) fprintf(output, " r._7 += a._6 * b._2;");
                        }
                        skip_this_if:
                        q = (q << 1);
                    }
                    p = (p << 1);
                }
            }
            fprintf(output, " return r; }\n");
        }
    }
    fprintf(output, "\n");
}

void output_geometric_products(FILE *output)
{
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (i == 0 && j == 0) continue;
            // linear combination
            unsigned int lhs_lc = valid[i];
            unsigned int rhs_lc = valid[j];
            unsigned int result_lc = 0;

            {
                unsigned int p = 1;
                for (int k = 0; k < 8; k++)
                {
                    unsigned int q = 1;
                    for (int s = 0; s < 8; s++)
                    {
                        unsigned int r = 0;
                        if (has(lhs_lc, p) && has(rhs_lc, q))
                        {
                            if (p == q)                        { result_lc = (result_lc | G_E0); goto skip_this_if_2; }

                            if (p == G_E0)                       result_lc = (result_lc | q);
                            if (q == G_E0)                       result_lc = (result_lc | p);

                            if (p == G_E1 && q == G_E2)          result_lc = (result_lc | G_E1E2);
                            if (p == G_E1 && q == G_E3)          result_lc = (result_lc | G_E3E1);
                            if (p == G_E1 && q == G_E1E2)        result_lc = (result_lc | G_E2);
                            if (p == G_E1 && q == G_E2E3)        result_lc = (result_lc | G_E1E2E3);
                            if (p == G_E1 && q == G_E3E1)        result_lc = (result_lc | G_E3);
                            if (p == G_E1 && q == G_E1E2E3)      result_lc = (result_lc | G_E2E3);

                            if (p == G_E2 && q == G_E1)          result_lc = (result_lc | G_E1E2);
                            if (p == G_E2 && q == G_E3)          result_lc = (result_lc | G_E2E3);
                            if (p == G_E2 && q == G_E1E2)        result_lc = (result_lc | G_E1);
                            if (p == G_E2 && q == G_E2E3)        result_lc = (result_lc | G_E3);
                            if (p == G_E2 && q == G_E3E1)        result_lc = (result_lc | G_E1E2E3);
                            if (p == G_E2 && q == G_E1E2E3)      result_lc = (result_lc | G_E3E1);

                            if (p == G_E3 && q == G_E1)          result_lc = (result_lc | G_E3E1);
                            if (p == G_E3 && q == G_E2)          result_lc = (result_lc | G_E2E3);
                            if (p == G_E3 && q == G_E1E2)        result_lc = (result_lc | G_E1E2E3);
                            if (p == G_E3 && q == G_E2E3)        result_lc = (result_lc | G_E2);
                            if (p == G_E3 && q == G_E3E1)        result_lc = (result_lc | G_E1);
                            if (p == G_E3 && q == G_E1E2E3)      result_lc = (result_lc | G_E1E2);

                            if (p == G_E1E2 && q == G_E1)        result_lc = (result_lc | G_E2);
                            if (p == G_E1E2 && q == G_E2)        result_lc = (result_lc | G_E1);
                            if (p == G_E1E2 && q == G_E3)        result_lc = (result_lc | G_E1E2E3);
                            if (p == G_E1E2 && q == G_E2E3)      result_lc = (result_lc | G_E3E1);
                            if (p == G_E1E2 && q == G_E3E1)      result_lc = (result_lc | G_E2E3);
                            if (p == G_E1E2 && q == G_E1E2E3)    result_lc = (result_lc | G_E3);

                            if (p == G_E2E3 && q == G_E1)        result_lc = (result_lc | G_E1E2E3);
                            if (p == G_E2E3 && q == G_E2)        result_lc = (result_lc | G_E3);
                            if (p == G_E2E3 && q == G_E3)        result_lc = (result_lc | G_E2);
                            if (p == G_E2E3 && q == G_E1E2)      result_lc = (result_lc | G_E3E1);
                            if (p == G_E2E3 && q == G_E3E1)      result_lc = (result_lc | G_E1E2);
                            if (p == G_E2E3 && q == G_E1E2E3)    result_lc = (result_lc | G_E1);

                            if (p == G_E3E1 && q == G_E1)        result_lc = (result_lc | G_E3);
                            if (p == G_E3E1 && q == G_E2)        result_lc = (result_lc | G_E1E2E3);
                            if (p == G_E3E1 && q == G_E3)        result_lc = (result_lc | G_E1);
                            if (p == G_E3E1 && q == G_E1E2)      result_lc = (result_lc | G_E2E3);
                            if (p == G_E3E1 && q == G_E2E3)      result_lc = (result_lc | G_E1E2);
                            if (p == G_E3E1 && q == G_E1E2E3)    result_lc = (result_lc | G_E2);

                            if (p == G_E1E2E3 && q == G_E1)      result_lc = (result_lc | G_E2E3);
                            if (p == G_E1E2E3 && q == G_E2)      result_lc = (result_lc | G_E3E1);
                            if (p == G_E1E2E3 && q == G_E3)      result_lc = (result_lc | G_E1E2);
                            if (p == G_E1E2E3 && q == G_E1E2)    result_lc = (result_lc | G_E3);
                            if (p == G_E1E2E3 && q == G_E2E3)    result_lc = (result_lc | G_E1);
                            if (p == G_E1E2E3 && q == G_E3E1)    result_lc = (result_lc | G_E2);
                        }
                        skip_this_if_2:
                        q = (q << 1);
                    }
                    p = (p << 1);
                }
            }
            if (result_lc == 0) continue;
            result_lc = minimal_uber_space(result_lc);

            print_g(output, result_lc);
            fprintf(output, " operator * (");
            print_g(output, lhs_lc);
            fprintf(output, " a, ");
            print_g(output, rhs_lc);
            fprintf(output, " b) { ");
            print_g(output, result_lc);
            fprintf(output, " r = %s;", result_lc == G_E0 ? "0.f" : "{}");

            {
                unsigned int p = 1;
                for (int k = 0; k < 8; k++)
                {
                    unsigned int q = 1;
                    for (int s = 0; s < 8; s++)
                    {
                        if (has(lhs_lc, p) && has(rhs_lc, q))
                        {
                            if (p == q)                             {
                                                                        fprintf(output, " r%s %s= (a",
                                                                            (result_lc == G_E0) ? "" : "._0",
                                                                            (p == G_E1E2 || p == G_E2E3 || p == G_E3E1 || p == G_E1E2E3) ? "-" : "+");
                                                                        if (lhs_lc != G_E0)
                                                                            fprintf(output, "._%d", k);
                                                                        fprintf(output, " * b");
                                                                        if (rhs_lc != G_E0)
                                                                            fprintf(output, "._%d", s);
                                                                        fprintf(output, ");");
                                                                    }
                            else if (p == G_E0)                     {
                                                                        fprintf(output, " r");
                                                                        if (result_lc != G_E0)
                                                                            fprintf(output, "._%d", s);
                                                                        fprintf(output, " += (a");
                                                                        if (lhs_lc != G_E0)
                                                                            fprintf(output, "._%d", k);
                                                                        fprintf(output, " * b");
                                                                        if (rhs_lc != G_E0)
                                                                            fprintf(output, "._%d", s);
                                                                        fprintf(output, ");");
                                                                    }
                            else if (q == G_E0)                     {
                                                                        fprintf(output, " r");
                                                                        if (result_lc != G_E0)
                                                                            fprintf(output, "._%d", k);
                                                                        fprintf(output, " += (a");
                                                                        if (lhs_lc != G_E0)
                                                                            fprintf(output, "._%d", k);
                                                                        fprintf(output, " * b");
                                                                        if (rhs_lc != G_E0)
                                                                            fprintf(output, "._%d", s);
                                                                        fprintf(output, ");");
                                                                    }

                            else if (p == G_E1 && q == G_E2)          fprintf(output, " r._4 += (a._1 * b._2);");
                            else if (p == G_E1 && q == G_E3)          fprintf(output, " r._6 -= (a._1 * b._3);");
                            else if (p == G_E1 && q == G_E1E2)        fprintf(output, " r._2 += (a._1 * b._4);");
                            else if (p == G_E1 && q == G_E2E3)        fprintf(output, " r._7 += (a._1 * b._5);");
                            else if (p == G_E1 && q == G_E3E1)        fprintf(output, " r._3 -= (a._1 * b._6);");
                            else if (p == G_E1 && q == G_E1E2E3)      fprintf(output, " r._5 += (a._1 * b._7);");

                            else if (p == G_E2 && q == G_E1)          fprintf(output, " r._4 -= (a._2 * b._1);");
                            else if (p == G_E2 && q == G_E3)          fprintf(output, " r._5 += (a._2 * b._3);");
                            else if (p == G_E2 && q == G_E1E2)        fprintf(output, " r._1 -= (a._2 * b._4);");
                            else if (p == G_E2 && q == G_E2E3)        fprintf(output, " r._3 += (a._2 * b._5);");
                            else if (p == G_E2 && q == G_E3E1)        fprintf(output, " r._7 += (a._2 * b._6);");
                            else if (p == G_E2 && q == G_E1E2E3)      fprintf(output, " r._6 += (a._2 * b._7);");

                            else if (p == G_E3 && q == G_E1)          fprintf(output, " r._6 += (a._3 * b._1);");
                            else if (p == G_E3 && q == G_E2)          fprintf(output, " r._5 -= (a._3 * b._2);");
                            else if (p == G_E3 && q == G_E1E2)        fprintf(output, " r._7 += (a._3 * b._4);");
                            else if (p == G_E3 && q == G_E2E3)        fprintf(output, " r._2 -= (a._3 * b._5);");
                            else if (p == G_E3 && q == G_E3E1)        fprintf(output, " r._1 += (a._3 * b._6);");
                            else if (p == G_E3 && q == G_E1E2E3)      fprintf(output, " r._4 += (a._3 * b._7);");

                            else if (p == G_E1E2 && q == G_E1)        fprintf(output, " r._2 -= (a._4 * b._1);");
                            else if (p == G_E1E2 && q == G_E2)        fprintf(output, " r._1 += (a._4 * b._2);");
                            else if (p == G_E1E2 && q == G_E3)        fprintf(output, " r._7 += (a._4 * b._3);");
                            else if (p == G_E1E2 && q == G_E2E3)      fprintf(output, " r._6 -= (a._4 * b._5);");
                            else if (p == G_E1E2 && q == G_E3E1)      fprintf(output, " r._5 += (a._4 * b._6);");
                            else if (p == G_E1E2 && q == G_E1E2E3)    fprintf(output, " r._3 -= (a._4 * b._7);");

                            else if (p == G_E2E3 && q == G_E1)        fprintf(output, " r._7 += (a._5 * b._1);");
                            else if (p == G_E2E3 && q == G_E2)        fprintf(output, " r._3 -= (a._5 * b._2);");
                            else if (p == G_E2E3 && q == G_E3)        fprintf(output, " r._2 += (a._5 * b._3);");
                            else if (p == G_E2E3 && q == G_E1E2)      fprintf(output, " r._6 += (a._5 * b._4);");
                            else if (p == G_E2E3 && q == G_E3E1)      fprintf(output, " r._4 -= (a._5 * b._6);");
                            else if (p == G_E2E3 && q == G_E1E2E3)    fprintf(output, " r._1 -= (a._5 * b._7);");

                            else if (p == G_E3E1 && q == G_E1)        fprintf(output, " r._3 += (a._6 * b._1);");
                            else if (p == G_E3E1 && q == G_E2)        fprintf(output, " r._7 += (a._6 * b._2);");
                            else if (p == G_E3E1 && q == G_E3)        fprintf(output, " r._1 -= (a._6 * b._3);");
                            else if (p == G_E3E1 && q == G_E1E2)      fprintf(output, " r._5 -= (a._6 * b._4);");
                            else if (p == G_E3E1 && q == G_E2E3)      fprintf(output, " r._4 += (a._6 * b._5);");
                            else if (p == G_E3E1 && q == G_E1E2E3)    fprintf(output, " r._2 -= (a._6 * b._7);");

                            else if (p == G_E1E2E3 && q == G_E1)      fprintf(output, " r._5 += (a._7 * b._1);");
                            else if (p == G_E1E2E3 && q == G_E2)      fprintf(output, " r._6 += (a._7 * b._2);");
                            else if (p == G_E1E2E3 && q == G_E3)      fprintf(output, " r._4 += (a._7 * b._3);");
                            else if (p == G_E1E2E3 && q == G_E1E2)    fprintf(output, " r._3 -= (a._7 * b._4);");
                            else if (p == G_E1E2E3 && q == G_E2E3)    fprintf(output, " r._1 -= (a._7 * b._5);");
                            else if (p == G_E1E2E3 && q == G_E3E1)    fprintf(output, " r._2 -= (a._7 * b._6);");
                        }
                        q = (q << 1);
                    }
                    p = (p << 1);
                }
            }
            fprintf(output, " return r; }\n");
        }
        fprintf(output, "\n");
    }
}

void output_conjugate(FILE *output)
{
    for (int i = 1; i < 11; i++)
    {
        // linear combination
        unsigned int lc = valid[i];

        fprintf(output, "void conjugate(");
        print_g(output, lc);
        fprintf(output, "& a) { ");

        int p = 1;
        for (int k = 0; k < 8; k++)
        {
            bases b = (bases) p;
            if (has(lc, b) && (b >= G_E1E2)) fprintf(output, "a._%d = -a._%d; ", k, k);
            p = (p << 1);
        }
        fprintf(output, "}\n");
    }
    fprintf(output, "\n");
}

void output_conjugated(FILE *output)
{
    for (int i = 1; i < 11; i++)
    {
        // linear combination
        unsigned int lc = valid[i];

        print_g(output, lc);
        fprintf(output, " conjugated(");
        print_g(output, lc);
        fprintf(output, " a) { ");
        print_g(output, lc);
        fprintf(output, " r; ");

        int p = 1;
        for (int k = 0; k < 8; k++)
        {
            bases b = (bases) p;
            if (has(lc, b))
            {
                if (b >= G_E1E2)
                    fprintf(output, "r._%d = -a._%d; ", k, k);
                else
                    fprintf(output, "r._%d = a._%d; ", k, k);
            }
            p = (p << 1);
        }
        fprintf(output, "return r; }\n");
    }
    fprintf(output, "\n");
}

void output_equals_operators(FILE *output, char const *op, char const *con)
{
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (i == 0 && j == 0) continue;

            // linear combination
            unsigned int lhs_lc = valid[i];
            unsigned int rhs_lc = valid[j];

            fprintf(output, "bool32 operator %s (", op);
            print_g(output, lhs_lc);
            fprintf(output, " a, ");
            print_g(output, rhs_lc);
            fprintf(output, " b) { return ");

            bool first_printed = false;

            int p = 1;
            for (int k = 0; k < 8; k++)
            {
                bases b = (bases) p;

                if (has(lhs_lc, b) || has(rhs_lc, b))
                {
                    if (first_printed) fprintf(output, " %s ", con);

                    if (has(lhs_lc, b))
                    {
                        fprintf(output, "(a");
                        if (lhs_lc != G_E0) fprintf(output, "._%d", k);
                    }
                    else
                    {
                        fprintf(output, "(0.f");
                    }

                    if (has(rhs_lc, b))
                    {
                        fprintf(output, " %s b", op);
                        if (rhs_lc != G_E0) fprintf(output, "._%d", k);
                    }
                    else
                    {
                        fprintf(output, " %s 0.f", op);
                    }

                    fprintf(output, ")");
                    first_printed = true;
                }
                p = (p << 1);
            }
            if (!first_printed) fprintf(output, "false");

            fprintf(output, "; }\n");
        }
        fprintf(output, "\n");
    }
}

void output_equals_functions(FILE *output, char const *con)
{
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (i == 0 && j == 0) continue;

            // linear combination
            unsigned int lhs_lc = valid[i];
            unsigned int rhs_lc = valid[j];

            fprintf(output, "bool32 equal(");
            print_g(output, lhs_lc);
            fprintf(output, " a, ");
            print_g(output, rhs_lc);
            fprintf(output, " b) { return ");

            bool first_printed = false;

            int p = 1;
            for (int k = 0; k < 8; k++)
            {
                bases b = (bases) p;

                if (has(lhs_lc, b) || has(rhs_lc, b))
                {
                    if (first_printed) fprintf(output, " %s ", con);

                    if (has(lhs_lc, b))
                    {
                        fprintf(output, "::math::equal(a");
                        if (lhs_lc != G_E0) fprintf(output, "._%d", k);
                    }
                    else
                    {
                        fprintf(output, "::math::equal(0.f");
                    }

                    if (has(rhs_lc, b))
                    {
                        fprintf(output, ", b");
                        if (rhs_lc != G_E0) fprintf(output, "._%d", k);
                    }
                    else
                    {
                        fprintf(output, ", 0.f");
                    }

                    fprintf(output, ")");
                    first_printed = true;
                }
                p = (p << 1);
            }
            if (!first_printed) fprintf(output, "false");

            fprintf(output, "; }\n");
        }
        fprintf(output, "\n");
    }
}

void output_length_squared_functions(FILE *output)
{
    for (int i = 1; i < 11; i++)
    {
        // linear combination
        unsigned int lc = valid[i];

        fprintf(output, "float32 length_squared(");
        print_g(output, lc);
        fprintf(output, " a) { float32 r = inner(a, conjugated(a)); return r; }\n");
    }
    fprintf(output, "\n");
}

void output_length_functions(FILE *output)
{
    for (int i = 1; i < 11; i++)
    {
        // linear combination
        unsigned int lc = valid[i];

        fprintf(output, "float32 length(");
        print_g(output, lc);
        fprintf(output, " a) { ");

        if (lc == G_E0) fprintf(output, "return ::math::absolute(a);");
        else if (lc == G_E1) fprintf(output, "return ::math::absolute(a._1);");
        else if (lc == G_E2) fprintf(output, "return ::math::absolute(a._2);");
        else if (lc == G_E3) fprintf(output, "return ::math::absolute(a._3);");
        else if (lc == G_E1E2) fprintf(output, "return ::math::absolute(a._4);");
        else if (lc == G_E2E3) fprintf(output, "return ::math::absolute(a._5);");
        else if (lc == G_E3E1) fprintf(output, "return ::math::absolute(a._6);");
        else if (lc == G_E1E2E3) fprintf(output, "return ::math::absolute(a._7);");
        else fprintf(output, "return square_root(length_squared(a));");
        fprintf(output, " }\n");
    }
    fprintf(output, "\n");
}

void output_normalize_functions(FILE *output)
{
    for (int i = 1; i < 11; i++)
    {
        // linear combination
        unsigned int lc = valid[i];

        fprintf(output, "void normalize(");
        print_g(output, lc);
        fprintf(output, "& a) { float32 norm = length(a);");

        int p = 1;
        for (int k = 0; k < 8; k++)
        {
            bases b = (bases) p;

            if (has(lc, b))
            {
                fprintf(output, " a");
                if (lc != G_E0) fprintf(output, "._%d", k);
                fprintf(output, " /= norm;");
            }

            p = (p << 1);
        }
        fprintf(output, " }\n");
    }
    fprintf(output, "\n");
}

void output_normalized_functions(FILE *output)
{
    for (int i = 1; i < 11; i++)
    {
        // linear combination
        unsigned int lc = valid[i];

        print_g(output, lc);
        fprintf(output, " normalized(");
        print_g(output, lc);
        fprintf(output, " a) { normalize(a); return a; }\n");
    }
    fprintf(output, "\n");
}

void output_operators(FILE *output)
{
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            if (i == 0 && j == 0) continue; // Exclude f(float, float)

            // linear combination
            unsigned int lhs_lc = valid[i];
            unsigned int rhs_lc = valid[j];
            unsigned int result_lc = valid[i] | valid[j];
            result_lc = minimal_uber_space(result_lc);

            if (result_lc == 0xff) continue;

            print_g(output, result_lc);
            fprintf(output, " operator %s (", "+");
            print_g(output, lhs_lc);
            fprintf(output, " a, ");
            print_g(output, rhs_lc);
            fprintf(output, " b) { ");
            print_g(output, result_lc);
            fprintf(output, " r;");

            int p = 1;
            for (int k = 0; k < 8; k++)
            {
                bases b = (bases) p;
                if (has(result_lc, b))
                {
                    fprintf(output, " r");
                    if (result_lc != G_E0) fprintf(output, "._%d", k);
                    fprintf(output, " = ");

                    if (has(lhs_lc, b) && has(rhs_lc, b))
                    {
                        fprintf(output, "a");
                        if (lhs_lc != G_E0) fprintf(output, "._%d", k);

                        fprintf(output, " %s b", "+");
                        if (rhs_lc != G_E0) fprintf(output, "._%d", k);
                        fprintf(output, ";");
                    }
                    else if (has(lhs_lc, b) && !has(rhs_lc, b))
                    {
                        fprintf(output, "a");
                        if (lhs_lc != G_E0) fprintf(output, "._%d", k);
                        fprintf(output, ";");
                    }
                    else if (!has(lhs_lc, b) && has(rhs_lc, b))
                    {
                        fprintf(output, "b");
                        if (rhs_lc != G_E0) fprintf(output, "._%d", k);
                        fprintf(output, ";");
                    }
                    else
                    {
                        fprintf(output, "0.f;");
                    }
                }
                p = (p << 1);
            }
            fprintf(output, " return r; }\n");
        }
        fprintf(output, "\n");
    }
}

int main()
{
    FILE *f = fopen("g3_operators.cpp", "w+");

    output_unary_minus_operators(f);
    output_additive_operators(f, "+");
    output_additive_operators(f, "-");
    output_inner_products(f);
    output_outer_products(f);
    output_geometric_products(f);
    output_conjugate(f);
    output_conjugated(f);
    output_equals_operators(f, "==", "&&");
    output_equals_operators(f, "!=", "||");
    output_equals_functions(f, "&&");
    output_length_squared_functions(f);
    output_length_functions(f);
    output_normalize_functions(f);
    output_normalized_functions(f);

    output_operators(stdout);

    fclose(f);

    return 0;
}
