// This is a generated source file for Chilkat version 9.4.1.34
#ifndef _C_CkCharset_W_H
#define _C_CkCharset_W_H
#include "chilkatDefs.h"

#include "Chilkat_C.h"

HCkCharsetW CkCharsetW_Create(void);
HCkCharsetW CkCharsetW_Create2(BOOL bForDso);
void CkCharsetW_Dispose(HCkCharsetW handle);
void CkCharsetW_getAltToCharset(HCkCharsetW cHandle, HCkStringW retval);
void CkCharsetW_putAltToCharset(HCkCharsetW cHandle, const wchar_t *newVal);
void CkCharsetW_getDebugLogFilePath(HCkCharsetW cHandle, HCkStringW retval);
void CkCharsetW_putDebugLogFilePath(HCkCharsetW cHandle, const wchar_t *newVal);
int CkCharsetW_getErrorAction(HCkCharsetW cHandle);
void CkCharsetW_putErrorAction(HCkCharsetW cHandle, int newVal);
void CkCharsetW_getFromCharset(HCkCharsetW cHandle, HCkStringW retval);
void CkCharsetW_putFromCharset(HCkCharsetW cHandle, const wchar_t *newVal);
void CkCharsetW_getLastErrorHtml(HCkCharsetW cHandle, HCkStringW retval);
void CkCharsetW_getLastErrorText(HCkCharsetW cHandle, HCkStringW retval);
void CkCharsetW_getLastErrorXml(HCkCharsetW cHandle, HCkStringW retval);
void CkCharsetW_getLastInputAsHex(HCkCharsetW cHandle, HCkStringW retval);
void CkCharsetW_getLastInputAsQP(HCkCharsetW cHandle, HCkStringW retval);
void CkCharsetW_getLastOutputAsHex(HCkCharsetW cHandle, HCkStringW retval);
void CkCharsetW_getLastOutputAsQP(HCkCharsetW cHandle, HCkStringW retval);
BOOL CkCharsetW_getSaveLast(HCkCharsetW cHandle);
void CkCharsetW_putSaveLast(HCkCharsetW cHandle, BOOL newVal);
void CkCharsetW_getToCharset(HCkCharsetW cHandle, HCkStringW retval);
void CkCharsetW_putToCharset(HCkCharsetW cHandle, const wchar_t *newVal);
BOOL CkCharsetW_getVerboseLogging(HCkCharsetW cHandle);
void CkCharsetW_putVerboseLogging(HCkCharsetW cHandle, BOOL newVal);
void CkCharsetW_getVersion(HCkCharsetW cHandle, HCkStringW retval);
int CkCharsetW_CharsetToCodePage(HCkCharsetW cHandle, const wchar_t *charsetName);
BOOL CkCharsetW_CodePageToCharset(HCkCharsetW cHandle, int codePage, HCkStringW outCharset);
BOOL CkCharsetW_ConvertData(HCkCharsetW cHandle, HCkByteDataW inData, HCkByteDataW outData);
BOOL CkCharsetW_ConvertFile(HCkCharsetW cHandle, const wchar_t *srcPath, const wchar_t *destPath);
BOOL CkCharsetW_ConvertFileNoPreamble(HCkCharsetW cHandle, const wchar_t *srcPath, const wchar_t *destPath);
BOOL CkCharsetW_ConvertFromUtf16(HCkCharsetW cHandle, HCkByteDataW uniData, HCkByteDataW outMbData);
BOOL CkCharsetW_ConvertHtml(HCkCharsetW cHandle, HCkByteDataW htmlIn, HCkByteDataW outHtml);
BOOL CkCharsetW_ConvertHtmlFile(HCkCharsetW cHandle, const wchar_t *srcPath, const wchar_t *destPath);
BOOL CkCharsetW_ConvertToUtf16(HCkCharsetW cHandle, HCkByteDataW mbData, HCkByteDataW outUniData);
BOOL CkCharsetW_EntityEncodeDec(HCkCharsetW cHandle, const wchar_t *inStr, HCkStringW outStr);
BOOL CkCharsetW_EntityEncodeHex(HCkCharsetW cHandle, const wchar_t *inStr, HCkStringW outStr);
BOOL CkCharsetW_GetHtmlCharset(HCkCharsetW cHandle, HCkByteDataW htmlData, HCkStringW outCharset);
BOOL CkCharsetW_GetHtmlFileCharset(HCkCharsetW cHandle, const wchar_t *htmlPath, HCkStringW outCharset);
BOOL CkCharsetW_HtmlDecodeToStr(HCkCharsetW cHandle, const wchar_t *str, HCkStringW outStr);
BOOL CkCharsetW_HtmlEntityDecode(HCkCharsetW cHandle, HCkByteDataW inData, HCkByteDataW outData);
BOOL CkCharsetW_HtmlEntityDecodeFile(HCkCharsetW cHandle, const wchar_t *srcPath, const wchar_t *destPath);
BOOL CkCharsetW_IsUnlocked(HCkCharsetW cHandle);
BOOL CkCharsetW_LowerCase(HCkCharsetW cHandle, const wchar_t *inStr, HCkStringW outStr);
BOOL CkCharsetW_ReadFile(HCkCharsetW cHandle, const wchar_t *path, HCkByteDataW outData);
BOOL CkCharsetW_ReadFileToString(HCkCharsetW cHandle, const wchar_t *path, const wchar_t *srcCharset, HCkStringW outStr);
BOOL CkCharsetW_SaveLastError(HCkCharsetW cHandle, const wchar_t *path);
void CkCharsetW_SetErrorBytes(HCkCharsetW cHandle, const unsigned char *pByteData, unsigned long szByteData);
void CkCharsetW_SetErrorString(HCkCharsetW cHandle, const wchar_t *str, const wchar_t *charset);
BOOL CkCharsetW_UnlockComponent(HCkCharsetW cHandle, const wchar_t *unlockCode);
BOOL CkCharsetW_UpperCase(HCkCharsetW cHandle, const wchar_t *inStr, HCkStringW outStr);
BOOL CkCharsetW_UrlDecodeStr(HCkCharsetW cHandle, const wchar_t *inStr, HCkStringW outStr);
BOOL CkCharsetW_VerifyData(HCkCharsetW cHandle, const wchar_t *charset, HCkByteDataW charData);
BOOL CkCharsetW_VerifyFile(HCkCharsetW cHandle, const wchar_t *charset, const wchar_t *path);
BOOL CkCharsetW_WriteFile(HCkCharsetW cHandle, const wchar_t *path, HCkByteDataW dataBuf);
BOOL CkCharsetW_WriteStringToFile(HCkCharsetW cHandle, const wchar_t *str, const wchar_t *path, const wchar_t *charset);
const wchar_t *CkCharsetW__altToCharset(HCkCharsetW cHandle);
const wchar_t *CkCharsetW__codePageToCharset(HCkCharsetW cHandle, int codePage);
const wchar_t *CkCharsetW__debugLogFilePath(HCkCharsetW cHandle);
const wchar_t *CkCharsetW__entityEncodeDec(HCkCharsetW cHandle, const wchar_t *inStr);
const wchar_t *CkCharsetW__entityEncodeHex(HCkCharsetW cHandle, const wchar_t *inStr);
const wchar_t *CkCharsetW__fromCharset(HCkCharsetW cHandle);
const wchar_t *CkCharsetW__getHtmlCharset(HCkCharsetW cHandle, HCkByteDataW htmlData);
const wchar_t *CkCharsetW__getHtmlFileCharset(HCkCharsetW cHandle, const wchar_t *htmlPath);
const wchar_t *CkCharsetW__htmlCharset(HCkCharsetW cHandle, HCkByteDataW htmlData);
const wchar_t *CkCharsetW__htmlDecodeToStr(HCkCharsetW cHandle, const wchar_t *str);
const wchar_t *CkCharsetW__htmlFileCharset(HCkCharsetW cHandle, const wchar_t *htmlPath);
const wchar_t *CkCharsetW__lastErrorHtml(HCkCharsetW cHandle);
const wchar_t *CkCharsetW__lastErrorText(HCkCharsetW cHandle);
const wchar_t *CkCharsetW__lastErrorXml(HCkCharsetW cHandle);
const wchar_t *CkCharsetW__lastInputAsHex(HCkCharsetW cHandle);
const wchar_t *CkCharsetW__lastInputAsQP(HCkCharsetW cHandle);
const wchar_t *CkCharsetW__lastOutputAsHex(HCkCharsetW cHandle);
const wchar_t *CkCharsetW__lastOutputAsQP(HCkCharsetW cHandle);
const wchar_t *CkCharsetW__lowerCase(HCkCharsetW cHandle, const wchar_t *inStr);
const wchar_t *CkCharsetW__readFileToString(HCkCharsetW cHandle, const wchar_t *path, const wchar_t *srcCharset);
const wchar_t *CkCharsetW__toCharset(HCkCharsetW cHandle);
const wchar_t *CkCharsetW__upperCase(HCkCharsetW cHandle, const wchar_t *inStr);
const wchar_t *CkCharsetW__urlDecodeStr(HCkCharsetW cHandle, const wchar_t *inStr);
const wchar_t *CkCharsetW__version(HCkCharsetW cHandle);
#endif
