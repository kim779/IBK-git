#ifndef	_JINFO_H
#define	_JINFO_H

#pragma once


//
//	control symbol name : 17413
//

struct	_jinfo {
	char    codx[12];		// �����ڵ�
	char    hnam[50];		// �����
	char    jgub[1];		// �屸�� : '1':�ŷ��� '2':�ڽ���, '3' :��3����, '4':ETF, '6':ELW, '7':ELW-�������� 
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
	char    jggb[12];		// �� ����
	char    lock[12];		// ������
	char    dyga[8];		// ��밡
	char    ujgb[12];		// ��������
	char    ujcd[6];		// RTS �����ڵ�
	char    jisu[8];		// ��������
	char    diff[8];		// �������ϴ��
	char    rate[8];		// �������
	char    gvol[12];		// �����ŷ���(����:õ��)
	char    unio[6];		// KRX 100 (0:�Ϲ� 1:��������)
	char	rned[2];		// ��������
	char	dist[2];		// �������
	char	frgb[8];		// �ܱ��� ��������
	char	rlnk[1];		// ������('A'~'E', 'X', ' ')
	char    vist[1];		// ������ ��ȭ ��ġ
	char    vitime[6];		// ������ ��ȭ ��ġ �ߵ� �ð�
	char	vitype[1];		// ������ ��ȭ ��ġ Ÿ��(����,����,��/����)
	char	sijang[40];		// ������ġ
};

struct  _jinfo3 {			// �� HTS �������� (������ ���ΰ�)
	char    codx[12];		// �����ڵ�
	char    hnam[50];		// �����
	char    jgub[1];		// �屸�� : '1':�ŷ��� '2':�ڽ���, '3' :��3����, '4':ETF, '6':ELW, '7':ELW-��������
	char	jrat[3];		// ���ű���
	char	nrat[3];		// �ſ����ű��� 20070718 
	char    krgb[12];		// ��������
	char	rned[2];		// ��������
	char	dist[2];		// �������
	char    vist[1];		// ������ ��ȭ ��ġ
	char    vitime[6];		// ������ ��ȭ ��ġ �ߵ� �ð�
	char	vitype[1];		// ������ ��ȭ ��ġ Ÿ��(����,����,��/����)
	char	fill[12];
};

#define L_jinfo		sizeof(struct _jinfo)
#define L_jinfo3	sizeof(struct _jinfo3)

class Cjinfo_small
{
public:
	Cjinfo_small()
	{
	}

	CString		codx;		// �����ڵ�
	CString		hnam;		// �����
	CString		jrat;		// ���ű���
	CString		nrat;		// �ſ����ű��� 20070718 
	CString		jggb;		// �� ����
};

class Cjinfo
{
public:
	Cjinfo()
	{
		codx = hnam = jgub = size = k2gb = usun = ksmm = amga = _T("");
		jbkm = siga = sjjs = sijang = _T("");

		krgb = jggb = lock = dyga = ujgb = ujcd = _T("");
		jisu = diff = rate = gvol = unio = frgb = _T("");
	}

public:
	CString		codx;		// �����ڵ�
	CString		hnam;		// �����
	CString		jgub;		// �屸�� : '1':KOSPI, 2':KOSPI200, '3':KOSDAQ, '4 :��������, '5':ETF
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
	CString		frgb;		// �ܱ��� ��������
	CString		rlnk;
//	CString		str2302;	//2012.05.04 KSJ ��������
	CString		str1377;	//2013.06.14 KSJ �ŸŴ���
	CString		str1971;	//2013.06.14 KSJ �����ڹ���
	CString		rned;		//��������
	CString		dist;		//�������
	CString		fill;		//�ʷ�
	CString		vist;		//������
	CString		vitime;		//������ �ߵ� �ð�
	CString		vitype;		// ������ ��ȭ ��ġ Ÿ��(����,����,��/����)
	CString		sijang;		//������ġ
};

#endif // _JINFO_H