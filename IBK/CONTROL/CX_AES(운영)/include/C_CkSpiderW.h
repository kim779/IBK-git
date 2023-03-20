// This is a generated source file for Chilkat version 9.4.1.34
#ifndef _C_CkSpider_W_H
#define _C_CkSpider_W_H
#include "chilkatDefs.h"

#include "Chilkat_C.h"

HCkSpiderW CkSpiderW_Create(void);
HCkSpiderW CkSpiderW_Create2(BOOL bForDso);
void CkSpiderW_Dispose(HCkSpiderW handle);
BOOL CkSpiderW_getAvoidHttps(HCkSpiderW cHandle);
void CkSpiderW_putAvoidHttps(HCkSpiderW cHandle, BOOL newVal);
void CkSpiderW_getCacheDir(HCkSpiderW cHandle, HCkStringW retval);
void CkSpiderW_putCacheDir(HCkSpiderW cHandle, const wchar_t *newVal);
BOOL CkSpiderW_getChopAtQuery(HCkSpiderW cHandle);
void CkSpiderW_putChopAtQuery(HCkSpiderW cHandle, BOOL newVal);
int CkSpiderW_getConnectTimeout(HCkSpiderW cHandle);
void CkSpiderW_putConnectTimeout(HCkSpiderW cHandle, int newVal);
void CkSpiderW_getDebugLogFilePath(HCkSpiderW cHandle, HCkStringW retval);
void CkSpiderW_putDebugLogFilePath(HCkSpiderW cHandle, const wchar_t *newVal);
void CkSpiderW_getDomain(HCkSpiderW cHandle, HCkStringW retval);
BOOL CkSpiderW_getFetchFromCache(HCkSpiderW cHandle);
void CkSpiderW_putFetchFromCache(HCkSpiderW cHandle, BOOL newVal);
int CkSpiderW_getHeartbeatMs(HCkSpiderW cHandle);
void CkSpiderW_putHeartbeatMs(HCkSpiderW cHandle, int newVal);
void CkSpiderW_getLastErrorHtml(HCkSpiderW cHandle, HCkStringW retval);
void CkSpiderW_getLastErrorText(HCkSpiderW cHandle, HCkStringW retval);
void CkSpiderW_getLastErrorXml(HCkSpiderW cHandle, HCkStringW retval);
BOOL CkSpiderW_getLastFromCache(HCkSpiderW cHandle);
void CkSpiderW_getLastHtml(HCkSpiderW cHandle, HCkStringW retval);
void CkSpiderW_getLastHtmlDescription(HCkSpiderW cHandle, HCkStringW retval);
void CkSpiderW_getLastHtmlKeywords(HCkSpiderW cHandle, HCkStringW retval);
void CkSpiderW_getLastHtmlTitle(HCkSpiderW cHandle, HCkStringW retval);
void CkSpiderW_getLastModDate(HCkSpiderW cHandle, SYSTEMTIME *retval);
void CkSpiderW_getLastModDateStr(HCkSpiderW cHandle, HCkStringW retval);
void CkSpiderW_getLastUrl(HCkSpiderW cHandle, HCkStringW retval);
int CkSpiderW_getMaxResponseSize(HCkSpiderW cHandle);
void CkSpiderW_putMaxResponseSize(HCkSpiderW cHandle, int newVal);
int CkSpiderW_getMaxUrlLen(HCkSpiderW cHandle);
void CkSpiderW_putMaxUrlLen(HCkSpiderW cHandle, int newVal);
int CkSpiderW_getNumAvoidPatterns(HCkSpiderW cHandle);
int CkSpiderW_getNumFailed(HCkSpiderW cHandle);
int CkSpiderW_getNumOutboundLinks(HCkSpiderW cHandle);
int CkSpiderW_getNumSpidered(HCkSpiderW cHandle);
int CkSpiderW_getNumUnspidered(HCkSpiderW cHandle);
void CkSpiderW_getProxyDomain(HCkSpiderW cHandle, HCkStringW retval);
void CkSpiderW_putProxyDomain(HCkSpiderW cHandle, const wchar_t *newVal);
void CkSpiderW_getProxyLogin(HCkSpiderW cHandle, HCkStringW retval);
void CkSpiderW_putProxyLogin(HCkSpiderW cHandle, const wchar_t *newVal);
void CkSpiderW_getProxyPassword(HCkSpiderW cHandle, HCkStringW retval);
void CkSpiderW_putProxyPassword(HCkSpiderW cHandle, const wchar_t *newVal);
int CkSpiderW_getProxyPort(HCkSpiderW cHandle);
void CkSpiderW_putProxyPort(HCkSpiderW cHandle, int newVal);
int CkSpiderW_getReadTimeout(HCkSpiderW cHandle);
void CkSpiderW_putReadTimeout(HCkSpiderW cHandle, int newVal);
BOOL CkSpiderW_getUpdateCache(HCkSpiderW cHandle);
void CkSpiderW_putUpdateCache(HCkSpiderW cHandle, BOOL newVal);
void CkSpiderW_getUserAgent(HCkSpiderW cHandle, HCkStringW retval);
void CkSpiderW_putUserAgent(HCkSpiderW cHandle, const wchar_t *newVal);
BOOL CkSpiderW_getVerboseLogging(HCkSpiderW cHandle);
void CkSpiderW_putVerboseLogging(HCkSpiderW cHandle, BOOL newVal);
void CkSpiderW_getVersion(HCkSpiderW cHandle, HCkStringW retval);
int CkSpiderW_getWindDownCount(HCkSpiderW cHandle);
void CkSpiderW_putWindDownCount(HCkSpiderW cHandle, int newVal);
void CkSpiderW_AddAvoidOutboundLinkPattern(HCkSpiderW cHandle, const wchar_t *pattern);
void CkSpiderW_AddAvoidPattern(HCkSpiderW cHandle, const wchar_t *pattern);
void CkSpiderW_AddMustMatchPattern(HCkSpiderW cHandle, const wchar_t *pattern);
void CkSpiderW_AddUnspidered(HCkSpiderW cHandle, const wchar_t *url);
BOOL CkSpiderW_CanonicalizeUrl(HCkSpiderW cHandle, const wchar_t *url, HCkStringW outStr);
void CkSpiderW_ClearFailedUrls(HCkSpiderW cHandle);
void CkSpiderW_ClearOutboundLinks(HCkSpiderW cHandle);
void CkSpiderW_ClearSpideredUrls(HCkSpiderW cHandle);
BOOL CkSpiderW_CrawlNext(HCkSpiderW cHandle);
BOOL CkSpiderW_FetchRobotsText(HCkSpiderW cHandle, HCkStringW outStr);
BOOL CkSpiderW_GetAvoidPattern(HCkSpiderW cHandle, int index, HCkStringW outStr);
BOOL CkSpiderW_GetBaseDomain(HCkSpiderW cHandle, const wchar_t *domain, HCkStringW outStr);
BOOL CkSpiderW_GetFailedUrl(HCkSpiderW cHandle, int index, HCkStringW outStr);
BOOL CkSpiderW_GetOutboundLink(HCkSpiderW cHandle, int index, HCkStringW outStr);
BOOL CkSpiderW_GetSpideredUrl(HCkSpiderW cHandle, int index, HCkStringW outStr);
BOOL CkSpiderW_GetUnspideredUrl(HCkSpiderW cHandle, int index, HCkStringW outStr);
BOOL CkSpiderW_GetUrlDomain(HCkSpiderW cHandle, const wchar_t *url, HCkStringW outStr);
void CkSpiderW_Initialize(HCkSpiderW cHandle, const wchar_t *domain);
BOOL CkSpiderW_RecrawlLast(HCkSpiderW cHandle);
BOOL CkSpiderW_SaveLastError(HCkSpiderW cHandle, const wchar_t *path);
void CkSpiderW_SkipUnspidered(HCkSpiderW cHandle, int index);
void CkSpiderW_SleepMs(HCkSpiderW cHandle, int millisec);
const wchar_t *CkSpiderW__avoidPattern(HCkSpiderW cHandle, int index);
const wchar_t *CkSpiderW__baseDomain(HCkSpiderW cHandle, const wchar_t *domain);
const wchar_t *CkSpiderW__cacheDir(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__canonicalizeUrl(HCkSpiderW cHandle, const wchar_t *url);
const wchar_t *CkSpiderW__debugLogFilePath(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__domain(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__failedUrl(HCkSpiderW cHandle, int index);
const wchar_t *CkSpiderW__fetchRobotsText(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__getAvoidPattern(HCkSpiderW cHandle, int index);
const wchar_t *CkSpiderW__getBaseDomain(HCkSpiderW cHandle, const wchar_t *domain);
const wchar_t *CkSpiderW__getFailedUrl(HCkSpiderW cHandle, int index);
const wchar_t *CkSpiderW__getOutboundLink(HCkSpiderW cHandle, int index);
const wchar_t *CkSpiderW__getSpideredUrl(HCkSpiderW cHandle, int index);
const wchar_t *CkSpiderW__getUnspideredUrl(HCkSpiderW cHandle, int index);
const wchar_t *CkSpiderW__getUrlDomain(HCkSpiderW cHandle, const wchar_t *url);
const wchar_t *CkSpiderW__lastErrorHtml(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__lastErrorText(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__lastErrorXml(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__lastHtml(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__lastHtmlDescription(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__lastHtmlKeywords(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__lastHtmlTitle(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__lastModDateStr(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__lastUrl(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__outboundLink(HCkSpiderW cHandle, int index);
const wchar_t *CkSpiderW__proxyDomain(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__proxyLogin(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__proxyPassword(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__spideredUrl(HCkSpiderW cHandle, int index);
const wchar_t *CkSpiderW__unspideredUrl(HCkSpiderW cHandle, int index);
const wchar_t *CkSpiderW__urlDomain(HCkSpiderW cHandle, const wchar_t *url);
const wchar_t *CkSpiderW__userAgent(HCkSpiderW cHandle);
const wchar_t *CkSpiderW__version(HCkSpiderW cHandle);
#endif
