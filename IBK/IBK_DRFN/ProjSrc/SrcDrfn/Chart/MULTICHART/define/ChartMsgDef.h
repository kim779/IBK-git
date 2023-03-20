#ifndef __CHART_MSG_DEF__BYGM0604
#define __CHART_MSG_DEF__BYGM0604
#include <afxtempl.h>

#include "../../commontr/TRDefine.h"

#define UMSG_INPUTCODETOINPUTBAR		WM_USER+1	// �����ڵ带 ������.
#define UMSG_SEND2SERVER				WM_USER+2	// ������ ��ȸ�� ������.
#define UMSG_RECEIVEFROMSERVER			WM_USER+3	// �����κ��� ��ȸ����� �޴´�.
#define UMSG_CHANGEMODE					WM_USER+4	// �����Է¸�带 �ٲ۴�.
#define UMSG_SHOWINDICATORCONFIGDLG		WM_USER+5	// ��ǥ����â�� ���δ�.
#define UMSG_SETTYPE					WM_USER+6	// ��Ʈ�� ������Ÿ���� �����Ѵ�.
#define UMSG_SETCNTS					WM_USER+7	// ��Ʈ�� �����ͼ��� �����Ѵ�.
#define UMSG_SETZOOMVALUE				WM_USER+8	// ��Ʈ�� �������� �����ͼ��� �����Ѵ�.
#define UMSG_SETINITIALCHARTDATA		WM_USER+9	// ��Ʈ�� �⺻�ʱⰪ�� ������.
#define UMSG_SHOWREGFAVORITEDLG			WM_USER+10	// ���ã��â�� ���δ�.
#define UMSG_SETMATRIX					WM_USER+11	// ��Ƽ��Ʈ�� �迭�� �ٲ۴�.
#define UMSG_REQUESTMORE				WM_USER+12	// ������ �߰� �����͸� �䱸�Ѵ�.
#define UMSG_RESETCHART					WM_USER+13	// ��Ʈ�� �ʱ�ȭ�Ѵ�.
#define UMSG_APPLYFAVORITE				WM_USER+14	// ���ã�⸦ �����Ѵ�.
#define UMSG_GETNAMEINDICATOR			WM_USER+15	// ��ǥ������ �����´�.
#define UMSG_HASGRAPH					WM_USER+16	// �ش�׷����� �ִ��� üũ�Ѵ�.
#define UMSG_ADDCHARTBLOCK				WM_USER+17	// �ش�׷����� �߰��Ѵ�.
#define UMSG_RESETOCX					WM_USER+18	// OCX�� �ʱ�ȭ�Ѵ�.
#define UMSG_SETJONGMOKLINK				WM_USER+19	// ���񿬵������� �Ѵ�.
#define UMSG_SETTYPELINK				WM_USER+20	// �������������� �Ѵ�.
#define UMSG_LOADCHART					WM_USER+21	// ��Ʈ�� �ε��Ѵ�.
#define UMSG_SAVECHART					WM_USER+22	// ��Ʈ�� �����Ѵ�.
#define UMSG_ADDUPJONGDATA				WM_USER+23	// ���������͸� �߰��Ѵ�.
#define UMSG_FIXDATE					WM_USER+24	// ���ڸ� ������Ų��.
#define UMSG_INPUTCODEFROMINPUTBAR		WM_USER+25	// InputBar�κ��� �����ڵ带 �޾� �ݿ��Ѵ�.
#define UMSG_SHOWSCALE					WM_USER+26	// X,Y���� Scale ���⸦ �����Ѵ�.
#define UMSG_SETVIEWENDTIME				WM_USER+27	// ��Ʈ�� ����� ��������¥�� �����Ѵ�.
#define UMSG_UPDATEINDEX				WM_USER+28	// ��Ʈ�� ��ǥ�� ���� �ݿ��Ѵ�.
#define UMSG_GETENDDATEFROMINPUTBAR		WM_USER+29	// InputBar���� �Ⱓ������ �Ǿ��ִٸ� �ش� ������ ��¥
#define UMSG_SENDSHAREDCODE				WM_USER+30	// Main�����޸𸮿����� �ڵ带 ������.
#define UMSG_SETONECHART				WM_USER+31	// ���õ��Ѱ��� ��Ʈ�� ������ �������� �����Ѵ�.
#define UMSG_SENDTOMSGBOX				WM_USER+32	// �޼����ڽ��� �޼����� ������.
#define UMSG_ADDMARKETDATA				WM_USER+33	// ���嵥���͸� �߰��Ѵ�.
#define UMSG_GETTYPEFROMINPUTBAR		WM_USER+34	// ������Ÿ���� �޴´�.
#define UMSG_GETTYPENUMFROMINPUTBAR		WM_USER+35	// ������Ÿ���ǰ����� �޴´�.
#define UMSG_UPDATEREVISED				WM_USER+36	// �����ְ� ��뿩�θ� ������.
#define UMSG_GETREVISED					WM_USER+37	// �����ְ� ��뿩�θ� �޴´�.
#define UMSG_CLEARLINK					WM_USER+38	// ����,�ð������������Ѵ�.
#define UMSG_MULTICODES					WM_USER+39	// �ټ������� ��ȸ�Ѵ�.
#define UMSG_SENDSYNCCODE				WM_USER+40	// �ڵ带 ����ȭ�Ѵ�.
#define UMSG_INPUTINSHAREDCODELIST		WM_USER+41	// �����ڵ帮��Ʈ�������� �Է��Ѵ�.
#define UMSG_INPUTUPJONGCODE			WM_USER+42	// �����ڵ带 �Է��Ѵ�.
#define UMSG_GETZOOMVALUE				WM_USER+43	// ��Ʈ�� �������� �����ͼ��� �����´�.
#define UMSG_SHOWENVIRDLG				WM_USER+44	// ȯ�漳��DLG�� �����ش�.
#define UMSG_GETINC						WM_USER+45	// �߰�, ���� ������ �޴´�.
#define UMSG_EDITCODE_INIT				WM_USER+46  // �ѱ�����ã�� �ʱ�ȭ...
#define	UMSG_SENDSHARETIME				WM_USER+47  // �ð�����...
#define	UMSG_GETSAVED					WM_USER+48  // �������������� �޴´�...
#define	UMSG_GETTIMESHARSYSEAGE			WM_USER+49	// �ð������� ��뿩�θ� �޴´�...
#define	UMSG_SETTIMESHARSYSEAGE			WM_USER+50	// �ð������� ��뿩�θ� ������...
#define	UMSG_SETUPPERLOWPRICEMARKUSEAGE	WM_USER+51	// �����Ѱ�ǥ�� ��뿩�θ� ������...
#define	UMSG_GETUPPERLOWPRICEMARKUSEAGE	WM_USER+52	// �����Ѱ�ǥ�� ��뿩�θ� �޴´�...
#define	UMSG_RATEVALUE					WM_USER+53	// ����/Dollar/��ȭ ������ �����ϸ� ������Ʈ �ݿ��Ѵ�.
#define	UMSG_SETSHOWLOCK				WM_USER+54	// �� ���� ǥ�� �����Ѵ�.
#define	UMSG_GETSHOWLOCK				WM_USER+55	// �� ���� ǥ�� �����Ѵ�.
#define	UMSG_SENDJCODECHECKED			WM_USER+56	// Check�ϴ� �����ڵ带 ������.
#define UMSG_GETJCODECHECKEDFROMCHART	WM_USER+57	// Check�ϴ� �����ڵ带 �����´�.
#define UMSG_GETJCODECHECKEDFROMLEFTBAR	WM_USER+58	// Check�ϴ� �����ڵ带 �����´�.
#define	UMSG_SENDJCODSYNHIGHLIGHTED		WM_USER+59	// ����ǥ�õ� ������ �����Ѵ�.
#define	UMSG_GETJONGMOKCODE				WM_USER+60	// ���õ���Ʈ��������������.
#define	UMSG_UPDATECODE					WM_USER+61	// WmdMapView������Update���޴´�.
#define	UMSG_SETCHARTHOGA				WM_USER+62	// ��Ʈ-ȣ�� �� ���� WndMapView�� ������.
#define	UMSG_GETFAVORITETHINGSMNG		WM_USER+63	// CFavoriteThingsMng Pointer�� �����´�.
#define UMSG_UPDATEMATRIX				WM_USER+64	// ȭ��迭�� �������Ѵ�.
#define UMSG_INPUTCODETOCHARTMNG		WM_USER+65	// �����ڵ带 ������.
#define UMSG_GETCNTOFCHART				WM_USER+66	// ��Ʈ�� ���� �����´�.
#define UMSG_GETSTATUSENABLE			WM_USER+67	// ��ȸ�� ���¸� �����´�.
#define UMSG_CHANGEMODEBYINPUTBAR		WM_USER+68	// �����Է¸�带 �ٲ۴�. 0: ����, 1:�߰�
#define UMSG_SETACCOUNTINFO				WM_USER+69	// ���������� ������.
#define UMSG_SETHIGHLIGHTINFO			WM_USER+70	// Highlight ������ ������.
#define UMSG_GETJCODECHECKEDFROMCODESRC	WM_USER+71	// Check List�� �����´�.
#define UMSG_SETNORMALIZEDMINUTETIMERULE	WM_USER+72	// �����ǥ�ø� �����Ѵ�.
#define UMSG_GETNORMALIZEDMINUTETIMERULE	WM_USER+73	// �����ǥ�ø� �����´�.
#define UMSG_ANALYSISPATTERN				WM_USER+74	// ChartPatternAddin.dll
#define UMSG_CHARTGUIDELINE					WM_USER+75	// ChartGuideLine.dll
#define UMSG_INPUTFUTURECODE				WM_USER+76	// �����ڵ带 �Է��Ѵ�.
#define UMSG_GETDRAGMODETYPE				WM_USER+77	// �Ⱓ�м���������� �����´�.
#define UMSG_SETDRAGMODETYPE				WM_USER+78	// �Ⱓ�м���������� ������.
#define UMSG_SETORDEREDINFO					WM_USER+79	// ü�᳻�������� ����.
#define UMSG_LOADSYSTEMTRADING				WM_USER+80

//2005. 04. 25 ----------------- Start Add New User message ---------------------------------------
// add by sy, nam
#define USMG_SHOW_DATAWND					WM_USER+81	//chart data list�� ������ �����Ѵ�.
#define USMG_NOTIFY_REVISED					WM_USER+82  //�����ְ� ��뿩�θ� ��Ʈ�ѿ� �ݿ��ϵ��� �Ѵ�.
#define UMSG_GET_TICKMINDAY_VALUE			WM_USER+83  //���� ���õ� ��Ʈ�� Tick/Min/Day�� ������ �����´�.
#define UMSG_USE_LOG						WM_USER+84	//�α� ��뿩�θ� chart ocx�� ������.
#define UMSG_GETLOG							WM_USER+85	//�α� ��뿩�θ� InputBar�κ��� �޴´�.
#define UMSG_CHANGE_CHARTVALUE				WM_USER+86	//�߰��� ��Ʈ�� ������ ����� ��� InputBar�� ��Ʈ �޺��� CTabChartList���� ���õ� �׸��� �����Ѵ�.
#define UMSG_GET_TIME_FROM_DATETIMEPICKER	WM_USER+87	//������ٿ��� �ϴ� ������ �޷���Ʈ���� ������ ��ȸ �������� ���´�.
#define UMSG_ADDITEM_BTNCLICK				WM_USER+88	//����Ʈ�� �����߰� ��ư Ŭ��
#define UMSG_DELITEM_BTNCLICK				WM_USER+89	//����Ʈ�� ������� ��ư Ŭ��
#define UMSG_INITIAL_BTNCLICK				WM_USER+90	//����Ʈ�� �ʱ�ȭ   ��ư Ŭ��
#define UMSG_ADDITEM						WM_USER+91	//����Ʈ�� ���õ� Tab���� ���� ���õ� ������ CLeftbar�� ������.
#define UMSG_GET_COMPARECHART_DATA			WM_USER+92	//����Ʈ�� �߰��Ǵ� ���񿡴��� TR��ȸ�� ���ٿ��� ��û�Ѵ�.

#define UMSG_LOADSAVECHART					WM_USER+93	// ��Ʈ�� �����ϰ� �ε��Ѵ�.

#define UMSG_INPUTFOREIGNCODE				WM_USER+100 // �ؿ����� �ڵ带 �Է��Ѵ�.


#define USMG_PLEASE_RESIZE					WM_USER+150 //Floating ȣ��â Resize
#define USMG_CHANGE_ZORDER					WM_USER+151 //Floating ȣ��â Change Z-Order
#define UMSG_FORCE_SHOW_HOGAWND				WM_USER+152 //Floating ȣ��â ������ ���̵��� �Ѵ�.
#define UMSG_SETONEPAGECNT					WM_USER+153 //05.06. 30 ����Ʈ���� ������ �߰��Ǵ� ��Ʈ ȭ�� ���� ����.
#define UMSG_GET_POINTER_OF_DBMGR			WM_USER+154 //05.06. 30 ��Ƽ��Ʈ MainDlg�� CDBMgr* �� ��û.
#define UMSG_SET_POINTER_OF_DBMGR			WM_USER+155 //05.06. 30 ��Ƽ��Ʈ ���� CDBMgr* �� ��û�� �����쿡�� ����.

#define UMSG_CHANGE_COMPARECHART_TYPE		WM_USER+156 //05.07. 14 ����Ʈ ���� ��Ʈ�� ���¸� �ٲٵ��� ChartMng�� ��Ÿ ���õ� Window���� �����Ѵ�.
#define UMSG_INIT_COMPARECHART				WM_USER+157 //05.07. 14 ����Ʈ ���� ��� Window�ʱ�ȭ 
#define UMSG_SENDTR_HOGA_DATA				WM_USER+158 //05.07. 15 ���� ���̾˷α׿� ȣ���� ��û�Ѵ�.
#define UMSG_DISCONNECT_HOGA_REAL			WM_USER+159 //05.07. 15 ���� ���̾˷α׿� ȣ������ Disconnect�� ��û�Ѵ�.
#define UMSG_SETTYPE_NO_REQUEST				WM_USER+160 //05.07. 19 ��ȸ ������ ���Ѵ�.

#define UMSG_RE_REQUEST_TRDATA				WM_USER+161 //05.07. 20 ����Ʈ�� TR�� �ٽ� ��û�ϵ��� ����Ʈ�� �޼����� ������.
#define UMSG_COMPARE_TR_JONMOK_CNT			WM_USER+162 //05.07. 21 ����Ʈ�� TR��û ���� ������ ���Ѵ�.
#define UMSG_MOVE_CHART_VIEWRANGE			WM_USER+163 //05.07. 28 �� ��Ʈ�� ���̴� ������ ���Ѵ�.
#define UMSG_REPLAYCHART_READY				WM_USER+164 //05.08. 01 ������Ʈ�� ���� �غ� �ϵ��� �Ѵ�.
#define UMSG_RECEIVE_TR09501				WM_USER+165 //05.07. 29 �ڽ���/�ڽ��� ����/����/�׸� ������� �޴´�.

#define UMSG_RECEIVE_FIND0301				WM_USER+166 //05.07. 30 ���ǰ˻� ������� �޴´�.
#define UMSG_DISCONNECT_ALLREAL				WM_USER+167 //05.08. 02 ��Ʈ�� ��� Real �����Ѵ�.
#define UMSG_DESTROY_FLOATINGHOGA			WM_USER+168 //05.08. 02 ȣ�� ������ â�� Destroy
#define UMSG_GET_FUTOPTGUBUN				WM_USER+169 //05.08. 05 �����ɼ� ���а��� Return
#define UMSG_SET_ENDREQUESTDATE				WM_USER+170 //05.08. 09 ��Ʈ ��ȸ ��û ��¥�� �����Ѵ�.

#define UMSG_GET_CHARTMODE					WM_USER+171 //05.08. 11 TR��û ���� ���� ��Ʈ ��尡 �������� �˱� ���� ��û
#define UMSG_GET_REPLAY_STARTTIME			WM_USER+172 //05.08. 11 TR��û ���� ���� ���� ����ϰ�� ���� ���۽ð� ��û 
#define UMSG_SETLOG							WM_USER+173 //05.08.23  �α׸� ��뿩�θ� InputBar�� üũ�ϵ��� �Ѵ�.
#define UMSG_STOP_JIPYOCYCLE				WM_USER+174 //05.08.24  ��ǥ ������ Stop
#define UMS_SETCHART_DATEINFO				WM_USER+175 //05.08.25  ���õ� ��Ʈ�� ����(CExtInputBarDlg)���� ����� ��¥ ������ �ѱ��.

#define UMSG_SHOW_HOGADLG					WM_USER+176 //05.08.25  ��Ʈ ������¡ �� Floating Hoga Dialog�� ���Ž��� �ش�.
#define UMSG_ENABLE_SENDHOGATR				WM_USER+177 //05.08.29  ȣ�� ��ȸ�� �������� ���θ� ������.
#define UMSG_CHECK_BE_COMPLETE_QUERYDATA	WM_USER+178 //05.09.05  TR�� ��ȸ�� �Ǽ� ����� �Դ��� ����.
#define UMSG_SETTEXT_CODE_INPUTBAR			WM_USER+179 //05.09.05  ���� �ڵ常 Set�Ѵ�.
#define UMSG_GETJONGMOKLINK					WM_USER+180 //05.09.06	���� ����ȭ �������� üũ�Ѵ�.

#define UMSG_END_REQUEST_CHARTDATA			WM_USER+181 //05.09.08	��Ʈ ��ȸ�� �������� InputBar�� �˸���.
#define UMSG_SIGNAL_CLICK					WM_USER+182 //05.09.09  ��ȣ�� Click
#define UMSG_STOP_APPLY_EXPTOOOL			WM_USER+183 //05.09.10  �ܺ����ٿ��� ���� �ٸ��� ���õɶ� Ÿ ��Ʈ�鿡�� ���ӱ׸��� ������Ų��.
#define UMSG_FIREGSHAREDATA					WM_USER+184 //05.09.09  ��ƮSharedData
#define UMSG_GETENVSINK						WM_USER+185 //05.09.23  ȯ�� ����ȭ üũ ����.

#define UMSG_GET_REPLAY_DAYBEFORE			WM_USER+186 //05.10.14  ���� ���� n���� �� n����  �����´�.
#define UMSG_ENABLE_PLAYBTN					WM_USER+187 //05.10.14  ���� ���¿��� �޾ƾ� �� �����͸� �� �޾��� ��� Play��ư Ȱ��ȭ �Ѵ�.
#define UMSG_CHARTODRTRACE					WM_USER+188 //OrderTrace.dll
#define UMSG_CHART_INIT_FROMOCX				WM_USER+189 //OCX�κ��� �ʱ�ȭ ��ư�� ������ �� ChartMng�� �� ������ �����ش�.
#define UMSG_ADDSYSTEMTRADINGINDEX			WM_USER+190 //05.10.27 �ý���Ʈ���̵���ǥ�� �߰��Ѵ�. gm0604

#define UMSG_EXTINPUT_CHANGEDATE			WM_USER+191 //05.10.31  ExtInputBarDlg���� ������ ���û��¿��� ��¥�� �ٲ��� TR��ȸ�� ������.
#define UMSG_CLEAR_MAREKT_TR				WM_USER+192 //05.11.01  ��Ʈ ������ ��ȸ�� �� ���ư��� �� ��� ������ǥ TR�غ� ���¸� Clear��Ų��.
#define UMSG_RECV_MARKET_TR					WM_USER+193 //05.11.08  ���� ��ǥ TR�� �޴´�.
#define UMSG_EDIT_FOCUS						WM_USER+194 //05.11.08  �����ڵ忡 ��Ŀ��
#define UMSG_GET_CANDLE_CNT					WM_USER+195 //05.11.17  8��/12��/24�� ��Ʈ�� �� ������ ���� �⺻ �������� Get

#define UMSG_EXT_TOOL_RESTORE				WM_USER+196 //05.11.24  �ܺ����ٿ��� �ʱ�ȭ�� ������ ��� �ش� ��Ʈ�� ������ �� �Ǿ��ִ��� Ȯ���� ������ �ѱ��� �ʱ� ���� �����ϴ� �޼��� 
#define UMSG_DEL_CHART_BLOCK				WM_USER+197 //05.11.30  ��Ʈ �� ����.
#define UMSG_APPLY_ANALYZE_PERIOD_ADDIN		WM_USER+198 //AnalyzePeriodAddin.dll Call
#define UMSG_CHART_PRINT					WM_USER+199 //06.01.23  ����Ʈ
#define UMSG_CHART_EXCEL					WM_USER+200 //06.01.23  ������ ������

#define UMSG_SHOWINNERTOOLBAR				WM_USER+201	// X,Y���� Scale ���⸦ �����Ѵ�.
#define UMSG_SAVEJIPYOONEFILE				WM_USER+202	// 06.05.02 ��/��/��/��/ƽ ���� Ȥ�� ���� ���� �����Ͽ� ����
#define UMSG_GETSAVEJIPYOONEFILE			WM_USER+203	// 06.05.02 ��/��/��/��/ƽ ���� Ȥ�� ���� ���� �����Ͽ� ����
#define UMSG_ONECLICK_HIDE					WM_USER+204 //06.10.02  ��Ŭ�� �����
#define UMSG_DWMM_CHART						WM_USER+205 //06.10.02  ��/��/��/�� ��Ʈ
#define UMSG_PERIOD_SUMMARY					WM_USER+206 //06.11.08  �Ⱓ���
//2007.01.17 add by LYH  ���ٹ�ư ����ȭ
#define UMSG_SYNC_TOOLBAR					WM_USER+208 //07.01.17  ���ٹ�ư ����ȭ
#define UMSG_DROPCODE						WM_USER+209 //07.01.17  ���ٹ�ư ����ȭ
#define UMSG_GETLOADSAVECODE				WM_USER+210	//07.02.08  �����Ҷ� �ڵ�� �о���� GET
#define UMSG_SETLOADSAVECODE				WM_USER+211	//07.02.08  �����Ҷ� �ڵ�� �о���� SET

#define UMSG_REQUEST_FORQUERYOPT			WM_USER+212 //08.04.29	������ȸ�� ���� ��ȸ
#define UMSG_GETQUERYOPT					WM_USER+213 //08.04.29	������ȸ ����
const UINT RMSG_SETQUERYOPT = ::RegisterWindowMessage(_T("RMSG_SETQUERYOPT"));

#define UMSG_RUN_AUTOTIMER					WM_USER+220 //08.04.29	�������� ó��
#define UMSG_IS_PLAYSTATE					WM_USER+221 //08.04.29	�������� ����Ȯ��

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
//20110502 �̹��� >>
#define	UMSG_GET_COMMON_DATA				WM_USER+224
#define	UMSG_SET_COMMON_DATA				WM_USER+225
#define	UMSG_IS_PARENT_STDDIALOG			WM_USER+226
//20110502 �̹��� <<
#define UMSG_STANDBY_TR_CALL				WM_USER+227	// ��⿭
#define UMSG_RELAY_POSSSIBLE_CHECK			WM_USER+228 // �������� ���ɿ��� Ȯ��
//------------------------------ End Add New User message   ---------------------------------------

// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
//20110502 �̹��� >>
#define	COMDATA_SAVE_JIPYO_PER_CODE_PERIOD	0
#define	COMDATA_SAVE_JIPYO_PER_CODE_PERIOD_MENU_ID	1
//20110502 �̹��� <<

//2005. 07. 27 add by sy, nam =======================================================
//�ܺ� ���ٿ� �����ϱ� ���� 
//Main�� ���� Ȱ��ȭ�� Dll�� �ڵ��� wParam�� �Ǿ� �ѱ��.
// 1. Main <-> DLL
const UINT RMSG_CHARTMAIN		= ::RegisterWindowMessage(_T("RMSG_CHARTMAIN"));

// 2. DLL->Chart OCX
const UINT RMS_ANALYSIS_FUNC	= ::RegisterWindowMessage(_T("RMS_ANALYSIS_FUNC"));
//===================================================================================

//@�����߰�
const UINT RMSG_GET_USERID = ::RegisterWindowMessage(_T("RMSG_GET_USERID"));	// User ID ���
//@�����߰�

//2005. 08. 24 add by sy, nam =======================================================
//��ǥ �������⸦ ���߰� �ϱ� ���� 
//�޼����� ������.
//const UINT RMS_STOP_JIPYO_CYCLE	= ::RegisterWindowMessage(_T("RMS_STOP_JIPYO_CYCLE"));
//====================================================================================

// ��Ų ó��
const UINT RMSG_CHANGE_SKINCOLOR = ::RegisterWindowMessage(_T("RMSG_CHANGE_SKINCOLOR"));
const UINT RMSG_GET_SKINCOLOR = ::RegisterWindowMessage(_T("RMSG_GET_SKINCOLOR"));

// ���ν����� ���氡���� Method �߰� - ojtaso (20081013)
const UINT RMSG_CHANGEVERTSCALETYPE = ::RegisterWindowMessage(_T("RMSG_CHANGEVERTSCALETYPE"));

const UINT RMSG_CHANGE_CROSSLINE = ::RegisterWindowMessage(_T("RMSG_CHANGE_CROSSLINE"));

const UINT RMSG_ADD_PLATFORM_MENU = ::RegisterWindowMessage(_T("RMSG_ADD_PLATFORM_MENU"));

const UINT RMSG_SETINITIALCHARTDATA = ::RegisterWindowMessage(_T("RMSG_SETINITIALCHARTDATA"));

const UINT RMSG_GETINFO = ::RegisterWindowMessage(_T("RMSG_GETINFO"));
const UINT RMSG_SETINFO = ::RegisterWindowMessage(_T("RMSG_SETINFO"));

// 05.05.30 add by sy, nam =========================================
#define FROM_TABCHART						1	//UMSG_CHANGE_CHARTVALUE�� ���� ���޼����� ���۽�Ų Wnd ������ 
#define FROM_INPUTBAR						2	//UMSG_CHANGE_CHARTVALUE�� ���� ���޼����� ���۽�Ų Wnd ������ 
#define FROM_TABJIPYO_MARKET				3	//UMSG_CHANGE_CHARTVALUE�� ���� ���޼����� ���۽�Ų Wnd ������ 
												//������ǥ�� ������ ������.
#define FROM_EXTINPUTBAR_MARKET				4	//UMSG_CHANGE_CHARTVALUE�� ���� ���޼����� ���۽�Ų Wnd ������ 
												//������ǥ�� ������ ������.
//===================================================================================

// message definition From Invoke DLL 
#define UMSG_SEND2SERVERFROMINVOKE			WM_USER+500	
#define UMSG_GETCODEFROMINVOKE				WM_USER+501
#define UMSG_GETMAINHANDLEFROMINVOKE		WM_USER+502

// message definition To Invoke DLL 
#define INVOKEMSG_SETACCOUNTNQUOTECODE		1
#define INVOKEMSG_GETAVGINFO				2
#define INVOKEMSG_INIT						100
#define INVOKEMSG_RECEIVEDATA				101
#define INVOKEMSG_CHANGECODE				102
#define INVOKEMSG_SHOWORDEREDINFO			103
#define INVOKEMSG_RECEIVEREALDATA			105
// ���� ���ؼ� �ǽð� ���� ǥ�� - ojtaso (20090210)
#define INVOKEMSG_RECALCGUIDELINE			107



#define DLLTITLENAME			"������Ʈ"

//#define ENVINFOFILE				"\\MultiChartInfo.dat"
#define ENVINFOORDERCHART		"\\MultiOrderChartInfo.dat"


//#define STOCK_ENVINFOFILE		"\\1570_MultiChartInfo.dat"
//#define FUTOPT_ENVINFOFILE		"\\2800_MultiChartInfo.dat"
//#define UPJONG_ENVINFOFILE		"\\1580_MultiChartInfo.dat"
//#define FOREIGN_ENVINFOFILE		"\\6500_MultiChartInfo.dat"

#define STOCK_ENVINFOFILE		"Stock_ChartInfo.dat"
#define UPJONG_ENVINFOFILE		"Upjong_ChartInfo.dat"
#define FUTOPT_ENVINFOFILE		"FutOpt_ChartInfo.dat"
#define FOREIGN_ENVINFOFILE		"Foreign_ChartInfo.dat"
#define ENVINFOFILE		"ChartInfo.dat"

//===========================================================================
// 2005. 10. 12
//===========================================================================
#define JIPYOSYNC_FILE			"JipyoSync.dat"
// vntsorl:[0001250] [5001] ������Ʈ - ���ֿ�����ƽ �ֱ⺰�� ��ǥ ���� �� �������� �ٸ��� �� �� �ֵ��� ������û
#define	JIPYORQ_FILE			"JipyoRQ.dat"		//20110502 �̹��� >>

#define TICK_JIPYOSYNC			"Tick_"
#define SEC_JIPYOSYNC			"Sec_"
#define MIN_JIPYOSYNC			"Min_"
#define DAY_JIPYOSYNC			"Day_"
#define WEEK_JIPYOSYNC			"Week_"
#define MONTH_JIPYOSYNC			"Month_"
#define YEAR_JIPYOSYNC			"Year_"

//===========================================================================


//#define STOCK_ENVINFOORDERCHART			"\\1570_MultiOrderChartInfo.dat"
//#define FUTOPT_ENVINFOORDERCHART		"\\2800_MultiOrderChartInfo.dat"
//#define UPJONG_ENVINFOORDERCHART		"\\1580_MultiOrderChartInfo.dat"
//#define FOREIGN_ENVINFOORDERCHART		"\\6500_MultiOrderChartInfo.dat"

#define STOCK_ENVINFOORDERCHART			"\\0301_MultiOrderChartInfo.dat"
#define FUTOPT_ENVINFOORDERCHART		"\\0303_MultiOrderChartInfo.dat"
#define UPJONG_ENVINFOORDERCHART		"\\0302_MultiOrderChartInfo.dat"
#define FOREIGN_ENVINFOORDERCHART		"\\0304_MultiOrderChartInfo.dat"

#define	NID_CODEINPUT				4578	//05.06.21 �ֽ�/���� ���� �ڵ� Ctrl Resource ID
#define	NID_ACCTINPUT				4579	//05.06.21 ���� ���� �ڵ� Ctrl Resource ID
#define	NID_UPJONGINPUT				4580	//05.06.21 ���� ���� �ڵ� Ctrl Resource ID
#define	NID_FOREIGNINPUT			4581	//05.06.21 �ؿ����� ���� �ڵ� Ctrl Resource ID
#define	NID_FUTOPTINPUT				4582	//05.06.22 �����ɼ�  ���� �ڵ� Ctrl Resource ID


#define TAB_CHART		"����"
#define TAB_ASSISTINDEX "��ǥ"
#define TAB_FAVORITEGRP "����"
#define TAB_CODESEARCH	"����"
#define TAB_ORDER		"�ֹ�"
#define TAB_MYFAVORITE	"�����"
#define TAB_SYSTEM		"����"		// 2005. 05. 16 add by sy, nam
//#define TAB_SYSTEMTRADING	"�ý���Ʈ���̵�" // 2005.10.25 gm0604
#define TAB_SYSTEMTRADING	"����" // 2005.10.25 gm0604

//#define TABNO_CHART				(0)
//#define TABNO_ASSISTINDEX		(1)
//#define TABNO_FAVORITEGRP		(2)
//#define TABNO_CODESEARCH		(3)
//#define TABNO_MYFAVORITE		(4)
//#define TABNO_ORDER				(5)
//#define TABNO_SYSTEMTRADING		(6)		// 2005. 10. 26 add by gm0604
//
//#define TABNO_SYSTEM			(7)		// 2005. 05. 16 add by sy, nam
//
//#ifdef _DEBUG
//#define STOCK_TABTOTALCNT		(7)		// 2005. 11. 1 gm0604
//#else
//#define STOCK_TABTOTALCNT		(7)		// 2005. 05. 16 add by sy, nam �ϴ� �ý������� ����.
////#define STOCK_TABTOTALCNT		(6)		// 2005. 05. 16 add by sy, nam �ϴ� �ý������� ����.
//#endif

#define TABNO_CODESEARCH		(0)
#define TABNO_ASSISTINDEX		(1)
#define TABNO_SYSTEMTRADING		(2)		// 2005. 10. 26 add by gm0604
#define TABNO_CHART				(3)		// 2006. 09. 19 remove by LYH
#define TABNO_FAVORITEGRP		(4)
#define TABNO_CODESEARCHUP		(5)
#define TABNO_CODESEARCHFO		(6)

#define TABNO_CODESEARCHFR		(8)		// �ؿ�
#define TABNO_CODESEARCHELW		(9)		// ELW

#define TABNO_CODESEARCHCMM		(10)	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)

#define TABNO_SYSTEM			(4)		// 2005. 05. 16 add by sy, nam
#define TABNO_MYFAVORITE		(4)

#ifdef _DEBUG
#define STOCK_TABTOTALCNT		(2)		// 2005. 11. 1 gm0604
#else
#define STOCK_TABTOTALCNT		(2)		// 2005. 05. 16 add by sy, nam �ϴ� �ý������� ����.
#endif

#define UPJONG_TABTOTALCNT		(3)		// 2005. 05. 16 add by sy, nam
#define FUTOPT_TABTOTALCNT		(3)		// 2005. 05. 16 add by sy, nam
//#define UPJONG_TABTOTALCNT		(4)		// 2005. 05. 16 add by sy, nam // 2005. 11. 14 gm0604
//#define FUTOPT_TABTOTALCNT		(5)		// 2005. 05. 16 add by sy, nam // 2005. 11. 14 gm0604

//#define DOWN_LIMIT				'3'		//���� 2005. 05. 17 add by sy, nam
//#define DOWN					'4'		//�϶� 2005. 05. 17 add by sy, nam
//#define EQUAL					'5'		//���� 2005. 05. 17 add by sy, nam
//#define UP						'6'		//��� 2005. 05. 17 add by sy, nam
//#define UP_LIMIT				'7'		//���� 2005. 05. 17 add by sy, nam

#define DOWN_LIMIT				'4'		//���� 2005. 05. 17 add by sy, nam
#define DOWN					'5'		//�϶� 2005. 05. 17 add by sy, nam
#define EQUAL					'3'		//���� 2005. 05. 17 add by sy, nam
#define UP_LIMIT				'1'		//���� 2005. 05. 17 add by sy, nam
#define UP						'2'		//��� 2005. 05. 17 add by sy, nam

#define CODE_STOCK_LENGTH		6
#define CODE_STOCK_LENGTH2		6
#define	ACCT_LENGTH				13
//#define UPJONG_LENGTH			3		// ��ȭ������ ���̰� 3
//#define UPJONG_LENGTH			4		// ���ο����� ���̰� 4
//#define UPJONG_LENGTH			5		// KOSCOM������ ���̰� 4
#define UPJONG_LENGTH			3		// Sygene������ ���̰� 3
#define FUTOPT_LENGTH			8
//#define	FOREIGN_LENGTH			4
#define	KQFUT_LENGTH			5		//�ڽ��� ���� ���� ���� 
#define	KQOPT_LENGTH			10		//�ڽ��� �ɼ� ���� ���� 
#define	KQOPT_LENGTH12			12
#define ELW_LENGTH			7		// ELW �Ｚ: ���̰� 7
#define COMMODITY_LENGTH			8	// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)


// �������� �߰� UI : �������� - ojtaso (20070522)
#define CHANGE_CHART			0		//��ȯ���		2005. 05. 19 add by sy, nam
#define ADD_NEW_CHART			1		//�߰����		2005. 05. 19 add by sy, nam
#define	ADD_NEW_ITEM			2		//�����߰�
#define REPLAY_CHART			2		//������		2005. 08. 01 add by sy, nam
#define OVERLAPP_CHART			3		//��ø���		2005. 05. 19 add by sy, nam

#define TICK_DATA_CHART			'0'		// Tick	��Ʈ	2005. 05. 19 add by sy, nam
#define MIN_DATA_CHART			'1'		// ��	��Ʈ	2005. 05. 19 add by sy, nam
#define DAY_DATA_CHART			'2'		// ��	��Ʈ	2005. 05. 19 add by sy, nam
#define WEEK_DATA_CHART			'3'		// ��	��Ʈ	2005. 05. 19 add by sy, nam
#define MONTH_DATA_CHART		'4'		// ��	��Ʈ	2005. 05. 19 add by sy, nam
#define YEAR_DATA_CHART			'5'		// ��	��Ʈ	2005. 05. 19 add by sy, nam
//#define SEC10_DATA_CHART		'9'		// �Ｚ�����������	// 10�� ��Ʈ	2005. 09. 28 add by sy, nam
//#define SEC30_DATA_CHART		'6'		// 30�� ��Ʈ	2005. 09. 28 add by sy, nam
#define SEC_DATA_CHART			'6'		// ��   ��Ʈ	2005. 09. 28 add by sy, nam
#define HOUR_DATA_CHART			'7'		// ��	��Ʈ	2006. 09. 27 �Ｚ��Ʈ

//#define DAY_DATA_CHART			'1'		// ��	��Ʈ	2005. 05. 19 add by sy, nam
//#define WEEK_DATA_CHART			'2'		// ��	��Ʈ	2005. 05. 19 add by sy, nam
//#define MONTH_DATA_CHART		'3'		// ��	��Ʈ	2005. 05. 19 add by sy, nam
//#define YEAR_DATA_CHART			'5'		// ��	��Ʈ	2005. 05. 19 add by sy, nam
//#define TICK_DATA_CHART			'6'		// Tick	��Ʈ	2005. 05. 19 add by sy, nam
//#define MIN_DATA_CHART			'7'		// ��	��Ʈ	2005. 05. 19 add by sy, nam
//#define SEC10_DATA_CHART		'8'		// 10�� ��Ʈ	2005. 09. 28 add by sy, nam
//#define SEC30_DATA_CHART		'9'		// 30�� ��Ʈ	2005. 09. 28 add by sy, nam

//==============================================================================
//2005. 06. 24 add by sy, nam
//���� ������..SetReal Price �Լ��� �б�� ������ ����
#define STOCK_REAL				0
#define UPJONG_REAL				1
#define FUTURE_REAL				2
#define OPTION_REAL				3
#define ELW_REAL				5
#define COMMODITY_REAL			6		// ��ǰ�����ɼ���Ʈ - ojtaso (20071001)
#define FO_REAL					7		// �����ɼ���ǥ - alzioyes(20110124)
#define SFUTURE_REAL			8		// �ֽļ����ǽð� - �̻��(20130618)
//==============================================================================



//1.����.2.�� 3.ǲ 4.�������� K. �ڽ��� ���� 
#define FUTURE_CODE				'1'		// �����ڵ�		2005. 06. 02 add by sy, nam
#define CALL_OPT_CODE			'2'		// �� �ɼ�		2005. 06. 02 add by sy, nam
#define PUT_OPT_CODE			'3'		// ǲ �ɼ�		2005. 06. 02 add by sy, nam
#define SPREAD_CODE				'4'		// ��������		2005. 06. 02 add by sy, nam
#define QFUTURE_CODE			'K'		// �ڽ��� ����  2005. 09. 22 add by sy, nam


// 05. 06. 22 add by sy, nam
// ��, %, $ ���� ���� 
#define WON_TYPE				0
#define PERCENT_TYPE			1
#define DOLLAR_TYPE				2
// --------------------------------------------------------------------------
//  Chart\COMMONTR\TRDefine.h���� �����ϵ��� ������.
// --------------------------------------------------------------------------
//#define QUERY_STRMARKET_10452	"HWS10452" //2005.10.20  ������ ���ż� ��
//#define QUERY_STR_FSTAR			"HWS10416" //2005.11.01	 ��Ÿ ����
// #define QUERY_STRJONGMOK		"d1010"
// #define QUERY_STRUPJONG			"d1011"
// #define QUERY_STRFUTOPT			"d1012"
// #define QUERY_STRSTOCKFUTOPT	"d1013"
// #define QUERY_STRFOREIGN		"d1014"
// #define QUERY_STRMARKET			"d1015"
// #define QUERY_STRDOLLAR			"d1018"
// #define QUERY_STOCK_HOGA		"c1002"
// #define QUERY_FUTOPT_HOGA		"f0201"
// #define QUERY_STOCKFUT_HOGA		"s3301"
// #define QUERY_STOCKOPT_HOGA		"f3201"
// #define QUERY_ACCOUNTCODE		"a4017"
// #define QUERY_ACCOUNTCONC		"a4018"
//#define QUERY_STRKQFUTOPT		"HWS10420" //2005.06.24  �ڽ��ڼ����ɼ� ��Ʈ����Ÿ ��ȸ
//#define QUERY_STRSTOCKOPT		"HWS10421" //2005.06.24  �ֽĿɼ� ��Ʈ����Ÿ ��ȸ
//#define QUERY_STRTHEME_GROUP	"HWS10410" //2005.06.23
//#define QUERY_STRTHEME_ITEM		"HWS10411" //2005.06.23
//#define QUERY_STRSTOCK_COMP		"HWS10430" //2005.07.12  �ֽ� ����Ʈ �������� ��ȸ
#define QUERY_STR_AVGMP			"HWS10418" //05.12.01	����� ���� �̵���� �� ��ȸ 

#define LINK_FUTURE_CODE		"101AA"		//07.01.22 ���ἱ�� �ڵ�(�Ｚ)

#define MEMCPYBYTARSIZE(_Tar,_Src) memcpy(_Tar,_Src,sizeof(_Tar));
#define MEMCPYBYTARSIZE2(_Tar,_Src,_lOffset) {memcpy(&_Tar,&_Src[_lOffset],sizeof(_Tar));_lOffset+=sizeof(_Tar);}
#define MEMCPYBYSRCSIZE(_Tar,_Src,_lOffset) {memcpy(&_Tar[_lOffset],(void*)&_Src,sizeof(_Src)); _lOffset+=sizeof(_Src);}
#define MEMCPYWITHNULL(szTarget,szSrc) {memcpy(szTarget,szSrc,sizeof(szSrc)); szTarget[sizeof(szSrc)] = NULL;}
#define SHIFTCONTROL(Wnd,nWidth,rect)	{Wnd.GetWindowRect(rect);ScreenToClient(rect);Wnd.SetWindowPos(&CWnd::wndTop,rect.left+nWidth,0,0,0,SWP_NOSIZE);Wnd.Invalidate();}

#define GAP_TABDLG			(4)
#define HEIGHTOFJONGSTATUS	(20)
#define WIDTHOFBUTTON		(40)
#define HEIGHTOFCOMBO		(20)
#define HEIGHTOFTOOLBARBUTTON	(19)
#define WIDTHOFTOOLBARBUTTON	(20)

//#define	CRLEFTBARBACK	RGB(221,224,233)
//#define	CRBACK			RGB(221,224,233)
#define	CRLEFTBARBACK	RGB(225,226,231)
//#define	CRBACK			RGB(239,242,247)
#define	CRBACK			RGB(222,228,234)
//#define CRGRID			RGB(239,243,246)
#define CRGRID			RGB(255,255,255)
#define CRSELECTED		RGB(180,200,240)
#define DEFAULT_BKGRND_COLOR		RGB(213,213,213)

//@�����߰�
//#define	CLR_SIDEBAR_BKGRND			RGB(177,199,234)
//#define	CLR_SPLIT_BKGRND			RGB(177,199,234)
//#define	CLR_LEFTBAR_BKGRND			RGB(177,199,234)
//#define	CLR_INPUTBAR_BKGRND			RGB(177,199,234)
//#define	CLR_EXTINPUTBAR_BKGRND		RGB(177,199,234)
//#define	CLR_TOOLBAR_BKGRND			RGB(177,199,234)
#define		CLR_FONT_DEFAULT			RGB(0,0,1)
//@�����߰�


#define DEFAULTCNTATONCE (400)
#define MINICNTATONCE	(120)
#define GRDMX			(3000)
#define CHTMX			(5200)
#define CHTMXREQUEST	(5000)
#define BOJO_MSG_LENGTH	(300)


#define GWANSIM_GRPNO_LEN	(2)		//2005. 07. 25
#define GWANSIM_MAX			(100)	//2005. 07. 25


#define ERRORMSG1	"�ش������� ���̻� �߰��Ҽ������ϴ�."
#define ERRORMSG2	"����������ǥ�� [��]/[��]/[��]������ ������ �����մϴ�."
#define ERRORMSG3	"����� ������ �����Ͻʽÿ�."
#define ERRORMSG4	"������ǥ�� [��]������ ������ �����մϴ�."
#define ERRORMSG5	"��ȸ�� �����ǰ� �ֽ��ϴ�. �ٽ� ��ȸ�Ͻñ� �ٶ��ϴ�."
#define ERRORMSG6	"��ȸ�� ������ �߻��Ǿ����ϴ�. �ٽ� ��ȸ�Ͻñ� �ٶ��ϴ�."
#define ERRORMSG7	"������ ��Ʈȭ���� �����ϴ�."

#define IMGLIST_15X15		0x0001
#define IMGLIST_20X20		0x0002
#define IMGLIST_20X20TOOL	0x0004
#define IMGLIST_46X20		0x0008
#define IMGLIST_50X20		0x0010
#define IMGLIST_28X22		0x0020
#define IMGLIST_17X20		0x0040
#define IMGLIST_43X20		0x0080
#define IMGLIST_58X20		0x0100
#define IMGLIST_46X20GWN	0x0200 
#define IMGLIST_8X25		0x0400 
#define IMGLIST_151X21		0x0800 
#define IMGLIST_72X20		0x1000 

typedef struct {
	CString m_szKey;		// ������
	CString m_szValue;		// ������
}STSHAREDDATA;

typedef struct {
	CString m_strCode;
	CString m_strTime;
//@�����߰�
	UINT	m_nDefault;
	UINT	m_nMode;
//@�����߰�
}STINPUTCODEDATA;

typedef struct {
	CString m_strCode;
	BOOL	m_bDontPutInShared;
	BOOL	m_bRequestWithoutHesitation;
}STINPUTCODEDATA2;

typedef struct {
	BYTE	m_bType;	//�ֽ�:0,����:1.����:2,�ɼ�:3
	CString m_strCode;
}ST_CODEDATA;

typedef CList<ST_CODEDATA, ST_CODEDATA>		LIST_CODEDATA;
//bDontPutInShared				�� �ڵ������ �־�� �ϴ��� �����̰�, 
//bRequestWithoutHesitation     ���� ��ȸ�ڵ��ϰ� ������ �����ڵ尡 ���� AP��ȸ�� �ȵǵ��� 
//								�Ǿ��ִµ�  �� TRUE�� �ϸ� �����Ͽ��ο� ������� AP��ȸ�϶�� ���

// Favorite Searcher Info Structure
typedef struct  {
	char m_chTitle[64];
	long m_lTabNum;
	long m_lSubData1;
	long m_lSubData2;
	long m_lSubData3;
	long m_lSubData4;
	long m_lSubData5;
	char m_chData1[64];
	char m_chData2[64];
	char m_chData3[64];
}stFvrData;

// Chart Basical Info Structure
typedef struct  {
	long m_lTotalCnt;
	long m_lZoomValue;
	long m_lStartDate;		//2005. 07. 25 add by sy, nam
	long m_lEndDate;
	char m_chType;			// ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5)
	CString m_strTick;		// 2Byte Tick Number
	CString m_strSec;		// 2Byte Sec Number
	CString m_strMin;		// 2Byte Min Number
	CString m_strDay;		// 2Byte Day Number
	CString m_strHour;		// 2Byte Day Number
	CString m_strCode;
	int  m_nChartNumericValue;
	BOOL m_bFackedCandle;
	int  m_nSelCol;			//2005. 05. 24 add by sy, nam  InputBar�� ��Ÿ ���� ���õ� ��Ʈ�� Index�� ��Ʈ ������ ���� Read
	int  m_nSelRow;			//2005. 05. 24 add by sy, nam  InputBar�� ��Ÿ ���� ���õ� ��Ʈ�� Index�� ��Ʈ ������ ���� Read

	BOOL m_bRevised;		//05. 08. 22 - �����ְ�
	BOOL m_bLog;			//05. 08. 22 - �α� 
	CString m_strChartLineType ;// 05. 08. 23 - ��Ʈ ��� ���ڿ�
	BOOL m_bStartDateState;	//05. 08. 25 - �������� ���õǾ� �ִ��� ����.
	
}stChartInitData;

// Server Input Structure
typedef struct  {
	CString		m_strTRCode;
	void*		m_pBuffer;
	long		m_lLength;
	CWnd*		m_pWnd;
	int			m_nType;		// 0: Normal 1:Invoke
	void*		m_pRqstChartItem;	//TR�� ��û�� ChartItem
}stSend2Server;




// Server Output Structure
typedef struct  {
	CString		m_strTRCode;
	void *		m_pBuffer;
	DWORD		m_dwLength;
}stReceiveFromServer;

typedef struct  {
	HWND		m_hChartMng;
	CWnd*		m_pChartItem;
	void *		m_pBuffer;	
	DWORD		m_dwLength;
}stMSGToInvoke;


// Chart Equity History Info Structure(In Server)
//typedef struct _S_33100_S
//{
//	char    date[8];			/* ��¥     		*/
//	char    open[10];			/* �ð�     		*/
//	char    high[10];			/* ��     		*/
//	char    low[10];			/* ����     		*/
//	char    close[10];			/* ����   			*/
//	char    volume[10];     	/* �ŷ���   		*/
//	char    value[12];     	/* �ŷ����    20351005 �������߰�   		*/
//	char	rights[1];			/* ����, �Ǹ���..	*/
//} S33100_S;


// Chart Equity History Info Structure (In Client)
//typedef struct _S33100_CHART
//{
//	char    tmp[5];         			/* Chart Count      */
//	char    bojomsg[BOJO_MSG_LENGTH];   /* Chart Bojo Data  */
//	S33100_S  chart[CHTMX];
//} S33100_CHART;

// Chart Equity  Info Structure (In Server's Output )
//typedef struct _S33100_O 
//{
//	char	name[20];			/* �����			*/
//	char	jang[10];			/* �屸�� : 'KOSPI', 'KOSDAQ', 'KOSPI200' */
//	char    price[10]; 		    /* ���簡   		*/
//	char    sign[1];        	/* �����ȣ 		*/
//	char    change[10];     	/* ���     		*/
//	char    chrate[6];      	/* �����   		*/
//	char    volume[10];     	/* �ŷ���   		*/
//	char    offer[10];      	/* �ŵ�ȣ�� 		*/
//	char    bid[10];        	/* �ż�ȣ�� 		*/
//	char    preprice[10];   	/* ���ذ�   		*/
//	char    nkey[14];           /* ����Ű           */
//	char    Cchtsize[ 6];		/* Chart Size       */
//	//------------- PosForChart
//	char    tmp[5];         	/* Chart Count      */
//	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  90->256���� �ٲ� 05.05.06*/
//	S33100_S  chart[CHTMX];
//} S33100_O;

//@��������
typedef struct _S_33100_S
{
	char    date[8];			/* ��¥     		*/
	char    open[10];			/* �ð�     		*/
	char    high[10];			/* ��     		*/
	char    low[10];			/* ����     		*/
	char    close[10];			/* ����   			*/
	char    volume[10];     	/* �ŷ���   		*/
	//char    value[12];     	/* �ŷ����    20351005 �������߰�   		*/
	char	rights[3];			/* ����, �Ǹ���..	*/
} S33100_S;

typedef struct _S33100_CHART
{
	char    tmp[5];         			/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];   /* Chart Bojo Data  */
	S33100_S  chart[CHTMX];
} S33100_CHART;

typedef struct _S33100_O 
{
	char	name[20];			/* �����			*/
	char    price[10]; 		    /* ���簡   		*/
//	char    open[10];			/* �ð�             */
//	char    high[10];			/* ��             */
//	char    low[10];			/* ����             */
	char    sign[1];        	/* �����ȣ 		*/
	char    change[10];     	/* ���     		*/
	char    chrate[6];      	/* �����   		*/
//	char    volume[10];     	/* �ŷ���   		*/
//	char    uplmtprice[10];		/* ���Ѱ�           */
//	char    dnlmtprice[10];		/* ���Ѱ�           */
	char    preprice[10];   	/* ���ذ�   		*/
	char    nkey[14];           /* ����Ű           */
	//------------- PosForChart
	char    tmp[5];         	/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  90->256���� �ٲ� 05.05.06*/
	S33100_S  chart[CHTMX];
} S33100_O;

//// Chart Equity History Request
//typedef struct _S33100_I
//{
//	char    shcode		[7];		/* �����ڵ�								*/
//	char	date		[8];		/* �˻� ������							*/
//	char	num			[4];		/* �˻��� ������ ����					*/
//	char	unit		[3];		/* ƽ/��/�� ��ȸ���ݴ���				*///<========2005.08.11
//	char    button		[1];		/* ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5)	*/
//	char    modGb		[1];		/* �����ְ� ���� 1:�����ְ��ݿ�			*/	
//	char	nflag		[1];		/* ����(1) / ��ȸ(0) ������ 			*/
//	char	nkey		[14];		/* ���� / ���� Fetch Key				*/
//	char	nDataGubun	[1];		/* '0': ����, '1':����					*/
//	char    ImgRgb		[1];        /* '0':����� '1':�Ǻ�					*/
//	char    Irecordgb	[1];		/* ���� ���а�(1)						*///<========2005.08.11
//	char    nDayBefore	[2];		/* ���� ���� n���� 						*///<========2005.10.14
//	char    Itime		[6];		/* ����ð� (hhmmss)					*///<========2005.08.11 	
//	/// don't sent to server..
//	char	chNull;
//} S33100_I;
//@��������
//@�����߰�
// �ֽ���ȸ

//typedef struct
//{
//	char	shcode		[15];	/* �����ڵ�			*/
//	char	date		[8];	/* �˻�������		*/
//	char	num			[4];	/* ��ȸ�Ǽ�			*/
//	char	unit		[3];	/* ��ȸ����(ƽ/��/��)								*/	
//	char	button		[1];	/* ��ȸ����(ƽ(0)/��(1)/��(2)/��(3)/��(4)/��(5))	*/
//	char	modGb		[1];	/* �����ְ�(1:�����ְ��ݿ�)							*/
//	char	nflag		[1];	/* ��ȸ���(����(1) / ��ȸ(0) ������)				*/
//	char	nkey		[21];	/* �˻�Ű(���� / ���� Fetch Key)					*/
//	char	nDataGubun	[1];	/* ���ݱ���('0': ����, '1':����)					*/
//	char	ImgRgb		[1];	/* ������('0':����� '1':�Ǻ�)						*/
//	char	Irecordgb	[1];	/* ���ⱸ�а�(1)	*/
//	char	nDayBefore	[2];	/* �������n����	*/
//	char	Itime		[6];	/* ����ð�(hhmmss) */
//	char	nOpt		[1];	/* ������ȸ���� */
//	// don't sent to server..
//	char	chNull;
//}p0602InBlock;
//
//typedef struct
//{
//	char	name		[20];	/* �����			*/
//	char	jang		[10];	/* �屸��			*/
//	char	price		[10];	/* ���簡			*/
//	char	sign		[1];	/* �����ȣ			*/
//	char	change		[10];	/* ���				*/
//	char	chrate		[6];	/* �����			*/
//	char	volume		[10];	/* �ŷ���			*/
//	char	offer		[10];	/* �ŵ�ȣ��			*/
//	char	bid			[10];	/* �ż�ȣ��			*/
//	char	preprice	[10];	/* ���ذ�			*/
//	char	nkey		[14];	/* ����Ű			*/
//	char	cntsz		[6];	/* ��Ʈ������		*/
//	char	fcnt		[5];	/* ��Ʈī��Ʈ		*/
//	char	bojomsg		[256];	/* �����޽���		*/
//}p0602OutBlock1;
//
//typedef struct
//{
//	char	date		[8];	/* ��¥				*/
//	char	open		[10];	/* �ð�				*/
//	char	high		[10];	/* ��				*/
//	char	low			[10];	/* ����				*/
//	char	close		[10];	/* ����				*/
//	char	volume		[10];	/* �ŷ���			*/
//	char	value		[12];	/* �ŷ����			*/
//	char	rights		[1];	/* �Ǹ���			*/
//}p0602OutBlock2;
//
//// ������ȸ
//typedef struct
//{
//	char		Sjanggb[1];		/*  [�屸��]�屸��     */
//	char		Supcode[3];		/*  [�����ڵ�]�����ڵ�     */
//	char		Sdate[8];		/*  [�˻�������]�˻�������     */
//	char		Smaxbong[4];	/*  [�˻��ҵ����Ͱ���]�˻��ҵ����Ͱ���     */
//	char		Sunit[3];		/*  [N��(ƽ�н���)]N��(ƽ�н���)     */
//	char		Sbtngb[1];		/*  [��ȸ����]��ȸ����     */
//	char		Snextgb[1];		/*  [����/��ȸ]����/��ȸ     */
//	char		Snextkey[21];	/*  [��ġŰ]��ġŰ     */
//	char		nOpt[1];		/* ������ȸ���� */
//	// don't sent to server..
//	char	chNull;
//}p0603InBlock;
//
//typedef struct
//{
//	char		name[20];		/*  [�����]�����     */
//	char		jang[10];		/*  [�屸��]�屸��     */
//	char		price[10];		/*  [���簡]���簡     */
//	char		sign[1];		/*  [�����ȣ]�����ȣ     */
//	char		change[10];		/*  [���]���     */
//	char		chrate[6];		/*  [�����]�����     */
//	char		volume[10];		/*  [�ŷ���]�ŷ���     */
//	char		offer[10];		/*  [�ŵ�ȣ��]�ŵ�ȣ��     */
//	char		bid[10];		/*  [�ż�ȣ��]�ż�ȣ��     */
//	char		preprice[10];	/*  [���ذ�]���ذ�     */
//	char		nkey[14];		/*  [����Ű]����Ű     */
//	char		cntsz[6];		/*  [��Ʈ������]��Ʈ������     */
//	char		fcnt[5];		/*  [��Ʈī��Ʈ]��Ʈī��Ʈ     */
//	char		bojomsg[256];	/*  [�����޽���]�����޽���     */
//}p0603OutBlock1;
//
//typedef struct
//{
//	char		date[8];		/*  [��¥]��¥     */
//	char		open[10];		/*  [�ð�]�ð�     */
//	char		high[10];		/*  [��]��     */
//	char		low[10];		/*  [����]����     */
//	char		close[10];		/*  [����]����     */
//	char		volume[10];		/*  [�ŷ���]�ŷ���     */
//	char		value[12];		/*  [�ŷ����]�ŷ����     */
//	char		rights[1];		/*  [�Ǹ���]�Ǹ���     */
//}p0603OutBlock2;
//
//// ������ȸ
//typedef struct
//{
//	char		Scode[15];		/*  [�����ڵ�]�����ڵ�     */
//	char		Sdate[8];		/*  [�˻�������]�˻�������     */
//	char		Smaxbong[4];		/*  [�˻��ҵ����Ͱ���]�˻��ҵ����Ͱ���     */
//	char		Sunit[3];		/*  [N��(ƽ�н���)]N��(ƽ�н���)     */
//	char		Sbtngb[1];		/*  [����(ƽ�����ֿ�)]����(ƽ�����ֿ�)     */
//	char		Snextgb[1];		/*  [��ȸ����(������ȸ)]��ȸ����(������ȸ)     */
//	char		Snextkey[21];		/*  [��ġŰ]��ġŰ     */
//	char		Simgrgb[1];		/*  [������(����Ǻ�)]������(����Ǻ�)     */
//	char		Srecordgb[1];		/*  [���ⱸ�а�]���ⱸ�а�     */
//	char		Sredate[2];		/*  [�������n����]�������n����     */
//	char		Stime[6];		/*  [����ð�(hhmmss)]����ð�(hhmmss)     */
//	char		nOpt[1];		/* ������ȸ���� */
//	// don't sent to server..
//	char	chNull;
//}p0604InBlock;
//
//typedef struct
//{
//	char		name[20];		/*  [�����]�����     */
//	char		jang[10];		/*  [�屸��]�屸��     */
//	char		price[10];		/*  [���簡]���簡     */
//	char		sign[1];		/*  [�����ȣ]�����ȣ     */
//	char		change[10];		/*  [���]���     */
//	char		chrate[6];		/*  [�����]�����     */
//	char		volume[10];		/*  [�ŷ���]�ŷ���     */
//	char		offer[10];		/*  [�ŵ�ȣ��]�ŵ�ȣ��     */
//	char		bid[10];		/*  [�ż�ȣ��]�ż�ȣ��     */
//	char		preprice[10];	/*  [���ذ�]���ذ�     */
//	char		nkey[14];		/*  [����Ű]����Ű     */
//	char		cntsz[6];		/*  [��Ʈ������]��Ʈ������     */
//	char		fcnt[5];		/*  [��Ʈī��Ʈ]��Ʈī��Ʈ     */
//	char		bojomsg[256];	/*  [�����޽���]�����޽���     */
//}p0604OutBlock1;
//
//typedef struct
//{
//	char		date[8];		/*  [��¥]��¥     */
//	char		open[10];		/*  [�ð�]�ð�     */
//	char		high[10];		/*  [��]��     */
//	char		low[10];		/*  [����]����     */
//	char		close[10];		/*  [����]����     */
//	char		volume[10];		/*  [�ŷ���]�ŷ���     */
//	char		value[12];		/*  [�ŷ����]�ŷ����     */
//	char		rights[1];		/*  [�Ǹ���]�Ǹ���     */
//}p0604OutBlock2;
//
//// ELW��ȸ
//typedef struct
//{
//	char		Scode[15];		/*  [�����ڵ�]�����ڵ�     */
//	char		Sdate[8];		/*  [�˻�������]�˻�������     */
//	char		Smaxbong[4];		/*  [��ȸ�Ǽ�]��ȸ�Ǽ�     */
//	char		Sunit[3];		/*  [N��(ƽ�н���)]N��(ƽ�н���)     */
//	char		Sbtngb[1];		/*  [����(ƽ�����ֿ�)]����(ƽ�����ֿ�)     */
//	char		Snextgb[1];		/*  [��ȸ����(������ȸ)]��ȸ����(������ȸ)     */
//	char		Snextkey[21];		/*  [��ġŰ]��ġŰ     */
//	char		Simggb[1];		/*  [������(����Ǽ�)]������(����Ǽ�)     */
//	char		nOpt[1];		/* ������ȸ���� */
//	// don't sent to server..
//	char	chNull;
//}p0605InBlock;
//
//typedef struct
//{
//	char		name[20];		/*  [�����]�����     */
//	char		jang[10];		/*  [�屸��]�屸��     */
//	char		price[10];		/*  [���簡]���簡     */
//	char		sign[1];		/*  [�����ȣ]�����ȣ     */
//	char		change[10];		/*  [���]���     */
//	char		chrate[6];		/*  [�����]�����     */
//	char		volume[10];		/*  [�ŷ���]�ŷ���     */
//	char		offer[10];		/*  [�ŵ�ȣ��]�ŵ�ȣ��     */
//	char		bid[10];		/*  [�ż�ȣ��]�ż�ȣ��     */
//	char		preprice[10];	/*  [���ذ�]���ذ�     */
//	char		nkey[14];		/*  [����Ű]����Ű     */
//	char		cntsz[6];		/*  [��Ʈ������]��Ʈ������     */
//	char		fcnt[5];		/*  [��Ʈī��Ʈ]��Ʈī��Ʈ     */
//	char		bojomsg[256];	/*  [�����޽���]�����޽���     */
//}p0605OutBlock1;
//
//typedef struct
//{
//	char		date[8];		/*  [��¥]��¥     */
//	char		open[10];		/*  [�ð�]�ð�     */
//	char		high[10];		/*  [��]��     */
//	char		low[10];		/*  [����]����     */
//	char		close[10];		/*  [����]����     */
//	char		volume[10];		/*  [�ŷ���]�ŷ���     */
//	char		value[12];		/*  [�ŷ����]�ŷ����     */
//	char		rights[1];		/*  [�Ǹ���]�Ǹ���     */
//}p0605OutBlock2;

//@�����߰�

// Chart Futures History Request
typedef struct _S36100_I
{
	char    jmcode		[15];			/* �����ڵ�							*/
	char	date		[8];			/* �˻� ������						*/
	char	num			[4];			/* �˻��� ������ ����				*/
	char    unit		[3];			/* �Ⱓ  Number						*///<========2005.08.11
	char    button		[1];			/* ƽ(0)/30��(1)/��(2)/��(3)/��(4)	*/
	char	nflag		[1];			/* ����(1) / ��ȸ(0) ������ 		*/
	char	nkey		[14];			/* ���� / ���� Fetch Key			*/
	char    ImgRgb		[1];			/* '0':����� '1':�Ǻ�              */
	char    Irecordgb	[1];			/* ���� ���а�(1)					*///<========2005.08.11
	char    nDayBefore	[2];			/* ���� ���� n���� 					*///<========2005.10.14
	char    Itime		[6];			/* ����ð� (hhmmss)				*///<========2005.08.11  	

	/// don't sent to server..
	char	chNull;
} S36100_I;

// Chart Futures Info Structure (In Server's Output )
typedef struct _S_36100_S
{
	char    date[8];			/* ��¥     		*/
	char    open[10];			/* �ð�     		*/
	char    high[10];			/* ��     		*/
	char    low[10];			/* ����     		*/
	char    close[10];			/* ����   			*/
	char    volume[10];     	/* �ŷ���   		*/
	//char    value[12];     		/* �ŷ����   		*/
} S36100_S;

// Chart Future History Info Structure (In Client)
typedef struct _S36100_CHART
{
	char    tmp[5];         	/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  */
	S36100_S  chart[CHTMX];
} S36100_CHART;

// Chart Futures Info Structure (In Server's Output )
typedef struct _S36100_O 
{
	char	name[20];			/* �����			*/
	char    price[10]; 		    /* ���簡   		*/
//	char    open[10];			/* �ð�             */
//	char    high[10];			/* ��             */
//	char    low[10];			/* ����             */    
	char    sign[1];        	/* �����ȣ 		*/
	char    change[10];     	/* ���     		*/
	char    chrate[6];      	/* �����   		*/
//	char    volume[10];     	/* �ŷ���   		*/
//	char    uplmtprice[10];		/* ���Ѱ�           */
//	char    dnlmtprice[10];		/* ���Ѱ�           */
	char    preprice[10];   	/* ���ذ�   		*/
	//char	medoho[10];			/* �ŵ�ȣ�� 		*/
	//char	mesuho[10];			/* �ż�ȣ�� 		*/

   char    nkey    [ 14];   /* ����Ű           */
	//char    Cchtsize[ 6];		/* Chart Size       */

	//------------- PosForChart
	char    tmp[5];         	/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  */

	S36100_S  chart[CHTMX];

} S36100_O;

typedef struct _S35100_I
{
	char    radio		[1];			// �屸�� '1'-KOSPI, '2'-KOSDAQ, '3'-KOSPI200
	//char    upcode		[3];			// �����ڵ�             
	char    upcode		[4];			// �����ڵ�             //KOSCOM 4�ڸ�
	char    date		[8];			// �˻� ������          
	char    num			[4];			// �˻��� ������ ����   
	char    unit		[3];			/* �Ⱓ  Number			*///<============2005.08.11
	char    button		[1];			// 30��(0)/��(1)/��(2)/��(3)/��(4)
	char    nflag		[1];			// ����(1) / ��ȸ(0) ������     
	char    nkey		[14];           // ���� / ���� Fetch Key        
	//char	nDataGubun	[1];			/* 0: ����, 1:���� */

	/// don't sent to server..
	char	chNull;
} S35100_I;

typedef struct _S_35100_S
{
	char    date[8];            // ��¥             
	char    open[10];           // �ð�             
	char    high[10];           // ��             
	char    low[10];            // ����             
	char    close[10];          // ����             
	char    volume[10];         // �ŷ���           
	//char    value[12];			// �ŷ����    20351005 �������߰�          
} S35100_S;

typedef struct _S35100_CHART
{
	char    tmp[5];         	/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  */
	S35100_S  chart[CHTMX];
} S35100_CHART;

typedef struct _S35100_O
{
	char    name[20];           // �����           
	char    price[10];          // ���簡           
//	char    open[10];			/* �ð�             */
//	char    high[10];			/* ��             */
//	char    low[10];			/* ����             */    
	char    sign[1];            // �����ȣ         
	char    change[10];         // ���             
	char    chrate[6];          // �����           
//	char    volume[10];         // �ŷ���           
	char    preprice[10];       // ���ذ�           

	char    nkey[14];           // ����Ű 
 	//char    Cchtsize[ 6];		/* Chart Size       */
	
	//==========================================================
	char    tmp[5];             // Chart Count      
	char    bojomsg[BOJO_MSG_LENGTH];        // Chart Bojo Data  

	S35100_S  chart[CHTMX];
} S35100_O;	

//<<<<<<<<<<<<  �׸�����Ʈ�˻� : 10410 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct _D10410_I
{
    char    dtGb        [ 1];   /* 0:��ü 1:�˻� 2:���� */
    char    jmcode      [ 6];   /* �����ڵ�             */
    char    text        [50];   /* �˻���               */
} D10410_I;

typedef struct _D10410_OG
{
	char    grptmcd     [ 3];   /* �׸��ڵ� */
	char    grptmnm     [20];   /* �׸���   */	

    //char    grptmnm     [30];   /* �׸���   */
	//char    grptmcd     [ 4];   /* �׸��ڵ� */
} D10410_OG;

typedef struct _D10410_O
{
    char        grdCnt      [ 4];
   // char        grdBjMsg    [18];
    D10410_OG   grdData     [GRDMX];
} D10410_O;





//<<<<<<<<<<<<  �׸�����˻� : 10411 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct _D10411_I
{
    char    grptmcd      [ 3];  /* �׸��ڵ� */
    char    filtercode  [ 1];   /* 1:��ü, 2:����, 3:�켱, 4:����, 5:����    default: '1' �� ����*/
    char    limitvol    [12];   /* �ּ� �ŷ���(����: õ��)                   default: "0           " ���� ����*/
} D10411_I;

typedef struct _D10411_OG
{
    char    sign        [ 1];
    char    hname       [20];
    char    jmcode      [ 6];
    char    price       [ 9];
    char    change      [ 9];
    char    chgrate     [ 9];
    char    volume      [10];
    char    showgb      [ 1];   /* �����߰�-����϶��� ���� */
    char    showcnt     [ 3];   /* �����߰�-����϶��� ���� */
    char    lmtgb       [ 1];   /* �����ѱ���   */
    char    lmtcnt      [ 4];   /* �����Ѱ���   */
    char    updngb      [ 1];   /* ����϶����� */
    char    updncnt     [ 4];   /* ����϶����� */
    char    statgb      [ 1];   /* �屸��       */
} D10411_OG;

typedef struct _D10411_O
{
    char        grptmnm     [20]; //Han Hwa 30 -> 20

    char        sang        [ 3];
    char        up          [ 3];
    char        bo          [ 3];
    char        dn          [ 3];
    char        ha          [ 3];
    char        tot         [ 5];

    char        uprate      [ 6];
    char        rate        [ 6];
    char        uprate_sign [ 1];
    //char        grdCnt    [ 6]; //Han Hwa
    //char        grdBjMsg   [18];//Han Hwa
    D10411_OG   grdData     [GRDMX]; //�������� �������� Output�� grdData�� ������ 
	                                 //Client���� �۾��ϱ� ���ϰ� �ϱ� ���� ���δ�.  
} D10411_O;

//<<<<  ���嵿��, �����ڵ���, ��ǥ��ȣ ����Ʈ TR : 20010, 20020, 20030 >>>>

//=============================================== ��ǲ ==============
typedef struct _D20010_I
{
   char    dtGb        [ 1];   /* 0:��ü 1:�˻� 2:���� */
   char    Gb          [ 1];   /* 1:�ü� 2:������ 3:��ǥ��ȣ */
   char    jmcode      [ 6];   /* �����ڵ�             */
   char    text        [50];   /* �˻���               */
} D20010_I;

//=============================================== �ƿ�ǲ�ݺ� =======
typedef struct _D20010_OG
{
    char    grptmnm     [30];   /* �׸���     */
    char    grptmcd     [ 4];   /* �׸��ε��� */
} D20010_OG;

//=============================================== �ƿ�ǲ ============
typedef struct _D20010_O
{
    char    dtGb        [ 1];		/* 0:��ü 1:�˻� 2:���� */
    char    Gb          [ 1];		/* 1:�ü� 2:������ 3:��ǥ��ȣ 4:��õ���� 5:��������*/

    char        grdCnt      [ 6];           /* �׸��� ī��Ʈ(4) + �����޼��� ī��Ʈ(2) */
    char        grdBjMsg    [18];           /* �����޼��� */
    D20010_OG   grdData     [GRDMX];        /* �ݺ�����Ÿ (D20010_OG * �׸��� ī��Ʈ) */
} D20010_O;

//<<<<  ���嵿��, �����ڵ���, ��ǥ��ȣ ���� ����Ÿ TR : 20011, 20021, 20031 >>>>
    
//=============================================== ��ǲ ==============
typedef struct _D20011_I
{
    char    grptmcd      [ 4];  	/* �׸��ε��� */
    char    filtercode  [ 1];   /* 1:��ü, 2:����, 3:�켱, 4:����, 5:����   default: '1' �� ���� */
    char    limitvol    [12];   /* �ּ� �ŷ���(����: õ��)                  default: "0           " ���� ���� */
} D20011_I;
//=============================================== �ƿ�ǲ�ݺ� =======
typedef struct _D20011_OG
{
  //  char    grptmcd	[ 4];		/* <= �߰��κ�  */
    char    sign        [ 1];  /* �����ȣ */
    char    hname       [20];  /* �����   */
    char    jmcode      [ 6];  /* �����ڵ� */
    char    price       [ 9];  /* ���簡   */
    char    change      [ 9];  /* ���ϴ�� */
    char    chgrate     [ 9];  /* �����   */
    char    volume      [10];  /* �ŷ���   */
    char    showgb      [ 1];   /* �����߰�-����϶��� ���� */
    char    showcnt     [ 3];   /* �����߰�-����϶��� ���� */
    char    lmtgb       [ 1];   /* �����ѱ���   */
    char    lmtcnt      [ 4];   /* �����Ѱ���   */
    char    updngb      [ 1];   /* ����϶����� */
    char    updncnt     [ 4];   /* ����϶����� */
    char    statgb      [ 1];   /* �屸��       */
} D20011_OG;
//=============================================== �ƿ�ǲ ============
typedef struct _D20011_O
{
    char        grptmcd      [ 4];		/* <= �߰��κ�  */

	char        grptmnm     [30];  	/* �׸��� */    
	char        sang        [ 3];  	/* ��������� */
	char        up          [ 3];  	/* �������� */
	char        bo          [ 3];  	/* ��������� */
	char        dn          [ 3];  	/* �϶������ */
	char        ha          [ 3];  	/* ��������� */
	char        tot         [ 5];  	/* ��ü����� */   
	char        uprate      [ 6];  	/* ��º���   */
	char        rate        [ 6];  	/* ��մ���� */ 
	char        uprate_sign [ 1];	/* ��º��� ��ȣ */

	char        grdCnt      [ 6];   /* �׸��� ī��Ʈ(4) + �����޼��� ī��Ʈ(2) */
	char        grdBjMsg    [18];   /* �����޼��� */                             
    D20011_OG   grdData     [GRDMX];/* �ݺ�����Ÿ (D20011_OG * �׸��� ī��Ʈ) */ 
} D20011_O;





/* ���� ��ȸ ���� OUTPUT */
typedef struct
{
	char    winid           [ 4];   /* Window ID (Input���� �״�� ������.  */
	char    cnt             [ 4];
} TR_10310_O;
#define TR_10310_O_SZ   sizeof(TR_10310_O)

typedef struct
{
	char    under           [ 1];   /* ���������� :  0:����  1:����  2:���� */
	char    jg_tp           [ 1];   /* ����                                 */
	char    jg_code         [15];   /* �����ڵ�                             */
	char    jg_name         [30];   /* �����                               */
	char    price           [ 9];   /* ���簡                               */
	char    change          [ 6];   /* ���ϴ��                             */
	char    change_rate     [ 5];   /* �����                               */
	char    sign            [ 1];   /* �����ȣ                             */
	char    volume          [12];   /* �����ŷ���                           */
	char    offerho         [ 9];   /* �ŵ�ȣ��                             */
	char    bidho           [ 9];   /* �ż�ȣ��                             */
	char    ofremain        [12];   /* �ŵ��ܷ�                             */
	char    bidremain       [12];   /* �ż��ܷ�                             */
	char    offertot        [12];   /* �Ѹŵ��ܷ�                           */
	char    bidtot          [12];   /* �Ѹż��ܷ�                           */
	char    openam          [ 9];   /* �ð�                                 */
	char    high            [ 9];   /* ��                                 */
	char    low             [ 9];   /* ����                                 */
	char    value           [14];   /* �ŷ��ݾ�                             */
	char    uplmtprice      [ 9];   /* ���Ѱ�                               */
	char    dnlmtprice      [ 9];   /* ���Ѱ�                               */
	char    openyak         [ 9];   /* �̰�����������                       */
	char    parvalue        [ 9];   /* �׸鰡                               */
	char    presign         [ 1];   /* ���ϵ����ȣ                         */
	char    updnlmtcont_tp  [ 1];   /* ���ӱ���                             */
	char    updnlmtcont     [ 2];   /* ���ӱ���                             */
    char    updnlmtcont_tp2 [ 1];   /* ���ӻ���/���ѱ���                    �ű� */
    char    updnlmtcont2    [ 2];   /* ���ӻ���/���ѱ���                    �ű� */
	char    baseprice        [ 9];   /* ���ذ�                               */
	char    cvolume         [ 9];   /* ü�ᷮ                               */
	char    open_chrate     [ 5];   /* �ð����ϴ����                       */
	char    high_chrate     [ 5];   /* �����ϴ����                       */
	char    low_chrate      [ 5];   /* �������ϴ����                       */
	char    hightime        [ 6];   /* ���ð�                     */
	char    lowtime         [ 6];   /* �����ð�                     */
    // ����ü�� �߰� - cst 2004/09/10
    char    expsign         [ 1];   /* ����ü�� ��ȣ                        */
    char    expprice        [ 9];   /* ����ü�� ��                          */
    char    expchang        [ 9];   /* ����ü�� ���                        */
    char    expchgrt        [ 9];   /* ����ü�� �����                      */
    char    expvol          [12];   /* ����ü�� �ŷ���                      */

} TR_10310_OS;

typedef struct
{
	char    winid           [ 4];   /* Window ID                            */
	char    jgcnt           [ 4];   /* ���񰳼�                             */
} TR_10310_I;

typedef struct
{
	char    jg_tp           [ 1];   /* ����                                 */
									/* '1':�峻, '2':�ڽ���,  '3':����      */
									/* '4':�ɼ�, '5':��3����, '6':�ֽĿɼ�  */
	char    jg_code         [15];   /* �����ڵ�                             */
} TR_10310_IS;



typedef struct {
	char szBuffer[12]; 
	char shcode[7];//* �����ڵ�
	char nAvg1[3]; //* ����1 
	char nAvg2[3]; //* ����2 
	char nAvg3[3]; //* ����3 
	char nAvg4[3]; //* ����4 
	/// don't sent to server..
	char	chNull;
} D10418_I; 


typedef struct _D10418_O { 
	char sBuffer[12]; // Use Only Client 
	char sShcode[7]; // Short Code 
	char sName[20]; // Code Name 
	char sCurPrice[16]; // Current Price 
	char sSum1[16]; // ���� ��� 5�� �̶� �׷��� ������ ������ ���� 4���� �� 
	char sSum2[16]; // ���� ��� 20�� �̶� �׷��� ������ ������ ���� 19���� �� 
	char sSum3[16]; // ���� ��� 60�� �̶� �׷��� ������ ������ ���� 59���� �� 
	char sSum4[16]; // ���� ��� 120�� �̶� �׷��� ������ ������ ���� 119���� �� 
	/// don't sent to server..
	char	chNull;
} D10418_O; 




//-->Input
typedef struct  {
    char    sBuffer[12];	// Use Only Client 
    char    b_n01[11];      //���¹�ȣ
    char    b_n02[ 8];      //����(�Ÿ�����)
    char    b_n03[12];      //�����ڵ�(�⺻��(��ü)-'%')
    char    b_n04[40];      //������ȸ(�⺻��-'       ')
    char    b_n05[ 1];      //��ȸ����( 0 : ����  1: ���� 2: ������ 9:Ư���� )
}DK_6102_MQ_IN;


//-->Output 
typedef struct  {

    char    b_n17[12];      //�����ȣ
    char    b_n18[40];      //�����
    char    b_n19[10];      //�ŵ�������
    char    b_n20[ 9];      //����
    char    b_n21[ 9];      //�ܰ�
    char    b_n22[15];      //�����ݾ�
    char    b_n23[15];      //������+������
    char    b_n24[15];      //����ݾ�
    char    b_n25[20];      //�ſ뱸�и�
    char    b_n26[ 8];      //������
    char    b_n27[15];      //�ſ�ݾ�
    char    b_n28[ 9];      //����
    char    b_n29[15];      //��ü��
    char    b_n30[ 2];      //���屸�� (01:���� , 02:OTCBB , 03:�ڽ���, 04:�ŷ���, 05:�ڽ���(ECN) , 06:�ŷ���(ECN))
    char    b_n31[40];      //������ȸ
}DK_6102_MQ_LOOP;


typedef struct  {
    char    sBuffer[12]; // Use Only Client 
    char    b_n01[15];      //�ŵ�(�����ݾ�)��
    char    b_n02[15];      //�ŵ�(������+������)��
    char    b_n03[15];      //�ŵ�(����ݾ�)��
    char    b_n04[15];      //�ŵ�(�ſ�,����ݾ�)��
    char    b_n05[15];      //�ŵ�(����)��
    char    b_n06[15];      //�ŵ�(��ü��)��
    char    b_n07[15];      //�ŵ�(�����ݾ�)��
    char    b_n08[15];      //�ż�(�����ݾ�)��
    char    b_n09[15];      //�ż�(������+������)��
    char    b_n10[15];      //�ż�(����ݾ�)��
    char    b_n11[15];      //�ż�(�ſ�,����ݾ�)��
    char    b_n12[15];      //�ż�(����)��
    char    b_n13[15];      //�ż�(��ü��)��
    char    b_n14[15];      //�ż�(�����ݾ�)��
    char    b_n15[ 8];      //�Ÿ�����
    char    b_n16[ 8];      //��������

    DK_6102_MQ_LOOP occur[20];
}DK_6102_MQ_OUT;


//=============================================================================
// 2005. 07. 06 �߰� 
// �ؿ� ���� ��Ʈ �����͸� ��ȸ�ϱ� ���� ���� 
//=============================================================================
typedef struct _D03040_I
{
	char    codegb	[ 1];	/* ���񱸺�							*/
	char    jmcode	[14];	/* �����ڵ�							*/
	char	date	[ 8];	/* �˻� ������						*/
	char	num		[ 4];	/* �˻��� ������ ����				*/
	char    unit    [ 3];	/* min Number						*///<==========2005. 08.11 
	char    button	[ 1];	/* ��(0)/��(1)/��(2)/��(3)/��(4)	*/
	char	nflag	[ 1];	/* ����(1) / ��ȸ(0) ������			*/
	char	nkey	[14];	/* ���� / ���� Fetch Key			*///<==========2005. 08.11 
} D03040_I;


//typedef struct _D03040_OG
//{
//	char    date	[ 8];	/* ��¥     		*/
//	char    open	[10];	/* �ð�     		*/
//	char    high	[10];	/* ��     		*/
//	char    low		[10];	/* ����     		*/
//	char    close	[10];	/* ����   		*/
//	char    volume	[10];  	/* �ŷ���   		*/
//	//char    value	[11];  	/* �ŷ����   		*/
//} D03040_OG;
//
//typedef struct _S03040_CHART
//{
//	char    chart_cnt	[5];         /* Chart Count      */
//	char    bojomsg		[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  */
//	D03040_OG  chart	[CHTMX];
//} S03040_CHART;
//
//typedef struct _D03040_O
//{
//	char	name	[40];	/* �����		*/
//	char    price	[10];   /* ���簡   		*/
//	char    sign	[ 1];  	/* �����ȣ 		*/
//	char    change	[10];  	/* ���     		*/
//	char    chrate	[ 6];  	/* �����   		*/
//	char    volume	[10];  	/* �ŷ���   		*/
//	char	medoho	[10];	/* �ŵ�ȣ�� 		*/
//	char	mesuho	[10];	/* �ż�ȣ�� 		*/
//	char    nkey	[14];   /* ����Ű           */
// 	char    Cchtsize[ 6];		/* Chart Size       */
//	//=========================================================================
//	char    chart_cnt[ 5];   /* Chart Count      */
//	char    bojomsg	 [BOJO_MSG_LENGTH];   /* Chart Bojo Data  */
//	D03040_OG chart[CHTMX];
//	
//} D03040_O;

#define D03040_S   S36100_S
#define D03040_O   S36100_O
#define S03040_CHART   S36100_CHART
////=============================================================================

typedef struct 
{
	bool bRealCandle	;	// �Ǻ� or �Ǻ� + ���
	bool bShowHLBar		;	// �����ѹ� ǥ��
	bool bShowRight		;	// �ϸ��� / ���� ǥ��
	bool bRecvTimeShare	;	// �ð����� ����
	bool bAutoSave		;	// ��Ʈ ����� �ڵ����� ����
	bool bShowLog		;	// ��Ʈ �α�ǥ�� ����
	bool bRevisedValue	;	// �����ְ� ǥ�� ����
	bool bStartDate		;	// ��Ʈ�� ���õǰ� �ִ� ��¥�� ��� ���ٿ��� ��ȸ �������� �����ߴ��� ����.
	//bool bShowCurPrice	;	// ��Ʈ Y �� �������� ���簡 ǥ�� 

} CHART_CONFIG_INFO;

//-----------------------------------------------------------------------------
// Author		:				Date :2006.9.20
// Reason		: ELW
//-----------------------------------------------------------------------------
typedef struct _S01007_I 
{
	char    shcode  [15];       /* �����ڵ�                             */
	char    date	[ 8];       /* �˻� ������                          */
	char    num		[ 4];       /* �˻��� ������ ����                   */
	char    unit    [ 3];       /* Tick, Sec(10s, 30s), Min, Day Number */
	char    button	[ 1];       /* ƽ(0)/��(1)/��(2)/��(3)/��(4)        */
	char    nflag	[ 1];       /* ����(1) / ��ȸ(0) ������             */
	char    nkey	[14];       /* ���� / ���� Fetch Key                */
} S01007_I;
#define S01007_I_SIZE    sizeof(S01007_I)

typedef struct _S01007_S
{
	char    date[8];			/* ��¥     		*/
	char    open[10];			/* �ð�     		*/
	char    high[10];			/* ��     		*/
	char    low[10];			/* ����     		*/
	char    close[10];			/* ����   			*/
	char    volume[10];     	/* �ŷ���   		*/
} S01007_S;

typedef struct _S01007_CHART
{
	char    tmp[5];         			/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];   /* Chart Bojo Data  */
	S01007_S  chart[CHTMX];
} S01007_CHART;

typedef struct _S01007_O 
{
	char	name[20];			/* �����			*/
	char    price[10]; 		    /* ���簡   		*/
	char    open[10];			/* �ð�             */
	char    high[10];			/* ��             */
	char    low[10];			/* ����             */
	char    sign[1];        	/* �����ȣ 		*/
	char    change[10];     	/* ���     		*/
	char    chrate[6];      	/* �����   		*/
	char    volume[10];     	/* �ŷ���   		*/
	char    uplmtprice[10];		/* ���Ѱ�           */
	char    dnlmtprice[10];		/* ���Ѱ�           */
	char    preprice[10];   	/* ���ذ�   		*/
	char    nkey[14];           /* ����Ű           */
	//------------- PosForChart
	char    tmp[5];         	/* Chart Count      */
	char    bojomsg[BOJO_MSG_LENGTH];    	/* Chart Bojo Data  90->256���� �ٲ� 05.05.06*/
	S01007_S  chart[CHTMX];
} S01007_O;




//#define	STOCK_CHART		0
//#define FUTOPT_CHART	1
//#define UPJONG_CHART	2

// �������� ��Ʈ ���� : �������� - ojtaso (20070122)
typedef struct _ST_SELCHART {
	//	CString			m_strRQ;				// ChartItem�� RQ (MultiChart�� ������� ����)
	CString			m_strChartGubun;		// ���屸��
	CString			m_strItemCode;			// �����ڵ�
	CString			m_strItemName;			// �����
	char			m_cType;				// �ֱ�
	CString			m_strTickMinDayNumber;	// ƽ ī��Ʈ	
	long			m_lCount;				// ������ ī��Ʈ	
	int				m_nChartNumericValueType;	// ��Ʈ�� ��, %, $�� ���а��� : �������� - ojtaso (20070411)
	CString			m_strKey;
} ST_SELCHART;

// ��ǰ�����ɼ� 2�� - ojtaso (20071016)
/*
typedef struct _codeinfo{
	int	nInfo[5];

	struct _codeinfo()
	{
		for (int i=0; i < 5; i++)
			nInfo[i] = 0;
	}

}codeinfo;
*/
// �� �� ǲ �ɼ� �� ����Ʈ 
class COptionItem
{
public: 
	COptionItem(){};
	virtual ~COptionItem(){};

	//DATA
	CString m_strCode;
	int m_nCallPut;	// 2:call, 3:put
	int m_nYear;	// �����
	int m_nMonth;	// ����� 
	int m_nEvtPrice;

	//FUNCTION
	void SetItem(CString strCode, int nCallPut, int nYear, int nMonth)
	{
		m_strCode = strCode;
		m_nCallPut = nCallPut;
		m_nYear = nYear;
		m_nMonth = nMonth;
		m_nEvtPrice = 0;
	};
	void SetItem(CString strCode, int nCallPut, int nYear, int nMonth, int nEvtPrice)
	{
		m_strCode = strCode;
		m_nCallPut = nCallPut;
		m_nYear = nYear;
		m_nMonth = nMonth;
		m_nEvtPrice = nEvtPrice;
	};
};

#endif //__CHART_MSG_DEF__BYGM0604