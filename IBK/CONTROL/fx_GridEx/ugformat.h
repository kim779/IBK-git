/*************************************************************************
				Class Declaration : CUGCellFormat
**************************************************************************
	Source file : ugformat.cpp
	Header file : ugformat.h
	Copyright ? Dundas Software Ltd. 1994 - 2002, All Rights Reserved

	Purpose
		This class is used for the formating the 
		cells data for display and/or for editing.
*************************************************************************/
#pragma once


class CUGCell;

class UG_CLASS_DECL CUGCellFormat: public CObject
{
public:
	CUGCellFormat();
	virtual ~CUGCellFormat();

	virtual void ApplyDisplayFormat(CUGCell *cell);
	
	virtual int  ValidateCellInfo(CUGCell *cell);
};

