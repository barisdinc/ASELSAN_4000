#!/usr/bin/env python3
"""Compare per-function differential test outputs (C lifted vs. emulator)."""
import sys, collections
def load(p):
    tr = collections.OrderedDict(); cur = None
    for line in open(p):
        if line.startswith('T '): cur = line[2:].strip(); tr[cur] = [[], None]
        elif line.startswith('E '): tr[cur][1] = line[2:].split()
        else: tr[cur][0].append(line.strip())
    return tr
c, e = load(sys.argv[1]), load(sys.argv[2])
res = collections.Counter(); bad = []
for k in c:
    (tc, ec), (te, ee) = c[k], e.get(k, [[], ['?']])
    sc, se = ec[0], ee[0]
    if 'X' in (sc, se): res['skip (bad dispatch / garbage)'] += 1; continue
    if 'T' in (sc, se):
        n = min(len(tc), len(te))
        res['timeout, prefix ' + ('ok' if tc[:n] == te[:n] else 'DIFF')] += 1
        if tc[:n] != te[:n]: bad.append((k, 'prefix', sc, se))
        continue
    if tc == te and ec == ee: res['identical'] += 1
    elif tc == te: res['trace ok, end state differs'] += 1; bad.append((k, 'state', ec, ee))
    else: res['DIFF'] += 1; bad.append((k, 'trace', sc, se))
for k, v in sorted(res.items()): print('%-34s %d' % (k, v))
fn = collections.Counter(b[0].split()[0] for b in bad)
print('functions with problems:', len(fn))
for b in bad[:25]: print(' ', b)
