#ifndef _INTERFACEDLL_H
#define _INTERFACEDLL_H

#include "InterfaceTypes.h"

#ifdef _SKEURE 
#include "session_api.h"
#endif
#ifdef _AFXDLL 
#include "./sk_pc_identity.h"
#endif
//_AFXDLL

#define IN
#define OUT
#define IO

#ifdef __cplusplus
extern "C" {
#endif

	/* �Ϲݱ�� : SD_API_CONTEXT(_NEW)�� �����ϱ����ؼ� ����ϸ� ���� �ȵ� */
	/* SESSION_MANAGER pSm�� sk_if_SessionCreate() ȣ�������� �ʱ�ȭ�Ǿ� �־�� ��, �ݵ�� !!! */
	__declspec(dllimport) int sk_if_SessionCreate(SESSION_MANAGER *pSm, LPCTSTR server, LPCTSTR service) ;
	__declspec(dllimport) int sk_if_SessionDelete(SESSION_MANAGER *pSm) ;
	__declspec(dllimport) int sk_if_SessionPutData(SESSION_MANAGER *pSm, UString *pData) ;
	__declspec(dllimport) int sk_if_SessionGetData(SESSION_MANAGER *pSm, UString *pData) ;

	__declspec(dllimport) BOOL sk_if_SetSessionServicePort(int port) ;
	//	[2005/06/21] SESSION MANAGER DELETE
	__declspec(dllimport) BOOL sk_if_ClearSessionService(LPCTSTR server, LPCTSTR service) ;

	__declspec(dllimport) void sk_if_CleanUp(int Device = 0) ;
	__declspec(dllimport) void sk_if_SetGlobalCardPin(const char *pin) ;

	__declspec(dllimport) BOOL sk_if_PEM2BIN(const char *pPEM, unsigned char **ppBIN, int *pnLen) ;
	__declspec(dllimport) BOOL sk_if_BIN2PEM(const unsigned char *pBin, int nLen, char **ppPEM) ;
	__declspec(dllimport) BOOL sk_if_DnExtract(const unsigned char *pCert, int nLen, char *pDn300Bytes) ;
	__declspec(dllimport) BOOL sk_if_PublicKeyExtract(const unsigned char *pCert, int nLen, PKEY_CONTEXT *pKeyContext) ;

	/* not supported start */
	__declspec(dllimport) BOOL sk_if_EncryptPrivateKey(
						const unsigned char *pDecryptedKey, int nDecryptedKeyLen,
						char *pszPasswd,
						      unsigned char *pEncryptedKey, int *pnEncryptedKeyLen) ;
	__declspec(dllimport) BOOL sk_if_DecryptPrivateKey(
						const unsigned char *pEncryptedKey, int nEncryptedKeyLen,
						char *pszPasswd,
						      unsigned char *pDecryptedKey, int *pnDecryptedKeyLen) ;
	/* not supported end */
	__declspec(dllimport) BOOL sk_if_EncryptPrivateKey_Ext(
						const unsigned char *pDecryptedKey, int nDecryptedKeyLen,
						R_INFO *pR,
						char *pszPasswd,
						unsigned char *pEncryptedKey, int *pnEncryptedKeyLen) ;
	__declspec(dllimport) BOOL sk_if_DecryptPrivateKey_Ext(
						const unsigned char *pEncryptedKey, int nEncryptedKeyLen,
						char *pszPasswd,
						unsigned char *pDecryptedKey, int *pnDecryptedKeyLen,
						R_INFO *pR) ;

/* 2000.08.30 new added */
	__declspec(dllimport) BOOL sk_if_DownloadCRLs(const char *pSignCertBase64, char **ppCRLBase64, char **ppARLBase64) ;
	__declspec(dllimport) BOOL sk_if_DownloadSignCert(const char *pDN, char **ppSignCertBase64) ;

	/* general function for certificate downloading */
	__declspec(dllimport) int  sk_if_cert_DownloadCertificate(const char *pDN, int nType, UString *puCert) ;
													/* nType = E_KM_CERT, E_SN_CERT */

	__declspec(dllimport) BOOL sk_if_VerifySignCert(const char *pSignCertBase64) ;
	__declspec(dllimport) BOOL sk_if_VerifySignCertV2(const char *pSignCertBase64, int nType) ;	/* 0,1,2 */
	__declspec(dllimport) BOOL sk_if_GetSignCert(SD_API_CONTEXT *pContext, char **ppSignCertBase64) ;
	__declspec(dllimport) BOOL sk_if_GetSignKey(SD_API_CONTEXT *pContext, char **ppSignKeyBase64) ;

	__declspec(dllimport) BOOL sk_if_SearchRenewedCertificate(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_RestoreCertificateInfo(SD_API_CONTEXT *pContext) ;

	/* primitive storage api */
	__declspec(dllimport) int sk_if_SelectStorage(LPCTSTR pszTitleMsg, BYTE nDisableCurrentMedia, BYTE *pnStorage) ;
	
	/* api for CertManager */
	/* CAUTION !!!!!!!!
	   You must initialize variable pointed by "pContext", OR
				set "pContext" to NULL pointer OR exact values
	   If you don't, then the result can be fatal */
	__declspec(dllimport) LPCTSTR sk_if_GetLastErrorMsg() ;

	/* old certman api - Recommend : do not use */
	__declspec(dllimport) BOOL sk_if_CertManager_Backup(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_Recover(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_Issue(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_Suspend(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_Revoke(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_KeyNew(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_CertNew(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertManager_ChangePWD(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_VerifyPassword(SD_API_CONTEXT *pContext) ;

	/* old certman api - Recommend : use this */
	__declspec(dllimport) BOOL sk_if_CertManager(SD_API_CONTEXT_NEW *pContext, CONTEXT_TYPE nContextType) ;


	/* select old function : only returned signkey */
	__declspec(dllimport) BOOL sk_if_CertSetSelect(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertSetSelectV2(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_CertSetListing(SD_API_CONTEXT *pContext) ; // view all (Query)
	__declspec(dllimport) BOOL sk_if_CertSetListingV2(SD_API_CONTEXT *pContext) ;

	/* select new function */
	__declspec(dllimport) BOOL sk_if_CertSetSelectExt(SD_API_CONTEXT_NEW *pContext, CONTEXT_TYPE ctype, SEARCH_TYPE stype) ;


	// find matched context
	__declspec(dllimport) int sk_if_SelectMatchedContext(SD_API_CONTEXT *pContext, const char *pIDofDN, const char *pSuffixofDN) ;
	__declspec(dllimport) int sk_if_SelectMatchedContextExt(SD_API_CONTEXT_NEW *pContext, const char *pIDofDN, const char *pSuffixofDN) ;
	__declspec(dllimport) int sk_if_SelectFirstMatchedContext(SD_API_CONTEXT *pContext, const char *pIDofDN, const char *pSuffixofDN) ;


	/* ȣ������ ����������(SD_API_CONTEXT or SD_API_CONTEXT_NEW)�� setting �Ǿ���ϴ� �Լ� */
	__declspec(dllimport) BOOL sk_if_RemoveFromMedia(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_ChangeStorageMedia(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) BOOL sk_if_RemoveFromMediaExt(SD_API_CONTEXT_NEW *pContext) ;
	__declspec(dllimport) BOOL sk_if_ChangeStorageMediaExt(SD_API_CONTEXT_NEW *pContext) ;


	__declspec(dllimport) void sk_if_MemFree(void *pBuf) ;
	__declspec(dllimport) int  sk_if_Hash(int nAlg, UString *pIn, CString128 *pOut) ;
	__declspec(dllimport) int  sk_if_cert_Hash_const(APP_CONTEXT *pApp_Context, UString *pIn, CString128 *pOut) ;

	__declspec(dllimport) int sk_if_cert_download_crl(LPCTSTR dn, UString *pCrl, int writeFlag) ;
	// recommended/must-be before-call for other _cert_ APIs
	__declspec(dllimport) int sk_if_cert_InitContextApp(APP_CONTEXT *pApp_Context, char *pTargetDN, int crl_update) ;

	// preset 
	// this function is recommended, fully support all ?cert? apis
	__declspec(dllimport) int sk_if_cert_preset_context(APP_CONTEXT *pApp_Context, const SD_API_CONTEXT *pContext) ;

	// ?static? function is not recommended !!!
	__declspec(dllimport) int sk_if_cert_set_static_context(APP_CONTEXT *pApp_Context, const SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) int sk_if_cert_static_context(APP_CONTEXT *pApp_Context, const char *pContextID) ;
	__declspec(dllimport) int sk_if_cert_static_contextV2(APP_CONTEXT *pApp_Context,
												const char *pContextID, const char *pPasswd) ;
	__declspec(dllimport) int sk_if_cert_static_context_release() ;


	__declspec(dllimport) int sk_if_cert_SignData(APP_CONTEXT *pApp_Context, const char *pPassword, UString *puIn, UString *puOut) ;
	__declspec(dllimport) int sk_if_cert_SignDataWithR(APP_CONTEXT *pApp_Context, const char *pPassword, UString *puIn, UString *puOut, UString *puR) ;


	__declspec(dllimport) int
				sk_if_cert_CMSSignDataInfo(APP_CONTEXT *pApp_Context,
						const char *pszPassword,
						int	bMakeContentInfoOrNot,
						UString *pIn, UString *pOut, UString *pR) ;

	__declspec(dllimport) int
				sk_if_cert_CMSVerifySignedDataInfo(APP_CONTEXT *pApp_Context,
						int	bMakedContentInfoOrNot,
						UString *pIn, UString *pOut,
						PKEY_CONTEXT *pKeyContext,
						VERIFY_EXTENTION_INFO_V2 *pExtention) ;

	/* �Ʒ� api�� ������� ���� : �������� �������� �˼��� ���� */
	__declspec(dllimport) int sk_if_cert_SignDataWith1HashedVID
										(APP_CONTEXT *pApp_Context,
										 const char *pPassword, const char *idn,
										 UString *puIn, UString *puOut, UString *p1Hashed) ;

	__declspec(dllimport) int sk_if_cert_SignFile(APP_CONTEXT *pApp_Context, const char *pPassword, int nOutToFile, const char *pFilePath, UString *pSignInfo) ;
	__declspec(dllimport) int sk_if_cert_SignData_notEncode(APP_CONTEXT *pApp_Context, const char *pPassword, UString *puIn, UString *puOut, CERTKEY_CONTEXT *pCertKeyContext) ;

	__declspec(dllimport) int sk_if_cert_VerifySignedData(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut) ;
	__declspec(dllimport) int sk_if_cert_VerifySignedFile(APP_CONTEXT *pApp_Context, const char *pSignFilePath, const char *pOriginFilePath, UString *pVerifiedInfo) ;
	__declspec(dllimport) int sk_if_cert_VerifySignedData_notDecode(APP_CONTEXT *pApp_Context, UString *puInText, UString *puInSign, CERTKEY_CONTEXT *pCertKeyContext) ;

	__declspec(dllimport) int sk_if_cert_VerifySignedFileWithSignature(APP_CONTEXT *pApp_Context, UString *pSignInfo, const char *pOriginFilePath, SIGN_FILE_INFO *pOriginalFileInfo) ;

	/* �ǵ����̸�, sk_if_cert_VerifySignedData_ExtV2�� ����ϵ��� �� ��*/
	/* sk_if_cert_VerifySignedData_ExtV2�� ����� ���
	   sk_if_cert_VerifySignedData_Ext ����� �״�� �����ϱ� ���ؼ���
	   VERIFY_EXTENTION_INFO_V2 ���� ����ִ� ��������
	   sk_if_CertExtract�� �̿��ؼ� ������ �����ϴ� ����� �����ϱ⸦ ������ */
	__declspec(dllimport) int sk_if_cert_VerifySignedData_Ext(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut, VERIFY_EXTENTION_INFO *pExt) ;
	__declspec(dllimport) int sk_if_cert_VerifySignedData_ExtV2(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut, VERIFY_EXTENTION_INFO_V2 *pExt) ;
	__declspec(dllimport) int sk_if_cert_VerifySignedData_ExtV2_Integrate(APP_CONTEXT *pApp_Context,UString *puIn, UString *puOut,VERIFY_EXTENTION_INFO_V2 *pExt);

	__declspec(dllimport) int sk_if_cert_EncryptData(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut) ;
	__declspec(dllimport) int sk_if_cert_DecryptData(APP_CONTEXT *pApp_Context, const char *pPassword, UString *puIn, UString *puOut) ;

	__declspec(dllimport) int sk_if_cert_GetSessionKey(APP_CONTEXT *pApp_Context) ;

	__declspec(dllimport) int sk_if_cert_Encrypt(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut) ;
	__declspec(dllimport) int sk_if_cert_Decrypt(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut) ;

	/* 2001.09.21 */
	__declspec(dllimport) int sk_if_cert_EncryptDataByCert(APP_CONTEXT *pApp_Context, const UString *pIn, UString *pOut, const UString *pECert) ;
	__declspec(dllimport) int sk_if_cert_DecryptDataByKey(APP_CONTEXT *pApp_Context, const UString *pIn, UString *pOut, const UString *pEKey, const char *pszPasswd) ;


	__declspec(dllimport) void sk_if_cert_MemFree(void *pBuf) ;

	__declspec(dllimport) int sk_if_cert_VerifySignedDataAndGetPubKey(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut, PKEY_CONTEXT *pKeyContext) ;
	__declspec(dllimport) int sk_if_cert_VerifySignedDataUsingPubKey(APP_CONTEXT *pApp_Context, UString *puIn, UString *puOut, PKEY_CONTEXT *pKeyContext) ;

	__declspec(dllimport) int sk_if_cert_GetCaCertififcates(const UString *pCert, UString *pCa, UString *pRootCa) ;

	__declspec(dllimport) int sk_if_cert_PathValidationOnOff(int nFlag) ;
	/* Global Path Validation Flag

		case 0 :	= FLAG_VALIDATE_NO  ;
		case 1 :	= FLAG_VALIDATE_TRY ;
		case 2 :	= FLAG_VALIDATE_YES ;

		:: api�������� ���� �ٶ����� ���ñ���ü ���� ���� (APP_CONTEXT)
	*/
	/* ��Ÿ */
	__declspec(dllimport) int sk_if_CountCertSet() ;
	__declspec(dllimport) int sk_if_GetLastErrorCode() ;

	__declspec(dllimport) int sk_if_GetPkcs12Ptr
					(const char *pszPasswd, unsigned char **pp12str, int *pLen ) ;

	__declspec(dllimport) int sk_if_GetPkcs12PtrWithR
					(const char *dn, char *pszPasswd, UString *pP12Out, UString *pROut, int rtype) ;

	__declspec(dllimport) int sk_if_GetPkcs12PtrWithR_ebid
					(const char *dn, char *pszPasswd, unsigned char **pp12str, int *pLen,
													  unsigned char **ppR, int *pRLen, int rtype) ;

	__declspec(dllimport) BOOL sk_if_ChangePasswd
					( const char *pszOldPasswd, const char *pszNewPasswd ) ;
	__declspec(dllimport) BOOL sk_if_ChangePasswdV2
					(SD_API_CONTEXT_NEW *pContext, const char *pszOldPasswd, const char *pszNewPasswd) ;

	__declspec(dllimport) BOOL sk_if_ValidateCert(const char *pSignCertBase64) ;

	__declspec(dllimport) int sk_if_CertificateStatus(SD_API_CONTEXT *pContext) ;

	__declspec(dllimport) int sk_if_StoreContextP12(SD_API_CONTEXT_NEW *pContext) ;

	/* 2001.04.03 : Must be called after sk_if_CertSetSelect or sk_if_CertSetListing */
	__declspec(dllimport) int sk_if_UpdateSignCert(SD_API_CONTEXT *pContext) ;
	__declspec(dllimport) int sk_if_SetSmartCardPort(int nPort) ;

	__declspec(dllimport) void sk_if_ConfigMenu() ;
	__declspec(dllimport) void sk_if_ConfigMenuX(LPCTSTR cmdline) ;

	__declspec(dllimport) int sk_if_SetOldCertUpdateMode(int nMode) ;
	__declspec(dllimport) int sk_if_SetSelectDialogMode(int nMode) ;
	__declspec(dllimport) int sk_if_SetDisketteDrive(int nDrive) ;
	__declspec(dllimport) int sk_if_SetMiscMedia(int nMedia) ;

	/* 2001.08.09 */
	__declspec(dllimport) char *sk_if_cert_EncodeUTF8(LPCTSTR pStr) ;
	__declspec(dllimport) char *sk_if_cert_DecodeUTF8(LPCTSTR pUtf8) ;

	/* 2001.08.16 */
	__declspec(dllimport) BOOL sk_if_CertExtract(const unsigned char *pCert, int nLen, ExtCertInfo *pCertInfo) ;

	/* 2001.08.17 */
	__declspec(dllimport) void sk_if_ViewCertificate(SD_API_CONTEXT_NEW *pContext) ;



	/* for coregate : 2001/12/05 */
	__declspec(dllimport) BOOL sk_if_SetCardApi(const char *pszCardApiName) ;
	__declspec(dllimport) BOOL sk_if_SetCmpPort80() ;


	__declspec(dllimport) int sk_if_StoreContext(SD_API_CONTEXT_NEW *pContext) ;

	__declspec(dllimport) int sk_if_cert_exportp12
					(const char *pszPasswd,
					 const UString *pKey,
					 const UString *pCert,
					 UString *pPFX) ;

	__declspec(dllimport) int sk_if_cert_importp12
					(const char *pszPasswd,
					 const UString *pPFX,
					 UString *pKey,
					 UString *pCert, int *pnCertType
					 ) ;

	__declspec(dllimport) void sk_if_About() ; // program information dialog box

	/* 1 : old-compatible mode, 0 : new-mode */
	__declspec(dllimport) void sk_if_SetApiCompatibleMode(int mode) ;
	/* if 1 then pbes,pbkdf == v1.5, use old directory, donot validate rootcert */

	__declspec(dllimport) int  sk_if_cert_Get_VID_Information(UString *pECert, VID_INFO *pVID) ;
	__declspec(dllimport) int  sk_if_cert_Get_R_Information(UString *pECert, UString *pEKey,
											char *pszPassword, R_INFO *pR, /* can be NULL */VID_INFO *pVID ) ;

	__declspec(dllimport) int  sk_if_cert_Hash(int nAlg, UString *pIn, UString *pOut) ;
	__declspec(dllimport) int  sk_if_SetWrongPasswordLimit(int count) ;

	__declspec(dllimport) int  sk_if_VerifyRootCertUserConfirm(const UString *rCert, int mode) ; /* do not use */
	__declspec(dllimport) int  sk_if_cert_VerifyCertWithR(const UString *pR, char *idn, const UString *pCert, int certType) ;
	__declspec(dllimport) int  sk_if_cert_IdentifyCertificate(const UString *pCert, const char *idn, const UString *pR, int nEncFlag) ;
	__declspec(dllimport) int  sk_if_cert_IdentifyCertificate1Hash(const UString *pCert, const UString *p1Hash) ;
	__declspec(dllimport) int  sk_if_cert_GetCaNameFromDn(const char *pDn, char *ca_name_max50) ;

	__declspec(dllimport) int  sk_if_VerifySignCert_ebid(int certType, unsigned char *pCert, int nLen) ;
	 #define sk_if_cert_CheckSignCertValidation(a) sk_if_VerifySignCert_ebid(0, (a)->value, (a)->length)

	__declspec(dllimport) BOOL sk_if_Identify(SD_API_CONTEXT *pSignContext, const char *idn) ;


	__declspec(dllimport) int  sk_if_CertSetSearch(CertSetList **ppSearchResult,
								CONTEXT_TYPE ctype, SEARCH_TYPE stype) ;
	__declspec(dllimport) int  sk_if_CurrentCertSetRead(CertSetList *pToReadCertSet,
								SD_API_CONTEXT_NEW *pContext) ;
	__declspec(dllimport) int  sk_if_CertSetResultFree(CertSetList **ppSearchResult) ;

	__declspec(dllimport) void sk_if_SetLanguageMode(int english_mode) ;
	__declspec(dllimport) int  sk_if_GetLanguageMode() ;

	__declspec(dllimport) int  sk_if_SetDeviceOrder(int *new_map) ;
	//-------------------------------------------------------------
	// ������ ��åâ�� �����ü �̹��� ���� ���� ��� ���� /*[8.2.1.1]*/
	// ����Ʈ ������ [REMOVABLE, PKCS11_KEY, SMART_CARD ,HARD_DISK] 
	// [cf] sk_if_SetDeviceOrder �� ������ �˻��� �켱�����̰� �� �Լ��� �̹��� ��ġ�� ������ 
	__declspec(dllimport) int  sk_if_SetDeviceViewOrder(int *new_map) ; 
	//--------------------------------------------------------------
	__declspec(dllimport) int  sk_if_SetCPSFileLocation(const char *filepath) ;

	__declspec(dllimport) void sk_if_DialogModalMode(HWND hParentWnd) ;
	__declspec(dllimport) int  sk_if_SetPasswordEncMode(int boolFlag) ;


	__declspec(dllimport) int  sk_if_SetLogoFilePath(LPCTSTR logofile_408x64) ;
	__declspec(dllimport) int  sk_if_SetCertNewUrlInfo(LPCTSTR url) ;
	__declspec(dllimport) int  sk_if_SetExipreCheckSkip(int boolFlag) ;

	__declspec(dllimport) BOOL sk_if_CMP_SetCmpConfig(const char *ip, int port) ;
	
	//-----------------------------------------------------------------------------------------
	//	- ������ ���ý� �����ü�� �������� 
	//	: sk_if_SetScanByDialogChoiceMode(int mode )
	//	   mode 0  ����ó�� �ڵ����� ��� �����ü�� �������� �˻�
	//	   mode 1  ������ ����â���� �ϵ��ũ�� default�� �����ְ� �����ü ���þ�������
	//		   	   ���������� �ش��ü�� �˻�

	__declspec(dllimport) BOOL sk_if_SetScanByDialogChoiceMode(int mode) ;

	// VER [5.12.0.1]   if Storage Media is available , return TRUE   
	__declspec(dllimport) BOOL sk_if_SelectStorageCheck( BYTE CheckForMedia , int MsgFlag ) ;

	// VER [6.4.0.1]	 IC card pin check function
	__declspec(dllimport)  int sk_if_CardPinCheck(const char *inPin, char **ppoutPin , int Flag);

	//------------------------------------------------------------------------------------------
	// VER [6.7.0.1]
	__declspec(dllimport) int  sk_if_SetKeySaferMode(int boolFlag);
	
	/*  boolFlag
	       1 ŷ���������
	       2 ��ī���ͳ�
	       3 ��ö�� ������
	       4 ��Ŀ��

	       ���Ⱦ�ü�� ������ �Է½�  boolFlag = boolFlag + (������ * 16)
	*/
	//------------------------------------------------------------------------------------------
	// VER [8.2.0.5] 

	// parameter 
	//      mode =>	0  ����   1   ���� ����  16 ���� ����  256 ��������  4096 �ſ�ī�� 
	//      pOid => ��å OID ���� �Է�   (oid end ǥ�� ';')
	
	// ȣ�� �� ) ������������ ����â���� �����ְ� ���� ��� 
	//           sk_if_SetPolicyFilter(1+16, "");
	//
	//           �������� �������� OID�� 1.2.410.200004.5.1.1.4 �Ǵ�  1.2.410.200004.5.1.1.5 �� ������ ���� 
	//			 sk_if_SetPolicyFilter(256, "1.2.410.200004.5.1.1.4;1.2.410.200004.5.1.1.5;");
	//         
	//           ��å ���͸� Ŭ���� 
	//			 sk_if_SetPolicyFilter(0,NULL)
	//  

	__declspec(dllimport) void  sk_if_SetPolicyFilter(int mode, const char *pOid = "" );

	//-------------------------------------------------------------------------------------------
	//������ �����ü ����â���� �޺��ڽ��� �ֻ��� �ε��� �� ���� API 
	__declspec(dllexport) int sk_if_SetTopSaveDevice(BYTE nDevice);
	
	/*-------------------------------------------------------------------------------------------
	  : ������ ����â���� ����� �̹��� �ΰ� URL�� ���� �ٿ�޾� ����ϴ� ��� 
	  
	   �� �Ķ���� 
		 BannerURL  : �ٿ�ε� ��� URL (�ش� ������ SignKorea ���� �����ϴ� Ű�� ����Ǿ����.)
		 LocalPath  : PC�� ����Ǵ� ���ϸ� ( �ʿ�� ���)

	   �� ��� 
		  ���� : TRUE
		  ���� : FALSE      
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) BOOL sk_if_SetLogoURL(const char *pBannerURL,const char *pLocalPath) ;
	

	
	//------------------------------------------------------------------------------------------
	// VER [9.9.1.7]
	__declspec(dllimport) int sk_if_SetLibraryPath(int mode, LPCTSTR strPath);
	/*
	 : ������ ��⿡�� �̿��ϴ� �ܺξ�ü�� ���̺귯�� ������ġ�� �����ϴ� ���
	  
	   �� �Ķ���� 
		 mode     : 1)Ű���� ���ȸ��
		 strPath  : ���� ������ ��ġ�Ǿ� �ִ� PATH

	   �� ��� 
		  ���� : TRUE
		  ���� : FALSE      
	--------------------------------------------------------------------------------------------*/
	
	/*-------------------------------------------------------------------------------------------
		BOOL sk_if_GetMacAddress(char * macAddr , int mode)
   		
	     : ȣ���ϴ� �ý����� macAddr �� ip ������ ȹ���ϴ� ��� 
		  
		  �� �Ķ���� 
			 macAddr  : ��弳���� ����  MAC�� IP ���� ���ڿ�
			 maxLen   : (�������� �޸� ������)���ڿ� �ִ� ����
			 mode     : �ʿ����� ����   ���� 4bit 0 - MAC��   1 - MAC;IP;     2 - IP��
			                            ���� 4bit 0 - first Device, 1(16) - default Device, 2(32) - All Device
			            
��		 �� ��� 
	������	���� : TRUE     (WEB)  Mac;IP����Ʈ��   
			���� : FALSE    (WEB)  NULL

	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) BOOL sk_if_GetMacAddress(char * macAddr , int maxLen, int mode) ;

	/*-------------------------------------------------------------------------------------------
	   int sk_if_GetEncryptedPassword(const char *password, char *enc_password33Bytes)

		: �н����� ��ȣȭ �Լ� 
        : ��ȣȭ�� ������Ŷ ���ο��� �ڵ����� ���� 
  
	  �� �Ķ���� 
	     password : �н����� ���� 
		 enc_password33Bytes : ��ȣȭ ��� (33 byte )
	   	  
	  �� ��� 
	     return : encrypted size  �÷��� + 16 = 17
		                          �÷��� + 32 = 33
	--------------------------------------------------------------------------------------------*/
    __declspec(dllimport) int sk_if_GetPCIdentity( PC_INFO_CONTEXT * pContext, int nMode );
	/*--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_GetEncryptedPassword(const char *password, char *enc_password33Bytes );
	
	/*-------------------------------------------------------------------------------------------
	   int sk_if_GetEncryptedPasswordB64(const char *pPassword, char *pOutBuf, int *nOutlen );

		: Ű���� ���Ⱦ�ü�� ������ �н����� ��ȣȭ �Լ� 
		: ��ȣȭ�� ��������й�ȣ�� BASE64�� ���ڵ��Ǿ� ��µ�  
        : ACTIVE-X�� �̿��Ͽ� ���ڼ����ϴ� WTS���� �̿밡��  
		  (HTS�� ���� �ݵ�� BASE64�� ���ڵ� �� ��, �����ؾ� �̿밡��)     

  
	  �� �Ķ���� 
	     pPassword : ������ �н����� ���� 
		 pOutBuf :  BASE64 ���ڵ��� ��ȣȭ ��� ����� ����  (?? byte )
		 nOutlen : (in)  alloc�� pOutBuf Size,  0 ������, ȣ���ϸ� ��ȯ�� buffer size�� nOutLen�� �����.
		           (out) pOutBuf �� ��ȿ �������   
	   	           
	  �� ��� 
	     return 0  [����] : pPassword�� �ʱ�ȭ 
	           -1  [����] :	�н����� �ڸ��� �ʰ�(30),  pOutBuf�� Size�� ���� ��� 
	--------------------------------------------------------------------------------------------*/
	__declspec(dllexport) int sk_if_GetEncryptedPasswordB64(const char *pPassword, char *pOutBuf, int *nOutlen );

	/*-------------------------------------------------------------------------------------------
	 int sk_if_SetWebAccMode(int mode);
		: ����� �� ���ټ� ��� ���� �ɼ�  
  
	  �� �Ķ���� 
	    boolFlag  :    [1] ����    [0] ����   [-1] ���� ���� �ɼ�
	  �� ��� 
	��	    ������ �ɼ�  
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_SetWebAccMode(int mode);


	/*-------------------------------------------------------------------------------------------
	 //[9.9.2.0] ���� ����
	 int sk_if_SetSiteInform(int mode, int nCode, const char *pEncValue );
		: �̿��� ���� �� ���Ἲ ���� �Լ�. 
  
	  �� �Ķ���� 
	    mode  :    [1] SIGNKOREA ȸ����   [2] USIM ȸ���� �ڵ�   [-1] ���� ���� �̿��� ��ȸ
		nCode :   ����Ʈ �ڵ�
		pEncValue : ��������
	  �� ��� 
	��	  ���� ����  
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_SetSiteInform(int mode, int nCode, const char *pEncValue );

	/*-------------------------------------------------------------------------------------------
	 int sk_if_cert_exportp12_New
	[8.2.2.0] [2010/01/11] #3  ��ȣȭ�� ������ ���� PKCS#12 API 
  
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_cert_exportp12_New (IN const char *pszPasswd, IN const UString *pKey, IN const UString *pCert, IN const UString *pKmKey, IN const UString *pKmCert, OUT UString *pPFX);
	__declspec(dllimport) int sk_if_cert_importp12_New (IN const char *pszPasswd, IN const UString *pPFX, OUT UString *pKey, OUT UString *pCert, OUT UString *pKmKey, OUT UString *pKmCert, OUT int *pnCertType);

	
	/*-------------------------------------------------------------------------------------------
	[9.9.7.2] [2017/06/15] #212 PFX ���� read & context ���� ���
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_cert_SetContextbyReadP12 (IN const UString *pPFX, IN/*OUT*/SD_API_CONTEXT_NEW *pContext,IN int mode);
	/*-------------------------------------------------------------------------------------------

	[9.9.7.0 ] [2016/8/24] ������ ����Ʈ�� ��ȣ �̵� ���
  
	--------------------------------------------------------------------------------------------*/
	__declspec(dllimport) int sk_if_cert_export_mobile (IN const char *pszPasswd, IN const UString *pKey, IN const UString *pCert, IN const UString *pKmKey, IN const UString *pKmCert);
	__declspec(dllimport) int sk_if_cert_import_mobile (IN const char *pszPasswd, OUT UString *pKey, OUT UString *pCert, OUT UString *pKmKey, OUT UString *pKmCert);

	/*-------------------------------------------------------------------------------------------

	[9.7.0.0]][2011/05/09] #122 ������ ������� Ʈ���� ���  */
  
	__declspec(dllexport) void sk_if_show_tray(void);
    __declspec(dllexport) void sk_if_hide_tray(void);

	//[9.9.7.4] [2017/12/15] #222 ���������� ���� �ӽ�Ű ���� ���� ��� �߰�
	__declspec(dllimport) int sk_if_Appcert_GenKeyAndSign
		(HWND hwnd, SD_API_CONTEXT_NEW * Context, char * pUserDn, bool bUseVerifyCode, UString *pInputData, UString *pOutSignData,UString *pR);
	
	/*==============================================================================================
	ONLY FOR USED IN SKEURE 
    ��ȣ��Ű��(SKEURE) ������ ����ϴ� API
  
   :  crypto Session API FOR Active-X 
   :  ������⿡���� ����� �������ϴ�.
	//*-----------------------------------------------------------------------------------------*/
#ifdef _SKEURE 	
	__declspec(dllimport) void sk_if_Free_SessionInformation(SESSIONINFO_CONTEXT *pSess);
	__declspec(dllimport) int  sk_if_Get_SessionInformation(const char *svrInfo, SESSIONINFO_CONTEXT *pSess);
	__declspec(dllimport) void sk_if_Close_SessionInformation(const char *svrInfo, unsigned char *SessionID, int nSessionID);

	__declspec(dllimport) void sk_if_crypto_SessionClose(APP_CONTEXT_SESSION *pApp_Context);
	__declspec(dllimport) int  sk_if_crypto_SessionInit(APP_CONTEXT_SESSION *pApp_Context);

//	[2008/10/15]
	__declspec(dllimport) int sk_if_cert_RSAEncrypt( const UString *pIn, UString *pOut, const UString *pECert, int nEncFlag = 0);
	__declspec(dllimport) int sk_if_cert_RSADecrypt( const UString *pIn, UString *pOut,	const UString *pEKey, const char *pszPasswd);

	/*=============================================================================================*/
#endif

#ifdef __cplusplus
}
#endif

#endif // _INTERFACEDLL_H
