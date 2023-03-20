

/*
(1) ����� ���� �������̽��� �߰�
// {{ get/setAll Interface implementation
	// made by Choi Jin Won ( 2003. 02. 21 )
	class CAllPropGetterSetter : public IBaseAllGetterSetter
	{
	public:
		virtual void getAllCtrlProperty(CStringList* plist);
		virtual BOOL setAllCtrlProperty(CStringList* plist);
		virtual void setBaseDesk(long lKey);
		virtual void setRscMng(long lKey);
	} m_xAllGetterSetter;
	friend class CAllPropGetterSetter;
// }}



(2) cpp�� ���� �����θ� �߰� (����: classOrig�� ���� define���� ���� ��Ʈ�ѿ� �°� ����
/////////////////////////////////////////////////////////////////////////////
// CAllPropGetterSetter Implementation
#define classOrig	CHtcComboCtrl
void classOrig::CAllPropGetterSetter::getAllCtrlProperty(CStringList* plist)
{
	METHOD_PROLOGUE(classOrig, AllGetterSetter)
	
	pThis->GetAllProperties2((long)plist);
}

BOOL classOrig::CAllPropGetterSetter::setAllCtrlProperty(CStringList* plist)
{
	METHOD_PROLOGUE(classOrig, AllGetterSetter)

	return pThis->SetAllProperties2((long)plist);
}

void classOrig::CAllPropGetterSetter::setBaseDesk(long lKey)
{
	METHOD_PROLOGUE(classOrig, AllGetterSetter)

	return pThis->SetBaseDesk(lKey);
}

void classOrig::CAllPropGetterSetter::setRscMng(long lKey)
{
	METHOD_PROLOGUE(classOrig, AllGetterSetter)

	return pThis->SetRscMng(lKey);	
}
/////////////////////////////////////////////////////////////////////////////
*/

interface IBaseAllGetterSetter
{
public:
		virtual void getAllCtrlProperty(CStringList* plist) PURE;
		virtual BOOL setAllCtrlProperty(CStringList* plist) PURE;
		virtual void setBaseDesk(long lKey) PURE;
		virtual void setRscMng(long lKey) PURE;
};