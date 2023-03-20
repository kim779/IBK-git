#ifndef __DATAIO__
#define __DATAIO__

#define MAX_R       30

// struct pibo4013_grid {
//     char    c_rtcd[9];  /* ��RTS CODE           */
//     char    c_mdif[8];  /* ������(����)         */
//     char    c_mgjv[8];  /* �ݹ̰�������         */
//     char    c_gvol[8];  /* �ݰŷ���             */
//     char    c_msga[8];  /* �ݸż�               */
//     char    c_mdga[8];  /* �ݸŵ�               */
//     char    c_rate[8];  /* �ݵ����             */
//     char    c_diff[7];  /* �ݴ��               */
//     char    c_curr[8];  /* �����簡             */
// 	char    c_ycur[8];  /* �ݿ���				*/
//     char    hang[8];    /* ��簡               */
//     char    jihs[8];    /* ����ȯ��             */
//     char    p_rtcd[9];  /* ǲRTS CODE           */
//     char    p_curr[8];  /* ǲ���簡             */
// 	char    p_ycur[8];  /* ǲ����             */
//     char    p_diff[7];  /* ǲ���               */
//     char    p_rate[8];  /* ǲ�����             */
//     char    p_mdga[8];  /* ǲ�ŵ�               */
//     char    p_msga[8];  /* ǲ�ż�               */
//     char    p_gvol[8];  /* ǲ�ŷ���             */
//     char    p_mgjv[8];  /* ǲ�̰�������         */
//     char    p_mdif[8];  /* ǲ����(����)         */
// };

struct pibo4013_grid
{
	char	c_rtcd[9];	// ��RTS CODE
	char	c_jega[8];
	char	c_koga[8];
	char	c_siga[8];
	char	c_raw[8];
	char	c_bega[8];
	char	c_theta[8];
	char	c_gamma[8];
	char	c_delta[8];
	char	c_mdif[8];	// ������(����)
	char	c_mgjv[8];	// �ݹ̰�������
	char	c_gvol[8];	// �ݰŷ���
	char	c_msga[8];	// �ݸż�
	char	c_mdga[8];	// �ݸŵ�
	char	c_rate[8];	// �ݵ����
	char	c_diff[7];	// �ݴ��
	char	c_curr[8];	// �����簡
	char	c_ycur[8];	// �ݿ���
	char	hang[8];	// ��簡
	char	jihs[8];	// ����ȯ��

	char	p_rtcd[9];	// ǲRTS CODE
	char	p_curr[8];	// ǲ���簡
	char	p_ycur[8];	// ǲ����
	char	p_diff[7];	// ǲ���
	char	p_rate[8];	// ǲ�����
	char	p_mdga[8];	// ǲ�ŵ�
	char	p_msga[8];	// ǲ�ż�
	char	p_gvol[8];	// ǲ�ŷ���
	char	p_mgjv[8];	// ǲ�̰�������
	char	p_mdif[8];	// ǲ����(����)
	char	p_delta[8];
	char	p_gamma[8];
	char	p_theta[8];
	char	p_bega[8];
	char	p_raw[8];
	char	p_siga[8];
	char	p_koga[8];
	char	p_jega[8];
};

struct pibo4013_mod
{
	char	grec[4];
	struct	pibo4013_grid	grid[MAX_R];
};

typedef struct _OPTION_DATA
{
	char	code[12];
	int	mdif;	// ����(����)
	int	mgjv;	// �̰�������
	int	gvol;	// �ŷ���
	double	gjga;	// ���ذ�
	double	msga;	// �ż�ȣ��
	double	mdga;	// �ŵ�ȣ��
	double	rate;	// �����
	double	diff;	// ���ϴ��
	double	curr;	// ���簡
	double	ycurr;	// ����
	double	hsga;	// ��簡
	double	jisu;	// ����ȯ��
	double	grdo;	// ������
	double	irga;	// �̷а�
	double	sggc;	// �ð���ġ
	double	rho;	// �ο�
	double	vega;	// ����
	double	teta;	// ��Ÿ
	double	gmma;	// ����
	double	delt;	// ��Ÿ
	double	iv;	// IV
	double	siga;	// �ð�
	double	koga;	// ��
	double	jega;	// ����
	BOOL	b_mdif,b_mgjv,b_gvol,b_gjga,b_msga,b_mdga,b_rate,b_diff,b_curr,b_hsga,b_ycurr;
	BOOL	b_jisu,b_grdo,b_irga,b_sggc,b_row,b_vega,b_teta,b_gmma,b_delt,b_iv;
	BOOL	b_siga,b_koga,b_jega;
	int	grid_row, drawcnt;

	struct _OPTION_DATA()		{ ZeroMemory(this, sizeof(struct _OPTION_DATA)); }
	void	set_all(BOOL bSet)	{ b_mdif = b_mgjv = b_gvol = b_gjga = b_msga = b_mdga = b_rate = b_diff = b_curr = b_ycurr = b_hsga = b_jisu = b_grdo = b_irga = b_sggc = b_row = b_vega = b_teta = b_gmma = b_delt = b_iv = bSet; }
	void	set_mdif(int _mdif)	{ if (mdif != _mdif) { mdif = _mdif; b_mdif = TRUE; }}
	void	set_mgjv(int _mgjv)	{ if (mgjv != _mgjv) { mgjv = _mgjv; b_mgjv = TRUE; }}
	void	set_gvol(int _gvol)	{ if (gvol != _gvol) { gvol = _gvol; b_gvol = TRUE; }}
	void	set_gjga(double _gjga)	{ if (gjga != _gjga) { gjga = _gjga; b_gjga = TRUE; }}
	void	set_msga(double _msga)	{ if (msga != _msga) { msga = _msga; b_msga = TRUE; }}
	void	set_mdga(double _mdga)	{ if (mdga != _mdga) { mdga = _mdga; b_mdga = TRUE; }}
	void	set_rate(double _rate)	{ if (rate != _rate) { rate = _rate; b_rate = TRUE; }}
	void	set_diff(double _diff)	{ if (diff != _diff) { diff = _diff; b_diff = TRUE; }}
	void	set_curr(double _curr)	{ if (curr != _curr) { curr = _curr; b_curr = TRUE; }}
	void	set_ycurr(double _ycurr){ if (ycurr != _ycurr) { ycurr = _ycurr; b_ycurr = TRUE; }}
	void	set_hsga(double _hsga)	{ if (hsga != _hsga) { hsga = _hsga; b_hsga = TRUE; }}
	void	set_jisu(double _jisu)	{ if (jisu != _jisu) { jisu = _jisu; b_jisu = TRUE; }}
	void	set_grdo(double _grdo)	{ if (grdo != _grdo) { grdo = _grdo; b_grdo = TRUE; }}
	void	set_irga(double _irga)	{ if (irga != _irga) { irga = _irga; b_irga = TRUE; }}
	void	set_sggc(double _sggc)	{ if (sggc != _sggc) { sggc = _sggc; b_sggc = TRUE; }}
	void	set_rho (double _row)	{ if (rho  != _row) { rho  = _row ; b_row  = TRUE; }}
	void	set_vega(double _vega)	{ if (vega != _vega) { vega = _vega; b_vega = TRUE; }}
	void	set_teta(double _teta)	{ if (teta != _teta) { teta = _teta; b_teta = TRUE; }}
	void	set_gmma(double _gmma)	{ if (gmma != _gmma) { gmma = _gmma; b_gmma = TRUE; }}
	void	set_delt(double _delt)	{ if (delt != _delt) { delt = _delt; b_delt = TRUE; }}
	void	set_iv(double _iv)	{ if (iv   != _iv ) { iv   = _iv  ; b_iv   = TRUE; }}
	void	set_siga(double _siga)	{ if (siga  != _siga) { siga  = _siga ; b_siga  = TRUE; }}
	void	set_koga(double _koga)	{ if (koga  != _koga) { koga  = _koga ; b_koga  = TRUE; }}
	void	set_jega(double _jega)	{ if (jega  != _jega) { jega  = _jega ; b_jega  = TRUE; }}
	BOOL	is_mdif ()		{ const BOOL b = b_mdif;  b_mdif=FALSE;  return b; }
	BOOL	is_mgjv()		{ const BOOL b = b_mgjv;  b_mgjv=FALSE;  return b; }
	BOOL	is_gvol()		{ const BOOL b = b_gvol;  b_gvol=FALSE;  return b; }
	BOOL	is_gjga()		{ const BOOL b = b_gjga;  b_gjga=FALSE;  return b; }
	BOOL	is_msga()		{ const BOOL b = b_msga;  b_msga=FALSE;  return b; }
	BOOL	is_mdga()		{ const BOOL b = b_mdga;  b_mdga=FALSE;  return b; }
	BOOL	is_rate()		{ const BOOL b = b_rate;  b_rate=FALSE;  return b; }
	BOOL	is_diff()		{ const BOOL b = b_diff;  b_diff=FALSE;  return b; }
	BOOL	is_curr()		{ const BOOL b = b_curr;  b_curr=FALSE;  return b; }
	BOOL	is_ycurr()		{ const BOOL b = b_ycurr; b_ycurr=FALSE; return b; }
	BOOL	is_hsga()		{ const BOOL b = b_hsga;  b_hsga=FALSE;  return b; }
	BOOL	is_jisu()		{ const BOOL b = b_jisu;  b_jisu=FALSE;  return b; }
	BOOL	is_grdo()		{ const BOOL b = b_grdo;  b_grdo=FALSE;  return b; }
	BOOL	is_irga()		{ const BOOL b = b_irga;  b_irga=FALSE;  return b; }
	BOOL	is_sggc()		{ const BOOL b = b_sggc;  b_sggc=FALSE;  return b; }
	BOOL	is_rho ()		{ const BOOL b = b_row ;  b_row =FALSE;  return b; }
	BOOL	is_vega()		{ const BOOL b = b_vega;  b_vega=FALSE;  return b; }
	BOOL	is_teta()		{ const BOOL b = b_teta;  b_teta=FALSE;  return b; }
	BOOL	is_gmma()		{ const BOOL b = b_gmma;  b_gmma=FALSE;  return b; }
	BOOL	is_delt()		{ const BOOL b = b_delt;  b_delt=FALSE;  return b; }
	BOOL	is_iv  ()		{ const BOOL b = b_iv;    b_iv  =FALSE;  return b; }
	BOOL	is_siga()		{ const BOOL b = b_siga;  b_siga =FALSE; return b; }
	BOOL	is_koga()		{ const BOOL b = b_koga;  b_koga =FALSE; return b; }
	BOOL	is_jega()		{ const BOOL b = b_jega;  b_jega =FALSE; return b; }
	BOOL	is_call()		{ return code[1] == '2'; }
	BOOL	is_put()		{ return code[1] == '3'; }
} OPTION_DATA;

#endif
