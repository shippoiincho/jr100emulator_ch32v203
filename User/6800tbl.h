INLINE void aba(void);
INLINE void abx(void);
INLINE void adca_di(void);
INLINE void adca_ex(void);
INLINE void adca_im(void);
INLINE void adca_ix(void);
INLINE void adcb_di(void);
INLINE void adcb_ex(void);
INLINE void adcb_im(void);
INLINE void adcb_ix(void);
INLINE void adcx_im(void);
INLINE void adda_di(void);
INLINE void adda_ex(void);
INLINE void adda_im(void);
INLINE void adda_ix(void);
INLINE void addb_di(void);
INLINE void addb_ex(void);
INLINE void addb_im(void);
INLINE void addb_ix(void);
INLINE void addd_di(void);
INLINE void addd_ex(void);
INLINE void addx_ex(void);
INLINE void addd_im(void);
INLINE void addd_ix(void);
INLINE void aim_di(void);
INLINE void aim_ix(void);
INLINE void anda_di(void);
INLINE void anda_ex(void);
INLINE void anda_im(void);
INLINE void anda_ix(void);
INLINE void andb_di(void);
INLINE void andb_ex(void);
INLINE void andb_im(void);
INLINE void andb_ix(void);
INLINE void asl_ex(void);
INLINE void asl_ix(void);
INLINE void asla(void);
INLINE void aslb(void);
INLINE void asld(void);
INLINE void asr_ex(void);
INLINE void asr_ix(void);
INLINE void asra(void);
INLINE void asrb(void);
INLINE void bcc(void);
INLINE void bcs(void);
INLINE void beq(void);
INLINE void bge(void);
INLINE void bgt(void);
INLINE void bhi(void);
INLINE void bita_di(void);
INLINE void bita_ex(void);
INLINE void bita_im(void);
INLINE void bita_ix(void);
INLINE void bitb_di(void);
INLINE void bitb_ex(void);
INLINE void bitb_im(void);
INLINE void bitb_ix(void);
INLINE void ble(void);
INLINE void bls(void);
INLINE void blt(void);
INLINE void bmi(void);
INLINE void bne(void);
INLINE void bpl(void);
INLINE void bra(void);
INLINE void brn(void);
INLINE void bsr(void);
INLINE void bvc(void);
INLINE void bvs(void);
INLINE void cba(void);
INLINE void clc(void);
INLINE void cli(void);
INLINE void clr_ex(void);
INLINE void clr_ix(void);
INLINE void clra(void);
INLINE void clrb(void);
INLINE void clv(void);
INLINE void cmpa_di(void);
INLINE void cmpa_ex(void);
INLINE void cmpa_im(void);
INLINE void cmpa_ix(void);
INLINE void cmpb_di(void);
INLINE void cmpb_ex(void);
INLINE void cmpb_im(void);
INLINE void cmpb_ix(void);
INLINE void cmpx_di(void);
INLINE void cmpx_ex(void);
INLINE void cmpx_im(void);
INLINE void cmpx_ix(void);
INLINE void com_ex(void);
INLINE void com_ix(void);
INLINE void coma(void);
INLINE void comb(void);
INLINE void daa(void);
INLINE void dec_ex(void);
INLINE void dec_ix(void);
INLINE void deca(void);
INLINE void decb(void);
INLINE void des(void);
INLINE void dex(void);
INLINE void eim_di(void);
INLINE void eim_ix(void);
INLINE void eora_di(void);
INLINE void eora_ex(void);
INLINE void eora_im(void);
INLINE void eora_ix(void);
INLINE void eorb_di(void);
INLINE void eorb_ex(void);
INLINE void eorb_im(void);
INLINE void eorb_ix(void);
//INLINE void illegal(void);
static void illegal(void);
INLINE void inc_ex(void);
INLINE void inc_ix(void);
INLINE void inca(void);
INLINE void incb(void);
INLINE void ins(void);
INLINE void inx(void);
INLINE void jmp_ex(void);
INLINE void jmp_ix(void);
INLINE void jsr_di(void);
INLINE void jsr_ex(void);
INLINE void jsr_ix(void);
INLINE void lda_di(void);
INLINE void lda_ex(void);
INLINE void lda_im(void);
INLINE void lda_ix(void);
INLINE void ldb_di(void);
INLINE void ldb_ex(void);
INLINE void ldb_im(void);
INLINE void ldb_ix(void);
INLINE void ldd_di(void);
INLINE void ldd_ex(void);
INLINE void ldd_im(void);
INLINE void ldd_ix(void);
INLINE void lds_di(void);
INLINE void lds_ex(void);
INLINE void lds_im(void);
INLINE void lds_ix(void);
INLINE void ldx_di(void);
INLINE void ldx_ex(void);
INLINE void ldx_im(void);
INLINE void ldx_ix(void);
INLINE void lsr_ex(void);
INLINE void lsr_ix(void);
INLINE void lsra(void);
INLINE void lsrb(void);
INLINE void lsrd(void);
INLINE void mul(void);
INLINE void neg_ex(void);
INLINE void neg_ix(void);
INLINE void nega(void);
INLINE void negb(void);
INLINE void nop(void);
INLINE void oim_di(void);
INLINE void oim_ix(void);
INLINE void ora_di(void);
INLINE void ora_ex(void);
INLINE void ora_im(void);
INLINE void ora_ix(void);
INLINE void orb_di(void);
INLINE void orb_ex(void);
INLINE void orb_im(void);
INLINE void orb_ix(void);
INLINE void psha(void);
INLINE void pshb(void);
INLINE void pshx(void);
INLINE void pula(void);
INLINE void pulb(void);
INLINE void pulx(void);
INLINE void rol_ex(void);
INLINE void rol_ix(void);
INLINE void rola(void);
INLINE void rolb(void);
INLINE void ror_ex(void);
INLINE void ror_ix(void);
INLINE void rora(void);
INLINE void rorb(void);
INLINE void rti(void);
INLINE void rts(void);
INLINE void sba(void);
INLINE void sbca_di(void);
INLINE void sbca_ex(void);
INLINE void sbca_im(void);
INLINE void sbca_ix(void);
INLINE void sbcb_di(void);
INLINE void sbcb_ex(void);
INLINE void sbcb_im(void);
INLINE void sbcb_ix(void);
INLINE void sec(void);
INLINE void sei(void);
INLINE void sev(void);
INLINE void sta_di(void);
INLINE void sta_ex(void);
INLINE void sta_im(void);
INLINE void sta_ix(void);
INLINE void stb_di(void);
INLINE void stb_ex(void);
INLINE void stb_im(void);
INLINE void stb_ix(void);
INLINE void std_di(void);
INLINE void std_ex(void);
INLINE void std_im(void);
INLINE void std_ix(void);
INLINE void sts_di(void);
INLINE void sts_ex(void);
INLINE void sts_im(void);
INLINE void sts_ix(void);
INLINE void stx_di(void);
INLINE void stx_ex(void);
INLINE void stx_im(void);
INLINE void stx_ix(void);
INLINE void suba_di(void);
INLINE void suba_ex(void);
INLINE void suba_im(void);
INLINE void suba_ix(void);
INLINE void subb_di(void);
INLINE void subb_ex(void);
INLINE void subb_im(void);
INLINE void subb_ix(void);
INLINE void subd_di(void);
INLINE void subd_ex(void);
INLINE void subd_im(void);
INLINE void subd_ix(void);
INLINE void swi(void);
INLINE void tab(void);
INLINE void tap(void);
INLINE void tba(void);
INLINE void tim_di(void);
INLINE void tim_ix(void);
INLINE void tpa(void);
INLINE void tst_ex(void);
INLINE void tst_ix(void);
INLINE void tsta(void);
INLINE void tstb(void);
INLINE void tsx(void);
INLINE void txs(void);
INLINE void undoc1(void);
INLINE void undoc2(void);
INLINE void wai(void);

INLINE void cpx_di(void);
INLINE void cpx_ex(void);
INLINE void cpx_im(void);
INLINE void cpx_ix(void);

INLINE void nim_ix(void);
INLINE void oim_ix_mb8861(void);
INLINE void xim_ix(void);
INLINE void tmm_ix(void);
INLINE void adx_im(void);
INLINE void adx_ex(void);

static void (*m6800_insn[0x100])(void) = {
illegal,nop,	illegal,illegal,illegal,illegal,tap,	tpa,
inx,	dex,	clv,	sev,	clc,	sec,	cli,	sei,
sba,	cba,	illegal,illegal,illegal,illegal,tab,	tba,
illegal,daa,	illegal,aba,	illegal,illegal,illegal,illegal,
bra,	brn,	bhi,	bls,	bcc,	bcs,	bne,	beq,
bvc,	bvs,	bpl,	bmi,	bge,	blt,	bgt,	ble,
tsx,	ins,	pula,	pulb,	des,	txs,	psha,	pshb,
illegal,rts,	illegal,rti,	illegal,illegal,wai,	swi,
nega,	illegal,illegal,coma,	lsra,	illegal,rora,	asra,
asla,	rola,	deca,	illegal,inca,	tsta,	illegal,clra,
negb,	illegal,illegal,comb,	lsrb,	illegal,rorb,	asrb,
aslb,	rolb,	decb,	illegal,incb,	tstb,	illegal,clrb,
neg_ix, illegal,illegal,com_ix, lsr_ix, illegal,ror_ix, asr_ix,
asl_ix, rol_ix, dec_ix, illegal,inc_ix, tst_ix, jmp_ix, clr_ix,
neg_ex, illegal,illegal,com_ex, lsr_ex, illegal,ror_ex, asr_ex,
asl_ex, rol_ex, dec_ex, illegal,inc_ex, tst_ex, jmp_ex, clr_ex,
suba_im,cmpa_im,sbca_im,illegal,anda_im,bita_im,lda_im, sta_im,
eora_im,adca_im,ora_im, adda_im,cmpx_im,bsr,	lds_im, sts_im,
suba_di,cmpa_di,sbca_di,illegal,anda_di,bita_di,lda_di, sta_di,
eora_di,adca_di,ora_di, adda_di,cmpx_di,jsr_di, lds_di, sts_di,
suba_ix,cmpa_ix,sbca_ix,illegal,anda_ix,bita_ix,lda_ix, sta_ix,
eora_ix,adca_ix,ora_ix, adda_ix,cmpx_ix,jsr_ix, lds_ix, sts_ix,
suba_ex,cmpa_ex,sbca_ex,illegal,anda_ex,bita_ex,lda_ex, sta_ex,
eora_ex,adca_ex,ora_ex, adda_ex,cmpx_ex,jsr_ex, lds_ex, sts_ex,
subb_im,cmpb_im,sbcb_im,illegal,andb_im,bitb_im,ldb_im, stb_im,
eorb_im,adcb_im,orb_im, addb_im,illegal,illegal,ldx_im, stx_im,
subb_di,cmpb_di,sbcb_di,illegal,andb_di,bitb_di,ldb_di, stb_di,
eorb_di,adcb_di,orb_di, addb_di,illegal,illegal,ldx_di, stx_di,
subb_ix,cmpb_ix,sbcb_ix,illegal,andb_ix,bitb_ix,ldb_ix, stb_ix,
eorb_ix,adcb_ix,orb_ix, addb_ix,illegal,illegal,ldx_ix, stx_ix,
subb_ex,cmpb_ex,sbcb_ex,illegal,andb_ex,bitb_ex,ldb_ex, stb_ex,
eorb_ex,adcb_ex,orb_ex, addb_ex,illegal,illegal,ldx_ex, stx_ex
};
