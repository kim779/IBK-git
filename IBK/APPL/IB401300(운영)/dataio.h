#pragma once

#define MAX_R       30
#define TK_PIBO4013		(1)
#define TK_JISU 		(2)
#define TK_OPTION		(4)
#define TK_FUTURE		(100)

typedef struct _OPTION_HEADER {
	char	cjym[11][6];		// CALL �ɼ� ���� Ÿ��Ʋ
	char	pjym[11][6];		// PUT  �ɼ� ���� Ÿ��Ʋ
} OPTION_HEADER;

typedef struct _FUTURE_HEADER {
    char    codx[8];        /* �����ڵ� (8)                 */
    char    hnam[20];       /* �ѱ۸�                       */
    char    enam[20];       /* ������                       */
    char    mchk;           /* �Ÿ��������                 */
							/* 0x01:���尡���              */
							/* 0x02:���Ǻ����������        */
							/* 0x04:���������������        */
} FUTURE_HEADER;

struct pibo4013_mid 
{
	char type[1];
	char prd[1];
    char func[1];    /* start    'F'         */
    char mymd[6];    /* �����               */
};

struct pibo4013_grid {
    char    c_rtcd[9];  /* ��RTS CODE           */
	char	c_jega[8];
	char	c_koga[8];
	char	c_siga[8];
	char	c_raw[8];
	char	c_bega[8];
	char	c_theta[8];
	char	c_gamma[8];
	char	c_delta[8];
    char    c_mdif[8];  /* ������(����)         */
    char    c_mgjv[8];  /* �ݹ̰�������         */
    char    c_gvol[8];  /* �ݰŷ���             */
    char    c_msga[8];  /* �ݸż�               */
    char    c_mdga[8];  /* �ݸŵ�               */
    char    c_rate[8];  /* �ݵ����             */
    char    c_diff[7];  /* �ݴ��               */
    char    c_curr[8];  /* �����簡             */
	char    c_ycur[8];  /* �ݿ���				*/
    char    hang[8];    /* ��簡               */
    char    jihs[8];    /* ����ȯ��             */
    char    p_rtcd[9];  /* ǲRTS CODE           */
    char    p_curr[8];  /* ǲ���簡             */
	char    p_ycur[8];  /* ǲ����             */
    char    p_diff[7];  /* ǲ���               */
    char    p_rate[8];  /* ǲ�����             */
    char    p_mdga[8];  /* ǲ�ŵ�               */
    char    p_msga[8];  /* ǲ�ż�               */
    char    p_gvol[8];  /* ǲ�ŷ���             */
    char    p_mgjv[8];  /* ǲ�̰�������         */
    char    p_mdif[8];  /* ǲ����(����)         */
	char	p_delta[8];
	char	p_gamma[8];
	char	p_theta[8];
	char	p_bega[8];
	char	p_raw[8];
	char	p_siga[8];
	char    p_koga[8];
	char	p_jega[8];
};

struct pibo4013_mod {
    char    atmg[8];    /* ATM  */
    char    grec[4];
    struct  pibo4013_grid grid[MAX_R];
};

struct pibo4013_rank {
    int     hang;               /* ��簡       */
    int     jihs;               /* ����ȯ���   */
    struct  mjopsi  *c_opsi;    /* call �ɼ�    */
    struct  mjopsi  *p_opsi;    /* put �ɼ�     */
};


typedef struct _OPTION_DATA
{
	char code[12];
	int     mdif;	// ����(����)
	int     mgjv;	// �̰�������
	int     gvol;	// �ŷ���
	double  gjga;	// ���ذ�
	double  msga;	// �ż�ȣ��
	double  mdga;	// �ŵ�ȣ��
	double  rate;	// �����
	double  diff;	// ���ϴ��
	double  curr;	// ���簡
	double  ycurr;	// ����
	double  hsga;	// ��簡
	double  jisu;	// ����ȯ��
	double  grdo;	// ������
	double  irga;	// �̷а�
	double  sggc;	// �ð���ġ
	double  rho;	// �ο�
	double  vega;	// ����
	double  teta;	// ��Ÿ
	double  gmma;	// ����
	double  delt;	// ��Ÿ
	double  krxrho;	// �ο�
	double  krxvega;	// ����
	double  krxteta;	// ��Ÿ
	double  krxgmma;	// ����
	double  krxdelt;	// ��Ÿ
	double  iv;	// IV
	double  siga; //�ð�
	double  koga; //��
	double  jega; //����
	BOOL b_mdif,b_mgjv,b_gvol,b_gjga,b_msga,b_mdga,b_rate,b_diff,b_curr,b_hsga,b_ycurr;
	BOOL b_jisu,b_grdo,b_irga,b_sggc,b_row,b_vega,b_teta,b_gmma,b_delt,b_iv,b_krxrow,b_krxvega,b_krxteta,b_krxgmma,b_krxdelt;
	BOOL b_siga,b_koga,b_jega;
	int  grid_row;

	struct _OPTION_DATA() { ZeroMemory(this, sizeof(struct _OPTION_DATA)); }
	void set_all(BOOL bSet) { b_krxrow = b_krxvega = b_krxteta = b_krxgmma = b_krxdelt = b_mdif = b_mgjv = b_gvol = b_gjga = b_msga = b_mdga = b_rate = b_diff = b_curr = b_ycurr = b_hsga = b_jisu = b_grdo = b_irga = b_sggc = b_row = b_vega = b_teta = b_gmma = b_delt = b_iv = b_jega = b_koga = b_siga = bSet; }
	void set_mdif(int     _mdif) { if (mdif != _mdif ) { mdif = _mdif; b_mdif = TRUE; }}
	void set_mgjv(int     _mgjv) { if (mgjv != _mgjv ) { mgjv = _mgjv; b_mgjv = TRUE; }}
	void set_gvol(int     _gvol) { if (gvol != _gvol ) { gvol = _gvol; b_gvol = TRUE; }}
	void set_gjga(double  _gjga) { if (gjga != _gjga ) { gjga = _gjga; b_gjga = TRUE; }}
	void set_msga(double  _msga) { if (msga != _msga ) { msga = _msga; b_msga = TRUE; }}
	void set_mdga(double  _mdga) { if (mdga != _mdga ) { mdga = _mdga; b_mdga = TRUE; }}
	void set_rate(double  _rate) { if (rate != _rate ) { rate = _rate; b_rate = TRUE; }}
	void set_diff(double  _diff) { if (diff != _diff ) { diff = _diff; b_diff = TRUE; }}
	void set_curr(double  _curr) { if (curr != _curr ) { curr = _curr; b_curr = TRUE; }}
	void set_ycurr(double  _ycurr) { if (ycurr != _ycurr ) { ycurr = _ycurr; b_ycurr = TRUE; }}
	void set_hsga(double  _hsga) { if (hsga != _hsga ) { hsga = _hsga; b_hsga = TRUE; }}
	void set_jisu(double  _jisu) { if (jisu != _jisu ) { jisu = _jisu; b_jisu = TRUE; }}
	void set_grdo(double  _grdo) { if (grdo != _grdo ) { grdo = _grdo; b_grdo = TRUE; }}
	void set_irga(double  _irga) { if (irga != _irga ) { irga = _irga; b_irga = TRUE; }}
	void set_sggc(double  _sggc) { if (sggc != _sggc ) { sggc = _sggc; b_sggc = TRUE; }}
	void set_rho (double  _row ) { if (rho  != _row  ) { rho  = _row ; b_row  = TRUE; }}
	void set_vega(double  _vega) { if (vega != _vega ) { vega = _vega; b_vega = TRUE; }}
	void set_teta(double  _teta) { if (teta != _teta ) { teta = _teta; b_teta = TRUE; }}
	void set_gmma(double  _gmma) { if (gmma != _gmma ) { gmma = _gmma; b_gmma = TRUE; }}
	void set_delt(double  _delt) { if (delt != _delt ) { delt = _delt; b_delt = TRUE; }}
	void set_krxrho (double  _krxrow ) { if (krxrho  != _krxrow  ) { krxrho  = _krxrow ; b_krxrow  = TRUE; }}
	void set_krxvega(double  _krxvega) { if (krxvega != _krxvega ) { krxvega = _krxvega; b_krxvega = TRUE; }}
	void set_krxteta(double  _krxteta) { if (krxteta != _krxteta ) { krxteta = _krxteta; b_krxteta = TRUE; }}
	void set_krxgmma(double  _krxgmma) { if (krxgmma != _krxgmma ) { krxgmma = _krxgmma; b_krxgmma = TRUE; }}
	void set_krxdelt(double  _krxdelt) { if (krxdelt != _krxdelt ) { krxdelt = _krxdelt; b_krxdelt = TRUE; }}
	void set_iv  (double  _iv  ) { if (iv   != _iv   ) { iv   = _iv  ; b_iv   = TRUE; }}
	void set_siga (double _siga) { if (siga  != _siga  ) { siga  = _siga ; b_siga  = TRUE; }}
	void set_koga (double _koga) { if (koga  != _koga  ) { koga  = _koga ; b_koga  = TRUE; }}
	void set_jega (double _jega) { if (jega  != _jega  ) { jega  = _jega ; b_jega  = TRUE; }}
	BOOL is_mdif() { const BOOL b=b_mdif; b_mdif=FALSE; return b; }
	BOOL is_mgjv() { const BOOL b=b_mgjv; b_mgjv=FALSE; return b; }
	BOOL is_gvol() { const BOOL b=b_gvol; b_gvol=FALSE; return b; }
	BOOL is_gjga() { const BOOL b=b_gjga; b_gjga=FALSE; return b; }
	BOOL is_msga() { const BOOL b=b_msga; b_msga=FALSE; return b; }
	BOOL is_mdga() { const BOOL b=b_mdga; b_mdga=FALSE; return b; }
	BOOL is_rate() { const BOOL b=b_rate; b_rate=FALSE; return b; }
	BOOL is_diff() { const BOOL b=b_diff; b_diff=FALSE; return b; }
	BOOL is_curr() { const BOOL b=b_curr; b_curr=FALSE; return b; }
	BOOL is_ycurr() { const BOOL b=b_ycurr; b_ycurr=FALSE; return b; }
	BOOL is_hsga() { const BOOL b=b_hsga; b_hsga=FALSE; return b; }
	BOOL is_jisu() { const BOOL b=b_jisu; b_jisu=FALSE; return b; }
	BOOL is_grdo() { const BOOL b=b_grdo; b_grdo=FALSE; return b; }
	BOOL is_irga() { const BOOL b=b_irga; b_irga=FALSE; return b; }
	BOOL is_sggc() { const BOOL b=b_sggc; b_sggc=FALSE; return b; }
	BOOL is_rho () { const BOOL b=b_row ; b_row =FALSE; return b; }
	BOOL is_vega() { const BOOL b=b_vega; b_vega=FALSE; return b; }
	BOOL is_teta() { const BOOL b=b_teta; b_teta=FALSE; return b; }
	BOOL is_gmma() { const BOOL b=b_gmma; b_gmma=FALSE; return b; }
	BOOL is_delt() { const BOOL b=b_delt; b_delt=FALSE; return b; }
	BOOL is_krxrho () { const BOOL b=b_krxrow ; b_krxrow =FALSE; return b; }
	BOOL is_krxvega() { const BOOL b=b_krxvega; b_krxvega=FALSE; return b; }
	BOOL is_krxteta() { const BOOL b=b_krxteta; b_krxteta=FALSE; return b; }
	BOOL is_krxgmma() { const BOOL b=b_krxgmma; b_krxgmma=FALSE; return b; }
	BOOL is_krxdelt() { const BOOL b=b_krxdelt; b_krxdelt=FALSE; return b; }
	BOOL is_iv  () { const BOOL b=b_iv  ; b_iv  =FALSE; return b; }
	BOOL is_siga() { const BOOL b=b_siga; b_siga =FALSE; return b; }
	BOOL is_koga() { const BOOL b=b_koga; b_koga =FALSE; return b; }
	BOOL is_jega() { const BOOL b=b_jega; b_jega =FALSE; return b; }
} OPTION_DATA;

typedef struct _FUTURE_DATA
{
	char code[12];
	char hnam[40];
	double curr, diff, rate, irga, gryl, bsis, isis, gjga,ycurr;
	int gvol, mgjv, mdif;
	BOOL b_curr, b_diff, b_gjga, b_rate, b_irga, b_gryl, b_bsis, b_isis, b_gvol, b_mgjv, b_mdif, b_code, b_name, b_ycurr;
	int  grid_row;
	
	struct _FUTURE_DATA() { ZeroMemory(this, sizeof(struct _FUTURE_DATA)); }
	void set_code(LPCSTR _code) { sprintf(code, _code); b_code = TRUE; }
	void set_name(LPCSTR _name) { sprintf(hnam, _name); b_name = TRUE; }
	void set_curr(double _curr) { if (curr != _curr) { curr = _curr; b_curr = TRUE; } }
	void set_ycurr(double _ycurr) { if (ycurr != _ycurr) { ycurr = _ycurr; b_ycurr = TRUE; } }
	void set_diff(double _diff) { if (diff != _diff) { diff = _diff; b_diff = TRUE; } }
	void set_gjga(double _gjga) { if (gjga != _gjga) { gjga = _gjga; b_gjga = TRUE; } }
	void set_rate(double _rate) { if (rate != _rate) { rate = _rate; b_rate = TRUE; } }
	void set_irga(double _irga) { if (irga != _irga) { irga = _irga; b_irga = TRUE; } }
	void set_gryl(double _gryl) { if (gryl != _gryl) { gryl = _gryl; b_gryl = TRUE; } }
	void set_bsis(double _bsis) { if (bsis != _bsis) { bsis = _bsis; b_bsis = TRUE; } }
	void set_isis(double _isis) { if (isis != _isis) { isis = _isis; b_isis = TRUE; } }
	void set_gvol(int    _gvol) { if (gvol != _gvol) { gvol = _gvol; b_gvol = TRUE; } }
	void set_mgjv(int    _mgjv) { if (mgjv != _mgjv) { mgjv = _mgjv; b_mgjv = TRUE; } }
	void set_mdif(int    _mdif) { if (mdif != _mdif) { mdif = _mdif; b_mdif = TRUE; } }
	BOOL is_code() { const BOOL b=b_code; b_code=FALSE; return b; }
	BOOL is_name() { const BOOL b=b_name; b_name=FALSE; return b; }
	BOOL is_curr() { const BOOL b=b_curr; b_curr=FALSE; return b; }
	BOOL is_ycurr() { const BOOL b=b_ycurr; b_ycurr=FALSE; return b; }
	BOOL is_diff() { const BOOL b=b_diff; b_diff=FALSE; return b; }
	BOOL is_rate() { const BOOL b=b_rate; b_rate=FALSE; return b; }
	BOOL is_irga() { const BOOL b=b_irga; b_irga=FALSE; return b; }
	BOOL is_gryl() { const BOOL b=b_gryl; b_gryl=FALSE; return b; }
	BOOL is_bsis() { const BOOL b=b_bsis; b_bsis=FALSE; return b; }
	BOOL is_isis() { const BOOL b=b_isis; b_isis=FALSE; return b; }
	BOOL is_gvol() { const BOOL b=b_gvol; b_gvol=FALSE; return b; }
	BOOL is_mgjv() { const BOOL b=b_mgjv; b_mgjv=FALSE; return b; }
	BOOL is_mdif() { const BOOL b=b_mdif; b_mdif=FALSE; return b; }
} FUTURE_DATA;

typedef struct _JISU_DATA
{
	char code[12];
	char hnam[40];
	char time[20];
	double curr, pcur;
	BOOL b_curr, b_pcur;
	struct _JISU_DATA() { ZeroMemory(this, sizeof(struct _JISU_DATA)); }
	void set_curr(double _curr) { if (curr != _curr) { curr = _curr; b_curr = TRUE; } }
	void set_pcur(double _pcur) { if (pcur != _pcur) { pcur = _pcur; b_pcur = TRUE; } }
	BOOL is_curr() { const BOOL b=b_curr; b_curr=FALSE; return b; }
	BOOL is_pcur() { const BOOL b=b_pcur; b_pcur=FALSE; return b; }
} JISU_DATA;

