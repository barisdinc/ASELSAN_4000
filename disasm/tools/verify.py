import sys,re; import os; sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import srec
H=os.path.join(os.path.dirname(os.path.abspath(__file__)),'..','..')+'/'
for c in 'DEKPRSVY':
    mem=srec.load(H+'YA4000%s.HEX'%c)
    got={}; dup=0; lab={}
    lines=open(H+'disasm/YA4000%s.asm'%c).read().split('\n')
    for l in lines:
        m=re.match(r'^([A-Za-z_][A-Za-z0-9_]*):',l)
        if m: pass
    labels={}
    for i,l in enumerate(lines):
        m=re.match(r'^([A-Za-z_]\w*):$',l)
        if m:
            for l2 in lines[i+1:i+4]:
                mm=re.search(r'; ([0-9A-F]{6})[: ]',l2)
                if mm and not l2.startswith(';'): labels[m.group(1)]=int(mm.group(1),16); break
    for l in lines:
        if l.startswith(';'): continue
        m=re.search(r'; ([0-9A-F]{6}): ((?:[0-9A-F]{2} ?)+)(?:  \[.*\])?$',l)
        if m:
            a=int(m.group(1),16)
            for k,b in enumerate(m.group(2).split()):
                if a+k in got: dup+=1
                got[a+k]=int(b,16)
            continue
        m=re.match(r'\s+\.BYTE (\S+)\s+; ([0-9A-F]{6})',l)
        if m:
            a=int(m.group(2),16)
            for k,b in enumerate(m.group(1).split(',')):
                if a+k in got: dup+=1
                got[a+k]=int(b[1:],16)
            continue
        m=re.match(r'\s+\.WORD (\S+)\s+; ([0-9A-F]{6})',l)
        if m:
            a=int(m.group(2),16); v=m.group(1)
            v=int(v[1:],16) if v.startswith('$') else labels[v]&0xFFFF
            for k,b in enumerate((v&0xFF,v>>8)):
                if a+k in got: dup+=1
                got[a+k]=b
    # overlay copy at 0x80.. duplicates ROM bytes: compare against mem plus overlay
    ov={a-0x10100+0x80:mem[a] for a in range(0x10100,0x105C8) if a in mem}
    ref=dict(mem); ref.update(ov)
    miss=[a for a in ref if a not in got]; bad=[a for a in got if ref.get(a)!=got[a]]
    print(c,'bytes',len(ref),'listed',len(got),'missing',len(miss),'mismatch',len(bad),'dup',dup, ['%06X'%a for a in (miss+bad)[:5]])
