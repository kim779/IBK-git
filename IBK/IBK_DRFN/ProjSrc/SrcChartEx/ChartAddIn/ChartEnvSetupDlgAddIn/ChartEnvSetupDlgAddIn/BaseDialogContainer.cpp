// BaseDialogContainer.cpp: implementation of the CBaseDialogContainer class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "BaseDialogContainer.h"
#include "BaseDialog.h"


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Modifier		: 
// Comments		: Construction
//-----------------------------------------------------------------------------
CBaseDialogContainer::CBaseDialogContainer()
{

}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Modifier		: 
// Comments		: Destruction
//-----------------------------------------------------------------------------
CBaseDialogContainer::~CBaseDialogContainer()
{
	RemoveAll();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Return void : 
// Param  LPCTSTR pcszIndicatorName : Key�� ����� ��ǥ��
// Comments		: ��ǥ���� ���̾�α׸� ������ Map�� �߰��Ѵ�.
//-----------------------------------------------------------------------------
void CBaseDialogContainer::AddBaseDialog( LPCTSTR pcszIndicatorName, CBaseDialog *pChildDialog)
{
	m_mapBaseDialog.SetAt( pcszIndicatorName, pChildDialog);
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Param  LPCTSTR pcszIndicatorName : ������ ��ǥ��(Key��) 
// Comments		: ��ǥ���� ���̾�α׸� ã�� �����Ѵ�.
//-----------------------------------------------------------------------------
CBaseDialog *CBaseDialogContainer::GetBaseDialog( LPCTSTR pcszIndicatorName )
{
	if(pcszIndicatorName == "")
		return NULL;

	CBaseDialog *pBaseDialog = NULL;
	m_mapBaseDialog.Lookup( pcszIndicatorName, pBaseDialog);
	return pBaseDialog;
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Return void : 
// Comments		: ��ǥ����Dlg���� Map�� ������ ���� �����Ѵ�. 
//-----------------------------------------------------------------------------
void CBaseDialogContainer::RemoveAll()
{
	POSITION pos = m_mapBaseDialog.GetStartPosition();

	while( pos )
	{
		CBaseDialog * pBaseDlg = NULL;
		CString strIndicatorName;
		m_mapBaseDialog.GetNextAssoc( pos, strIndicatorName, pBaseDlg);
		if( pBaseDlg) delete pBaseDlg;
	}

	m_mapBaseDialog.RemoveAll();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Return POSITION : 
// Comments		: Map�� Start Position�� �����´�.
//-----------------------------------------------------------------------------
POSITION CBaseDialogContainer::GetStartPosition() const
{
	return m_mapBaseDialog.GetStartPosition();
}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Param  POSITION &pos : 
// Comments		: Map�� Item���� POSITION���� �����´�.
//-----------------------------------------------------------------------------
CBaseDialog *CBaseDialogContainer::GetNextAssoc( POSITION &pos )
{
	CString strKey = _T("");
	CBaseDialog *pBaseDialog = NULL;
	
	m_mapBaseDialog.GetNextAssoc( pos, strKey, pBaseDialog);

	return pBaseDialog;
}