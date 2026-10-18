# Erdős Problem #993 — non-log-concave trees up to n = 30, and a re-check of unimodality at n ≤ 30

**Claim checked:** the independence sequence i_0(T), i_1(T), ... of every tree T is unimodal.

## Status of this result

Unimodality for all trees on up to **32** vertices was already established by Tyler Satchel Orden
([Tyorden/erdos-993-trees-n31](https://github.com/Tyorden/erdos-993-trees-n31); posted on the
[#993 forum thread](https://www.erdosproblems.com/forum/thread/993), n ≤ 31 on 2026-08-02 and n = 32 on 2026-08-09).
The earlier frontier n ≤ 29 is due to Reynolds ([BrettRey/erdos-problem-993](https://github.com/BrettRey/erdos-problem-993)).

This repository is **not** a new unimodality frontier. It contributes:

1. **An independent re-check of unimodality for every n ≤ 30** with a separate implementation (in-process
   `OUTPROC` hook inside `gentreeg`, rather than `gentreeg | listg | parser`). No counterexample, in agreement
   with Orden and Reynolds.
2. **The number of trees whose independence sequence is not log-concave, for every n ≤ 30, with complete lists
   for n = 28, 29, 30.** We have not seen the n = 29 and n = 30 counts reported elsewhere, and the sequence is
   not in the OEIS (searched 2026-10-18).

## Non-log-concave trees on n vertices

| n | trees (OEIS A000055) | non-unimodal | non-log-concave | failure index k | list |
|---|---|---|---|---|---|
| 1–25 | | 0 | 0 | | |
| 26 | 279,793,450 | 0 | 2 | | |
| 27 | 751,065,460 | 0 | 0 | | |
| 28 | 2,023,443,032 | 0 | 19 | 14 (all 19) | `nonlc28.txt` |
| 29 | 5,469,566,585 | 0 | 7 | 15 (all 7) | `nonlc29.txt` |
| 30 | 14,830,871,802 | 0 | 121 | 15 (114), 16 (7) | `nonlc30.txt` |

Sequence a(n), n = 1..30: 0 (×25), 2, 0, 19, 7, 121.

- n = 26 reproduces the two non-log-concave trees of Kadrawi–Levit–Yosef–Mizrachi; n = 28 reproduces Reynolds's count of 19.
- In every case log-concavity fails at a single index near the tail, and every one of these trees is still unimodal.
- Each `nonlcNN.txt` line gives a parent array (vertex 1 is the root, 0 marks no parent) and the coefficients i_0 … i_α.

## Method
- Trees generated with nauty 2.8.8 `gentreeg` (Wright–Richmond–Odlyzko–McKay algorithm), split into 16 parts with `res/mod`.
- Each tree is processed in-process via gentreeg's `OUTPROC` hook (`unimodal.c`): independence polynomial by the standard tree DP (A_v = ∏(A_c + B_c), B_v = x ∏ A_c) with 64-bit integers; log-concavity tested with 128-bit products.
- n = 30: 22,949 CPU-seconds on 2 cores. n ≤ 29: run on a 16-thread PC (WSL Ubuntu, gcc 15); n = 28 and 29 together took 39 minutes wall-clock.

## Checks
- Tree counts for n = 4..30 match OEIS A000055 exactly. (For n ≤ 3, gentreeg's `res/mod` split repeats the single tree in several parts; this does not affect the counts above, which are all 0 there.)
- The coefficients, the log-concavity failure, and the unimodality of every listed tree (n = 28, 29, 30) were recomputed with an independent Python implementation (`recheck.py`, arbitrary-precision integers, different rooting):
  ```
  $ python recheck.py nonlc28.txt
  verified 19 trees; LC failure positions: [(14,)]
  $ python recheck.py nonlc29.txt
  verified 7 trees; LC failure positions: [(15,)]
  $ python recheck.py nonlc30.txt
  verified 121 trees; LC failure positions: [(15,), (16,)]
  ```

## Reproduce
Put `unimodal.c`, `proto.h` and `run.sh` next to an unpacked `nauty2_8_8/`, then:
```
./run.sh 26 27 28 29 30
```
`run.sh` builds the checker (`gcc -O3 -DOUTPROC=check_tree -DSUMMARY=report -include proto.h ...`), runs each order in
16 parallel parts, and prints a `SUMMARY` line per order; the flagged trees go to `out/nonlcNN.txt`.

## Disclosure
Code written and run with Claude (Anthropic) assistance; all numerical claims come from the executed runs.
