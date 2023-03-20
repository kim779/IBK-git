#pragma once
class	Citem
{
public:
	DWORD	m_flag{};
	COLORREF m_clr{};
	CString	m_maps;

public:
	Citem*	Clone()
	{
		std::unique_ptr<Citem>	item;

		item = std::make_unique<Citem>();
		item->m_flag = m_flag;
		item->m_clr  = m_clr;
		item->m_maps = m_maps;
		return item.release();
	}
};


//
//	�����ڵ� \t ���� \t �ð� \t ����� \t Ư�̻���
//	�ð�	: HHMM
//	����	: XXYYYYYY
//	Ư�̻��� : ����/������ ��쿡�� ����
//

//
//	�뱸��
//
#define S_NEWS		0x01000000      // ����/����
#define S_QUOTE		0x02000000      // �ü��м�
#define S_TREND		0x04000000      // �Ÿŵ���
#define S_CHART		0x08000000      // ��Ʈ��ȣ

#define S_CANL		0x40000000      // �������


//
//	����/����
//
#define F_GONG		0x00000001      // ����
#define F_NEWS		0x00000002      // ���񴺽�

//
//	�ü��м�
//
#define F_JUN_KOGA	0x00000001      // ���� �� ����
#define F_JUN_GVOL	0x00000002      // ���� �ŷ��� ����
#define F_DAN_SHGA	0x00000004      // ���� ���Ѱ� ����
#define F_DAN_HHGA	0x00000008      // ���� ���Ѱ� ����
#define F_MAMUL_5D	0x00000010      // �ֱ� 5�� �Ź��� ����
#define F_MAMUL_60D	0x00000020      // �ֱ� 60�� �Ź��� ����
#define F_SHGA_5D	0x00000040      // �ֱ� 5�� ù ���Ѱ�
#define F_GVOL_52M	0x00000080      // 52�� �ְ�ŷ��� ����
#define F_GVOL_5D	0x00000100      // �ֱ� 5�� �ְ�ŷ��� ����
#define F_3D_UP_15P	0x00000200      // �ֱ� 3�ϰ� �ְ���� 15%�̻�
#define F_5D_UP_20P	0x00000400      // �ֱ� 5�ϰ� �ְ���� 20%�̻�
#define F_KOGA_5D	0x00000800      // �ֱ� 5�� �Ű� ����
#define F_JEGA_5D	0x00001000      // �ֱ� 5�� ������ ����
#define F_UPDN_5P	0x00002000      // ���� �ְ�/���� ��� ����� 5%�̻�
#define F_SIGA_5P	0x00004000      // �ð���� 5%�̻� ���
#define F_UPXX_3D	0x00008000      // ���� 3�� ���
#define F_DOWN_3D	0x00010000      // ���� 3�� �϶�
#define F_MASU_200P	0x00020000      // ���� �ż����� 200%�̻�
#define F_RATE_20P	0x00040000      // ���� �ŷ��� ȸ���� 20%�̻�
#define	F_RE_SHGA	0x00100000	// ���Ѱ� ������
#define	F_RE_HHGA	0x00200000	// ���Ѱ� ������
#define	F_DAN_KOGA	0x00400000	// ���� �Ű� ����
#define	F_DAN_JEGA	0x00800000	// ���� ������ ����

//
//	�Ÿŵ���
//
#define F_WMASU		0x00000001      // �ܱ��� ���ż�
#define F_WMADO		0x00000002      // �ܱ��� ���ŵ�
#define F_WMASU_3D	0x00000004      // �ܱ��� 3�� ���� ���ż�
#define F_WMADO_3D	0x00000008      // �ܱ��� 3�� ���� ���ŵ�
#define F_TMASU_1	0x00000010      // �ܱ������ǻ� ���ż� �����̻�
#define F_TMADO_1	0x00000020      // �ܱ������ǻ� ���ŵ� �����̻�
#define F_TMASU_F	0x00000040      // �ܱ������ǻ� ù �ż��ŷ��߻�
#define F_TMADO_F	0x00000080      // �ܱ������ǻ� ù �ŵ��ŷ��߻�

//
//	��Ʈ��ȣ
//
#define F_JUGA_GC	0x00000001      // �ְ� ���ũ�ν�(5MA > 20MA)
#define F_JUGA_DC	0x00000002      // �ְ� ����ũ�ν�(5MA < 20MA)
#define F_GVOL_GC	0x00000004      // �ŷ��� ���ũ�ν�(5MA > 20MA)
#define F_GVOL_DC	0x00000008      // �ŷ��� ����ũ�ν�(5MA < 20MA)
#define F_3SUN_UP	0x00000010      // �Ｑ��ȯ�� ����ȯ
#define F_3SUN_DN	0x00000020      // �Ｑ��ȯ�� ����ȯ
#define F_5MAJ_UP	0x00000040      // �ְ� 5MA ���⵹��
#define F_5MAJ_DN	0x00000080      // �ְ� 5MA ���⵹��
#define F_5MAG_UP	0x00000100      // �ŷ��� 5MA ���⵹��

