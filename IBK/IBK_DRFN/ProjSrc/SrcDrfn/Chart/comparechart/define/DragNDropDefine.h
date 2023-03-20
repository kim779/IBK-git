#if !defined(AFX_DRAGNDROPDEFINE_H__0ED0F7C0_1C28_4C38_89E7_C86FEE5FAC73__INCLUDED_)
#define AFX_DRAGNDROPDEFINE_H__0ED0F7C0_1C28_4C38_89E7_C86FEE5FAC73__INCLUDED_

const UINT RMSG_DRAGSTART			= ::RegisterWindowMessage(_T("RMSG_DRAGSTART"));
const UINT RMSG_DRAGEND				= ::RegisterWindowMessage(_T("RMSG_DRAGEND"));

class CDragNDrop
{
public:
	CDragNDrop()
	{
		m_szKey = NULL;
		m_szValue = NULL;
		m_ptCursor = NULL;
		m_bDelete = TRUE;
	}

	~CDragNDrop()
	{
		if(m_szKey)
		{
			delete [] m_szKey;
			m_szKey = NULL;
		}

		if(m_szValue)
		{
			delete [] m_szValue;
			m_szValue = NULL;
		}

		if(m_ptCursor)
		{
			delete m_ptCursor;
			m_ptCursor = NULL;
		}
	}

	// DND ������ ����
	void SetData(LPPOINT ptCursor, HWND hWnd, LPTSTR szKey, LPTSTR szValue, BOOL bDelete = TRUE)
	{
		if(m_ptCursor)
			delete m_ptCursor;

		m_ptCursor = new POINT;
		m_ptCursor->x = ptCursor->x;
		m_ptCursor->y = ptCursor->y;

		m_hWndSource = hWnd;

		if(m_szKey)
			delete [] m_szKey;

		m_szKey = new char[::strlen(szKey) + 1];
		::strcpy(m_szKey, szKey);

		if(m_szValue)
			delete [] m_szValue;

		m_szValue = new char[::strlen(szValue) + 1];
		::strcpy(m_szValue, szValue);

		m_bDelete = bDelete;
	}

	// ����������� ������ ���� �� ������
	BOOL IsDelete()
	{
		return m_bDelete;
	}

	// ����Ű
	LPTSTR GetKey()
	{
		return m_szKey;
	}

	// ������
	LPTSTR GetValue()
	{
		return m_szValue;
	}

	// �巡�װ� ���۵� �ڵ�
	HWND GetHWndStart()
	{
		return m_hWndSource;
	}

	// ���콺 Ŀ�� ��ġ
	LPPOINT GetCursorPoint()
	{
		return m_ptCursor;
	}

protected:
	LPPOINT m_ptCursor;				// Ŀ����ġ
	HWND	m_hWndSource;			// ���۽���(�ڵ�)
	LPTSTR	m_szKey;				// ����Ű
	LPTSTR	m_szValue;				// ������
	BOOL	m_bDelete;				// ���������� �޸� �������� ����
};

#endif // !defined(AFX_DRAGNDROPDEFINE_H__0ED0F7C0_1C28_4C38_89E7_C86FEE5FAC73__INCLUDED_)
