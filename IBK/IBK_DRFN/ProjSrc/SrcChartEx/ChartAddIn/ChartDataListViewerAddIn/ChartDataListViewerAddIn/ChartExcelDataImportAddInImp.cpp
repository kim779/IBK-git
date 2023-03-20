// ChartExcelDataImportAddInImp.cpp: implementation of the CChartExcelDataImportAddInImp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartDataListViewerAddIn.h"
#include "ChartExcelDataImportAddInImp.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"
#include "../Include_Chart/Conversion.h"							// for CDataConversion
#include "../Include_Addin_134221/I134221/_IHelpMessageMap.h"		// for IHelpMessageMap

#include "BasicExcelVC6.h"
using namespace YExcel;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartExcelDataImportAddInImp::CChartExcelDataImportAddInImp( IChartOCX *p_pIChartOCX, IAddInDllBase *p_pIAddInDllBase)
	: CChartAddInBase( p_pIChartOCX, p_pIAddInDllBase)
{
	// (2008/3/3 - Seung-Won, Bae) for Multi Language
	m_hOcxWnd = m_pChartCWnd->GetSafeHwnd();
}

CChartExcelDataImportAddInImp::~CChartExcelDataImportAddInImp()
{

}

// (2006/2/15 - Seung-Won, Bae) Multiple Item in DLL
CString CChartExcelDataImportAddInImp::m_strAddInItemName = "EXCEL_IMPORT";

//////////////////////////////////////////////////////////////////////
// Define AddIn Event Map
//////////////////////////////////////////////////////////////////////
BEGIN_ADDIN_EVENT_MAP( CChartExcelDataImportAddInImp)
	ONADDINEVENT( OnToolCommand)
	ONADDINEVENT( OnPacketDataMultiItem)
	ONADDINEVENT( OnCmdMsg)
END_ADDIN_EVENT_MAP()

//////////////////////////////////////////////////////////////////////
// Overrides
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// (2006/1/17 - Seung-Won, Bae) Tool Interfaces
//		Caution! If the Event is processed, it returns TRUE.
//					and You have not to pass the Event to a next Event Routine.
//////////////////////////////////////////////////////////////////////

BOOL CChartExcelDataImportAddInImp::OnCmdMsg( UINT nID, int nCode, ICmdUI *p_pICmdUI)
{
	return FALSE;
#if 0	//Tool ID�� ���ǰ� �Ǹ� Ǭ��.	
	if( nID != ID_DLV_SHOW_DATA_LIST_VIEWER) return FALSE;

	if( CN_UPDATE_COMMAND_UI == nCode)
	{
		CCmdUI *pCmdUI = ( CCmdUI *)pExtra;
		if( !pCmdUI) return FALSE;
		pCmdUI->Enable( TRUE);
		return TRUE;
	}

	if( CN_COMMAND == nCode)
	{
		OnAddInToolCommand( EAI_OOA_ON_TOOL_FUNCTION, CToolOptionInfo::T_DATALIST_REPORT, 1);
		return TRUE;
	}
#endif
	return TRUE;
}

void CChartExcelDataImportAddInImp::OnToolCommand( const int p_nToolType, const int p_nCmdOption)
{
	if( CToolOptionInfo::T_EXCEL_IMPORT != p_nToolType) return;

	if(!p_nCmdOption) return;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	LoadExcel();	

	return;
}

BOOL CChartExcelDataImportAddInImp::InvokeAddIn(int p_nCommandType, long p_lData)
{
	AFX_MANAGE_STATE( AfxGetStaticModuleState());

	if(  p_nCommandType  != 1) return FALSE;

	return LoadExcel();
}

//////////////////////////////////////////////////////////////////////////
// ���� ���Ͽ��� �����͸� �ҷ����� ���� �������� �����͸��� �ҷ� �´�.
// �� �������� �����ʹ� �������� �ʴ´�.
// ��¥�����ʹ� ���������� �� ������ ��¥���� �͸��� �����Ѵ�.
// ��Ʈ�� ������ �Ǿ� �ִ� ��츸 �����Ѵ�. �� �����޽��� �� �̹� ������ �Ǿ� �ִ� ��쿡�� ��Ʈ�� �׸� �� �ִ�.
// �����޽����� ���� �� ��Ʈ�� ��� �����ʹ� �Է��� �ǳ�, ȭ�鿡 ǥ���ϱ� ���ؼ��� ����ڴ� �Ⱓ �ʱ�ȭ ���� �۾��� �ؾ��Ѵ�.
BOOL CChartExcelDataImportAddInImp::LoadExcel()
{
	CString strFilter = "*.xls";

	CFileDialog dlg(TRUE, "*.xls", "*.xls", OFN_FILEMUSTEXIST, strFilter);
	if(dlg.DoModal() == IDOK)
	{
		CString strFileName = dlg.GetPathName();
		BasicExcel e;

		if(!e.Load(strFileName)) return FALSE;
		//BasicExcelWorksheet* sheet1 = e.GetWorksheet("Sheet1");
		size_t iIndex = 0;
		BasicExcelWorksheet* sheet1 = e.GetWorksheet(iIndex);
		if (sheet1)
		{
			// import start
			//////////////////////////////////////////////////////////////////////////
			// ���� �޽��� ���
			IHelpMessageMap *SubMsgMap = m_pIDefaultPacketManager->GetHelpMessageMap();
			CString strSubMsg;
			ILPCSTR strKey, strValue;
			POSITION pos  = SubMsgMap->GetStartPosition();
			while (pos)
			{
				SubMsgMap->GetNextAssoc(pos, strKey, strValue);
				strSubMsg.Insert(0,CString(strKey) + "=" + CString(strValue) + "@");
			}
			SubMsgMap->Release();
			SubMsgMap = NULL;
			//////////////////////////////////////////////////////////////////////////
			if(strSubMsg == "")	// �����޽����� ���� ��� �����͸� �Է��Ѵ�.
			{
				size_t maxRows = sheet1->GetTotalRows();
				size_t maxCols = sheet1->GetTotalCols();
				BasicExcelCell* CellPacketName;
				CString strPacketName;

				// data import
				for(int i=0;i<maxCols;i++)
				{
					// Packet �� ���
					strPacketName = "";
					CellPacketName = sheet1->Cell(0,i);
					strPacketName = CellPacketName->GetWString();
					
					IPacket *pIPacket = NULL;
					// if(strPacketName.Find("����") >= 0)
					CString strDate;
					strDate.LoadString(IDS_DATE);
					if(strPacketName.Find(strDate) >= 0)
					{
						//pIPacket = m_pIDefaultPacketManager->GetPacket("�ڷ�����");
						//strPacketName = "�ڷ�����";
						pIPacket = m_pIDefaultPacketManager->GetPacket(_MTEXT(C0_DATE_TIME));
						strPacketName = _MTEXT(C0_DATE_TIME);
					}
					// else if(strPacketName.Find("�ŷ���") >= 0)	
					else if(strPacketName.Find( _LTEXT3( C3_VOLUME)) >= 0)	
					{
						//pIPacket = m_pIDefaultPacketManager->GetPacket("�⺻�ŷ���");
						//strPacketName = "�⺻�ŷ���";
						pIPacket = m_pIDefaultPacketManager->GetPacket(_MTEXT(C0_VOLUME));
						strPacketName = _MTEXT(C0_VOLUME);
					}
					else
					{
						strPacketName = _LTOM0( strPacketName);
						pIPacket = m_pIDefaultPacketManager->GetPacket( strPacketName);
					}

					if(!pIPacket)
					{
					//	CString strMsg;
					//	strMsg.Format("[%s]��Ŷ�� �����Ǿ����� �ʽ��ϴ�.\n ��Ŷ ���� �Ǵ� ���� ������ �׸��� Ȯ���Ͻñ� �ٶ��ϴ�.", strPacketName);
					//	AfxMessageBox(strMsg);
						//continue;
						return TRUE;
					}

					pIPacket->ClearData(true);
					ILPCSTR szPacketType = pIPacket->GetType();
					CString strPacketType( szPacketType);
					if( strPacketType == _MTEXT( C6_CHARACTER))
					{
						CString strMsg;
						// strMsg = "�ؽ�Ʈ �����ʹ� ����Ʈ�� �� �����ϴ�.";
						strMsg.LoadString(IDS_TEXTIMPORT);
						AfxMessageBox(strMsg);
					}
					else
					{
						CTimeSpan t(0, 0, 0, 0 );
						//for(int j=1; j<maxRows; j++)
						for(int j=maxRows-1; j>0; j--)
						{
							// if(strPacketName == "�ڷ�����")	_MTEXT(C0_DATE_TIME)
							if(strPacketName == _MTEXT(C0_DATE_TIME))
							{
								//////////////////////////////////////////////////////////////////////////
								// �Ʒ��� ������ ��쿡 ���� �ٲ� �� �ִ�.
								if( ( strPacketType == "YYYYMMDD") || ( strPacketType == "YYMMDD") ||
									( strPacketType == "YYYYMM") || ( strPacketType == "YYYYMMDD") ||
									( strPacketType == "YYMM") || ( strPacketType == "MMDD") )
									pIPacket->AppendData(sheet1->Cell(j,i)->GetDate());
								else
									pIPacket->AppendData(sheet1->Cell(j,i)->GetTime());
							}
							else
								pIPacket->AppendData(sheet1->Cell(j,i)->GetDouble());
						}					
					}
					pIPacket->Release();
				}
			}
			else
			{
				m_pIChartOCX->SetBinTrHelpMsg(strSubMsg);	// SetBinTr�� ���� ���� �޽��� ���� 
				size_t maxRows = sheet1->GetTotalRows();
				size_t maxCols = sheet1->GetTotalCols();
				BasicExcelCell* CellPacketName;
				CString strPacketName;
				
				double* p_dBuf;
				
				//////////////////////////////////////////////////////////////////////////
				// data import
				for(int i=0;i<maxCols;i++)
				{
					p_dBuf = new double[maxRows-1];
					memset(p_dBuf,0,sizeof(double)*(maxRows-1));
					
					// Packet �� ���
					strPacketName = "";
					CellPacketName = sheet1->Cell(0,i);
					strPacketName = CellPacketName->GetWString();
					
					IPacket *pIPacket = NULL;
					CString strDate;
					strDate.LoadString(IDS_DATE);
					// if(strPacketName.Find("����") >= 0)
					if(strPacketName.Find(strDate) >= 0)
					{
						//pIPacket = m_pIDefaultPacketManager->GetPacket("�ڷ�����");
						//strPacketName = "�ڷ�����";
						pIPacket = m_pIDefaultPacketManager->GetPacket(_MTEXT(C0_DATE_TIME));
						strPacketName = _MTEXT(C0_DATE_TIME);
					}
					// else if(strPacketName.Find("�ŷ���") >= 0)
					else if(strPacketName.Find( _LTEXT3( C3_VOLUME)) >= 0)
					{
						//pIPacket = m_pIDefaultPacketManager->GetPacket("�⺻�ŷ���");
						//strPacketName = "�⺻�ŷ���";
						pIPacket = m_pIDefaultPacketManager->GetPacket(_MTEXT(C0_VOLUME));
						strPacketName = _MTEXT(C0_VOLUME);
					}
					else
					{
						strPacketName = _LTOM0( strPacketName);
						pIPacket = m_pIDefaultPacketManager->GetPacket( strPacketName);
					}
					
					if(!pIPacket)
					{
						//	CString strMsg;
						//	strMsg.Format("[%s]��Ŷ�� �����Ǿ����� �ʽ��ϴ�.\n ��Ŷ ���� �Ǵ� ���� ������ �׸��� Ȯ���Ͻñ� �ٶ��ϴ�.", strPacketName);
						//	AfxMessageBox(strMsg);
						continue;
						//return TRUE;
					}
					
					pIPacket->ClearData(true);
					ILPCSTR szPacketType = pIPacket->GetType();
					CString strPacketType( szPacketType);
					if( strPacketType == _MTEXT(C6_CHARACTER))
					{
						CString strMsg;
						// strMsg = "�ؽ�Ʈ �����ʹ� ����Ʈ�� �� �����ϴ�.";
						strMsg.LoadString( IDS_TEXTIMPORT);
						AfxMessageBox(strMsg);
						
					}
					else
					{
						CTimeSpan t(0, 0, 0, 0 );
						for(int j=0; j<maxRows-1; j++)
						{
							// if(strPacketName == "�ڷ�����")
							if( strPacketName == _MTEXT(C0_DATE_TIME))
							{
								//////////////////////////////////////////////////////////////////////////
								// �Ʒ��� ������ ��쿡 ���� �ٲ� �� �ִ�.
								if( ( strPacketType == "YYYYMMDD") || ( strPacketType == "YYMMDD") ||
									( strPacketType == "YYYYMM") || ( strPacketType == "YYYYMMDD") ||
									( strPacketType == "YYMM") || ( strPacketType == "MMDD"))
									p_dBuf[maxRows-2-j] = sheet1->Cell(j+1,i)->GetDate();
								else
									p_dBuf[maxRows-2-j] = sheet1->Cell(j+1,i)->GetTime();
							}
							else
							// 20081013 �ڵ��� 8����/32������ 10�������� �о�´�. >>
							{
								CString strOutput;			// ��ȯ�� ���� ����� ����
								CString strInput = sheet1->Cell(j+1,i)->GetString();

								// (2008/10/19 - Seung-Won, Bae) Use double for Volume
								if( strInput.IsEmpty()) p_dBuf[maxRows-2-j] = sheet1->Cell(j+1,i)->GetDouble();
								else
								{
									// ��Ŷ�� ���´�.
									pIPacket = m_pIDefaultPacketManager->GetPacket(strPacketName);	
									if(pIPacket)
									{
										// 10�������� ��ȯ�Ѵ�.
										ILPCSTR szPacketType2 = pIPacket->GetType();
										CString strPacketType2( szPacketType2);
										BOOL bResult = CDataConversion::GetDataToDec( strInput, strPacketType2, strOutput);
										// ���ڿ��� double�� ��ȯ�Ѵ�.
										if( bResult)	p_dBuf[maxRows-2-j] = atof(strOutput);
										// ����� �����Ͱ� 32������ �ƴҶ��� 0���� �ִ´�.
										else			p_dBuf[maxRows-2-j] = 0;
									}
									else				p_dBuf[maxRows-2-j] = sheet1->Cell(j+1,i)->GetDouble();
								}
							}
							// 20081013 �ڵ��� <<
						}					
					}
					m_pIChartOCX->SetBinTrDoubleData(strPacketName, maxRows-1, p_dBuf);
					delete []p_dBuf;
					pIPacket->Release();
				}
				
				// import end
				m_pIChartOCX->SetBinTrEnd();
			}
		}
	}

	return TRUE;
}

// RQ�� Packet : �������� - ojtaso (20070111)
void CChartExcelDataImportAddInImp::OnPacketDataMultiItem(LPCTSTR lpszRQ, const char *p_szPacketNames, BOOL p_bWithReset)
{
	OnToolCommand( CToolOptionInfo::T_DATALIST_REPORT, 0);
	OnToolCommand( CToolOptionInfo::T_DATALIST_REPORT, 1);
}
