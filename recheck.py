import sys
# independent re-check: build adjacency from parent array, root anywhere, recursive DP with Python ints
def indpoly(n, par):
    adj = {v: [] for v in range(1, n+1)}
    for v in range(2, n+1):
        adj[v].append(par[v]); adj[par[v]].append(v)
    order, seen, stack = [], {1}, [1]
    parent = {1: 0}
    while stack:
        v = stack.pop(); order.append(v)
        for w in adj[v]:
            if w not in seen: seen.add(w); parent[w] = v; stack.append(w)
    def mul(p, q):
        r = [0]*(len(p)+len(q)-1)
        for i, a in enumerate(p):
            for j, b in enumerate(q): r[i+j] += a*b
        return r
    def add(p, q):
        m = max(len(p), len(q)); return [(p[i] if i < len(p) else 0)+(q[i] if i < len(q) else 0) for i in range(m)]
    out, inn = {}, {}
    for v in reversed(order):
        a, b = [1], [0, 1]
        for w in adj[v]:
            if parent.get(w) == v:
                a = mul(a, add(out[w], inn[w])); b = mul(b, out[w])
        out[v], inn[v] = a, b
    p = add(out[1], inn[1])
    while p[-1] == 0: p.pop()
    return p
ok = 0; shapes = set()
for line in open(sys.argv[1]):
    left, right = line.split('|')
    toks = left.split(); n = int(toks[1].split('=')[1]); parr = [0] + list(map(int, toks[3:]))
    coeffs = list(map(int, right.split(':')[1].split()))
    p = indpoly(n, parr)
    assert p == coeffs, line
    assert len(parr) == n+1 and n == 30
    lc_fail = [k for k in range(1, len(p)-1) if p[k]**2 < p[k-1]*p[k+1]]
    assert lc_fail
    m = p.index(max(p))
    assert all(p[i] <= p[i+1] for i in range(m)) and all(p[i] >= p[i+1] for i in range(m, len(p)-1))
    shapes.add(tuple(lc_fail)); ok += 1
    shapes_alpha = None
print("verified", ok, "trees; LC failure positions:", sorted(shapes))