#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum g2_basis
{
    G2_E0   = 0x1,
    G2_E1   = 0x2,
    G2_E2   = 0x4,
    G2_E1E2 = 0x8,
};
char const *names[] =
{
    "(null)", "_e0", "_e1", "_e0_e1", "_e2", "_e0_e2", "_e1_e2", "_e0_e1_e2",
    "_e1e2", "_e0_e1e2", "_e1_e1e2", "_e0_e1_e1e2", "_e2_e1e2", "_e0_e2_e1e2", "_e1_e2_e1e2", "_e0_e1_e2_e1e2"
};

bool has(unsigned int b, g2_basis c)
{
    bool result = (b & c) > 0;
    return result;
}

void output_plus_minus_float(FILE *output, char const *op)
{
    for (int i = 1; i < 16; i++)
    {
        fprintf(output, "%s operator %s (%s a, float32 x) { %s r;", names[i | G2_E0], op, names[i], names[i | G2_E0]);
        int p = 1;
        for (int k = 0; k < 4; k++)
        {
            g2_basis b = (g2_basis) p;
            if (has(i, b))
            {
                if (k == 0) fprintf(output, " r._0 = a._0 %s x;", op);
                else fprintf(output, " r._%d = a._%d;", k, k);
            }
            else if (k == 0)
            {
                fprintf(output, " r._0 = 0.f %s x;", op);
            }
            p = (p << 1);
        }
        fprintf(output, " return r; }\n");
        fprintf(output, "%s operator %s (float32 x, %s a) { %s r;", names[i | G2_E0], op, names[i], names[i | G2_E0]);
        p = 1;
        for (int k = 0; k < 4; k++)
        {
            g2_basis b = (g2_basis) p;
            if (has(i, b))
            {
                if (k == 0) fprintf(output, " r._0 = a._0 %s x;", op);
                else fprintf(output, " r._%d = a._%d;", k, k);
            }
            else if (k == 0)
            {
                fprintf(output, " r._0 = 0.f %s x;", op);
            }
            p = (p << 1);
        }
        fprintf(output, " return r; }\n");
    }
    fprintf(output, "\n");
}

void output_multiply_float(FILE *output)
{
    for (int i = 1; i < 16; i++)
    {
        fprintf(output, "%s operator * (%s a, float32 x) { %s r;", names[i], names[i], names[i]);
        int p = 1;
        for (int k = 0; k < 4; k++)
        {
            g2_basis b = (g2_basis) p;
            if (has(i, b))
            {
                fprintf(output, " r._%d = x * a._%d;", k, k);
            }
            p = (p << 1);
        }
        fprintf(output, " return r; }\n");
        fprintf(output, "%s operator * (float32 x, %s a) { %s r;", names[i], names[i], names[i]);
        p = 1;
        for (int k = 0; k < 4; k++)
        {
            g2_basis b = (g2_basis) p;
            if (has(i, b))
            {
                fprintf(output, " r._%d = x * a._%d;", k, k);
            }
            p = (p << 1);
        }
        fprintf(output, " return r; }\n");
    }
    fprintf(output, "\n");
}

void output_divide_float(FILE *output)
{
    for (int i = 1; i < 16; i++)
    {
        fprintf(output, "%s operator / (%s a, float32 x) { %s r;", names[i], names[i], names[i]);
        int p = 1;
        for (int k = 0; k < 4; k++)
        {
            g2_basis b = (g2_basis) p;
            if (has(i, b))
            {
                fprintf(output, " r._%d = x / a._%d;", k, k);
            }
            p = (p << 1);
        }
        fprintf(output, " return r; }\n");
    }
    fprintf(output, "\n");
}

void output_unary_minus_operators(FILE *output)
{
    for (int i = 1; i < 16; i++)
    {
        fprintf(output, "%s operator - (%s a) { %s r; ", names[i], names[i], names[i]);
        int p = 1;
        for (int k = 0; k < 4; k++)
        {
            g2_basis b = (g2_basis) p;
            if (has(i, b)) fprintf(output, "r._%d = -a._%d; ", k, k);
            p = (p << 1);
        }
        fprintf(output, "return r; }\n");
    }
    fprintf(output, "\n");
}

void output_additive_operators(FILE *output, char const *op)
{
    for (int i = 1; i < 16; i++)
    {
        for (int j = 1; j < 16; j++)
        {
            fprintf(output, "%s operator %s (%s a, %s b) ", names[i | j], op, names[i], names[j]);
            fprintf(output, "{ %s r; ", names[i | j]);

            int p = 1;
            for (int k = 0; k < 4; k++)
            {
                g2_basis b = (g2_basis) p;

                if (has(i | j, b))
                {
                    fprintf(output, "r._%d = ", k);
                    if (has(i, b) && has(j, b)) fprintf(output, "a._%d %s b._%d; ", k, op, k);
                    else if (has(i, b) && !has(j, b)) fprintf(output, "a._%d; ", k);
                    else if (!has(i, b) && has(j, b)) fprintf(output, "b._%d; ", k);
                }

                p = (p << 1);
            }
            fprintf(output, "return r; }\n");
        }
        fprintf(output, "\n");
    }
}

void output_inner_products(FILE *output)
{
    for (int i = 1; i < 16; i++)
    {
        for (int j = 1; j < 16; j++)
        {
            fprintf(output, "_e0 inner(%s a, %s b) { _e0 r; r._0 = 0.f", names[i], names[j]);

            int p = 1;
            for (int k = 0; k < 4; k++)
            {
                g2_basis b = (g2_basis) p;

                if (has(i, b) && has(j, b))
                {
                    if (k == 3)
                        fprintf(output, " - (a._%d * b._%d)", k, k);
                    else
                        fprintf(output, " + (a._%d * b._%d)", k, k);
                }

                p = (p << 1);
            }
            fprintf(output, "; return r; }\n");
        }
        fprintf(output, "\n");
    }
}

void output_outer_products(FILE *output)
{
    for (int i = 1; i < 16; i++)
    {
        for (int j = 1; j < 16; j++)
        {
            unsigned int result_b = 1;
            int u = 1;
            for (int k = 0; k < 4; k++)
            {
                int v = 1;
                for (int s = 0; s < 4; s++)
                {
                    if (has(i, (g2_basis) u) && has(j, (g2_basis) v))
                    {
                        if (k == 0)
                            result_b = result_b | v;
                        else if (s == 0)
                            result_b = result_b | u;
                        else if (k == s)
                            {}
                        else if (k == 1 && s == 2)
                            result_b = result_b | G2_E1E2;
                        else if (k == 2 && s == 1)
                            result_b = result_b | G2_E1E2;
                        else
                            {}
                    }
                    v = (v << 1);
                }
                u = (u << 1);
            }

            fprintf(output, "%s outer(%s a, %s b) { %s r = {}; ", names[result_b], names[i], names[j], names[result_b]);

            u = 1;
            for (int k = 0; k < 4; k++)
            {
                g2_basis b = (g2_basis) u;
                int v = 1;
                for (int s = 0; s < 4; s++)
                {
                    g2_basis e = (g2_basis) v;

                    if (has(i, b) && has(j, e))
                    {
                        if (k == 0)
                            fprintf(output, " r._%d += a._0 * b._%d;", s, s);
                        else if (s == 0)
                            fprintf(output, " r._%d += a._%d * b._0;", k, k);
                        else if (k == s)
                            {}
                        else if (k == 1 && s == 2)
                            fprintf(output, " r._3 += a._1 * b._2;");
                        else if (k == 2 && s == 1)
                            fprintf(output, " r._3 -= a._2 * b._1;");
                        else
                            {}
                    }

                    v = (v << 1);
                }
                u = (u << 1);
            }
            fprintf(output, " return r; }\n");
        }
        fprintf(output, "\n");
    }
}

void output_geometric_products(FILE *output)
{
    for (int i = 1; i < 16; i++)
    {
        for (int j = 1; j < 16; j++)
        {
            unsigned int result_b = 0;
            int u = 1;
            for (int k = 0; k < 4; k++)
            {
                int v = 1;
                for (int s = 0; s < 4; s++)
                {
                    if (has(i, (g2_basis) u) && has(j, (g2_basis) v))
                    {
                        if (k == 0 && s != 0)
                            result_b = result_b | v;
                        else if (k != 0 && s == 0)
                            result_b = result_b | u;
                        else if (k == s)
                            result_b = result_b | G2_E0;
                        else if (k == 1 && s == 2)
                            result_b = result_b | G2_E1E2;
                        else if (k == 1 && s == 3)
                            result_b = result_b | G2_E2;
                        else if (k == 2 && s == 1)
                            result_b = result_b | G2_E1E2;
                        else if (k == 2 && s == 3)
                            result_b = result_b | G2_E1;
                        else if (k == 3 && s == 1)
                            result_b = result_b | G2_E2;
                        else if (k == 3 && s == 2)
                            result_b = result_b | G2_E1;
                        else
                            result_b = result_b | (u ^ v);
                    }
                    v = (v << 1);
                }
                u = (u << 1);
            }

            fprintf(output, "%s operator * (%s a, %s b) { %s r = {};", names[result_b], names[i], names[j], names[result_b]);

            int p = 1;
            for (int k = 0; k < 4; k++)
            {
                g2_basis b = (g2_basis) p;

                int q = 1;
                for (int s = 0; s < 4; s++)
                {
                    g2_basis e = (g2_basis) q;

                    if (has(i, b) && has(j, e))
                    {
                        if (k == 0)
                            fprintf(output, " r._%d += a._0 * b._%d;", s, s);
                        else if (s == 0)
                            fprintf(output, " r._%d += a._%d * b._0;", k, k);
                        else if (k == 3 && s == 3)
                            fprintf(output, " r._0 -= a._%d * b._%d;", k, k);
                        else if (k == s)
                            fprintf(output, " r._0 += a._%d * b._%d;", k, k);
                        else if (k == 1 && s == 2)
                            fprintf(output, " r._3 += a._1 * b._2;");
                        else if (k == 1 && s == 3)
                            fprintf(output, " r._2 += a._1 * b._3;");
                        else if (k == 2 && s == 1)
                            fprintf(output, " r._3 -= a._2 * b._1;");
                        else if (k == 2 && s == 3)
                            fprintf(output, " r._1 -= a._2 * b._3;");
                        else if (k == 3 && s == 1)
                            fprintf(output, " r._2 -= a._3 * b._1;");
                        else if (k == 3 && s == 2)
                            fprintf(output, " r._1 += a._3 * b._2;");
                        else
                            fprintf(output, "\n#error generator did bad!\n");
                    }

                    q = (q << 1);
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
    FILE *f = fopen("g2_operators.cpp", "w+");

    output_plus_minus_float(f, "+");
    output_plus_minus_float(f, "-");
    output_multiply_float(f);
    output_divide_float(f);

    output_unary_minus_operators(f);
    output_additive_operators(f, "+");
    output_additive_operators(f, "-");
    output_inner_products(f);
    output_outer_products(f);
    output_geometric_products(f);
    fclose(f);

    return 0;
}
