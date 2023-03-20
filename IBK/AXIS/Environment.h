#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

/************************************************************************/
/* CEnvironment                                                         */
/************************************************************************/

class CEnvironment
{
private:
	CWnd*	m_axis;
public:
	CEnvironment(CWnd* m_wizard);
	~CEnvironment();
protected:
	virtual void LoadEnvironment() = 0;
	virtual void SaveEnvironment() = 0;
public:
	virtual void Refresh();
	virtual void Default() {}
};


/************************************************************************/
/* CMainEnvironment                                                     */
/************************************************************************/
class CMainEnvironment: public CEnvironment
{
public:
	int		m_maxChild;		// �ִ� ���ϵ������� ����
	int		m_childPos;		// �⺻ ���ϵ� ������
	int		m_startMap;		// ���۽� ������ ���� �ɼ�  1: ���� 2: ���� other: ����
	int		m_fontSize;		// ����Ʈ ��Ʈ ������
	bool	m_unflash;		// ��¦�Ÿ� ����
	bool	m_vsReflect;	// ����ȭ�鰣 Ʈ���� ����
	int		m_msgPos;		// �޽��� ������
	int		m_idleTime;		// Idle time
protected:
	virtual void LoadEnvironment();
	virtual void SaveEnvironment();
public:
	virtual void Default();
};


/************************************************************************/
/* CMarketEnvironment                                                   */
/************************************************************************/
enum { 
	open30min = 1,			// ����� 30����
	open10min = 21,			// ����� 10����
	open5min = 26,			// ����� 5����
	open1min = 30,			// ����� 1����
	open10sec = 31,			// ����� 10����
	open30sec = 33,			// ����� 30����
	close10min = 121,		// ������ 10����
	close5min = 126,		// ������ 5����
	close1min = 130,		// ������ 1����
	close10sec = 131,		// ������ 10����
	close30sec = 133		// ������ 30����
};

class CMarketEnvironment: public CEnvironment
{
public:
	int		m_msgPos;		// �� �޽��� ������
protected:
	virtual void LoadEnvironment();
	virtual void SaveEnvironment();
public:
	virtual void Default();
};


#endif // __ENVIRONMENT_H__