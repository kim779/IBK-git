// FileBuilder.h: interface for the CFileBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEBUILDER_H__A6E90D73_0AAB_4FB2_A817_144E654B0915__INCLUDED_)
#define AFX_FILEBUILDER_H__A6E90D73_0AAB_4FB2_A817_144E654B0915__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFileBuilder  
{
public:
	// file �� open �ϰ� data�� �д´�.
	CString FileRead(const CString& strFilePathAndName) const;
	// file �� �����ϰ� data�� ����.
	int FileCreatWrite(const CString& strFilePathAndName, const CString& strWriteData);
	bool FileCreat(const CString& strFilePathAndName);
	bool FileWrite(const CString& strFilePathAndName, const CString& strWriteData);
	// file ã��.
	bool FileFind(const CString& strFillePathAndName) const;

protected:
	CString GetMidToTrimDataIndex(const CString& strOrgData, const CString& strTrimData, const int nCount, const int nIndex) const;
};


// property ���� file
class CPropertyFileBuilder
{
public:
	bool GetPropertyFileData(const CString& strFilePathAndName, CStringList& proStrList) const;
};

#endif // !defined(AFX_FILEBUILDER_H__A6E90D73_0AAB_4FB2_A817_144E654B0915__INCLUDED_)
