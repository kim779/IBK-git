#ifndef	_JINFO_H
#define	_JINFO_H

#if _MSC_VER > 1000
#pragma once
#endif


//
//	control symbol name : 17413
//
enum CODE_KIND { CK_HCODE = 0, CK_ELW };
struct _jHeader{
	char    codx[12];		// �����ڵ�
	char    hnam[32];		// �����
	char    jgub[1];		// �屸�� : '1':�ŷ��� '2':�ڽ���, '3' :��3����, '5':ETF
};

#define L_jHeader	sizeof(struct _jHeader)
// ��������
struct	_hjinfo {
	char    codx[12];		// �����ڵ�
	char    hnam[32];		// �����
	char    jgub[1];		// �屸�� : '1':�ŷ��� '2':�ڽ���, '3' :��3����, '5':ETF
	char    size[12];		// ����Ը�
	char    k2gb[20];		// �ұ��� ����,����
	char    usun[10];		// ������/�켱��
	char    ksmm[2];		// ����
	char    amga[20];		// �׸鰡 8->20 ��������
	char    jbkm[20];		// �ں���(����:��) 12->20 ��������
	char    siga[12];		// ���Ͻð��Ѿ�(����:��)
	char    sjjs[12];		// �����ֽļ�(����:õ��)
	char	jrat[3];		// ���ű���
	char	nrat[3];		// �ſ����ű��� 20070718 
	char    krgb[12];		// "�ŷ�����", "�ŷ��ߴ�", "��������", "�̻�޵�", "��������", "����"
					// ���ڰ��, ��������, �������� 
	char    jggb[12];		// �� ����
	char    lock[12];		// ������
	char    dyga[8];		// ��밡
	char    ujgb[12];		// ��������
	char    ujcd[6];		// RTS �����ڵ�
	char    jisu[8];		// ��������
	char    diff[8];		// �������ϴ��
	char    rate[8];		// �������
	char    gvol[12];		// �����ŷ���(����:õ��)
	char    unio[10];		// KRX 100 (0:�Ϲ� 1:��������)

};

#define L_hjinfo		sizeof(struct _hjinfo)

class CjHeader
{
public:
	CjHeader()
	{
		codx = hnam = jgub = _T("");
	}

	virtual ~CjHeader()
	{
		TRACE("Delete CjHeader\n");	
	}

	CString		codx;		// �����ڵ�
	CString		hnam;		// �����
	CString		jgub;		// �屸�� : '1':KOSPI, 2':KOSPI200, '3':KOSDAQ, '4 :��������, '5':ETF
};

class Chjinfo : public CjHeader
{
public:
	Chjinfo() : CjHeader()
	{
		codx = hnam = jgub = size = k2gb = usun = ksmm = amga = _T("");
		jbkm = siga = sjjs = _T("");

		krgb = jggb = lock = dyga = ujgb = ujcd = _T("");
		jisu = diff = rate = gvol = unio = _T("");
	}
	
	virtual ~Chjinfo()
	{
		TRACE("Delete Chjinfo\n");
	}

public:
	CString		size;		// ����Ը�
	CString		k2gb;		// �ұ��� ����,����
	CString		usun;		// ������/�켱��
	CString		ksmm;		// ����
	CString		amga;		// �׸鰡 -
	CString		jbkm;		// �ں���(����:��) -
	CString		siga;		// ���Ͻð��Ѿ�(����:��)
	CString		sjjs;		// �����ֽļ�(����:õ��)
	CString		jrat;		// ���ű���
	CString		nrat;		// �ſ����ű��� 20070718 
	CString		krgb;		// ��������
	CString		jggb;		// �� ����
	CString		lock;		// ������
	CString		dyga;		// ��밡
	CString		ujgb;		// ��������
	CString		ujcd;		// RTS �����ڵ�
	CString		jisu;		// ��������
	CString		diff;		// �������ϴ��
	CString		rate;		// �������
	CString		gvol;		// �����ŷ���(����:õ��)
	CString		unio;		// KRX 100 (0:�Ϲ� 1:��������)
};

// ELW
struct  _ejinfo {            /* ELW ��������             */
	char    codx[12];       /* �����ڵ�                 */
	char    hnam[32];       /* �����                   */
	char    jgub[1];        /* ELW : 5                  */
	char    gcjs[20];       /* �����ڻ�                 */
	char    krye[10];       /* �Ǹ�����                 */
	char    khbe[10];       /* �Ǹ������             */
	char    gjbe[10];       /* �������                 */
	char    hbge[20];       /* �ѱ۹�������           */
	char    sjjs[9];        /* �����ֽļ� (õ)          */
	char    sgca[9];        /* ���డ(�ð��Ѿ�:��)      */
	char    hsga[10];       /* ��簡��                 */
	char    jhre[10];       /* ��ȯ����                 */
	char    sjil[10];       /* �������� (YYYY/MM/DD)    */
	char    edte[10];       /* �����ŷ��� (YYYY/MM/DD)  */
	char    mgil[10];       /* �������� (YYYY/MM/DD)    */
	char    jgil[10];       /* �������� (YYYY/MM/DD)    */
	char    jjis[5];        /* �����ϼ�                 */
	char    lpsa[20];       /* LPȸ����                 */
	char    lpjg[4];        /* LP�ֹ����ɿ���           */
	char    lpsu[15];       /* LP��������               */
	char    lpbr[6];        /* LP��������               */
};


#define L_ejinfo		sizeof(struct _ejinfo)

class Cejinfo : public CjHeader
{
public:
	Cejinfo() : CjHeader()
	{
		gcjs = krye = khbe = gjbe = hbge = sjjs = sgca = _T("");
		hsga = jhre = sjil = edte = mgil = _T("");
		jgil = jjis = lpsa = lpjg = lpsu = lpbr = _T("");
	}
	virtual ~Cejinfo()
	{
		TRACE("Delete Cejinfo\n");
	}

public:
	CString    gcjs;       /* �����ڻ�                 */
	CString    krye;       /* �Ǹ�����                 */
	CString    khbe;       /* �Ǹ������             */
	CString    gjbe;       /* �������                 */
	CString    hbge;       /* �ѱ۹�������           */
	CString    sjjs;        /* �����ֽļ� (õ)          */
	CString    sgca;        /* ���డ(�ð��Ѿ�:��)      */
	CString    hsga;       /* ��簡��                 */
	CString    jhre;       /* ��ȯ����                 */
	CString    sjil;       /* �������� (YYYY/MM/DD)    */
	CString    edte;       /* �����ŷ��� (YYYY/MM/DD)  */
	CString    mgil;       /* �������� (YYYY/MM/DD)    */
	CString    jgil;       /* �������� (YYYY/MM/DD)    */
	CString    jjis;        /* �����ϼ�                 */
	CString    lpsa;       /* LPȸ����                 */
	CString    lpjg;        /* LP�ֹ����ɿ���           */
	CString    lpsu;       /* LP��������               */
	CString    lpbr;        /* LP��������               */
};

#endif // _JINFO_H