import sys
def load(path):
    mem={}
    for line in open(path):
        line=line.strip()
        if not line.startswith('S'): continue
        t=line[1]; b=bytes.fromhex(line[2:])
        n=b[0]
        if t=='1': a=int.from_bytes(b[1:3],'big'); d=b[3:n]
        elif t=='2': a=int.from_bytes(b[1:4],'big'); d=b[4:n]
        elif t=='3': a=int.from_bytes(b[1:5],'big'); d=b[5:n]
        else: continue
        for i,x in enumerate(d): mem[a+i]=x
    return mem
def ranges(mem):
    ks=sorted(mem); out=[]; s=p=ks[0]
    for k in ks[1:]:
        if k!=p+1: out.append((s,p)); s=k
        p=k
    out.append((s,p)); return out
if __name__=='__main__':
    for f in sys.argv[1:]:
        m=load(f); print(f, len(m))
        for s,e in ranges(m): print('  %06X-%06X (%d)'%(s,e,e-s+1))
