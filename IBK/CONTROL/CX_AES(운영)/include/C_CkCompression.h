// This is a generated source file for Chilkat version 9.4.1.34
#ifndef _C_CkCompression_H
#define _C_CkCompression_H
#include "chilkatDefs.h"

#include "Chilkat_C.h"

HCkCompression CkCompression_Create(void);
void CkCompression_Dispose(HCkCompression handle);
void CkCompression_getAlgorithm(HCkCompression cHandle, HCkString retval);
void CkCompression_putAlgorithm(HCkCompression cHandle, const char *newVal);
void CkCompression_getCharset(HCkCompression cHandle, HCkString retval);
void CkCompression_putCharset(HCkCompression cHandle, const char *newVal);
void CkCompression_getDebugLogFilePath(HCkCompression cHandle, HCkString retval);
void CkCompression_putDebugLogFilePath(HCkCompression cHandle, const char *newVal);
void CkCompression_getEncodingMode(HCkCompression cHandle, HCkString retval);
void CkCompression_putEncodingMode(HCkCompression cHandle, const char *newVal);
int CkCompression_getHeartbeatMs(HCkCompression cHandle);
void CkCompression_putHeartbeatMs(HCkCompression cHandle, int newVal);
void CkCompression_getLastErrorHtml(HCkCompression cHandle, HCkString retval);
void CkCompression_getLastErrorText(HCkCompression cHandle, HCkString retval);
void CkCompression_getLastErrorXml(HCkCompression cHandle, HCkString retval);
BOOL CkCompression_getUtf8(HCkCompression cHandle);
void CkCompression_putUtf8(HCkCompression cHandle, BOOL newVal);
BOOL CkCompression_getVerboseLogging(HCkCompression cHandle);
void CkCompression_putVerboseLogging(HCkCompression cHandle, BOOL newVal);
void CkCompression_getVersion(HCkCompression cHandle, HCkString retval);
BOOL CkCompression_BeginCompressBytes(HCkCompression cHandle, HCkByteData bData, HCkByteData outData);
BOOL CkCompression_BeginCompressBytesENC(HCkCompression cHandle, HCkByteData bData, HCkString outStr);
BOOL CkCompression_BeginCompressString(HCkCompression cHandle, const char *str, HCkByteData outData);
BOOL CkCompression_BeginCompressStringENC(HCkCompression cHandle, const char *str, HCkString outStr);
BOOL CkCompression_BeginDecompressBytes(HCkCompression cHandle, HCkByteData bData, HCkByteData outData);
BOOL CkCompression_BeginDecompressBytesENC(HCkCompression cHandle, const char *str, HCkByteData outData);
BOOL CkCompression_BeginDecompressString(HCkCompression cHandle, HCkByteData bData, HCkString outStr);
BOOL CkCompression_BeginDecompressStringENC(HCkCompression cHandle, const char *str, HCkString outStr);
BOOL CkCompression_CompressBytes(HCkCompression cHandle, HCkByteData bData, HCkByteData outData);
BOOL CkCompression_CompressBytesENC(HCkCompression cHandle, HCkByteData bData, HCkString outStr);
BOOL CkCompression_CompressFile(HCkCompression cHandle, const char *srcPath, const char *destPath);
BOOL CkCompression_CompressString(HCkCompression cHandle, const char *str, HCkByteData outData);
BOOL CkCompression_CompressStringENC(HCkCompression cHandle, const char *str, HCkString outStr);
BOOL CkCompression_DecompressBytes(HCkCompression cHandle, HCkByteData bData, HCkByteData outData);
BOOL CkCompression_DecompressBytesENC(HCkCompression cHandle, const char *str, HCkByteData outData);
BOOL CkCompression_DecompressFile(HCkCompression cHandle, const char *srcPath, const char *destPath);
BOOL CkCompression_DecompressString(HCkCompression cHandle, HCkByteData bData, HCkString outStr);
BOOL CkCompression_DecompressStringENC(HCkCompression cHandle, const char *str, HCkString outStr);
BOOL CkCompression_EndCompressBytes(HCkCompression cHandle, HCkByteData outData);
BOOL CkCompression_EndCompressBytesENC(HCkCompression cHandle, HCkString outStr);
BOOL CkCompression_EndCompressString(HCkCompression cHandle, HCkByteData outData);
BOOL CkCompression_EndCompressStringENC(HCkCompression cHandle, HCkString outStr);
BOOL CkCompression_EndDecompressBytes(HCkCompression cHandle, HCkByteData outData);
BOOL CkCompression_EndDecompressBytesENC(HCkCompression cHandle, HCkByteData outData);
BOOL CkCompression_EndDecompressString(HCkCompression cHandle, HCkString outStr);
BOOL CkCompression_EndDecompressStringENC(HCkCompression cHandle, HCkString outStr);
BOOL CkCompression_MoreCompressBytes(HCkCompression cHandle, HCkByteData bData, HCkByteData outData);
BOOL CkCompression_MoreCompressBytesENC(HCkCompression cHandle, HCkByteData bData, HCkString outStr);
BOOL CkCompression_MoreCompressString(HCkCompression cHandle, const char *str, HCkByteData outData);
BOOL CkCompression_MoreCompressStringENC(HCkCompression cHandle, const char *str, HCkString outStr);
BOOL CkCompression_MoreDecompressBytes(HCkCompression cHandle, HCkByteData bData, HCkByteData outData);
BOOL CkCompression_MoreDecompressBytesENC(HCkCompression cHandle, const char *str, HCkByteData outData);
BOOL CkCompression_MoreDecompressString(HCkCompression cHandle, HCkByteData bData, HCkString outStr);
BOOL CkCompression_MoreDecompressStringENC(HCkCompression cHandle, const char *str, HCkString outStr);
BOOL CkCompression_SaveLastError(HCkCompression cHandle, const char *path);
BOOL CkCompression_UnlockComponent(HCkCompression cHandle, const char *unlockCode);
const char *CkCompression_algorithm(HCkCompression cHandle);
const char *CkCompression_beginCompressBytesENC(HCkCompression cHandle, HCkByteData bData);
const char *CkCompression_beginCompressStringENC(HCkCompression cHandle, const char *str);
const char *CkCompression_beginDecompressString(HCkCompression cHandle, HCkByteData bData);
const char *CkCompression_beginDecompressStringENC(HCkCompression cHandle, const char *str);
const char *CkCompression_charset(HCkCompression cHandle);
const char *CkCompression_compressBytesENC(HCkCompression cHandle, HCkByteData bData);
const char *CkCompression_compressStringENC(HCkCompression cHandle, const char *str);
const char *CkCompression_debugLogFilePath(HCkCompression cHandle);
const char *CkCompression_decompressString(HCkCompression cHandle, HCkByteData bData);
const char *CkCompression_decompressStringENC(HCkCompression cHandle, const char *str);
const char *CkCompression_encodingMode(HCkCompression cHandle);
const char *CkCompression_endCompressBytesENC(HCkCompression cHandle);
const char *CkCompression_endCompressStringENC(HCkCompression cHandle);
const char *CkCompression_endDecompressString(HCkCompression cHandle);
const char *CkCompression_endDecompressStringENC(HCkCompression cHandle);
const char *CkCompression_lastErrorHtml(HCkCompression cHandle);
const char *CkCompression_lastErrorText(HCkCompression cHandle);
const char *CkCompression_lastErrorXml(HCkCompression cHandle);
const char *CkCompression_moreCompressBytesENC(HCkCompression cHandle, HCkByteData bData);
const char *CkCompression_moreCompressStringENC(HCkCompression cHandle, const char *str);
const char *CkCompression_moreDecompressString(HCkCompression cHandle, HCkByteData bData);
const char *CkCompression_moreDecompressStringENC(HCkCompression cHandle, const char *str);
const char *CkCompression_version(HCkCompression cHandle);
#endif
