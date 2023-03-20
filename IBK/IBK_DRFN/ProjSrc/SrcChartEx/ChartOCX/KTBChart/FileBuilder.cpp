// FileBuilder.cpp: implementation of the CFileBuilder class.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileBuilder.h"

#include <io.h>
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////
// class CFileBuilder

// public =====================================================================
// file �� open �ϰ� data�� �д´�.
CString CFileBuilder::FileRead(const CString& strFilePathAndName) const
{
	if(strFilePathAndName.IsEmpty())
		return "";

	FILE *stream = fopen(strFilePathAndName, "rb");
	if(stream == NULL)
		return "";
	
	//fseek(stream, 0L, SEEK_SET);
	//long nbytes = _filelength(stream->_file);

	fseek(stream, 0L, SEEK_END);//tour2k
	long nbytes = ftell(stream);
	fseek(stream, 0L, SEEK_SET);

	char* buffer = new char[nbytes + 1];
	ZeroMemory(buffer, nbytes + 1);
	fread(buffer, sizeof(char), nbytes, stream);
	CString strData = buffer;

    fclose(stream);
	delete [] buffer;

	return strData;
}

// file �� �����ϰ� data�� ����. (�̹� file�� �����ϸ� �ش� file�� ������ ����.)
int CFileBuilder::FileCreatWrite(const CString& strFilePathAndName, const CString& strWriteData)
{
	// �ش� file �� ������ file ����.
	if(!FileFind(strFilePathAndName))
	{
		if(!FileCreat(strFilePathAndName))
			return -1;
	}

	// �ش� file�� data ����.
	return FileWrite(strFilePathAndName, strWriteData);
}

// file �����ϱ�.
bool CFileBuilder::FileCreat(const CString& strFilePathAndName)
{
	FILE *stream = fopen(strFilePathAndName, "wb+");
	if(stream == NULL)
		return false;

    fclose(stream);
	return true;
}

// file ����.
bool CFileBuilder::FileWrite(const CString& strFilePathAndName, const CString& strWriteData)
{
	// �ش� file�� data ����.
	FILE *stream = fopen(strFilePathAndName, "wb+");
	if(stream == NULL)
		return false;

	fseek(stream, 0L, SEEK_END);
	fwrite(strWriteData, sizeof(char), strWriteData.GetLength(), stream);
	//fprintf(stream, "%s", writeData);
    fclose(stream);

	return true;
}

// file ã��.
bool CFileBuilder::FileFind(const CString& strFillePathAndName) const
{
	if(strFillePathAndName.IsEmpty())
		return false;

	struct _finddata_t fileData;
	long hFile = _findfirst(strFillePathAndName, &fileData);
	if(hFile == -1L)
		return false;

	_findclose(hFile);
	return true;
}


///////////////////////////////////////////////////////////////////////////////
// class CPropertyFileBuilder

// public =====================================================================
bool CPropertyFileBuilder::GetPropertyFileData(const CString& strFilePathAndName, CStringList& proStrList) const
{
	CString strFileData = CFileBuilder().FileRead(strFilePathAndName);
	if(strFileData.IsEmpty())
		return false;

	return true;
}