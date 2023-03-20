// ChartUndoRedoManagerAddInImp.cpp: implementation of the CChartUndoRedoManagerAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartCommonAddIn.h"
#include "ChartUndoRedoManagerAddInImp.h"


#include "../Include_Chart/BlockBaseEnum.h"
#include "../Include_AddIn/AddInCommand.h"					// for EAI_DRDS_PACKET_END
#include "../Include_AddIn/I000000/_IChartManager.h"		// for IChartManager
#include "../Include_AddIn/I000000/_IChartOCX.h"			// for IChartOCX
#include "../Include_AddIn/I000000/_IAddInToolMgr.h"		// for IAddInToolMgr



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CChartUndoRedoManagerAddInImp::CChartUndoRedoManagerAddInImp(IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{

}


CChartUndoRedoManagerAddInImp::~CChartUndoRedoManagerAddInImp()
{
	OnResetMainBlock();
}


// Multiple Item in DLL
CString CChartUndoRedoManagerAddInImp::m_strAddInItemName = "UNDO_REDO_MANAGER";


//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartUndoRedoManagerAddInImp)
	ONADDINEVENT( OnAddInToolCommand_EAICommandType_char_long)
	ONADDINEVENT( OnResetMainBlock )
	ONADDINEVENT( OnToolCommand )
END_ADDIN_EVENT_MAP()



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 12 / 21
// Return BOOL	: 
// Param  	const EAICommandType p_eCommandID : 
// Param  const char *p_szParam : 
// Param  const long p_lParam : 
// Comments		: 
//-----------------------------------------------------------------------------
BOOL CChartUndoRedoManagerAddInImp::OnAddInToolCommand(	const EAICommandType p_eCommandID, const char *p_szParam, const long p_lParam)
{
	UNDO_REDO_MESSAGE_INFO *pMessageInfo = NULL;

	// 1. UndoList�� ����ؾ� �� Message�� ����� ���ο� Node�� ���� ����Ѵ�.
	if( EAI_UNDO_REDO_MANAGER_ADD_WORK_TO_MANAGER == p_eCommandID )
	{
		pMessageInfo = new UNDO_REDO_MESSAGE_INFO;
		pMessageInfo->strMessageName = p_szParam;
		pMessageInfo->lMessagelValue = p_lParam;
		
		m_lstUndoMessageInfo.AddTail(pMessageInfo);
		
		// 1.1 ���ο� Node�� ���� ����� ������ RedoList�� Clear �Ѵ�.
		if( m_lstRedoMessageInfo.GetCount() > 0)
		{
			POSITION pos = m_lstRedoMessageInfo.GetHeadPosition();

			while(pos)
			{
				pMessageInfo = NULL;
				pMessageInfo = m_lstRedoMessageInfo.GetAt(pos);

				if( pMessageInfo == NULL )	break;

				delete pMessageInfo;
				m_lstRedoMessageInfo.GetNext(pos);
			}
			m_lstRedoMessageInfo.RemoveAll();
		}

		return TRUE;
	}
	else if( EAI_UNDO_REDO_MANAGER_DELETE_REDO_WORK_LIST == p_eCommandID )
	{
		if( m_lstUndoMessageInfo.GetCount() <= 0 )
			return FALSE;

		POSITION pos = m_lstUndoMessageInfo.GetHeadPosition();

		while(pos)
		{
			pMessageInfo = NULL;
			pMessageInfo = m_lstUndoMessageInfo.GetAt(pos);

			if( pMessageInfo == NULL )	break;

			if( pMessageInfo->strMessageName == p_szParam && pMessageInfo->lMessagelValue == p_lParam )
			{
				delete pMessageInfo;
				m_lstUndoMessageInfo.RemoveAt(pos);
				break;
			}

			m_lstUndoMessageInfo.GetNext(pos);
		}
		
		// Node�� ������ ������ RedoList�� Clear �Ѵ�.
		if( m_lstRedoMessageInfo.GetCount() > 0)
		{
			POSITION pos = m_lstRedoMessageInfo.GetHeadPosition();

			while(pos)
			{
				pMessageInfo = NULL;
				pMessageInfo = m_lstRedoMessageInfo.GetAt(pos);

				if( pMessageInfo == NULL )	break;

				delete pMessageInfo;
				m_lstRedoMessageInfo.GetNext(pos);
			}
			m_lstRedoMessageInfo.RemoveAll();
		}

		return TRUE;
	}

	return FALSE;
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 12 / 21
// Return void	: 
// Param  const int p_nToolType : 
// Param  const int p_nCmdOption : 
// Comments		: 
//-----------------------------------------------------------------------------
void CChartUndoRedoManagerAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption )
{
	CToolOptionInfo::TOOLOPTION eToolOption = (CToolOptionInfo::TOOLOPTION)p_nToolType;

	// 1. Undo�� �϶�� Message�� ������ UndoList�� TailNode�� RedoList�� TailNode�� �̵��ϰ�
	// AddIn�鿡�� Message�� �����Ѵ�.
	if( eToolOption == CToolOptionInfo::T_UNDO_WORK )
	{
		if( m_lstUndoMessageInfo.GetCount() > 0 )
		{
			UNDO_REDO_MESSAGE_INFO *pMessageInfo = NULL;
			pMessageInfo = m_lstUndoMessageInfo.GetTail();

			if( pMessageInfo != NULL )
			{
				m_pIAddInToolMgr->OnAddInToolCommand( EAI_UNDO_REDO_MANAGER_UNDO_THIS_WORK,		\
													  pMessageInfo->strMessageName,				\
													  pMessageInfo->lMessagelValue );

				m_lstUndoMessageInfo.RemoveTail();
				m_lstRedoMessageInfo.AddTail(pMessageInfo);
			}
		}
	}

	// 2. Redo�� �϶�� Message�� ������ RedoList�� TailNode�� UndoList�� TailNode�� �̵��ϰ�
	// AddIn�鿡�� Message�� �����Ѵ�.
	else if( eToolOption == CToolOptionInfo::T_REDO_WORK )
	{
		if( m_lstRedoMessageInfo.GetCount() > 0 )
		{
			UNDO_REDO_MESSAGE_INFO *pMessageInfo = NULL;
			pMessageInfo = m_lstRedoMessageInfo.GetTail();

			if( pMessageInfo != NULL )
			{
				m_pIAddInToolMgr->OnAddInToolCommand( EAI_UNDO_REDO_MANAGER_REDO_THIS_WORK,		\
													  pMessageInfo->strMessageName,				\
													  pMessageInfo->lMessagelValue );

				m_lstRedoMessageInfo.RemoveTail();
				m_lstUndoMessageInfo.AddTail(pMessageInfo);
			}
		}
	}
	//>> (2008/12/17 - Seung-Lyong Park) �߼��� ��������� �� UndoRedo
	else if( eToolOption == CToolOptionInfo::T_DELETE_ALL )
	{
		// 1. Undo List�� ��ϵǾ� �ִ� Node���� �����Ѵ�.
		if( m_lstUndoMessageInfo.GetCount() > 0)
		{
			POSITION pos = m_lstUndoMessageInfo.GetHeadPosition();
			
			UNDO_REDO_MESSAGE_INFO *pMessageInfo;
			while(pos)
			{
				pMessageInfo = NULL;
				pMessageInfo = m_lstUndoMessageInfo.GetAt(pos);
				
				if( pMessageInfo == NULL )	break;
				delete pMessageInfo;
				m_lstUndoMessageInfo.GetNext(pos);
			}
		}
		
		// 2. Redo List�� ��ϵǾ� �ִ� Node���� �����Ѵ�.
		if( m_lstRedoMessageInfo.GetCount() > 0)
		{
			POSITION pos = m_lstRedoMessageInfo.GetHeadPosition();
			
			UNDO_REDO_MESSAGE_INFO *pMessageInfo;
			while(pos)
			{
				pMessageInfo = NULL;
				pMessageInfo = m_lstRedoMessageInfo.GetAt(pos);
				
				if( pMessageInfo == NULL )	break;
				delete pMessageInfo;
				m_lstRedoMessageInfo.GetNext(pos);
			}
		}
		
		m_lstUndoMessageInfo.RemoveAll();
		m_lstRedoMessageInfo.RemoveAll();
	}
	//<< (2008/12/17 - Seung-Lyong Park) �߼��� ��������� �� UndoRedo
}



//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2006 / 12 / 26
// Return void	: 
// Param  void : 
// Comments		: 
//-----------------------------------------------------------------------------
void CChartUndoRedoManagerAddInImp::OnResetMainBlock( void )
{
	// 1. Undo List�� ��ϵǾ� �ִ� Node���� �����Ѵ�.
	if( m_lstUndoMessageInfo.GetCount() > 0)
	{
		POSITION pos = m_lstUndoMessageInfo.GetHeadPosition();
	
		UNDO_REDO_MESSAGE_INFO *pMessageInfo;
		while(pos)
		{
			pMessageInfo = NULL;
			pMessageInfo = m_lstUndoMessageInfo.GetAt(pos);

			if( pMessageInfo == NULL )	break;
			delete pMessageInfo;
			m_lstUndoMessageInfo.GetNext(pos);
		}
	}

	// 2. Redo List�� ��ϵǾ� �ִ� Node���� �����Ѵ�.
	if( m_lstRedoMessageInfo.GetCount() > 0)
	{
		POSITION pos = m_lstRedoMessageInfo.GetHeadPosition();
	
		UNDO_REDO_MESSAGE_INFO *pMessageInfo;
		while(pos)
		{
			pMessageInfo = NULL;
			pMessageInfo = m_lstRedoMessageInfo.GetAt(pos);

			if( pMessageInfo == NULL )	break;
			delete pMessageInfo;
			m_lstRedoMessageInfo.GetNext(pos);
		}
	}

	m_lstUndoMessageInfo.RemoveAll();
	m_lstRedoMessageInfo.RemoveAll();
}