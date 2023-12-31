// Old:     PINMAME: All cmpx opcodes are different from the latest ones (10/2016) in MAME/MESS as it screws up AY emu of some machines
// Update!: cmpx opcodes from 10/2016 didn't pass the extended overflow bit when setting the overflow flag. Now fixed (see #if 0's).

/*

HNZVC

? = undefined
* = affected
- = unaffected
0 = cleared
1 = set
# = ccr directly affected by instruction
@ = special - carry set if bit 7 is set

*/

/* $00 ILLEGAL */
void illegal()
{
#ifdef HAS_HD6301
    TAKE_TRAP;
#endif
}

/* $01 NOP */
void nop()
{

}

/* $02 ILLEGAL */

/* $03 ILLEGAL */

/* $04 LSRD inherent -0*-* */
void lsrd()
{
    uint16_t t;
    CLR_NZC;
    t = D;
    CC |= (t & 0x0001);
    t >>= 1;
    SET_Z16(t);
    D = t;
}

/* $05 ASLD inherent ?**** */
void asld()
{
    int r;
    uint16_t t;
    t = D;
    r = t << 1;
    CLR_NZVC;
    SET_FLAGS16(t, t, r);
    D = r;
}

/* $06 TAP inherent ##### */
void tap()
{
    CC = A;
//    ONE_MORE_INSN();
//    one_more_insn = true;
}

/* $07 TPA inherent ----- */
void tpa()
{
    A = CC;
}

/* $08 INX inherent --*-- */
void inx()
{
    ++X;
    CLR_Z;
    SET_Z16(X);
}

/* $09 DEX inherent --*-- */
void dex()
{
    --X;
    CLR_Z;
    SET_Z16(X);
}

/* $0a CLV */
void clv()
{
    CLV;
}

/* $0b SEV */
void sev()
{
    SEV;
}

/* $0c CLC */
void clc()
{
    CLC;
}

/* $0d SEC */
void sec()
{
    SEC;
}

/* $0e CLI */
void cli()
{
    CLI;
//  ONE_MORE_INSN();
//    one_more_insn = true;
}

/* $0f SEI */
void sei()
{
    SEI;
//  ONE_MORE_INSN();
//    one_more_insn = true;
}

/* $10 SBA inherent -**** */
void sba()
{
    uint16_t t;
    t = A - B;
    CLR_NZVC;
    SET_FLAGS8(A, B, t);
    A = (uint8_t)t;
}

/* $11 CBA inherent -**** */
void cba()
{
    uint16_t t;
    t = A - B;
    CLR_NZVC;
    SET_FLAGS8(A, B, t);
}

/* $12 ILLEGAL */
void undoc1()
{
    X += RM(S + 1);
}

/* $13 ILLEGAL */
void undoc2()
{
    X += RM(S + 1);
}

/* $14 ILLEGAL */

/* $15 ILLEGAL */

/* $16 TAB inherent -**0- */
void tab()
{
    B=A;
    CLR_NZV;
    SET_NZ8(B);
}

/* $17 TBA inherent -**0- */
void tba()
{
    A = B;
    CLR_NZV;
    SET_NZ8(A);
}

/* $18 XGDX inherent ----- */ /* HD6301 only */
void xgdx()
{
    uint16_t t = X;
    X = D;
    D = t;
}

/* $19 DAA inherent (A) -**0* */
void daa()
{
    uint8_t msn, lsn;
    uint16_t t, cf = 0;
    msn = A & 0xf0;
    lsn = A & 0x0f;
    if(lsn > 0x09 || CC & 0x20) {
        cf |= 0x06;
    }
    if(msn > 0x80 && lsn > 0x09) {
        cf |= 0x60;
    }
    if(msn > 0x90 || (CC & 0x01)) {
        cf |= 0x60;
    }
    t = cf + A;
    CLR_NZV; /* keep carry from previous operation */
    SET_NZ8((uint8_t)t);
    SET_C8(t);
    A = (uint8_t)t;
}

/* $1a ILLEGAL */

/* $1a SLP */ /* HD6301 only */
void slp()
{
    /* wait for next IRQ (same as waiting of wai) */
 //   wai_state |= HD6301_SLP;
}

/* $1b ABA inherent ***** */
void aba()
{
    uint16_t t;
    t = A + B;
    CLR_HNZVC;
    SET_FLAGS8(A, B, t);
    SET_H(A, B, t);
    A = (uint8_t)t;
}

/* $1c ILLEGAL */

/* $1d ILLEGAL */

/* $1e ILLEGAL */

/* $1f ILLEGAL */

/* $20 BRA relative ----- */
void bra()
{
    uint8_t t;
    IMMBYTE(t);
    PC += SIGNED(t);
}

/* $21 BRN relative ----- */
void brn()
{
    uint8_t t;
    IMMBYTE(t);
}

/* $22 BHI relative ----- */
void bhi()
{
    uint8_t t;
    BRANCH(!(CC & 0x05));
}

/* $23 BLS relative ----- */
void bls()
{
    uint8_t t;
    BRANCH(CC & 0x05);
}

/* $24 BCC relative ----- */
void bcc()
{
    uint8_t t;
    BRANCH(!(CC & 0x01));
}

/* $25 BCS relative ----- */
void bcs()
{
    uint8_t t;
    BRANCH(CC & 0x01);
}

/* $26 BNE relative ----- */
void bne()
{
    uint8_t t;
    BRANCH(!(CC & 0x04));
}

/* $27 BEQ relative ----- */
void beq()
{
    uint8_t t;
    BRANCH(CC & 0x04);
}

/* $28 BVC relative ----- */
void bvc()
{
    uint8_t t;
    BRANCH(!(CC & 0x02));
}

/* $29 BVS relative ----- */
void bvs()
{
    uint8_t t;
    BRANCH(CC & 0x02);
}

/* $2a BPL relative ----- */
void bpl()
{
    uint8_t t;
    BRANCH(!(CC & 0x08));
}

/* $2b BMI relative ----- */
void bmi()
{
    uint8_t t;
    BRANCH(CC & 0x08);
}

/* $2c BGE relative ----- */
void bge()
{
    uint8_t t;
    BRANCH(!NXORV);
}

/* $2d BLT relative ----- */
void blt()
{
    uint8_t t;
    BRANCH(NXORV);
}

/* $2e BGT relative ----- */
void bgt()
{
    uint8_t t;
    BRANCH(!(NXORV||CC & 0x04));
}

/* $2f BLE relative ----- */
void ble()
{
    uint8_t t;
    BRANCH(NXORV||CC & 0x04);
}

/* $30 TSX inherent ----- */
void tsx()
{
    X = (S + 1);
}

/* $31 INS inherent ----- */
void ins()
{
    ++S;
}

/* $32 PULA inherent ----- */
void pula()
{
    PULLBYTE(A);
}

/* $33 PULB inherent ----- */
void pulb()
{
    PULLBYTE(B);
}

/* $34 DES inherent ----- */
void des()
{
    --S;
}

/* $35 TXS inherent ----- */
void txs()
{
    S = (X - 1);
}

/* $36 PSHA inherent ----- */
void psha()
{
    PUSHBYTE(A);
}

/* $37 PSHB inherent ----- */
void pshb()
{
    PUSHBYTE(B);
}

/* $38 PULX inherent ----- */
void pulx()
{
    PULLWORD(pX);
}

/* $39 RTS inherent ----- */
void rts()
{
    PULLWORD(pPC);
}

/* $3a ABX inherent ----- */
void abx()
{
    X += B;
}

/* $3b RTI inherent ##### */
void rti()
{
    PULLBYTE(CC);
    PULLBYTE(B);
    PULLBYTE(A);
    PULLWORD(pX);
    PULLWORD(pPC);
}

/* $3c PSHX inherent ----- */
void pshx()
{
    PUSHWORD(pX);
}

/* $3d MUL inherent --*-@ */
void mul()
{
    uint16_t t;
    t = A*B;
    CLR_C;
    if(t & 0x80) SEC;
    D = t;
}

/* $3e WAI inherent ----- */
void wai()
{
    /*
     * WAI stacks the entire machine state on the
     * hardware stack, then waits for an interrupt.
     */
//    wai_state |= MC6800_WAI;
    m6800.wai_state |= M6800_WAI;
    PUSHWORD(pPC);
    PUSHWORD(pX);
    PUSHBYTE(A);
    PUSHBYTE(B);
    PUSHBYTE(CC);
}

/* $3f SWI absolute indirect ----- */
void swi()
{
    PUSHWORD(pPC);
    PUSHWORD(pX);
    PUSHBYTE(A);
    PUSHBYTE(B);
    PUSHBYTE(CC);
    SEI;
    PCD = RM16(0xfffa);
}

/* $40 NEGA inherent ?**** */
void nega()
{
    uint16_t r;
    r = -A;
    CLR_NZVC;
    SET_FLAGS8(0, A, r);
    A = (uint8_t)r;
}

/* $41 ILLEGAL */

/* $42 ILLEGAL */

/* $43 COMA inherent -**01 */
void coma()
{
    A = ~A;
    CLR_NZV;
    SET_NZ8(A);
    SEC;
}

/* $44 LSRA inherent -0*-* */
void lsra()
{
    CLR_NZC;
    CC |= (A & 0x01);
    A >>= 1;
    SET_Z8(A);
}

/* $45 ILLEGAL */

/* $46 RORA inherent -**-* */
void rora()
{
    uint8_t r;
    r = (CC & 0x01) << 7;
    CLR_NZC;
    CC |= (A & 0x01);
    r |= A >> 1;
    SET_NZ8(r);
    A = r;
}

/* $47 ASRA inherent ?**-* */
void asra()
{
    CLR_NZC;
    CC |= (A & 0x01);
    A >>= 1;
    A |= ((A & 0x40) << 1);
    SET_NZ8(A);
}

/* $48 ASLA inherent ?**** */
void asla()
{
    uint16_t r;
    r = A << 1;
    CLR_NZVC;
    SET_FLAGS8(A, A, r);
    A = (uint8_t)r;
}

/* $49 ROLA inherent -**** */
void rola()
{
    uint16_t t, r;
    t = A;
    r = CC & 0x01;
    r |= t << 1;
    CLR_NZVC;
    SET_FLAGS8(t, t, r);
    A = (uint8_t)r;
}

/* $4a DECA inherent -***- */
void deca()
{
    --A;
    CLR_NZV;
    SET_FLAGS8D(A);
}

/* $4b ILLEGAL */

/* $4c INCA inherent -***- */
void inca()
{
    ++A;
    CLR_NZV;
    SET_FLAGS8I(A);
}

/* $4d TSTA inherent -**0- */
void tsta()
{
    CLR_NZVC;
    SET_NZ8(A);
}

/* $4e ILLEGAL */

/* $4f CLRA inherent -0100 */
void clra()
{
    A = 0;
    CLR_NZVC;
    SEZ;
}

/* $50 NEGB inherent ?**** */
void negb()
{
    uint16_t r;
    r = -B;
    CLR_NZVC;
    SET_FLAGS8(0, B, r);
    B = (uint8_t)r;
}

/* $51 ILLEGAL */

/* $52 ILLEGAL */

/* $53 COMB inherent -**01 */
void comb()
{
    B = ~B;
    CLR_NZV;
    SET_NZ8(B);
    SEC;
}

/* $54 LSRB inherent -0*-* */
void lsrb()
{
    CLR_NZC;
    CC |= (B & 0x01);
    B >>= 1;
    SET_Z8(B);
}

/* $55 ILLEGAL */

/* $56 RORB inherent -**-* */
void rorb()
{
    uint8_t r;
    r = (CC & 0x01) << 7;
    CLR_NZC;
    CC |= (B & 0x01);
    r |= B >> 1;
    SET_NZ8(r);
    B = r;
}

/* $57 ASRB inherent ?**-* */
void asrb()
{
    CLR_NZC;
    CC |= (B & 0x01);
    B >>= 1;
    B |= ((B & 0x40) << 1);
    SET_NZ8(B);
}

/* $58 ASLB inherent ?**** */
void aslb()
{
    uint16_t r;
    r = B << 1;
    CLR_NZVC;
    SET_FLAGS8(B, B, r);
    B = (uint8_t)r;
}

/* $59 ROLB inherent -**** */
void rolb()
{
    uint16_t t, r;
    t = B;
    r = CC & 0x01;
    r |= t << 1;
    CLR_NZVC;
    SET_FLAGS8(t, t, r);
    B = (uint8_t)r;
}

/* $5a DECB inherent -***- */
void decb()
{
    --B;
    CLR_NZV;
    SET_FLAGS8D(B);
}

/* $5b ILLEGAL */

/* $5c INCB inherent -***- */
void incb()
{
    ++B;
    CLR_NZV;
    SET_FLAGS8I(B);
}

/* $5d TSTB inherent -**0- */
void tstb()
{
    CLR_NZVC;
    SET_NZ8(B);
}

/* $5e ILLEGAL */

/* $5f CLRB inherent -0100 */
void clrb()
{
    B=0;
    CLR_NZVC;
    SEZ;
}

/* $60 NEG indexed ?**** */
void neg_ix()
{
    uint16_t r, t;
    IDXBYTE(t);
    r = -t;
    CLR_NZVC;
    SET_FLAGS8(0, t, r);
    WM(EAD, r);
}

/* $61 AIM --**0- */ /* HD6301 only */
void aim_ix()
{
    uint8_t t, r;
    IMMBYTE(t);
    IDXBYTE(r);
    r &= t;
    CLR_NZV;
    SET_NZ8(r);
    WM(EAD, r);
}

/* $62 OIM --**0- */ /* HD6301 only */
void oim_ix()
{
    uint8_t t, r;
    IMMBYTE(t);
    IDXBYTE(r);
    r |= t;
    CLR_NZV;
    SET_NZ8(r);
    WM(EAD, r);
}

/* $63 COM indexed -**01 */
void com_ix()
{
    uint8_t t;
    IDXBYTE(t);
    t = ~t;
    CLR_NZV;
    SET_NZ8(t);
    SEC;
    WM(EAD, t);
}

/* $64 LSR indexed -0*-* */
void lsr_ix()
{
    uint8_t t;
    IDXBYTE(t);
    CLR_NZC;
    CC |= (t & 0x01);
    t >>= 1;
    SET_Z8(t);
    WM(EAD, t);
}

/* $65 EIM --**0- */ /* HD6301 only */
void eim_ix()
{
    uint8_t t, r;
    IMMBYTE(t);
    IDXBYTE(r);
    r ^= t;
    CLR_NZV;
    SET_NZ8(r);
    WM(EAD, r);
}

/* $66 ROR indexed -**-* */
void ror_ix()
{
    uint8_t t, r;
    IDXBYTE(t);
    r = (CC & 0x01) << 7;
    CLR_NZC;
    CC |= (t & 0x01);
    r |= t >> 1;
    SET_NZ8(r);
    WM(EAD, r);
}

/* $67 ASR indexed ?**-* */
void asr_ix()
{
    uint8_t t;
    IDXBYTE(t);
    CLR_NZC;
    CC |= (t & 0x01);
    t >>= 1;
    t |= ((t & 0x40) << 1);
    SET_NZ8(t);
    WM(EAD, t);
}

/* $68 ASL indexed ?**** */
void asl_ix()
{
    uint16_t t, r;
    IDXBYTE(t);
    r = t << 1;
    CLR_NZVC;
    SET_FLAGS8(t, t, r);
    WM(EAD, r);
}

/* $69 ROL indexed -**** */
void rol_ix()
{
    uint16_t t, r;
    IDXBYTE(t);
    r = CC & 0x01;
    r |= t << 1;
    CLR_NZVC;
    SET_FLAGS8(t, t, r);
    WM(EAD, r);
}

/* $6a DEC indexed -***- */
void dec_ix()
{
    uint8_t t;
    IDXBYTE(t);
    --t;
    CLR_NZV;
    SET_FLAGS8D(t);
    WM(EAD, t);
}

/* $6b TIM --**0- */ /* HD6301 only */
void tim_ix()
{
    uint8_t t, r;
    IMMBYTE(t);
    IDXBYTE(r);
    r &= t;
    CLR_NZV;
    SET_NZ8(r);
}

/* $6c INC indexed -***- */
void inc_ix()
{
    uint8_t t;
    IDXBYTE(t);
    ++t;
    CLR_NZV;
    SET_FLAGS8I(t);
    WM(EAD, t);
}

/* $6d TST indexed -**0- */
void tst_ix()
{
    uint8_t t;
    IDXBYTE(t);
    CLR_NZVC;
    SET_NZ8(t);
}

/* $6e JMP indexed ----- */
void jmp_ix()
{
    INDEXED;
    PC = EA;
}

/* $6f CLR indexed -0100 */
void clr_ix()
{
    INDEXED;
    WM(EAD, 0);
    CLR_NZVC;
    SEZ;
}

/* $70 NEG extended ?**** */
void neg_ex()
{
    uint16_t r, t;
    EXTBYTE(t);
    r = -t;
    CLR_NZVC;
    SET_FLAGS8(0, t, r);
    WM(EAD, r);
}

/* $71 AIM --**0- */ /* HD6301 only */
void aim_di()
{
    uint8_t t, r;
    IMMBYTE(t);
    DIRBYTE(r);
    r &= t;
    CLR_NZV;
    SET_NZ8(r);
    WM(EAD, r);
}

/* $71 NIM --**0- */ /* MB8861 only */
void nim_ix()
{
    uint8_t t, r;
    IMMBYTE(t);
    IDXBYTE(r);
    r &= t;
    CLR_NZV;
    if(!r) {
        SEZ;
    } else {
        SEN;
    }
    WM(EAD, r);
}

/* $72 OIM --**0- */ /* HD6301 only */
void oim_di()
{
    uint8_t t, r;
    IMMBYTE(t);
    DIRBYTE(r);
    r |= t;
    CLR_NZV;
    SET_NZ8(r);
    WM(EAD, r);
}

/* $72 OIM --**0- */ /* MB8861 only */
void oim_ix_mb8861()
{
    uint8_t t, r;
    IMMBYTE(t);
    IDXBYTE(r);
    r |= t;
    CLR_NZV;
    if(!r) {
        SEZ;
    } else {
        SEN;
    }
    WM(EAD, r);
}

/* $73 COM extended -**01 */
void com_ex()
{
    uint8_t t;
    EXTBYTE(t);
    t = ~t;
    CLR_NZV;
    SET_NZ8(t);
    SEC;
    WM(EAD, t);
}

/* $74 LSR extended -0*-* */
void lsr_ex()
{
    uint8_t t;
    EXTBYTE(t);
    CLR_NZC;
    CC |= (t & 0x01);
    t >>= 1;
    SET_Z8(t);
    WM(EAD, t);
}

/* $75 EIM --**0- */ /* HD6301 only */
void eim_di()
{
    uint8_t t, r;
    IMMBYTE(t);
    DIRBYTE(r);
    r ^= t;
    CLR_NZV;
    SET_NZ8(r);
    WM(EAD, r);
}

/* $75 XIM --**-- */ /* MB8861 only */
void xim_ix()
{
    uint8_t t, r;
    IMMBYTE(t);
    IDXBYTE(r);
    r ^= t;
    CLR_NZ;
    if(!r) {
        SEZ;
    } else {
        SEN;
    }
    WM(EAD, r);
}

/* $76 ROR extended -**-* */
void ror_ex()
{
    uint8_t t, r;
    EXTBYTE(t);
    r = (CC & 0x01) << 7;
    CLR_NZC;
    CC |= (t & 0x01);
    r |= t >> 1;
    SET_NZ8(r);
    WM(EAD, r);
}

/* $77 ASR extended ?**-* */
void asr_ex()
{
    uint8_t t;
    EXTBYTE(t);
    CLR_NZC;
    CC |= (t & 0x01);
    t >>= 1;
    t |= ((t & 0x40) << 1);
    SET_NZ8(t);
    WM(EAD, t);
}

/* $78 ASL extended ?**** */
void asl_ex()
{
    uint16_t t, r;
    EXTBYTE(t);
    r = t << 1;
    CLR_NZVC;
    SET_FLAGS8(t, t, r);
    WM(EAD, r);
}

/* $79 ROL extended -**** */
void rol_ex()
{
    uint16_t t, r;
    EXTBYTE(t);
    r = CC & 0x01;
    r |= t << 1;
    CLR_NZVC;
    SET_FLAGS8(t, t, r);
    WM(EAD, r);
}

/* $7a DEC extended -***- */
void dec_ex()
{
    uint8_t t;
    EXTBYTE(t);
    --t;
    CLR_NZV;
    SET_FLAGS8D(t);
    WM(EAD, t);
}

/* $7b TIM --**0- */ /* HD6301 only */
void tim_di()
{
    uint8_t t, r;
    IMMBYTE(t);
    DIRBYTE(r);
    r &= t;
    CLR_NZV;
    SET_NZ8(r);
}

/* $7b TMM --***- */ /* MB8861 only */
void tmm_ix()
{
    uint8_t t, r;
    IMMBYTE(t);
    IDXBYTE(r);
    r &= t;
    CLR_NZV;
    if(!t || !r) {
        SEZ;
    } else if(r == t) {
        SEV;
    } else {
        SEN;
    }
}

/* $7c INC extended -***- */
void inc_ex()
{
    uint8_t t;
    EXTBYTE(t);
    ++t;
    CLR_NZV;
    SET_FLAGS8I(t);
    WM(EAD, t);
}

/* $7d TST extended -**0- */
void tst_ex()
{
    uint8_t t;
    EXTBYTE(t);
    CLR_NZVC;
    SET_NZ8(t);
}

/* $7e JMP extended ----- */
void jmp_ex()
{
    EXTENDED;
    PC = EA;
}

/* $7f CLR extended -0100 */
void clr_ex()
{
    EXTENDED;
    WM(EAD, 0);
    CLR_NZVC;
    SEZ;
}

/* $80 SUBA immediate ?**** */
void suba_im()
{
    uint16_t t, r;
    IMMBYTE(t);
    r = A - t;
    CLR_NZVC;
    SET_FLAGS8(A, t, r);
    A = (uint8_t)r;
}

/* $81 CMPA immediate ?**** */
void cmpa_im()
{
    uint16_t t, r;
    IMMBYTE(t);
    r = A - t;
    CLR_NZVC;
    SET_FLAGS8(A, t, r);
}

/* $82 SBCA immediate ?**** */
void sbca_im()
{
    uint16_t t, r;
    IMMBYTE(t);
    r = A - t - (CC & 0x01);
    CLR_NZVC;
    SET_FLAGS8(A, t, r);
    A = (uint8_t)r;
}

/* $83 SUBD immediate -**** */
void subd_im()
{
    uint32_t r, d;
    PAIR b;
    IMMWORD(b);
    d = D;
    r = d - b.d;
    CLR_NZVC;
    SET_FLAGS16(d, b.d, r);
    D = r;
}

/* $84 ANDA immediate -**0- */
void anda_im()
{
    uint8_t t;
    IMMBYTE(t);
    A &= t;
    CLR_NZV;
    SET_NZ8(A);
}

/* $85 BITA immediate -**0- */
void bita_im()
{
    uint8_t t, r;
    IMMBYTE(t);
    r = A & t;
    CLR_NZV;
    SET_NZ8(r);
}

/* $86 LDA immediate -**0- */
void lda_im()
{
    IMMBYTE(A);
    CLR_NZV;
    SET_NZ8(A);
}

/* is this a legal instruction? */
/* $87 STA immediate -**0- */
void sta_im()
{
    CLR_NZV;
    SET_NZ8(A);
    IMM8;
    WM(EAD, A);
}

/* $88 EORA immediate -**0- */
void eora_im()
{
    uint8_t t;
    IMMBYTE(t);
    A ^= t;
    CLR_NZV;
    SET_NZ8(A);
}

/* $89 ADCA immediate ***** */
void adca_im()
{
    uint16_t t, r;
    IMMBYTE(t);
    r = A + t + (CC & 0x01);
    CLR_HNZVC;
    SET_FLAGS8(A, t, r);
    SET_H(A, t, r);
    A = (uint8_t)r;
}

/* $8a ORA immediate -**0- */
void ora_im()
{
    uint8_t t;
    IMMBYTE(t);
    A |= t;
    CLR_NZV;
    SET_NZ8(A);
}

/* $8b ADDA immediate ***** */
void adda_im()
{
    uint16_t t, r;
    IMMBYTE(t);
    r = A + t;
    CLR_HNZVC;
    SET_FLAGS8(A, t, r);
    SET_H(A, t, r);
    A = (uint8_t)r;
}

/* $8c CMPX immediate -***- */
void cmpx_im()
{
    uint32_t r, d;
    PAIR b;
    IMMWORD(b);
    d = X;
    r = d - b.d;
    CLR_NZV;
    SET_NZ16(r);
    SET_V16(d, b.d, r);
}

/* $8c CPX immediate -**** (6801) */
void cpx_im()
{
    uint32_t r, d;
    PAIR b;
    IMMWORD(b);
    d = X;
    r = d - b.d;
    CLR_NZVC;
    SET_FLAGS16(d, b.d, r);
}


/* $8d BSR ----- */
void bsr()
{
    uint8_t t;
    IMMBYTE(t);
    PUSHWORD(pPC);
    PC += SIGNED(t);
}

/* $8e LDS immediate -**0- */
void lds_im()
{
    IMMWORD(pS);
    CLR_NZV;
    SET_NZ16(S);
}

/* $8f STS immediate -**0- */
void sts_im()
{
    CLR_NZV;
    SET_NZ16(S);
    IMM16;
    WM16(EAD, &pS);
}

/* $90 SUBA direct ?**** */
void suba_di()
{
    uint16_t t, r;
    DIRBYTE(t);
    r = A - t;
    CLR_NZVC;
    SET_FLAGS8(A, t, r);
    A = (uint8_t)r;
}

/* $91 CMPA direct ?**** */
void cmpa_di()
{
    uint16_t t, r;
    DIRBYTE(t);
    r = A - t;
    CLR_NZVC;
    SET_FLAGS8(A, t, r);
}

/* $92 SBCA direct ?**** */
void sbca_di()
{
    uint16_t t, r;
    DIRBYTE(t);
    r = A - t - (CC & 0x01);
    CLR_NZVC;
    SET_FLAGS8(A, t, r);
    A = (uint8_t)r;
}

/* $93 SUBD direct -**** */
void subd_di()
{
    uint32_t r, d;
    PAIR b;
    DIRWORD(b);
    d = D;
    r = d - b.d;
    CLR_NZVC;
    SET_FLAGS16(d, b.d, r);
    D = r;
}

/* $94 ANDA direct -**0- */
void anda_di()
{
    uint8_t t;
    DIRBYTE(t);
    A &= t;
    CLR_NZV;
    SET_NZ8(A);
}

/* $95 BITA direct -**0- */
void bita_di()
{
    uint8_t t, r;
    DIRBYTE(t);
    r = A & t;
    CLR_NZV;
    SET_NZ8(r);
}

/* $96 LDA direct -**0- */
void lda_di()
{
    DIRBYTE(A);
    CLR_NZV;
    SET_NZ8(A);
}

/* $97 STA direct -**0- */
void sta_di()
{
    CLR_NZV;
    SET_NZ8(A);
    DIRECT;
    WM(EAD, A);
}

/* $98 EORA direct -**0- */
void eora_di()
{
    uint8_t t;
    DIRBYTE(t);
    A ^= t;
    CLR_NZV;
    SET_NZ8(A);
}

/* $99 ADCA direct ***** */
void adca_di()
{
    uint16_t t, r;
    DIRBYTE(t);
    r = A + t + (CC & 0x01);
    CLR_HNZVC;
    SET_FLAGS8(A, t, r);
    SET_H(A, t, r);
    A = (uint8_t)r;
}

/* $9a ORA direct -**0- */
void ora_di()
{
    uint8_t t;
    DIRBYTE(t);
    A |= t;
    CLR_NZV;
    SET_NZ8(A);
}

/* $9b ADDA direct ***** */
void adda_di()
{
    uint16_t t, r;
    DIRBYTE(t);
    r = A + t;
    CLR_HNZVC;
    SET_FLAGS8(A, t, r);
    SET_H(A, t, r);
    A = (uint8_t)r;
}

/* $9c CMPX direct -***- */
void cmpx_di()
{
    uint32_t r, d;
    PAIR b;
    DIRWORD(b);
    d = X;
    r = d - b.d;
    CLR_NZV;
    SET_NZ16(r);
    SET_V16(d, b.d, r);
}

/* $9c CPX direct -**** (6801) */
void cpx_di()
{
    uint32_t r, d;
    PAIR b;
    DIRWORD(b);
    d = X;
    r = d - b.d;
    CLR_NZVC;
    SET_FLAGS16(d, b.d, r);
}

/* $9d JSR direct ----- */
void jsr_di()
{
    DIRECT;
    PUSHWORD(pPC);
    PC = EA;
}

/* $9e LDS direct -**0- */
void lds_di()
{
    DIRWORD(pS);
    CLR_NZV;
    SET_NZ16(S);
}

/* $9f STS direct -**0- */
void sts_di()
{
    CLR_NZV;
    SET_NZ16(S);
    DIRECT;
    WM16(EAD, &pS);
}

/* $a0 SUBA indexed ?**** */
void suba_ix()
{
    uint16_t t, r;
    IDXBYTE(t);
    r = A - t;
    CLR_NZVC;
    SET_FLAGS8(A, t, r);
    A = (uint8_t)r;
}

/* $a1 CMPA indexed ?**** */
void cmpa_ix()
{
    uint16_t t, r;
    IDXBYTE(t);
    r = A - t;
    CLR_NZVC;
    SET_FLAGS8(A, t, r);
}

/* $a2 SBCA indexed ?**** */
void sbca_ix()
{
    uint16_t t, r;
    IDXBYTE(t);
    r = A - t - (CC & 0x01);
    CLR_NZVC;
    SET_FLAGS8(A, t, r);
    A = (uint8_t)r;
}

/* $a3 SUBD indexed -**** */
void subd_ix()
{
    uint32_t r, d;
    PAIR b;
    IDXWORD(b);
    d = D;
    r = d - b.d;
    CLR_NZVC;
    SET_FLAGS16(d, b.d, r);
    D = r;
}

/* $a4 ANDA indexed -**0- */
void anda_ix()
{
    uint8_t t;
    IDXBYTE(t);
    A &= t;
    CLR_NZV;
    SET_NZ8(A);
}

/* $a5 BITA indexed -**0- */
void bita_ix()
{
    uint8_t t, r;
    IDXBYTE(t);
    r = A & t;
    CLR_NZV;
    SET_NZ8(r);
}

/* $a6 LDA indexed -**0- */
void lda_ix()
{
    IDXBYTE(A);
    CLR_NZV;
    SET_NZ8(A);
}

/* $a7 STA indexed -**0- */
void sta_ix()
{
    CLR_NZV;
    SET_NZ8(A);
    INDEXED;
    WM(EAD, A);
}

/* $a8 EORA indexed -**0- */
void eora_ix()
{
    uint8_t t;
    IDXBYTE(t);
    A ^= t;
    CLR_NZV;
    SET_NZ8(A);
}

/* $a9 ADCA indexed ***** */
void adca_ix()
{
    uint16_t t, r;
    IDXBYTE(t);
    r = A + t + (CC & 0x01);
    CLR_HNZVC;
    SET_FLAGS8(A, t, r);
    SET_H(A, t, r);
    A = (uint8_t)r;
}

/* $aa ORA indexed -**0- */
void ora_ix()
{
    uint8_t t;
    IDXBYTE(t);
    A |= t;
    CLR_NZV;
    SET_NZ8(A);
}

/* $ab ADDA indexed ***** */
void adda_ix()
{
    uint16_t t, r;
    IDXBYTE(t);
    r = A + t;
    CLR_HNZVC;
    SET_FLAGS8(A, t, r);
    SET_H(A, t, r);
    A = (uint8_t)r;
}

/* $ac CMPX indexed -***- */
void cmpx_ix()
{
    uint32_t r, d;
    PAIR b;
    IDXWORD(b);
    d = X;
    r = d - b.d;
    CLR_NZV;
    SET_NZ16(r);
    SET_V16(d, b.d, r);
}

/* $ac CPX indexed -**** (6801)*/
void cpx_ix()
{
    uint32_t r, d;
    PAIR b;
    IDXWORD(b);
    d = X;
    r = d - b.d;
    CLR_NZVC;
    SET_FLAGS16(d, b.d, r);
}

/* $ad JSR indexed ----- */
void jsr_ix()
{
    INDEXED;
    PUSHWORD(pPC);
    PC = EA;
}

/* $ae LDS indexed -**0- */
void lds_ix()
{
    IDXWORD(pS);
    CLR_NZV;
    SET_NZ16(S);
}

/* $af STS indexed -**0- */
void sts_ix()
{
    CLR_NZV;
    SET_NZ16(S);
    INDEXED;
    WM16(EAD, &pS);
}

/* $b0 SUBA extended ?**** */
void suba_ex()
{
    uint16_t t, r;
    EXTBYTE(t);
    r = A - t;
    CLR_NZVC;
    SET_FLAGS8(A, t, r);
    A = (uint8_t)r;
}

/* $b1 CMPA extended ?**** */
void cmpa_ex()
{
    uint16_t t, r;
    EXTBYTE(t);
    r = A - t;
    CLR_NZVC;
    SET_FLAGS8(A, t, r);
}

/* $b2 SBCA extended ?**** */
void sbca_ex()
{
    uint16_t t, r;
    EXTBYTE(t);
    r = A - t - (CC & 0x01);
    CLR_NZVC;
    SET_FLAGS8(A, t, r);
    A = (uint8_t)r;
}

/* $b3 SUBD extended -**** */
void subd_ex()
{
    uint32_t r, d;
    PAIR b;
    EXTWORD(b);
    d = D;
    r = d - b.d;
    CLR_NZVC;
    SET_FLAGS16(d, b.d, r);
    D = r;
}

/* $b4 ANDA extended -**0- */
void anda_ex()
{
    uint8_t t;
    EXTBYTE(t);
    A &= t;
    CLR_NZV;
    SET_NZ8(A);
}

/* $b5 BITA extended -**0- */
void bita_ex()
{
    uint8_t t, r;
    EXTBYTE(t);
    r = A & t;
    CLR_NZV;
    SET_NZ8(r);
}

/* $b6 LDA extended -**0- */
void lda_ex()
{
    EXTBYTE(A);
    CLR_NZV;
    SET_NZ8(A);
}

/* $b7 STA extended -**0- */
void sta_ex()
{
    CLR_NZV;
    SET_NZ8(A);
    EXTENDED;
    WM(EAD, A);
}

/* $b8 EORA extended -**0- */
void eora_ex()
{
    uint8_t t;
    EXTBYTE(t);
    A ^= t;
    CLR_NZV;
    SET_NZ8(A);
}

/* $b9 ADCA extended ***** */
void adca_ex()
{
    uint16_t t, r;
    EXTBYTE(t);
    r = A + t + (CC & 0x01);
    CLR_HNZVC;
    SET_FLAGS8(A, t, r);
    SET_H(A, t, r);
    A = (uint8_t)r;
}

/* $ba ORA extended -**0- */
void ora_ex()
{
    uint8_t t;
    EXTBYTE(t);
    A |= t;
    CLR_NZV;
    SET_NZ8(A);
}

/* $bb ADDA extended ***** */
void adda_ex()
{
    uint16_t t, r;
    EXTBYTE(t);
    r = A + t;
    CLR_HNZVC;
    SET_FLAGS8(A, t, r);
    SET_H(A, t, r);
    A = (uint8_t)r;
}

/* $bc CMPX extended -***- */
void cmpx_ex()
{
    uint32_t r, d;
    PAIR b;
    EXTWORD(b);
    d = X;
    r = d - b.d;
    CLR_NZV;
    SET_NZ16(r);
    SET_V16(d, b.d, r);
}

/* $bc CPX extended -**** (6801) */
void cpx_ex()
{
    uint32_t r, d;
    PAIR b;
    EXTWORD(b);
    d = X;
    r = d - b.d;
    CLR_NZVC;
    SET_FLAGS16(d, b.d, r);
}

/* $bd JSR extended ----- */
void jsr_ex()
{
    EXTENDED;
    PUSHWORD(pPC);
    PC = EA;
}

/* $be LDS extended -**0- */
void lds_ex()
{
    EXTWORD(pS);
    CLR_NZV;
    SET_NZ16(S);
}

/* $bf STS extended -**0- */
void sts_ex()
{
    CLR_NZV;
    SET_NZ16(S);
    EXTENDED;
    WM16(EAD, &pS);
}

/* $c0 SUBB immediate ?**** */
void subb_im()
{
    uint16_t t, r;
    IMMBYTE(t);
    r = B - t;
    CLR_NZVC;
    SET_FLAGS8(B, t, r);
    B = (uint8_t)r;
}

/* $c1 CMPB immediate ?**** */
void cmpb_im()
{
    uint16_t t, r;
    IMMBYTE(t);
    r = B - t;
    CLR_NZVC;
    SET_FLAGS8(B, t, r);
}

/* $c2 SBCB immediate ?**** */
void sbcb_im()
{
    uint16_t t, r;
    IMMBYTE(t);
    r = B - t - (CC & 0x01);
    CLR_NZVC;
    SET_FLAGS8(B, t, r);
    B = (uint8_t)r;
}

/* $c3 ADDD immediate -**** */
void addd_im()
{
    uint32_t r, d;
    PAIR b;
    IMMWORD(b);
    d = D;
    r = d + b.d;
    CLR_NZVC;
    SET_FLAGS16(d, b.d, r);
    D = r;
}

/* $c4 ANDB immediate -**0- */
void andb_im()
{
    uint8_t t;
    IMMBYTE(t);
    B &= t;
    CLR_NZV;
    SET_NZ8(B);
}

/* $c5 BITB immediate -**0- */
void bitb_im()
{
    uint8_t t, r;
    IMMBYTE(t);
    r = B & t;
    CLR_NZV;
    SET_NZ8(r);
}

/* $c6 LDB immediate -**0- */
void ldb_im()
{
    IMMBYTE(B);
    CLR_NZV;
    SET_NZ8(B);
}

/* is this a legal instruction? */
/* $c7 STB immediate -**0- */
void stb_im()
{
    CLR_NZV;
    SET_NZ8(B);
    IMM8;
    WM(EAD, B);
}

/* $c8 EORB immediate -**0- */
void eorb_im()
{
    uint8_t t;
    IMMBYTE(t);
    B ^= t;
    CLR_NZV;
    SET_NZ8(B);
}

/* $c9 ADCB immediate ***** */
void adcb_im()
{
    uint16_t t, r;
    IMMBYTE(t);
    r = B + t + (CC & 0x01);
    CLR_HNZVC;
    SET_FLAGS8(B, t, r);
    SET_H(B, t, r);
    B = (uint8_t)r;
}

/* $ca ORB immediate -**0- */
void orb_im()
{
    uint8_t t;
    IMMBYTE(t);
    B |= t;
    CLR_NZV;
    SET_NZ8(B);
}

/* $cb ADDB immediate ***** */
void addb_im()
{
    uint16_t t, r;
    IMMBYTE(t);
    r = B + t;
    CLR_HNZVC;
    SET_FLAGS8(B, t, r);
    SET_H(B, t, r);
    B = (uint8_t)r;
}

/* $CC LDD immediate -**0- */
void ldd_im()
{
    IMMWORD(pD);
    CLR_NZV;
    SET_NZ16(D);
}

/* is this a legal instruction? */
/* $cd STD immediate -**0- */
void std_im()
{
    IMM16;
    CLR_NZV;
    SET_NZ16(D);
    WM16(EAD, &pD);
}

/* $ce LDX immediate -**0- */
void ldx_im()
{
    IMMWORD(pX);
    CLR_NZV;
    SET_NZ16(X);
}

/* $cf STX immediate -**0- */
void stx_im()
{
    CLR_NZV;
    SET_NZ16(X);
    IMM16;
    WM16(EAD, &pX);
}

/* $d0 SUBB direct ?**** */
void subb_di()
{
    uint16_t t, r;
    DIRBYTE(t);
    r = B - t;
    CLR_NZVC;
    SET_FLAGS8(B, t, r);
    B = (uint8_t)r;
}

/* $d1 CMPB direct ?**** */
void cmpb_di()
{
    uint16_t t, r;
    DIRBYTE(t);
    r = B - t;
    CLR_NZVC;
    SET_FLAGS8(B, t, r);
}

/* $d2 SBCB direct ?**** */
void sbcb_di()
{
    uint16_t t, r;
    DIRBYTE(t);
    r = B - t - (CC & 0x01);
    CLR_NZVC;
    SET_FLAGS8(B, t, r);
    B = (uint8_t)r;
}

/* $d3 ADDD direct -**** */
void addd_di()
{
    uint32_t r, d;
    PAIR b;
    DIRWORD(b);
    d = D;
    r = d + b.d;
    CLR_NZVC;
    SET_FLAGS16(d, b.d, r);
    D = r;
}

/* $d4 ANDB direct -**0- */
void andb_di()
{
    uint8_t t;
    DIRBYTE(t);
    B &= t;
    CLR_NZV;
    SET_NZ8(B);
}

/* $d5 BITB direct -**0- */
void bitb_di()
{
    uint8_t t, r;
    DIRBYTE(t);
    r = B & t;
    CLR_NZV;
    SET_NZ8(r);
}

/* $d6 LDB direct -**0- */
void ldb_di()
{
    DIRBYTE(B);
    CLR_NZV;
    SET_NZ8(B);
}

/* $d7 STB direct -**0- */
void stb_di()
{
    CLR_NZV;
    SET_NZ8(B);
    DIRECT;
    WM(EAD, B);
}

/* $d8 EORB direct -**0- */
void eorb_di()
{
    uint8_t t;
    DIRBYTE(t);
    B ^= t;
    CLR_NZV;
    SET_NZ8(B);
}

/* $d9 ADCB direct ***** */
void adcb_di()
{
    uint16_t t, r;
    DIRBYTE(t);
    r = B + t + (CC & 0x01);
    CLR_HNZVC;
    SET_FLAGS8(B, t, r);
    SET_H(B, t, r);
    B = (uint8_t)r;
}

/* $da ORB direct -**0- */
void orb_di()
{
    uint8_t t;
    DIRBYTE(t);
    B |= t;
    CLR_NZV;
    SET_NZ8(B);
}

/* $db ADDB direct ***** */
void addb_di()
{
    uint16_t t, r;
    DIRBYTE(t);
    r = B + t;
    CLR_HNZVC;
    SET_FLAGS8(B, t, r);
    SET_H(B, t, r);
    B = (uint8_t)r;
}

/* $dc LDD direct -**0- */
void ldd_di()
{
    DIRWORD(pD);
    CLR_NZV;
    SET_NZ16(D);
}

/* $dd STD direct -**0- */
void std_di()
{
    DIRECT;
    CLR_NZV;
    SET_NZ16(D);
    WM16(EAD, &pD);
}

/* $de LDX direct -**0- */
void ldx_di()
{
    DIRWORD(pX);
    CLR_NZV;
    SET_NZ16(X);
}

/* $dF STX direct -**0- */
void stx_di()
{
    CLR_NZV;
    SET_NZ16(X);
    DIRECT;
    WM16(EAD, &pX);
}

/* $e0 SUBB indexed ?**** */
void subb_ix()
{
    uint16_t t, r;
    IDXBYTE(t);
    r = B - t;
    CLR_NZVC;
    SET_FLAGS8(B, t, r);
    B = (uint8_t)r;
}

/* $e1 CMPB indexed ?**** */
void cmpb_ix()
{
    uint16_t t, r;
    IDXBYTE(t);
    r = B - t;
    CLR_NZVC;
    SET_FLAGS8(B, t, r);
}

/* $e2 SBCB indexed ?**** */
void sbcb_ix()
{
    uint16_t t, r;
    IDXBYTE(t);
    r = B - t - (CC & 0x01);
    CLR_NZVC;
    SET_FLAGS8(B, t, r);
    B = (uint8_t)r;
}

/* $e3 ADDD indexed -**** */
void addd_ix()
{
    uint32_t r, d;
    PAIR b;
    IDXWORD(b);
    d = D;
    r = d + b.d;
    CLR_NZVC;
    SET_FLAGS16(d, b.d, r);
    D = r;
}

/* $e4 ANDB indexed -**0- */
void andb_ix()
{
    uint8_t t;
    IDXBYTE(t);
    B &= t;
    CLR_NZV;
    SET_NZ8(B);
}

/* $e5 BITB indexed -**0- */
void bitb_ix()
{
    uint8_t t, r;
    IDXBYTE(t);
    r = B & t;
    CLR_NZV;
    SET_NZ8(r);
}

/* $e6 LDB indexed -**0- */
void ldb_ix()
{
    IDXBYTE(B);
    CLR_NZV;
    SET_NZ8(B);
}

/* $e7 STB indexed -**0- */
void stb_ix()
{
    CLR_NZV;
    SET_NZ8(B);
    INDEXED;
    WM(EAD, B);
}

/* $e8 EORB indexed -**0- */
void eorb_ix()
{
    uint8_t t;
    IDXBYTE(t);
    B ^= t;
    CLR_NZV;
    SET_NZ8(B);
}

/* $e9 ADCB indexed ***** */
void adcb_ix()
{
    uint16_t t, r;
    IDXBYTE(t);
    r = B + t + (CC & 0x01);
    CLR_HNZVC;
    SET_FLAGS8(B, t, r);
    SET_H(B, t, r);
    B = (uint8_t)r;
}

/* $ea ORB indexed -**0- */
void orb_ix()
{
    uint8_t t;
    IDXBYTE(t);
    B |= t;
    CLR_NZV;
    SET_NZ8(B);
}

/* $eb ADDB indexed ***** */
void addb_ix()
{
    uint16_t t, r;
    IDXBYTE(t);
    r = B + t;
    CLR_HNZVC;
    SET_FLAGS8(B, t, r);
    SET_H(B, t, r);
    B = (uint8_t)r;
}

/* $ec LDD indexed -**0- */
void ldd_ix()
{
    IDXWORD(pD);
    CLR_NZV;
    SET_NZ16(D);
}

/* $ec ADX immediate -**** */ /* MB8861 only */
void adx_im()
{
    uint32_t r, d, t;
    IMMBYTE(t);
    d = X;
    r = d + t;
    CLR_NZVC;
    SET_FLAGS16(d, t, r);
    X = r;
}

/* $ed STD indexed -**0- */
void std_ix()
{
    INDEXED;
    CLR_NZV;
    SET_NZ16(D);
    WM16(EAD, &pD);
}

/* $ee LDX indexed -**0- */
void ldx_ix()
{
    IDXWORD(pX);
    CLR_NZV;
    SET_NZ16(X);
}

/* $ef STX indexed -**0- */
void stx_ix()
{
    CLR_NZV;
    SET_NZ16(X);
    INDEXED;
    WM16(EAD, &pX);
}

/* $f0 SUBB extended ?**** */
void subb_ex()
{
    uint16_t t, r;
    EXTBYTE(t);
    r = B - t;
    CLR_NZVC;
    SET_FLAGS8(B, t, r);
    B = (uint8_t)r;
}

/* $f1 CMPB extended ?**** */
void cmpb_ex()
{
    uint16_t t, r;
    EXTBYTE(t);
    r = B - t;
    CLR_NZVC;
    SET_FLAGS8(B, t, r);
}

/* $f2 SBCB extended ?**** */
void sbcb_ex()
{
    uint16_t t, r;
    EXTBYTE(t);
    r = B - t - (CC & 0x01);
    CLR_NZVC;
    SET_FLAGS8(B, t, r);
    B = (uint8_t)r;
}

/* $f3 ADDD extended -**** */
void addd_ex()
{
    uint32_t r, d;
    PAIR b;
    EXTWORD(b);
    d = D;
    r = d + b.d;
    CLR_NZVC;
    SET_FLAGS16(d, b.d, r);
    D = r;
}

/* $f4 ANDB extended -**0- */
void andb_ex()
{
    uint8_t t;
    EXTBYTE(t);
    B &= t;
    CLR_NZV;
    SET_NZ8(B);
}

/* $f5 BITB extended -**0- */
void bitb_ex()
{
    uint8_t t, r;
    EXTBYTE(t);
    r = B & t;
    CLR_NZV;
    SET_NZ8(r);
}

/* $f6 LDB extended -**0- */
void ldb_ex()
{
    EXTBYTE(B);
    CLR_NZV;
    SET_NZ8(B);
}

/* $f7 STB extended -**0- */
void stb_ex()
{
    CLR_NZV;
    SET_NZ8(B);
    EXTENDED;
    WM(EAD, B);
}

/* $f8 EORB extended -**0- */
void eorb_ex()
{
    uint8_t t;
    EXTBYTE(t);
    B ^= t;
    CLR_NZV;
    SET_NZ8(B);
}

/* $f9 ADCB extended ***** */
void adcb_ex()
{
    uint16_t t, r;
    EXTBYTE(t);
    r = B + t + (CC & 0x01);
    CLR_HNZVC;
    SET_FLAGS8(B, t, r);
    SET_H(B, t, r);
    B = (uint8_t)r;
}

/* $fa ORB extended -**0- */
void orb_ex()
{
    uint8_t t;
    EXTBYTE(t);
    B |= t;
    CLR_NZV;
    SET_NZ8(B);
}

/* $fb ADDB extended ***** */
void addb_ex()
{
    uint16_t t, r;
    EXTBYTE(t);
    r = B + t;
    CLR_HNZVC;
    SET_FLAGS8(B, t, r);
    SET_H(B, t, r);
    B = (uint8_t)r;
}

/* $fc LDD extended -**0- */
void ldd_ex()
{
    EXTWORD(pD);
    CLR_NZV;
    SET_NZ16(D);
}

/* $fc ADX immediate -**** */ /* MB8861 only */
void adx_ex()
{
    uint32_t r, d;
    PAIR b;
    EXTWORD(b);
    d = X;
    r = d + b.d;
    CLR_NZVC;
    SET_FLAGS16(d, b.d, r);
    X = r;
}

/* $fd STD extended -**0- */
void std_ex()
{
    EXTENDED;
    CLR_NZV;
    SET_NZ16(D);
    WM16(EAD, &pD);
}

/* $fe LDX extended -**0- */
void ldx_ex()
{
    EXTWORD(pX);
    CLR_NZV;
    SET_NZ16(X);
}

/* $ff STX extended -**0- */
void stx_ex()
{
    CLR_NZV;
    SET_NZ16(X);
    EXTENDED;
    WM16(EAD, &pX);
}
