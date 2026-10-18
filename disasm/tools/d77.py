#!/usr/bin/env python3
"""Mitsubishi MELPS 7700 (M37732) recursive-descent disassembler with M/X flag tracking."""
import sys, collections
import os; sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import srec

# ---------------------------------------------------------------- opcode tables
# mode codes:
#  imp, acc, immM, immX, imm8, dp, dpx, dpy, ind, indx, indy, lind, lindy, sr, sry,
#  abs, absx, absy, long, longx, rel8, rel16, jabs, jlong, jind, jindx, jlind,
#  bitdp, bitabs, bbdp, bbabs, ldmdp, ldmdpx, ldmabs, ldmabsx, mv, brk
OPS = {}
def alu(base, name, store=False):
    tbl = {0x01:'indx',0x03:'sr',0x05:'dp',0x07:'lind',0x09:'immM',0x0D:'abs',0x0F:'long',
           0x11:'indy',0x12:'ind',0x13:'sry',0x15:'dpx',0x17:'lindy',0x19:'absy',0x1D:'absx',0x1F:'longx'}
    for k,v in tbl.items():
        if store and k==0x09: continue
        OPS[base+k]=(name,v)
for b,n in [(0x00,'ORA'),(0x20,'AND'),(0x40,'EOR'),(0x60,'ADC'),(0xA0,'LDA'),(0xC0,'CMP'),(0xE0,'SBC')]: alu(b,n)
alu(0x80,'STA',store=True)
for b,n in [(0x00,'ASL'),(0x20,'ROL'),(0x40,'LSR'),(0x60,'ROR')]:
    OPS[b+0x06]=(n,'dp');OPS[b+0x0E]=(n,'abs');OPS[b+0x16]=(n,'dpx');OPS[b+0x1E]=(n,'absx');OPS[b+0x0A]=(n,'acc')
for b,n in [(0xC0,'DEC'),(0xE0,'INC')]:
    OPS[b+0x06]=(n,'dp');OPS[b+0x0E]=(n,'abs');OPS[b+0x16]=(n,'dpx');OPS[b+0x1E]=(n,'absx')
OPS.update({
 0x00:('BRK','brk'),0x1A:('DEC','acc'),0x3A:('INC','acc'),
 0x04:('SEB','bitdp'),0x0C:('SEB','bitabs'),0x14:('CLB','bitdp'),0x1C:('CLB','bitabs'),
 0x24:('BBS','bbdp'),0x2C:('BBS','bbabs'),0x34:('BBC','bbdp'),0x3C:('BBC','bbabs'),
 0xE8:('INX','imp'),0xC8:('INY','imp'),0xCA:('DEX','imp'),0x88:('DEY','imp'),
 0xE0:('CPX','immX'),0xE4:('CPX','dp'),0xEC:('CPX','abs'),
 0xC0:('CPY','immX'),0xC4:('CPY','dp'),0xCC:('CPY','abs'),
 0xA2:('LDX','immX'),0xA6:('LDX','dp'),0xAE:('LDX','abs'),0xB6:('LDX','dpy'),0xBE:('LDX','absy'),
 0xA0:('LDY','immX'),0xA4:('LDY','dp'),0xAC:('LDY','abs'),0xB4:('LDY','dpx'),0xBC:('LDY','absx'),
 0x86:('STX','dp'),0x8E:('STX','abs'),0x96:('STX','dpy'),
 0x84:('STY','dp'),0x8C:('STY','abs'),0x94:('STY','dpx'),
 0x64:('LDM','ldmdp'),0x74:('LDM','ldmdpx'),0x9C:('LDM','ldmabs'),0x9E:('LDM','ldmabsx'),
 0x10:('BPL','rel8'),0x30:('BMI','rel8'),0x50:('BVC','rel8'),0x70:('BVS','rel8'),
 0x90:('BCC','rel8'),0xB0:('BCS','rel8'),0xD0:('BNE','rel8'),0xF0:('BEQ','rel8'),
 0x80:('BRA','rel8'),0x82:('BRAL','rel16'),
 0x20:('JSR','jabs'),0x22:('JSRL','jlong'),0xFC:('JSR','jindx'),
 0x4C:('JMP','jabs'),0x5C:('JMPL','jlong'),0x6C:('JMP','jind'),0x7C:('JMP','jindx'),0xDC:('JMPL','jlind'),
 0x60:('RTS','imp'),0x6B:('RTL','imp'),0x40:('RTI','imp'),
 0x08:('PHP','imp'),0x28:('PLP','imp'),0x48:('PHA','imp'),0x68:('PLA','imp'),
 0x0B:('PHD','imp'),0x2B:('PLD','imp'),0x4B:('PHG','imp'),0x8B:('PHT','imp'),0xAB:('PLT','imp'),
 0x5A:('PHY','imp'),0x7A:('PLY','imp'),0xDA:('PHX','imp'),0xFA:('PLX','imp'),
 0xD4:('PEI','dp'),0xF4:('PEA','imm16'),0x62:('PER','rel16'),
 0x18:('CLC','imp'),0x38:('SEC','imp'),0x58:('CLI','imp'),0x78:('SEI','imp'),0xB8:('CLV','imp'),
 0xD8:('CLM','imp'),0xF8:('SEM','imp'),0xC2:('CLP','imm8'),0xE2:('SEP','imm8'),
 0xAA:('TAX','imp'),0xA8:('TAY','imp'),0x8A:('TXA','imp'),0x98:('TYA','imp'),0x9A:('TXS','imp'),
 0xBA:('TSX','imp'),0x9B:('TXY','imp'),0xBB:('TYX','imp'),
 0x1B:('TAS','imp'),0x3B:('TSA','imp'),0x5B:('TAD','imp'),0x7B:('TDA','imp'),
 0x44:('MVP','mv'),0x54:('MVN','mv'),
 0xCB:('WIT','imp'),0xDB:('STP','imp'),0xEA:('NOP','imp'),0xEB:('PSH','imm8'),0xFB:('PUL','imm8'),
})
# accumulator-B prefix (0x42): ops that take an accumulator
ACC_OPS = {'ORA','AND','EOR','ADC','STA','LDA','CMP','SBC'}
B_IMP = {0x0A:('ASL','accB'),0x1A:('DEC','accB'),0x2A:('ROL','accB'),0x3A:('INC','accB'),
         0x4A:('LSR','accB'),0x6A:('ROR','accB'),0x48:('PHB','imp'),0x68:('PLB','imp'),
         0x8A:('TXB','imp'),0x98:('TYB','imp'),0xAA:('TBX','imp'),0xA8:('TBY','imp'),
         0x1B:('TBS','imp'),0x3B:('TSB','imp'),0x5B:('TBD','imp'),0x7B:('TDB','imp')}
# 0x89 prefix
P89 = {}
for k,v in {0x01:'indx',0x03:'sr',0x05:'dp',0x07:'lind',0x09:'immM',0x0D:'abs',0x0F:'long',
            0x11:'indy',0x12:'ind',0x13:'sry',0x15:'dpx',0x17:'lindy',0x19:'absy',0x1D:'absx',0x1F:'longx'}.items():
    P89[k]=('MPY',v); P89[0x20+k]=('DIV',v)
P89.update({0x28:('XAB','imp'),0x49:('RLA','immM'),0xC2:('LDT','imm8')})

# ---------------------------------------------------------------- SFR names (M37732 / 7700 family)
SFR = {0x0A:'P4',0x0B:'P5',0x0C:'P4D',0x0D:'P5D',0x0E:'P6',0x0F:'P7',0x10:'P6D',0x11:'P7D',0x12:'P8',0x14:'P8D',
 0x1E:'ADCON',0x1F:'ADSWP',0x20:'AD0',0x22:'AD1',0x24:'AD2',0x26:'AD3',0x28:'AD4',0x2A:'AD5',0x2C:'AD6',0x2E:'AD7',
 0x30:'U0MOD',0x31:'U0BRG',0x32:'U0TB',0x33:'U0TB_H',0x34:'U0C0',0x35:'U0C1',0x36:'U0RB',0x37:'U0RB_H',
 0x38:'U1MOD',0x39:'U1BRG',0x3A:'U1TB',0x3B:'U1TB_H',0x3C:'U1C0',0x3D:'U1C1',0x3E:'U1RB',0x3F:'U1RB_H',
 0x40:'TCSTART',0x42:'TOSTART',0x44:'TUDF',0x46:'TA0',0x48:'TA1',0x4A:'TA2',0x4C:'TA3',0x4E:'TA4',
 0x50:'TB0',0x52:'TB1',0x54:'TB2',0x56:'TA0MOD',0x57:'TA1MOD',0x58:'TA2MOD',0x59:'TA3MOD',0x5A:'TA4MOD',
 0x5B:'TB0MOD',0x5C:'TB1MOD',0x5D:'TB2MOD',0x5E:'PMOD',0x60:'WDT',0x61:'WDTFS',0x62:'WAVEMOD',0x64:'PULSE1',0x65:'PULSE0',
 0x70:'ADIC',0x71:'U0TIC',0x72:'U0RIC',0x73:'U1TIC',0x74:'U1RIC',0x75:'TA0IC',0x76:'TA1IC',0x77:'TA2IC',
 0x78:'TA3IC',0x79:'TA4IC',0x7A:'TB0IC',0x7B:'TB1IC',0x7C:'TB2IC',0x7D:'INT0IC',0x7E:'INT1IC',0x7F:'INT2IC'}
for k,v in list(SFR.items()):
    if k in (0x46,0x48,0x4A,0x4C,0x4E,0x50,0x52,0x54,0x20,0x22,0x24,0x26,0x28,0x2A,0x2C,0x2E): SFR[k+1]=v+'_H'
PORTBITS = {
 0x0A:{3:'P43 KRISTAL_KAYDIRMA',4:'P44 KONTAK_ANAHTARI',5:'P45 ACIL_CAGRI',6:'P46 SUSTURMA_BILGISI',7:'P47 DIS_BAS_KONUS'},
 0x0B:{0:'P50 UYARI',1:'P51 TDSEC1',2:'P52 SC_TON',3:'P53 TDSEC2',4:'P54 TKS_URET/SUSTURMA_AYAR(PWM)',5:'P55 ALMA_SEC',6:'P56 GUC_AYAR(PWM)',7:'P57 SUSTURMA'},
 0x0E:{0:'P60 UZAK_VERI_GIRIS',1:'P61 GOSTERGE_SAAT(SCL)',2:'P62 GOSTERGE_VERI(SDA)',3:'P63 OKUMA_ISTEK0(INT1)',4:'P64 OKUMA_ISTEK1(INT2)',5:'P65 TKS_COZME(TB0IN)',6:'P66 SC_COZME(TB1IN)',7:'P67 SENTEZOR_KILIT'},
 0x0F:{0:'P70 SENTEZOR_YUKLE',1:'P71 GUC_ACMA',2:'P72 GONDERME_SES(AN2)',3:'P73 SICAKLIK(AN3)',4:'P74 13V2_OLCUM(AN4)',5:'P75 RSSI(AN5)',6:'P76 YANSIYAN_GUC(AN6)',7:'P77 GIDEN_GUC(AN7)'},
 0x12:{1:'P81 SENTEZOR_SEC',2:'P82 RS232_RX(RxD0)',3:'P83 RS232_TX(TxD0)',5:'P85 CBUS/SENT_SAAT(CLK1)',6:'P86 CBUS_YANIT(RxD1)',7:'P87 CBUS/SENT_VERI(TxD1)'},
}
for _d,_p in ((0x0C,0x0A),(0x0D,0x0B),(0x10,0x0E),(0x11,0x0F),(0x14,0x12)): PORTBITS[_d]={k:v+' yon' for k,v in PORTBITS[_p].items()}
ADNAMES={0x24:'AN2 GONDERME_SES',0x26:'AN3 SICAKLIK',0x28:'AN4 13V2',0x2A:'AN5 RSSI',0x2C:'AN6 YANSIYAN_GUC',0x2E:'AN7 GIDEN_GUC'}
def hwnote(ins, mem, a):
    m=ins['mode']
    if m in ('bitabs','bbabs','ldmabs'):
        adr=mem[a+1]|mem[a+2]<<8
        if adr in PORTBITS and m!='ldmabs':
            mk=mem[a+3] if ins['mx'][0] else mem[a+3]|mem[a+4]<<8
            names=[PORTBITS[adr].get(b,'P%X%d'%(adr,b)) for b in range(8) if mk>>b&1]
            return ' '.join(names)
    if m=='abs':
        adr=mem[a+1]|mem[a+2]<<8
        if adr in ADNAMES: return ADNAMES[adr]
    return ''

VECTORS = [(0xFFD6,'AD'),(0xFFD8,'UART1_TX'),(0xFFDA,'UART1_RX'),(0xFFDC,'UART0_TX'),(0xFFDE,'UART0_RX'),
 (0xFFE0,'TB2'),(0xFFE2,'TB1'),(0xFFE4,'TB0'),(0xFFE6,'TA4'),(0xFFE8,'TA3'),(0xFFEA,'TA2'),(0xFFEC,'TA1'),
 (0xFFEE,'TA0'),(0xFFF0,'INT2'),(0xFFF2,'INT1'),(0xFFF4,'INT0'),(0xFFF6,'WDT'),(0xFFF8,'DBC'),
 (0xFFFA,'BRK'),(0xFFFC,'ZERODIV'),(0xFFFE,'RESET')]   # M37732S4 datasheet Table 1

class Dis:
    def __init__(s, mem, transparent=frozenset()):
        s.mem = mem
        s.transparent = set(transparent)
        s.insn = {}          # addr -> (len, text_fn, flags_in)
        s.labels = {}        # addr -> name
        s.xref = collections.defaultdict(set)
        s.funcs = set()
        s.state = {}         # addr -> (m,x) seen
        s.warn = []
        s.jtables = {}       # addr -> list of targets
        s.dataref = collections.defaultdict(set)
        s.extern = {0x87A:'RAM_WAIT_TRAMPOLINE'}
        s.guessed=set()
        s.overlay=None
        s.swtab={}   # table addr -> (values, targets)
        sig=bytes.fromhex('8B5A48A30748ABD88A484AA8')
        s.switch_helpers=set()
        for a in mem:
            if mem.get(a)==0x8B and bytes(mem.get(a+i,0) for i in range(len(sig)))==sig: s.switch_helpers.add(a)
    def b(s,a): return s.mem.get(a)
    def w(s,a):
        lo,hi=s.mem.get(a),s.mem.get(a+1)
        return None if lo is None or hi is None else lo|hi<<8

    def decode(s, a, m, x):
        """returns (length, mnemonic, operand_text, flow, targets, newm, newx) or None"""
        mem=s.mem
        op=mem.get(a)
        if op is None: return None
        pre=''; p=a+1; accB=False
        if op==0x42:
            op2=mem.get(p)
            if op2 is None: return None
            if op2 in B_IMP:
                name,mode=B_IMP[op2]; p+=1
            elif op2 in OPS and OPS[op2][0] in ACC_OPS:
                name,mode=OPS[op2]; p+=1; accB=True
            else: return None
        elif op==0x89:
            op2=mem.get(p)
            if op2 is None or op2 not in P89: return None
            name,mode=P89[op2]; p+=1
            if name in ('MPY','DIV'): pass
        elif op in OPS:
            name,mode=OPS[op]
        else: return None
        if name in ACC_OPS or (mode=='acc'):
            reg='B' if accB else 'A'
        else: reg=None
        szM = 1 if m else 2; szX = 1 if x else 2
        def rd(n):
            nonlocal p
            v=0
            for i in range(n):
                bb=mem.get(p+i)
                if bb is None: raise IndexError
                v|=bb<<(8*i)
            p+=n; return v
        flow='seq'; tg=[]; nm,nx=m,x; opnd=''
        try:
            if mode in ('imp',): opnd=''
            elif mode=='acc': opnd='A'
            elif mode=='accB': opnd='B'
            elif mode=='brk': rd(1); flow='stop'
            elif mode=='immM':
                v=rd(szM); opnd='#$%0*X'%(szM*2,v)
            elif mode=='immX':
                v=rd(szX); opnd='#$%0*X'%(szX*2,v)
            elif mode=='imm8': v=rd(1); opnd='#$%02X'%v
            elif mode=='imm16': v=rd(2); opnd='#$%04X'%v
            elif mode=='dp': opnd='$%02X'%rd(1)
            elif mode=='dpx': opnd='$%02X,X'%rd(1)
            elif mode=='dpy': opnd='$%02X,Y'%rd(1)
            elif mode=='ind': opnd='($%02X)'%rd(1)
            elif mode=='indx': opnd='($%02X,X)'%rd(1)
            elif mode=='indy': opnd='($%02X),Y'%rd(1)
            elif mode=='lind': opnd='L($%02X)'%rd(1)
            elif mode=='lindy': opnd='L($%02X),Y'%rd(1)
            elif mode=='sr': opnd='$%02X,S'%rd(1)
            elif mode=='sry': opnd='($%02X,S),Y'%rd(1)
            elif mode in ('abs','absx','absy'):
                v=rd(2); opnd=s.absname(v)+{'abs':'','absx':',X','absy':',Y'}[mode]; s.dataref[v].add(a)
            elif mode in ('long','longx'):
                v=rd(3); opnd=s.longname(v)+(',X' if mode=='longx' else ''); s.dataref[v].add(a)
            elif mode=='rel8':
                r=rd(1); t=(p+(r-256 if r>127 else r)) & 0xFFFFFF
                t=(a&0xFF0000)|(t&0xFFFF)
                opnd=('L_%06X'%t,t); tg=[t]; flow='uncond' if name=='BRA' else 'cond'
            elif mode=='rel16':
                r=rd(2); t=(p+(r-65536 if r>32767 else r)); t=(a&0xFF0000)|(t&0xFFFF)
                if name=='PER': opnd='$%06X'%t
                else: opnd=('L_%06X'%t,t); tg=[t]; flow='uncond'
            elif mode=='jabs':
                v=rd(2); t=(a&0xFF0000)|v; opnd=('L_%06X'%t,t); tg=[t]
                flow='call' if name=='JSR' else 'uncond'
            elif mode=='jlong':
                v=rd(3); opnd=('L_%06X'%v,v); tg=[v]; flow='call' if name=='JSRL' else 'uncond'
            elif mode=='jind': v=rd(2); opnd='($%04X)'%v; flow='stop'
            elif mode=='jindx':
                v=rd(2); opnd='($%04X,X)'%v; flow='calltab' if name=='JSR' else 'jtab'; tg=[(a&0xFF0000)|v]
            elif mode=='jlind': v=rd(2); opnd='L($%04X)'%v; flow='stop'
            elif mode in ('bitdp','bitabs','bbdp','bbabs'):
                if mode in ('bitdp','bbdp'): ad='$%02X'%rd(1)
                else: v=rd(2); ad=s.absname(v); s.dataref[v].add(a)
                mk=rd(szM); opnd='#$%0*X,%s'%(szM*2,mk,ad)
                if mode in ('bbdp','bbabs'):
                    r=rd(1); t=(p+(r-256 if r>127 else r)); t=(a&0xFF0000)|(t&0xFFFF)
                    opnd=(opnd+',','L_%06X'%t,t); tg=[t]; flow='cond'
            elif mode in ('ldmdp','ldmdpx','ldmabs','ldmabsx'):
                if mode.startswith('ldmdp'): ad='$%02X'%rd(1)+(',X' if mode=='ldmdpx' else '')
                else:
                    v=rd(2); ad=s.absname(v)+(',X' if mode=='ldmabsx' else ''); s.dataref[v].add(a)
                mk=rd(szM); opnd='#$%0*X,%s'%(szM*2,mk,ad)
            elif mode=='mv':
                d=rd(1); sb=rd(1); opnd='$%02X,$%02X'%(d,sb)
            else: raise Exception(mode)
        except IndexError:
            return None
        if name in ('RTS','RTL','RTI','STP'): flow='stop'
        if name=='CLM': nm=0
        if name=='SEM': nm=1
        if name=='CLP':
            v=mem[a+1]; nm=0 if v&0x20 else m; nx=0 if v&0x10 else x
        if name=='SEP':
            v=mem[a+1]; nm=1 if v&0x20 else m; nx=1 if v&0x10 else x
        if name=='PLP' or (name=='PUL' and mem[a+1]&0x80): pass  # assume restores prior state
        if reg and opnd not in ('A','B'): mn='%s %s,'%(name,reg) if False else name
        mn=name
        if reg: mn = name + ('B' if reg=='B' and name in ('LDA','STA') else '')
        # use Mitsubishi style: LDA A,..  /  LDA B,..
        return dict(len=p-a,name=name,reg=reg,opnd=opnd,flow=flow,tg=tg,m=nm,x=nx,mode=mode)

    def absname(s,v):
        if v in SFR: return SFR[v]
        return '$%04X'%v
    def longname(s,v): return '$%06X'%v

    def run(s, entries):
        if not hasattr(s,'exitst'):
            s.exitst=collections.defaultdict(collections.Counter)
            s.pending=collections.defaultdict(list)
            s.owner={}; s.ambig=set()
        work=[(a,m,x,a) for a,m,x in entries]
        def resume(f):
            st=s.exitst[f].most_common(1)[0][0]
            if len(s.exitst[f])>1: s.ambig.add(f)
            for (ra,cf,m,x) in s.pending.pop(f,[]):
                work.append((ra,st[0],st[1],cf))
        while True:
            while work:
                a,m,x,fn=work.pop()
                s.trace(a,m,x,fn,work,resume)
            # unresolved continuations: fall back to caller state
            if not s.pending: break
            f=next(iter(s.pending))
            s.warn.append('%06X: callee exit flag state unknown, assuming caller state'%f)
            for (ra,cf,m,x) in s.pending.pop(f): work.append((ra,m,x,cf))
    def trace(s,a,m,x,fn,work,resume):
            while True:
                st=s.state.setdefault(a,set())
                if (m,x) in st: break
                st.add((m,x))
                d=s.decode(a,m,x)
                if d is None:
                    s.warn.append('%06X: invalid opcode %s (M=%d X=%d)'%(a,'%02X'%s.mem[a] if a in s.mem else '??',m,x)); break
                if a in s.insn:
                    o=s.insn[a]
                    if o['len']!=d['len'] or o['opnd']!=d['opnd']:
                        s.warn.append('%06X: reached with M=%d X=%d, previously M=%d X=%d (decodes differently)'%(a,m,x,*o['mx']))
                        break
                    o.setdefault('alt',set()).add((m,x))
                else:
                    d['mx']=(m,x); s.insn[a]=d; s.owner[a]=fn
                nxt=a+d['len']
                if d['name'] in ('RTS','RTL'):
                    first=not s.exitst[fn]
                    s.exitst[fn][(m,x)]+=1
                    if fn in s.pending: resume(fn)
                for t in d['tg']:
                    if d['flow']=='call' and t in s.extern:
                        s.labels.setdefault(t,s.extern[t]); s.xref[t].add(a); work.append((nxt,m,x,fn)); continue
                    if d['flow']=='call' and t in s.switch_helpers:
                        s.labels.setdefault(t,'__switch_byte'); s.xref[t].add(a); s.funcs.add(t)
                        if t not in s.state: work.append((t,0,0,t))
                        n=None
                        for k in range(a-4,a):
                            i2=s.insn.get(k)
                            if i2 and i2['name']=='LDX' and i2['mode']=='immX' and k+i2['len']<=a: n=int(i2['opnd'][2:],16)
                        if n is None:
                            s.warn.append('%06X: switch helper call, case count unknown'%a); continue
                        vals=[s.mem.get(nxt+k) for k in range(n)]
                        tg=[(a&0xFF0000)|s.w(nxt+n+2*k) for k in range(n+1)]
                        s.swtab[nxt]=(vals,tg)
                        for tt in tg:
                            s.labels.setdefault(tt,'L_%06X'%tt); s.xref[tt].add(a); work.append((tt,1,0,fn))
                        continue
                    if d['flow']=='call' and t in s.transparent:
                        s.funcs.add(t); s.labels.setdefault(t,'F_%06X'%t); s.xref[t].add(a)
                        work.append((t,m,x,t)); work.append((nxt,m,x,fn)); continue
                    if d['flow']=='call':
                        s.funcs.add(t); s.labels.setdefault(t,'F_%06X'%t); s.xref[t].add(a)
                        if t not in s.state: work.append((t,0,0,t))
                        if s.exitst[t]:
                            st2=s.exitst[t].most_common(1)[0][0]; work.append((nxt,st2[0],st2[1],fn))
                        else:
                            s.pending[t].append((nxt,fn,m,x))
                    elif d['flow'] in ('cond','uncond'):
                        s.labels.setdefault(t,'L_%06X'%t); s.xref[t].add(a); work.append((t,d['m'],d['x'],fn))
                    elif d['flow'] in ('jtab','calltab'):
                        tl=s.jumptable(a,t)
                        for tt in tl:
                            s.labels.setdefault(tt,'L_%06X'%tt); s.xref[tt].add(a)
                            work.append((tt,0,0,tt) if d['flow']=='calltab' else (tt,d['m'],d['x'],fn))
                        if d['flow']=='calltab': work.append((nxt,d['m'],d['x'],fn))
                if d['flow'] in ('stop','uncond','jtab','call'): break
                a,m,x=nxt,d['m'],d['x']

    def trial(s,a,limit):
        # linear trial decode with flag tracking; ok if it reaches a return/jump before invalid
        m=x=0; p=a; n=0
        while p<limit and n<4000:
            if p in s.insn: return n>2
            d=s.decode(p,m,x)
            if d is None: return False
            n+=1
            if d['name'] in ('BRK','STP'): return False
            if d['flow']=='call' and d['tg'] and not s.plausible(d['tg'][0]) and d['tg'][0] not in s.extern and d['tg'][0]!=0x80: return False
            if d['name'] in ('RTS','RTL','RTI') : return True
            m,x=d['m'],d['x']
            if d['flow']=='call' and d['tg']:
                t=d['tg'][0]
                if t not in s.transparent and t not in s.extern:
                    if s.exitst.get(t): m,x=s.exitst[t].most_common(1)[0][0]
                    else: return n>3
            p+=d['len']
        return False
    def gapscan(s):
        found=[]
        code=set()
        for a,ins in s.insn.items():
            for i in range(ins['len']): code.add(a+i)
        for a,ins in sorted(s.insn.items()):
            if ins['flow'] not in ('stop','uncond','jtab'): continue
            g=a+ins['len']
            if g in code or g not in s.mem or g in s.jtables: continue
            e=g
            while e in s.mem and e not in code: e+=1
            if s.mem[g]==0xFF or not s.plausible(g): continue
            if s.trial(g,e): found.append(g)
        return found

    def jumptable(s, at, t):
        out=[]; bank=at&0xFF0000; p=t
        limit=256
        for k in range(at-16,at):
            i=s.insn.get(k)
            if i and i['name']=='CMP' and i['mode']=='immM' and i['reg']=='A':
                nx=s.insn.get(k+i['len'])
                if nx and nx['name'] in ('BCC','BCS'):
                    limit=int(i['opnd'][2:],16)
        for i in range(limit):
            v=s.w(p)
            if v is None: break
            tt=bank|v
            if not s.plausible(tt) or (p!=t and p in s.labels): break
            out.append(tt); p+=2
        s.jtables[t]=(at,out)
        s.labels.setdefault(t,'JT_%06X'%t)
        return out
    def plausible(s,a):
        if s.overlay and s.overlay[0]<=a<s.overlay[1]: return False
        return (0x80<=a<0x10000 or 0x10100<=a<0x20000) and a in s.mem

FLAGOPS={'SEM','CLM','SEP','CLP','PLP','RTI'}
def compute_transparent(d):
    bad=set(); calls=collections.defaultdict(set)
    for a,i in d.insn.items():
        f=d.owner.get(a)
        if f is None: continue
        if i['name'] in FLAGOPS or (i['name']=='PUL' and d.mem[a+1]&0x80): bad.add(f)
        if i['flow']=='call' and i['tg']: calls[f].add(i['tg'][0])
        if i['flow'] in ('jtab','calltab'): bad.add(f)
    tr={f for f in d.funcs if f not in bad and d.exitst.get(f)}
    ch=True
    while ch:
        ch=False
        for f in list(tr):
            if any(c not in tr and c not in d.extern for c in calls[f]): tr.discard(f); ch=True
    return tr

def overlaps(d):
    cover={}
    for a,i in d.insn.items():
        for k in range(1,i['len']): cover[a+k]=a
    bad=set()
    for b in d.insn:
        if b in cover:
            a=cover[b]
            for z in (d.owner.get(a),d.owner.get(b)):
                if z in d.guessed: bad.add(z)
    return bad

def disassemble(path, outpath, extra_entries=(), passes=4):
    tr=frozenset(); bl=set()
    for i in range(passes):
        d=disassemble1(path, extra_entries, tr, bl)
        ntr=frozenset(compute_transparent(d))|tr
        nb=overlaps(d)-bl
        print('pass',i,'transparent funcs',len(ntr),'warn',len(d.warn),'bad guesses',len(nb))
        if ntr==tr and not nb: break
        tr=ntr; bl|=nb
    d.transparent_final=tr
    write(d, d.mem, path, outpath)
    return d

def disassemble1(path, extra_entries, tr, blacklist=()):
    mem=srec.load(path); d=Dis(mem,tr)
    entries=[]
    # RAM overlay: ROM 0x010100..0x0105C7 is copied to RAM 0x000080 (see copy loop near 0x0105F3)
    import re
    ks=sorted(mem); bb=bytes(mem[k] for k in ks); OV=None
    mt=re.search(rb'\x64\x00(..)\xF8\x64\x02(.)\xD8\x64\x03(..)\xF8\x64\x05(.)\xD8\x64\x06(..)\xF8\x64\x08(.)',bb,re.S)
    if mt:
        g=mt.groups()
        src=g[1][0]<<16|int.from_bytes(g[0],'little'); dst=g[3][0]<<16|int.from_bytes(g[2],'little')
        end=g[5][0]<<16|int.from_bytes(g[4],'little')
        OV=(src,end,dst)
        print('RAM overlay: ROM %06X-%06X -> RAM %06X (copy loop at %06X)'%(src,end-1,dst,ks[mt.start()]))
    if OV is None: OV=(0,0,0)
    for a in range(OV[0],OV[1]):
        if a in mem: mem[a-OV[0]+OV[2]]=mem[a]
    d.overlay=OV
    d.labels[0x80]='RAMCODE_ENTRY'
    for va,nm in VECTORS:
        v=d.w(va)
        if v: d.labels[v]='VEC_'+nm; entries.append((v,0,0))
    for e in extra_entries: entries.append((e,0,0)); d.labels.setdefault(e,'F_%06X'%e); d.funcs.add(e)
    d.run(entries)
    for it in range(30):
        f=[g for g in d.gapscan() if g not in blacklist]
        if not f: break
        for g in f:
            d.labels.setdefault(g,'F_%06X'%g); d.funcs.add(g); d.guessed.add(g)
        d.run([(g,0,0) for g in f])
    return d

def fmt(d, a, ins):
    o=ins['opnd']
    if isinstance(o,tuple):
        o=o[0]+d.labels.get(o[2],o[1]) if len(o)==3 else d.labels.get(o[1],o[0])
    name=ins['name']; reg=ins['reg']
    if reg and o not in ('A','B'): o='%s,%s'%(reg,o)
    elif reg and o in ('A','B'): o=reg
    return name, o

def write(d, mem, path, outpath):
    # mark code bytes
    code=set()
    for a,ins in d.insn.items():
        for i in range(ins['len']): code.add(a+i)
    out=open(outpath,'w')
    w=out.write
    w(';'+'='*78+'\n')
    w('; Disassembly of %s\n; CPU: Mitsubishi M37732 (MELPS 7700), compiler presumably NC77\n'%path.split('/')[-1])
    w('; Generated by d77.py (recursive descent, M/X flag tracking).\n')
    w('; Instructions: %d   Code bytes: %d   Functions: %d\n'%(len(d.insn),len(code),len(d.funcs)))
    w(';'+'='*78+'\n\n')
    for k in sorted(SFR): w('%-10s .EQU $%04X\n'%(SFR[k],k))
    w('\n')
    if d.warn:
        w('; ---- analysis warnings ----\n')
        for x in d.warn[:500]: w(';  '+x+'\n')
        w('\n')
    ks=sorted(mem); i=0; last_mx=None; prev=None
    while i<len(ks):
        a=ks[i]
        if prev is not None and a!=prev:
            w('\n        .ORG $%06X\n'%a); last_mx=None
        elif prev is None: w('        .ORG $%06X\n'%a)
        if a in d.labels:
            lab=d.labels[a]
            if a in d.funcs or lab.startswith('VEC_'):
                w('\n;' + '-'*60 + '\n')
                if a in getattr(d,'transparent_final',()): w('; (flag-transparent helper: keeps caller M/X state)\n')
                if a in d.guessed: w('; (no direct caller found - probably called via function pointer)\n')
                xr=sorted(d.xref.get(a,()))
                if xr: w('; called from: %s%s\n'%(', '.join('%06X'%x for x in xr[:12]),'' if len(xr)<=12 else ' ...(+%d)'%(len(xr)-12)))
            w('%s:\n'%lab)
        if a in d.insn:
            ins=d.insn[a]
            if ins['mx']!=last_mx:
                w('        .ASSUME M=%d,X=%d\n'%ins['mx']); last_mx=ins['mx']
            name,o=fmt(d,a,ins)
            byts=' '.join('%02X'%mem[a+j] for j in range(ins['len']))
            hn=hwnote(ins,mem,a)
            w('        %-5s %-28s ; %06X: %s%s\n'%(name,o,a,byts,('  ['+hn+']') if hn else ''))
            if ins['flow'] in ('stop','uncond','jtab'): w('\n')
            n=ins['len']
            # advance
            prev=a+n-1;
            while i<len(ks) and ks[i]<a+n: i+=1
            prev=a+n
            continue
        # data run
        j=i; run=[]
        while j<len(ks) and ks[j]==a+len(run) and ks[j] not in code and (len(run)==0 or ks[j] not in d.labels) and len(run)<16:
            run.append(mem[ks[j]]); j+=1
        if not run: run=[mem[a]]; j=i+1
        if a in d.swtab:
            vals,tg=d.swtab[a]
            w('        ; ---- inline switch table for __switch_byte: %d case values, %d targets (last = default)\n'%(len(vals),len(tg)))
            w('        .BYTE %-27s ; %06X  case values\n'%(','.join('$%02X'%v for v in vals),a))
            for k,t in enumerate(tg):
                w('        .WORD %-27s ; %06X  %s\n'%(d.labels.get(t,'$%04X'%(t&0xFFFF)),a+len(vals)+2*k,'default' if k==len(vals) else 'case $%02X'%vals[k]))
            n=len(vals)+2*len(tg)
            while i<len(ks) and ks[i]<a+n: i+=1
            prev=a+n; continue
        if a in d.jtables and d.jtables[a][1]:
            at,tl=d.jtables[a]
            for k,t in enumerate(tl):
                w('        .WORD %-27s ; %06X  case %d\n'%(d.labels.get(t,'$%04X'%(t&0xFFFF)),a+2*k,k))
            n=2*len(tl)
            while i<len(ks) and ks[i]<a+n: i+=1
            prev=a+n; continue
        asc=''.join(chr(c) if 32<=c<127 else '.' for c in run)
        w('        .BYTE %-27s ; %06X  |%s|\n'%(','.join('$%02X'%c for c in run),a,asc))
        i=j; prev=a+len(run)
    out.close()

if __name__=='__main__':
    src,dst=sys.argv[1],sys.argv[2]
    extra=[int(x,16) for x in sys.argv[3:]]
    d=disassemble(src,dst,extra)
    print('insns',len(d.insn),'funcs',len(d.funcs),'warn',len(d.warn))
    for x in d.warn[:40]: print(x)
