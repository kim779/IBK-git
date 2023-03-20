// HogaData.h: interface for the CHogaData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOGADATA_H__E646C4BF_16A6_44DB_BCB7_4372C383376E__INCLUDED_)
#define AFX_HOGADATA_H__E646C4BF_16A6_44DB_BCB7_4372C383376E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHogaData : public CObject  
{
public:	
	int					m_nRow;					// �׸��� �� ��
	int					m_nCol;					// �׸��� �� �� 
    UINT				m_nMask;

    COLORREF			m_crBkClr;				// ����
    COLORREF			m_crFgClr;				// �����

	CString				m_strText;				// Text in cell

	CHogaData();
	virtual ~CHogaData();
};



typedef struct _TGRID_1ROWINFO
{
	UINT	nRow;		// Row
	int		nPrice;		// ü�ᰡ(��<->��)
	
} TGRID_1ROWINFO, *PTGRID_1ROWINFO;



typedef struct _CHARTDATA
{
	int		nOpen;
	int		nHigh;
	int		nLow;
	int		nClose;

	int		nHighLimit;
	int		nLowLimit;

} CHARTDATA, *PCHARTDATA;



typedef struct __RsvdOdr{
	
	char code[6];			//�����ڵ� 
	char price[9];			//���簡	
	char acct[11];			//����

	char odrcon[4];			//���Ǳ���	sm0, so3, so2, so1, sc0, sb1, sb2, sb3
							//			bm0, bo3, bo2, bo1, bc0, bb1, bb2, bb3
	char odrcnt[9];			//�ֹ�����
	char Row[5];			//�׸��� Row ���� 
	char odrflag[1];		//�ֹ� ���� u: ��ǥ�� ���� ���Ľ� �ֹ� ���� , d: ��ǥ�� ���� ���Ľ� �ֹ� ����

//	//-----------------------------------------------------------------------------
//	// Author		: Dae-Sung, Byun	Date :2005/5/30
//	// Reason		: �ż� �ŵ� ������ ���� �÷��� �߰� 
//	//-----------------------------------------------------------------------------
//	// start
//	char odrgb[1];			// �ֹ� ���� ('s' : �ŵ�     'b' : �ż� )
//	// end
} RsvOdr, *PRsvOdr;

#define RSVODR_BUFF_SIZE	sizeof(RsvOdr)



typedef struct __TenHogaData{
	
//	char    offerho3		[ 9];			//�ŵ�3ȣ��(��)          
//	char    offerho2		[ 9];			//�ŵ�2ȣ��(��)          
//	char    offerho1		[ 9];			//�ŵ�1ȣ��(��)          
//	char    bidho1			[ 9];			//�ż�1ȣ��(��) 
//	char    bidho2			[ 9];			//�ż�2ȣ��(��) 
//	char    bidho3			[ 9];			//�ż�3ȣ��(��) 
	
	int		nHighLimit;		// ���Ѱ� ����
	int		nExtraUp;		// ��Ÿ�ŵ�ȣ�� ����

	int		nOfferho10;		// �ŵ� 10ȣ��
	int		nOfferho9;		// �ŵ� 9ȣ��
	int		nOfferho8;		// �ŵ� 8ȣ��
	int		nOfferho7;		// �ŵ� 7ȣ��
	int		nOfferho6;		// �ŵ� 6ȣ��
	int		nOfferho5;		// �ŵ� 5ȣ��
	int		nOfferho4;		// �ŵ� 4ȣ��
	int		nOfferho3;		// �ŵ� 3ȣ��
	int		nOfferho2;		// �ŵ� 2ȣ��
	int		nOfferho1;		// �ŵ� 1ȣ��

	int		nBidho1;		// �ż� 1ȣ��
	int		nBidho2;		// �ż� 2ȣ��
	int		nBidho3;		// �ż�	3ȣ��
	int		nBidho4;		// �ż�	4ȣ��
	int		nBidho5;		// �ż� 5ȣ��
	int		nBidho6;		// �ż� 6ȣ��
	int		nBidho7;		// �ż� 7ȣ��
	int		nBidho8;		// �ż�	8ȣ��
	int		nBidho9;		// �ż�	9ȣ��
	int		nBidho10;		// �ż� 10ȣ��

	int		nExtraDown;		// ��Ÿ�ż�ȣ�� ����
	int		nLowLimit;		// ���Ѱ� ����


} TenHogaData, *PTenHogaData;

#define TEN_HOGADATA_SIZE	sizeof(TenHogaData)



// ***********************************************************************************
//			��ž�ν�, �ֹ����� �ֹ��α� ����� 
// ***********************************************************************************
typedef struct __LOGDATA{
  char   date			[8];
  char   time			[6];
  char   flag			[1]; //0 - ����, 1 - �ű�����
  char   jmcode			[6];
  char   scrno			[5];
  char   odrgubun		[1];
  char   odrcnt			[9];
  char   stopprice		[9];
  char   odrprice		[9];
  char   odrpricegubun	[2]; //�ֹ��� ����..01:���� 05:���尡
  char   odrno			[5];
  char   msg			[80];
} LOGDATA, *PLOGDATA;
#define LOGDATA_SIZE sizeof(LOGDATA)

// ************************************************************************************


#endif // !defined(AFX_HOGADATA_H__E646C4BF_16A6_44DB_BCB7_4372C383376E__INCLUDED_)