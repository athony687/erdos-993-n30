# Erdős Problem #993 — exhaustive check of all trees on 30 vertices

**Claim checked:** the independence sequence i_0(T), i_1(T), ... of every tree T is unimodal.

**Result (2026-10-18):** All 14,830,871,802 non-isomorphic trees on 30 vertices have a unimodal independence sequence. No counterexample exists with 30 vertices. Previously published exhaustive range: n ≤ 29 (Reynolds, Zenodo 10.5281/zenodo.19100781, 2026-03).

**By-product:** exactly 121 trees on 30 vertices have an independence sequence that is not log-concave. In every case the failure is at a single index k = 15 or k = 16, and the sequence is still unimodal. The 121 trees (parent arrays and coefficients) are in `nonlc30.txt`.

## Method
- Trees generated with nauty 2.8.8 `gentreeg` (Wright–Richmond–Odlyzko–McKay algorithm), split into 16 parts with `res/mod`.
- Each tree is processed in-process via gentreeg's `OUTPROC` hook (`unimodal.c`): independence polynomial by the standard tree DP (A_v = ∏(A_c + B_c), B_v = x ∏ A_c) with 64-bit integers; log-concavity tested with 128-bit products.
- Total CPU time: 22,949 s (about 6.4 h) on 2 cores.

## Checks
- Tree counts for n = 1, 2, 5, 10, 15, 20, 22, 23, 26, 30 match OEIS A000055 exactly (n = 30: 14,830,871,802).
- n = 26 reproduces the two known non-log-concave trees (Kadrawi–Levit–Yosef–Mizrachi).
- The coefficients, the log-concavity failure, and the unimodality of all 121 flagged trees were recomputed with an independent Python implementation (`recheck.py`, arbitrary-precision integers, different rooting).

## Reproduce
```
gcc -O3 -o check -DOUTPROC=check_tree -DSUMMARY=report -I<nauty dir> \
    gentreeg.c gtools.c nauty.c nautil.c naugraph.c schreier.c naurng.c unimodal.c
for r in $(seq 0 15); do ./check -q 30 $r/16; done
```