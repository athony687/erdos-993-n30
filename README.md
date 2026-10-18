# Erdős Problem #993 — independent re-check at n = 30, with the non-log-concave trees

**Claim checked:** the independence sequence i_0(T), i_1(T), ... of every tree T is unimodal.

## Status of this result

Unimodality for all trees on up to **32** vertices was already established by Tyler Satchel Orden
([Tyorden/erdos-993-trees-n31](https://github.com/Tyorden/erdos-993-trees-n31); posted on the
[#993 forum thread](https://www.erdosproblems.com/forum/thread/993), n ≤ 31 on 2026-08-02 and n = 32 on 2026-08-09).
The earlier frontier n ≤ 29 is due to Reynolds ([BrettRey/erdos-problem-993](https://github.com/BrettRey/erdos-problem-993)).

This repository is **not** a new frontier. It contributes two things:

1. **An independent re-check of n = 30** with a separate implementation (in-process `OUTPROC` hook
   inside `gentreeg`, rather than `gentreeg | listg | parser`). All 14,830,871,802 trees on
   30 vertices have a unimodal independence sequence, in agreement with Orden.
2. **The complete list of non-log-concave trees on 30 vertices.** Exactly **121** trees on 30 vertices
   have an independence sequence that is not log-concave. In each case log-concavity fails at a single
   index: k = 15 for 114 trees and k = 16 for 7 trees. All 121 are still unimodal. For comparison,
   Reynolds reports 19 such trees at n = 28 (all at k = 14), and notes the n = 29 log-concavity audit as not
   yet completed. We have not seen the n = 30 count reported elsewhere. The 121 trees (parent arrays and
   coefficients) are in `nonlc30.txt`.

## Method
- Trees generated with nauty 2.8.8 `gentreeg` (Wright–Richmond–Odlyzko–McKay algorithm), split into 16 parts with `res/mod`.
- Each tree is processed in-process via gentreeg's `OUTPROC` hook (`unimodal.c`): independence polynomial by the standard tree DP (A_v = ∏(A_c + B_c), B_v = x ∏ A_c) with 64-bit integers; log-concavity tested with 128-bit products.
- Total CPU time: 22,949 s (about 6.4 h) on 2 cores.

## Checks
- Tree counts for n = 1, 2, 5, 10, 15, 20, 22, 23, 26, 30 match OEIS A000055 exactly (n = 30: 14,830,871,802).
- n = 26 reproduces the two known non-log-concave trees (Kadrawi–Levit–Yosef–Mizrachi).
- The coefficients, the log-concavity failure, and the unimodality of all 121 flagged trees were recomputed with an independent Python implementation (`recheck.py`, arbitrary-precision integers, different rooting):
  ```
  $ python recheck.py nonlc30.txt
  verified 121 trees; LC failure positions: [(15,), (16,)]
  ```

## Reproduce
```
gcc -O3 -o check -DOUTPROC=check_tree -DSUMMARY=report -I<nauty dir> \
    gentreeg.c gtools.c nauty.c nautil.c naugraph.c schreier.c naurng.c unimodal.c
for r in $(seq 0 15); do ./check -q 30 $r/16; done
```

## Disclosure
Code written and run with Claude (Anthropic) assistance; all numerical claims come from the executed runs.
