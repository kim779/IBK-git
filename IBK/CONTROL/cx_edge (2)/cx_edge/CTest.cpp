#include "pch.h"
#include "CTest.h"

#include <wincrypt.h>

CString URLEncode(const char* lpszURL)
{
	CString dest("");

	struct Encode
	{
	private:
		BOOL isexcept(char ch)
		{
			const char excepts[] = {
				'_', '.', 0x00
			};

			for (int i = 0; excepts[i]; i++)
				if (ch == excepts[i])
					return TRUE;
			return FALSE;
		}
	public:
		CString value;

		Encode(char ch)
		{
			if (isalpha(ch) || (ch >= '0' && ch <= '9') || isexcept(ch))
				value = ch;
			else
				value.Format("%%%02X", ch);
		}
	};

	for (int i = 0; i < (int)strlen(lpszURL); i++)
	{
		dest += Encode(lpszURL[i]).value;
	}

	return dest;
}

char* base64Encoder(BYTE input_str[], int len_str)
{
	// TODO: 여기에 구현 코드 추가.
	// Character set of base64 encoding scheme
	char char_set[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	// Resultant string
	char* res_str = (char*)malloc(1000 * sizeof(char));

	int index, no_of_bits = 0, padding = 0, val = 0, count = 0, temp;
	int i, j, k = 0;

	// Loop takes 3 characters at a time from
	// input_str and stores it in val
	for (i = 0; i < len_str; i += 3)
	{
		val = 0, count = 0, no_of_bits = 0;

		for (j = i; j < len_str && j <= i + 2; j++)
		{
			// binary data of input_str is stored in val
			val = val << 8;

			// (A + 0 = A) stores character in val
			val = val | input_str[j];

			// calculates how many time loop
			// ran if "MEN" -> 3 otherwise "ON" -> 2
			count++;

		}

		no_of_bits = count * 8;

		// calculates how many "=" to append after res_str.
		padding = no_of_bits % 3;

		// extracts all bits from val (6 at a time)
		// and find the value of each block
		while (no_of_bits != 0)
		{
			// retrieve the value of each block
			if (no_of_bits >= 6)
			{
				temp = no_of_bits - 6;

				// binary of 63 is (111111) f
				index = (val >> temp) & 63;
				no_of_bits -= 6;
			}
			else
			{
				temp = 6 - no_of_bits;

				// append zeros to right if bits are less than 6
				index = (val << temp) & 63;
				no_of_bits = 0;
			}
			res_str[k++] = char_set[index];
		}
	}

	// padding is done here
	for (i = 1; i <= padding; i++)
	{
		res_str[k++] = '=';
	}

	res_str[k] = '\0';

	return res_str;
}

CTest::CTest()
{
	
}

CTest::~CTest()
{

}

void CTest::Encryt(CString sID)
{
	CString slog;
	char* pc = NULL;

	HCRYPTPROV hCryptProv;
	HCRYPTKEY hKey;

	if (CryptAcquireContext(&hCryptProv, NULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, 0) == FALSE) {
		if (CryptAcquireContext(&hCryptProv, NULL, MS_ENH_RSA_AES_PROV_XP, PROV_RSA_AES, 0) == FALSE) {
			slog.Format("[HOB]   CryptAcquireContext error ");
			OutputDebugString(slog);
			return;
		}
	}

	const DWORD AES_KEY_LENGTH = 16;
	struct {
		BLOBHEADER hdr;
		DWORD cbKeySize;
		BYTE rgbKeyData[AES_KEY_LENGTH];
	} keyBlob;

	keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
	keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
	keyBlob.hdr.reserved = 0;
	keyBlob.hdr.aiKeyAlg = CALG_AES_128;
	keyBlob.cbKeySize = AES_KEY_LENGTH;
	CopyMemory(keyBlob.rgbKeyData, "!O7#8aksjdf67h53", AES_KEY_LENGTH);

	if (CryptImportKey(hCryptProv, (BYTE*)&keyBlob, sizeof(keyBlob), 0, 0, &hKey) == FALSE) {
		slog.Format("[HOB]   CryptImportKey error ");
		OutputDebugString(slog);
		return;
	}

	DWORD dwMode = CRYPT_MODE_ECB;
	if (CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&dwMode, 0) == FALSE) {
		slog.Format("[HOB]   CryptSetKeyParam1 error ");
		OutputDebugString(slog);
		return;
	}

	dwMode = PKCS5_PADDING;
	if (CryptSetKeyParam(hKey, KP_PADDING, (BYTE*)&dwMode, 0) == FALSE) {
		slog.Format("[HOB]   CryptSetKeyParam2 error ");
		OutputDebugString(slog);
		return;
	}

	//const char PLAINTEXT[] = "081144^20211231235959";
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString plainID;

	plainID.Format("%s^%04d%02d%02d%02d%02d%02d", sID, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	slog.Format("[HOB]   Register Open RegQueryValueEx  [%s]", plainID);
	OutputDebugString(slog);

	//plainID.Format("%s", PLAINTEXT);

	BYTE pData[128];
	char pData2[128];
	DWORD dwStrLen = plainID.GetLength();

	DWORD dwPadding = 16 - (dwStrLen & 0xF);
	DWORD dwDataLen = dwStrLen;
	DWORD dwBufferLen = dwStrLen + dwPadding;
	BOOL result;

	memset(pData, 0, 128);
	memset(pData2, 0, 128);

	//CopyMemory(pData, PLAINTEXT, dwStrLen);
	CopyMemory(pData, (LPSTR)(LPCTSTR)plainID, dwStrLen);

	result = CryptEncrypt(hKey, 0, TRUE, 0, pData, &dwDataLen, dwBufferLen);

	sprintf_s(pData2, 128, "%s", base64Encoder(pData, dwBufferLen));

	CString strUrl;
	strUrl = "http://hub-api.tibks.com/api/emp/login/sso?value=" + URLEncode(pData2);
	slog.Format("[HOB] URL =[ %s]", strUrl);
	OutputDebugString(slog);
	ShellExecute(NULL, "open", "chrome.exe", strUrl, NULL, SW_SHOW);
	//	ShellExecute(NULL, _T("open"), "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs", strUrl, NULL, SW_HIDE);
}