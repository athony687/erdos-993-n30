/* Erdős Problem #993 checker.
   Linked into nauty's gentreeg via -DOUTPROC=check_tree -DSUMMARY=report.
   For each free tree, compute the independence polynomial by tree DP and
   check (1) unimodality and (2) log-concavity of its coefficient sequence. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAXV 64
typedef unsigned long long u64;

static u64 n_trees = 0, n_nonunimodal = 0, n_nonlogconcave = 0, n_badorder = 0;

/* A[v] : independent sets of subtree(v) with v NOT chosen
   B[v] : independent sets of subtree(v) with v chosen        */
static u64 A[MAXV + 1][MAXV + 1], B[MAXV + 1][MAXV + 1];
static int degA[MAXV + 1], degB[MAXV + 1];

static void polymul(u64 *dst, int *dd, const u64 *p, int dp, const u64 *q, int dq)
{
    u64 tmp[MAXV + 1];
    int i, j;
    memset(tmp, 0, sizeof(u64) * (dp + dq + 1));
    for (i = 0; i <= dp; i++) {
        if (!p[i]) continue;
        for (j = 0; j <= dq; j++) tmp[i + j] += p[i] * q[j];
    }
    *dd = dp + dq;
    memcpy(dst, tmp, sizeof(u64) * (*dd + 1));
}

static void dump(const char *tag, int vpar[], int n, const u64 *c, int d)
{
    int i;
    fprintf(stderr, "%s n=%d par:", tag, n);
    for (i = 1; i <= n; i++) fprintf(stderr, " %d", vpar[i]);
    fprintf(stderr, " | coeffs:");
    for (i = 0; i <= d; i++) fprintf(stderr, " %llu", c[i]);
    fprintf(stderr, "\n");
}

void check_tree(FILE *f, int vpar[], int n)
{
    int v, i, d;
    u64 S[MAXV + 1], T[MAXV + 1];
    (void)f;
    n_trees++;
    for (v = 1; v <= n; v++) {
        A[v][0] = 1; degA[v] = 0;           /* empty product = 1 */
        B[v][0] = 0; B[v][1] = 1; degB[v] = 1; /* x */
    }
    /* children are processed before parents: need vpar[v] < v */
    for (v = n; v >= 2; v--) {
        int p = vpar[v], ds;
        if (p >= v || p < 1) { n_badorder++; return; }
        /* S = A_v + B_v */
        ds = degA[v] > degB[v] ? degA[v] : degB[v];
        for (i = 0; i <= ds; i++)
            S[i] = (i <= degA[v] ? A[v][i] : 0) + (i <= degB[v] ? B[v][i] : 0);
        polymul(A[p], &degA[p], A[p], degA[p], S, ds);
        polymul(B[p], &degB[p], B[p], degB[p], A[v], degA[v]);
    }
    /* total = A_1 + B_1 */
    d = degA[1] > degB[1] ? degA[1] : degB[1];
    for (i = 0; i <= d; i++)
        T[i] = (i <= degA[1] ? A[1][i] : 0) + (i <= degB[1] ? B[1][i] : 0);
    while (d > 0 && T[d] == 0) d--;

    /* unimodality: once a strict decrease happens, no strict increase */
    {
        int dec = 0, bad = 0;
        for (i = 1; i <= d; i++) {
            if (T[i] < T[i - 1]) dec = 1;
            else if (T[i] > T[i - 1] && dec) { bad = 1; break; }
        }
        if (bad) { n_nonunimodal++; dump("NONUNIMODAL", vpar, n, T, d); }
    }
    /* log-concavity: T[i]^2 >= T[i-1]*T[i+1]  (use 128-bit) */
    {
        int bad = 0;
        for (i = 1; i < d; i++) {
            unsigned __int128 l = (unsigned __int128)T[i] * T[i];
            unsigned __int128 r = (unsigned __int128)T[i - 1] * T[i + 1];
            if (l < r) { bad = 1; break; }
        }
        if (bad) { n_nonlogconcave++; if (n_nonlogconcave <= 1000000) dump("NONLC", vpar, n, T, d); }
    }
}

void report(unsigned long long nout, double cpu)
{
    fprintf(stderr,
            "RESULT trees=%llu (gentreeg count %llu) nonunimodal=%llu nonlogconcave=%llu badorder=%llu cpu=%.2fs\n",
            n_trees, nout, n_nonunimodal, n_nonlogconcave, n_badorder, cpu);
}