// GrpBasic.h: interface for the CGrpBasic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRPBASIC_H__DB94F0BB_D862_488C_8847_7B4E17506989__INCLUDED_)
#define AFX_GRPBASIC_H__DB94F0BB_D862_488C_8847_7B4E17506989__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
struct	_gBasic {
	int		fpr;			// �ð�
	int		hpr;			// ��
	int		lpr;			// ����
	int		epr;			// ����
	double		trn;			// �ŷ���
	double		amt;			// �ŷ����
	struct _gIndex	index;

	unsigned char	check;			// information check
	WORD		rate;			// check rate
	int		mgjy;			// �̰�������
//	WORD		ucnt;			// ��������
//	WORD		dcnt;			// �϶������


	//**WORD		fobtain;		// �ܱ��� ������
	//**WORD		fexhaust;		// �ܱ��� ������
	//**int		fmmvol;			// �ܱ��� ���ż���
	//** 2005.11.08 add by macho
	//int     fmmamt;			// �ܱ��� ���ż��ݾ�
	//int     orgvol;			// ����� ���ż���
	//int		orgamt;			// ����� ���ż��ݾ�
	//int		invdata[6];

	int	orgnvol;
	int orgnamt;
	int frgnvol;
	int frgnamt;
	int fobtain;
	int fexhaust;
};
#define	sz_gBASIC	sizeof(struct _gBasic)
*/

#define	GD_UNUSED		-999999999

class CGrpBasic  
{
public:
	CGrpBasic();
	virtual ~CGrpBasic();

public:
	bool		m_bDummy;
	int		m_iOpen;		// �ð�
	int		m_iHigh;		// ��
	int		m_iLow;			// ����
	int		m_iClose;		// ����
	int		m_iMgjy;		// �̰�������

	/** remarking by macho <begin>
	int		m_iOrgnVol;		// ��������ǥ ���(����)
	int		m_iOrgnAmt;		// ��������ǥ ���(�ݾ�)
	int		m_iFrgnVol;		// ��������ǥ �ܱ���(����)
	int		m_iFrgnAmt;		// ��������ǥ �ܱ���(�ݾ�)
	int		m_iFrgnObRate;		// �ܱ��� ������
	int		m_iFrgnExRate;		// �ܱ��� ������
	remarking by macho <end> **/

	//** add by macho <begin>
	int		m_frgnHave;		// �ܱ��κ�������
	int		m_frgnRate;		// �ܱ��κ�������;
	int		m_psnlPBuy;		// ���μ��ż�����
	int		m_frgnPBuy;		// �ܱ��μ��Ÿż���
	int		m_orgnPBuy;		// ������ż�����
	int		m_psnlTBuy;		// ���δ������ż�����
	int		m_frgnTBuy;		// �ܱ��δ������ż�����
	int		m_orgnTBuy;		// ����������ż�����
	//** add by macho <end>

	// 2007.01.01 schbang : ADR ��Ʈ
	int		m_iUpCnt;
	int		m_iDnCnt;


//	__int64		m_iVolume;		// �ŷ���
//	__int64		m_iTrdMoney;		// �ŷ����
	double		m_dVolume;		// �ŷ���
	double		m_dTrdMoney;		// �ŷ����


	//unsigned char	m_ucCheck;		// information check
	DWORD		m_dwCheck;		// information check
	WORD		m_wRate;		// check rate
	


//	struct _gIndex	index;
	struct _gIndex{
		struct _date {
			short		yy;	// year
			unsigned char	mm;	// month
			unsigned char	dd;	// day
		}date;
		struct _time {
			unsigned char	hh;	// hour
			unsigned char	mm;	// minute
			unsigned char	ss;	// second
		}time;
	}		m_index;


private:

public:

private:


};

#endif // !defined(AFX_GRPBASIC_H__DB94F0BB_D862_488C_8847_7B4E17506989__INCLUDED_)
