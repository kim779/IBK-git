// ChartManager.cpp: implementation of the CChartManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "chartguidelineaddin.h"
#include "ChartManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartManager::CChartManager()
{

}

CChartManager::~CChartManager()
{
	m_ChartList.RemoveAll();
}

void CChartManager::Add_ChartPointer(IChartAddIn *pAddIn)
{
	if(pAddIn != NULL)
	{
		IChartAddIn* pChartAddIn = pAddIn;
		pChartAddIn->AddRef(); //�� Inferface�� �������� AddRef
		m_ChartList.AddTail(pChartAddIn); //���� ��ü ������ ���� list�� �����Ѵ�.
	}
}

void CChartManager::Refresh_AllChart()
{
	IChartAddIn* pChartAddIn = NULL;
	
	POSITION pos = m_ChartList.GetHeadPosition();
	POSITION pos2 = NULL;
	while(!(pos == NULL))		
	{		
		pChartAddIn = (IChartAddIn*)m_ChartList.GetAt(pos);		
		if(pChartAddIn != NULL)
		{
			if(pChartAddIn->Release())
			{				
				pChartAddIn->InvokeAddIn(-1, 0);//���� ��Ű��
				pChartAddIn->AddRef();//�ٽ� Addref					
			}
			else //�̹� ��ȿȭ�� �����ʹ� �����Ѵ�.
			{	
				pos2 = pos;
				m_ChartList.GetNext(pos);
				m_ChartList.RemoveAt(pos2);
				pChartAddIn = NULL;
				continue;
			}
		}
		m_ChartList.GetNext(pos);
	}	
}