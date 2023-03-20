// ConfigSet.cpp: implementation of the CConfigSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConfigSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConfigSet::CConfigSet()
{
	InitMember();
}

CConfigSet::~CConfigSet()
{

}

//---------------------------------------------------------
// Author       : Sang-Yun, Nam			Date : 2005/01/28
// Modifier     : 
// Comments		: �ܺο��� ���� �Ѳ����� �ٲ� ��� ȣ��ȴ�.
//---------------------------------------------------------
void CConfigSet::SetConfigValue(const CConfigSet* p_pConfigSet)
{
	if(p_pConfigSet == NULL) return;

	m_bChkYSprice 		= p_pConfigSet->m_bChkYSprice 		;//���ϰ��� �ð�			üũ ����
	m_bChkYHprice 		= p_pConfigSet->m_bChkYHprice 		;//���ϰ��� ��			üũ ����
	m_bChkYLprice 		= p_pConfigSet->m_bChkYLprice 		;//���ϰ��� ����			üũ ����
	m_bChkYEprice 		= p_pConfigSet->m_bChkYEprice 		;//���ϰ��� ����			üũ ����
	m_bChkYSHL 			= p_pConfigSet->m_bChkYSHL 			;//���ϰ��� (��+��+��)/3	üũ ����
	
	m_bChkTSprice 		= p_pConfigSet->m_bChkTSprice 		;//���ϰ��� �ð�			üũ ����
	m_bChkTHprice 		= p_pConfigSet->m_bChkTHprice 		;//���ϰ��� ��			üũ ����
	m_bChkTLprice 		= p_pConfigSet->m_bChkTLprice 		;//���ϰ��� ����			üũ ����
	m_bChkTEprice 		= p_pConfigSet->m_bChkTEprice 		;//���ϰ��� ����			üũ ����
	m_bChkTHL 			= p_pConfigSet->m_bChkTHL 			;//���ϰ��� (��+��)/2		üũ ����
	m_bChkTSHL 			= p_pConfigSet->m_bChkTSHL 			;//���ϰ��� (��+��+��)/3	üũ ����
	m_bChkTHLE 			= p_pConfigSet->m_bChkTHLE 			;//���ϰ��� (��+��+(��*2))/4üũ ����
	m_bChkHighLimit		= p_pConfigSet->m_bChkHighLimit		;//���Ѱ�					üũ ����
	m_bChkLowLimit		= p_pConfigSet->m_bChkLowLimit		;//���Ѱ�				 üũ ����
	
	m_bChkPivot2Rst 	= p_pConfigSet->m_bChkPivot2Rst		;// �Ǻ� 2�� ���� 			üũ ����
	m_bChkPivot1Rst 	= p_pConfigSet->m_bChkPivot1Rst		;// �Ǻ� 1�� ���� 			üũ ����
	m_bChkPivotGLine 	= p_pConfigSet->m_bChkPivotGLine	;// �Ǻ� ���ؼ�  			üũ ����
	m_bChkPivot1GG 		= p_pConfigSet->m_bChkPivot1GG 		;// �Ǻ� 1�� ���� 			üũ ����
	m_bChkPivot2GG 		= p_pConfigSet->m_bChkPivot2GG 		;// �Ǻ� 2�� ���� 			üũ ����
	
	m_bChkDMKPreHPrice 	= p_pConfigSet->m_bChkDMKPreHPrice	;// Demark �����  		üũ ����
	m_bChkDMKGLine 		= p_pConfigSet->m_bChkDMKGLine 		;// Demark ���ؼ�			üũ ����
	m_bChkDMKPreLPrice 	= p_pConfigSet->m_bChkDMKPreLPrice	;// Demark ��������  		üũ ����
	
	m_bChk2Pyung1 		= p_pConfigSet->m_bChk2Pyung1		;// �̵���� ù��° �Է°�	üũ ����
	m_bChk2Pyung2 		= p_pConfigSet->m_bChk2Pyung2		;// �̵���� �ι�° �Է°�	üũ ����
	m_bChk2Pyung3 		= p_pConfigSet->m_bChk2Pyung3		;// �̵���� ����° �Է°�	üũ ����
	m_bChk2Pyung4 		= p_pConfigSet->m_bChk2Pyung4		;// �̵���� �׹�° �Է°�	üũ ����
	
	m_bChkPmmDanga 		= p_pConfigSet->m_bChkPmmDanga		;// ��ո��� �ܰ�			üũ ����
	m_bChkPmmPDanga 	= p_pConfigSet->m_bChkPmmPDanga		;// ��ո��� �ܰ� +n%		üũ ����
	m_bChkPmmMDanga 	= p_pConfigSet->m_bChkPmmMDanga		;// ��ո��� �ܰ� -n%		üũ ����
	
	
	
	m_clrYSprice 		= p_pConfigSet->m_clrYSprice		;//���ϰ��� �ð�			�� ������
	m_clrYHprice 		= p_pConfigSet->m_clrYHprice		;//���ϰ��� ��			�� ������
	m_clrYLprice 		= p_pConfigSet->m_clrYLprice		;//���ϰ��� ����			�� ������
	m_clrYEprice 		= p_pConfigSet->m_clrYEprice		;//���ϰ��� ����			�� ������
	m_clrYSHL 			= p_pConfigSet->m_clrYSHL 			;//���ϰ��� (��+��+��)/3	�� ������
	
	m_clrTSprice 		= p_pConfigSet->m_clrTSprice		;//���ϰ��� �ð�			�� ������
	m_clrTHprice 		= p_pConfigSet->m_clrTHprice		;//���ϰ��� ��			�� ������
	m_clrTLprice 		= p_pConfigSet->m_clrTLprice		;//���ϰ��� ����			�� ������
	m_clrTEprice 		= p_pConfigSet->m_clrTEprice		;//���ϰ��� ����			�� ������
	m_clrTHL 			= p_pConfigSet->m_clrTHL 			;//���ϰ��� (��+��)/2		�� ������
	m_clrTSHL 			= p_pConfigSet->m_clrTSHL 			;//���ϰ��� (��+��+��)/3	�� ������
	m_clrTHLE 			= p_pConfigSet->m_clrTHLE 			;//���ϰ��� (��+��+(��*2))/4�� ������
	m_clrHighLimit		= p_pConfigSet->m_clrHighLimit		;//���Ѱ�					�� ������
	m_clrLowLimit		= p_pConfigSet->m_clrLowLimit		;//���Ѱ�					�� ������
	
	m_clrPivot2Rst 		= p_pConfigSet->m_clrPivot2Rst 		;// �Ǻ� 2�� ����			�� ������
	m_clrPivot1Rst 		= p_pConfigSet->m_clrPivot1Rst 		;// �Ǻ� 1�� ����			�� ������
	m_clrPivotGLine 	= p_pConfigSet->m_clrPivotGLine		;// �Ǻ� ���ؼ�				�� ������
	m_clrPivot1GG 		= p_pConfigSet->m_clrPivot1GG 		;// �Ǻ� 1�� ����			�� ������
	m_clrPivot2GG 		= p_pConfigSet->m_clrPivot2GG 		;// �Ǻ� 2�� ����			�� ������
	
	m_clrDMKPreHPrice 	= p_pConfigSet->m_clrDMKPreHPrice	;// Demark �����			�� ������
	m_clrDMKGLine 		= p_pConfigSet->m_clrDMKGLine 		;// Demark ���ؼ�			�� ������
	m_clrDMKPreLPrice 	= p_pConfigSet->m_clrDMKPreLPrice	;// Demark ��������			�� ������
	
	m_clr2Pyung1 		= p_pConfigSet->m_clr2Pyung1		;// �̵���� ù��° �Է°�	�� ������
	m_clr2Pyung2 		= p_pConfigSet->m_clr2Pyung2		;// �̵���� �ι�° �Է°�	�� ������
	m_clr2Pyung3 		= p_pConfigSet->m_clr2Pyung3		;// �̵���� ����° �Է°�	�� ������
	m_clr2Pyung4 		= p_pConfigSet->m_clr2Pyung4		;// �̵���� �׹�° �Է°�	�� ������
	
	m_clrPmmDanga 		= p_pConfigSet->m_clrPmmDanga		;// ��ո��� �ܰ�			�� ������
	m_clrPmmPDanga 		= p_pConfigSet->m_clrPmmPDanga		;// ��ո��� �ܰ� +n%		�� ������
	m_clrPmmMDanga 		= p_pConfigSet->m_clrPmmMDanga		;// ��ո��� �ܰ� -n%		�� ������


	m_nLineYSprice		= p_pConfigSet->m_nLineYSprice		;//���ϰ��� �ð�			���� �β�
	m_nLineYHprice		= p_pConfigSet->m_nLineYHprice		;//���ϰ��� ��			���� �β�
	m_nLineYLprice		= p_pConfigSet->m_nLineYLprice		;//���ϰ��� ����			���� �β�
	m_nLineYEprice		= p_pConfigSet->m_nLineYEprice		;//���ϰ��� ����			���� �β�
	m_nLineYSHL			= p_pConfigSet->m_nLineYSHL			;//���ϰ��� (��+��+��)/3	���� �β�
	
	m_nLineTSprice		= p_pConfigSet->m_nLineTSprice		;//���ϰ��� �ð�			���� �β�
	m_nLineTHprice		= p_pConfigSet->m_nLineTHprice		;//���ϰ��� ��			���� �β�
	m_nLineTLprice		= p_pConfigSet->m_nLineTLprice		;//���ϰ��� ����			���� �β�
	m_nLineTEprice		= p_pConfigSet->m_nLineTEprice		;//���ϰ��� ����			���� �β�
	m_nLineTHL			= p_pConfigSet->m_nLineTHL			;//���ϰ��� (��+��)/2		���� �β�
	m_nLineTSHL			= p_pConfigSet->m_nLineTSHL			;//���ϰ��� (��+��+��)/3	���� �β�
	m_nLineTHLE			= p_pConfigSet->m_nLineTHLE			;//���ϰ��� (��+��+(��*2))/4 ���� �β�
	m_nLineHighLimit	= p_pConfigSet->m_nLineHighLimit	;//���Ѱ�					���� �β�
	m_nLineLowLimit		= p_pConfigSet->m_nLineLowLimit		;//���Ѱ�					���� �β�
	
	m_nLinePivot2Rst	= p_pConfigSet->m_nLinePivot2Rst	;// �Ǻ� 2�� ����			���� �β�
	m_nLinePivot1Rst	= p_pConfigSet->m_nLinePivot1Rst	;// �Ǻ� 1�� ����			���� �β�
	m_nLinePivotGLine	= p_pConfigSet->m_nLinePivotGLine	;// �Ǻ� ���ؼ�				���� �β�
	m_nLinePivot1GG		= p_pConfigSet->m_nLinePivot1GG		;// �Ǻ� 1�� ����			���� �β�
	m_nLinePivot2GG		= p_pConfigSet->m_nLinePivot2GG		;// �Ǻ� 2�� ����			���� �β�
	
	m_nLineDMKPreHPrice = p_pConfigSet->m_nLineDMKPreHPrice	;// Demark �����			���� �β�
	m_nLineDMKGLine		= p_pConfigSet->m_nLineDMKGLine		;// Demark ���ؼ�			���� �β�
	m_nLineDMKPreLPrice = p_pConfigSet->m_nLineDMKPreLPrice	;// Demark ��������			���� �β�
	
	m_nLine2Pyung1		= p_pConfigSet->m_nLine2Pyung1		;// �̵���� ù��° �Է°�	���� �β�
	m_nLine2Pyung2		= p_pConfigSet->m_nLine2Pyung2		;// �̵���� �ι�° �Է°�	���� �β�
	m_nLine2Pyung3		= p_pConfigSet->m_nLine2Pyung3		;// �̵���� ����° �Է°�	���� �β�
	m_nLine2Pyung4		= p_pConfigSet->m_nLine2Pyung4		;// �̵���� �׹�° �Է°�	���� �β�
	
	m_nLinePmmDanga		= p_pConfigSet->m_nLinePmmDanga		;// ��ո��� �ܰ�			���� �β�
	m_nLinePmmPDanga	= p_pConfigSet->m_nLinePmmPDanga	;// ��ո��� �ܰ� +n%		���� �β�
	m_nLinePmmMDanga	= p_pConfigSet->m_nLinePmmMDanga	;// ��ո��� �ܰ� -n%		���� �β�

	m_nDay2Pyung1		= p_pConfigSet->m_nDay2Pyung1		;//�ܼ� �̵���� ��ġ 1
	m_nDay2Pyung2		= p_pConfigSet->m_nDay2Pyung2		;//�ܼ� �̵���� ��ġ 2
	m_nDay2Pyung3		= p_pConfigSet->m_nDay2Pyung3		;//�ܼ� �̵���� ��ġ 3
	m_nDay2Pyung4		= p_pConfigSet->m_nDay2Pyung4		;//�ܼ� �̵���� ��ġ 4


	m_nPmmPDanga		= p_pConfigSet->m_nPmmPDanga;		// ��ո��� �ܰ� +n%		��
	m_nPmmMDanga		= p_pConfigSet->m_nPmmMDanga;		// ��ո��� �ܰ� -n%		��

	m_strAcct			= p_pConfigSet->m_strAcct;			// ���� ����

	m_bNotDrawLine		= p_pConfigSet->m_bNotDrawLine;		//���ؼ��� �׸��� ����

	m_bChkName		= p_pConfigSet->m_bChkName		;		// ����ǥ�� : �̸�	üũ ��ư
	m_bChkValue		= p_pConfigSet->m_bChkValue		;		// ����ǥ�� : ��	üũ ��ư
	m_nAlign		= p_pConfigSet->m_nAlign		;		// ����ǥ�� (����/������/����)
	
}


//---------------------------------------------------------
// Author       : Sang-Yun, Nam			Date : 2005/01/28
// Modifier     : 
// Comments		: �������� ���� �ʱ�ȭ �Ѵ�. ��ó�� �ε��� 
//                ȯ�� ���� ������ ���°�� �� ������ �ʱ�ȭ!!
//---------------------------------------------------------
void CConfigSet::InitMember()
{
	m_bNotDrawLine		= FALSE;//���ؼ� �׸���.

	m_bChkYSprice 		= FALSE;//���ϰ��� �ð�				üũ ����
	m_bChkYHprice 		= FALSE;//���ϰ��� ��				üũ ����
	m_bChkYLprice 		= FALSE;//���ϰ��� ����				üũ ����
	m_bChkYEprice 		= FALSE;//���ϰ��� ����				üũ ����
	m_bChkYSHL 			= FALSE;//���ϰ��� (��+��+��)/3		üũ ����
	
	m_bChkTSprice 		= FALSE;//���ϰ��� �ð�				üũ ����
	m_bChkTHprice 		= FALSE;//���ϰ��� ��				üũ ����
	m_bChkTLprice 		= FALSE;//���ϰ��� ����				üũ ����
	m_bChkTEprice 		= FALSE;//���ϰ��� ����				üũ ����
	m_bChkTHL 			= FALSE;//���ϰ��� (��+��)/2		üũ ����
	m_bChkTSHL 			= FALSE;//���ϰ��� (��+��+��)/3		üũ ����
	m_bChkTHLE 			= FALSE;//���ϰ��� (��+��+(��*2))/4 üũ ����
	m_bChkHighLimit		= FALSE;//���Ѱ�					üũ ����
	m_bChkLowLimit		= FALSE;//���Ѱ�					üũ ����
	
	m_bChkPivot2Rst 	= FALSE;// �Ǻ� 2�� ���� 			üũ ����
	m_bChkPivot1Rst 	= FALSE;// �Ǻ� 1�� ���� 			üũ ����
	m_bChkPivotGLine 	= FALSE;// �Ǻ� ���ؼ�  			üũ ����
	m_bChkPivot1GG 		= FALSE;// �Ǻ� 1�� ���� 			üũ ����
	m_bChkPivot2GG 		= FALSE;// �Ǻ� 2�� ���� 			üũ ����
	
	m_bChkDMKPreHPrice 	= FALSE;// Demark �����  		üũ ����
	m_bChkDMKGLine 		= FALSE;// Demark ���ؼ�			üũ ����
	m_bChkDMKPreLPrice 	= FALSE;// Demark ��������  		üũ ����
	
	m_bChk2Pyung1 		= FALSE;// �̵���� ù��° �Է°�	üũ ����
	m_bChk2Pyung2 		= FALSE;// �̵���� �ι�° �Է°�	üũ ����
	m_bChk2Pyung3 		= FALSE;// �̵���� ����° �Է°�	üũ ����
	m_bChk2Pyung4 		= FALSE;// �̵���� �׹�° �Է°�	üũ ����
	
	m_bChkPmmDanga 		= FALSE;// ��ո��� �ܰ�			üũ ����
	m_bChkPmmPDanga 	= FALSE;// ��ո��� �ܰ� +n%		üũ ����
	m_bChkPmmMDanga 	= FALSE;// ��ո��� �ܰ� -n%		üũ ����	
	
	m_clrYSprice 		= RGB(255,0,0);	//���ϰ��� �ð�				�� ������
	m_clrYHprice 		= RGB(255,0,0);	//���ϰ��� ��				�� ������
	m_clrYLprice 		= RGB(255,0,0);	//���ϰ��� ����				�� ������
	m_clrYEprice 		= RGB(255,0,0);	//���ϰ��� ����				�� ������
	m_clrYSHL 			= RGB(255,0,0);	//���ϰ��� (��+��+��)/3		�� ������
	
	m_clrTSprice 		= RGB(255,0,0);	//���ϰ��� �ð�				�� ������
	m_clrTHprice 		= RGB(255,0,0);	//���ϰ��� ��				�� ������
	m_clrTLprice 		= RGB(255,0,0);	//���ϰ��� ����				�� ������
	m_clrTEprice 		= RGB(255,0,0);	//���ϰ��� ����				�� ������
	m_clrTHL 			= RGB(255,0,0);	//���ϰ��� (��+��)/2		�� ������
	m_clrTSHL 			= RGB(255,0,0);	//���ϰ��� (��+��+��)/3		�� ������
	m_clrTHLE 			= RGB(255,0,0);	//���ϰ��� (��+��+(��*2))/4 �� ������
	m_clrHighLimit		= RGB(255,0,0);	//���Ѱ�					�� ������
	m_clrLowLimit		= RGB(255,0,0);	//���Ѱ�					�� ������
	
	m_clrPivot2Rst 		= RGB(255,0,0);	// �Ǻ� 2�� ����			�� ������
	m_clrPivot1Rst 		= RGB(255,0,0);	// �Ǻ� 1�� ����			�� ������
	m_clrPivotGLine 	= RGB(255,0,0);	// �Ǻ� ���ؼ�				�� ������
	m_clrPivot1GG 		= RGB(255,0,0);	// �Ǻ� 1�� ����			�� ������
	m_clrPivot2GG 		= RGB(255,0,0);	// �Ǻ� 2�� ����			�� ������
	
	m_clrDMKPreHPrice 	= RGB(255,0,0);	// Demark �����			�� ������
	m_clrDMKGLine 		= RGB(255,0,0);	// Demark ���ؼ�			�� ������
	m_clrDMKPreLPrice 	= RGB(255,0,0);	// Demark ��������			�� ������
	
	m_clr2Pyung1 		= RGB(255,0,0);	// �̵���� ù��° �Է°�	�� ������
	m_clr2Pyung2 		= RGB(255,0,0);	// �̵���� �ι�° �Է°�	�� ������
	m_clr2Pyung3 		= RGB(255,0,0);	// �̵���� ����° �Է°�	�� ������
	m_clr2Pyung4 		= RGB(255,0,0);	// �̵���� �׹�° �Է°�	�� ������
	
	m_clrPmmDanga 		= RGB(255,0,0);	// ��ո��� �ܰ�			�� ������
	m_clrPmmPDanga 		= RGB(255,0,0);	// ��ո��� �ܰ� +n%		�� ������
	m_clrPmmMDanga 		= RGB(255,0,0);	// ��ո��� �ܰ� -n%		�� ������

	m_nLineYSprice		= 1;//���ϰ��� �ð�				���� �β�
	m_nLineYHprice		= 1;//���ϰ��� ��				���� �β�
	m_nLineYLprice		= 1;//���ϰ��� ����				���� �β�
	m_nLineYEprice		= 1;//���ϰ��� ����				���� �β�
	m_nLineYSHL			= 1;//���ϰ��� (��+��+��)/3		���� �β�
	
	m_nLineTSprice		= 1;//���ϰ��� �ð�				���� �β�
	m_nLineTHprice		= 1;//���ϰ��� ��				���� �β�
	m_nLineTLprice		= 1;//���ϰ��� ����				���� �β�
	m_nLineTEprice		= 1;//���ϰ��� ����				���� �β�
	m_nLineTHL			= 1;//���ϰ��� (��+��)/2		���� �β�
	m_nLineTSHL			= 1;//���ϰ��� (��+��+��)/3		���� �β�
	m_nLineTHLE			= 1;//���ϰ��� (��+��+(��*2))/4 ���� �β�
	m_nLineHighLimit	= 1;//���Ѱ�					���� �β�
	m_nLineLowLimit		= 1;//���Ѱ�					���� �β�
	
	m_nLinePivot2Rst	= 1;// �Ǻ� 2�� ���� ���� �β�
	m_nLinePivot1Rst	= 1;// �Ǻ� 1�� ���� ���� �β�
	m_nLinePivotGLine	= 1;// �Ǻ� ���ؼ�	 ���� �β�
	m_nLinePivot1GG		= 1;// �Ǻ� 1�� ���� ���� �β�
	m_nLinePivot2GG		= 1;// �Ǻ� 2�� ���� ���� �β�
	
	m_nLineDMKPreHPrice = 1;// Demark �����  ���� �β�
	m_nLineDMKGLine		= 1;// Demark ���ؼ�	���� �β�
	m_nLineDMKPreLPrice = 1;// Demark ��������  ���� �β�
	
	m_nLine2Pyung1		= 1;// �̵���� ù��° �Է°� ���� �β�
	m_nLine2Pyung2		= 1;// �̵���� �ι�° �Է°� ���� �β�
	m_nLine2Pyung3		= 1;// �̵���� ����° �Է°� ���� �β�
	m_nLine2Pyung4		= 1;// �̵���� �׹�° �Է°� ���� �β�
	
	m_nLinePmmDanga		= 1;// ��ո��� �ܰ� ���� �β�
	m_nLinePmmPDanga	= 1;// ��ո��� �ܰ� +n% ���� �β�
	m_nLinePmmMDanga	= 1;// ��ո��� �ܰ� -n% ���� �β�

	m_nDay2Pyung1		= 5;	//�ܼ� �̵���� ��ġ 1
	m_nDay2Pyung2		= 20;	//�ܼ� �̵���� ��ġ 2
	m_nDay2Pyung3		= 60;	//�ܼ� �̵���� ��ġ 3
	m_nDay2Pyung4		= 120;	//�ܼ� �̵���� ��ġ 4

	m_clrPmmPDanga 		= 0;	// ��ո��� �ܰ� +n%		��
	m_clrPmmMDanga 		= 0;	// ��ո��� �ܰ� -n%		��

	m_strAcct			= _T("");	

	m_bChkName		= TRUE;		// ����ǥ�� : �̸�	üũ ��ư
	m_bChkValue		= TRUE;		// ����ǥ�� : ��	üũ ��ư
	m_nAlign		= 0;		// ����ǥ�� (����/������/����)
	
}

