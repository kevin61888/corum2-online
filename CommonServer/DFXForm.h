#ifndef __DFXFORM_H__
#define __DFXFORM_H__


#include "CommonServer.h"


void DFOutputDebugString(LPCTSTR pFormat, ...);


class DECLSPECIFIER DFSimpleXForm
{
#define MAX_KEY_LEN		32
#define SIMPLE_BASE_KEY	"ù±Ü¨ïñÜÃù±ÜôÜ¨ãý"

public:
	DFSimpleXForm();
	DFSimpleXForm(char Key[], int iKeyLen, int iSubKey);
	~DFSimpleXForm();

	void	SetBaseKey();
	void	SetKey(char Key[], int iKeyLen, int iSubKey);
	void	Convert(LPBYTE pSrc, int iSrcSize);

private:
	char	m_Key[MAX_KEY_LEN];
	int		m_iSubKey;
	int		m_iKeyLen;
};


class DECLSPECIFIER DFXform
{
	// for blowfish!
	typedef struct 
	{
		unsigned long S[4][256];
		unsigned long P[18];
	} blf_ctx; // 4168 bytes

	#define MAXKEYBYTES 56
	#define N			16
	#define noErr		0
	#define DATAERROR	-1
	#define KEYBYTES	16	// 128 bit

	#define SWAP(a,b) ((a)^=(b)^=(a)^=(b))

public:
	
	DFXform();
	virtual ~DFXform();

	void DFXform::UUEncode(const BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize);
	void DFXform::UUDecode(const BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize);

	void DFXform::XXEncode(const BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize);
	void DFXform::XXDecode(const BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize);

	// Transform Methods that are Cominations of Protected Methods. 
	void			Encode(LPBYTE pSrc, DWORD dwSrcSize, LPBYTE pDes, LPDWORD pdwDesSize);
	void			Decode(LPBYTE pSrc, DWORD dwSrcSize, LPBYTE pDes, LPDWORD pdwDesSize);

	// word size version.
	void			Encode(LPBYTE pSrc, WORD wdSrcSize, LPBYTE pDes, LPWORD pDesSize);
	void			Decode(LPBYTE pSrc, WORD wdSrcSize, LPBYTE pDes, LPWORD pwdDesSize);

	void			KeyEncrypt(LPBYTE Data);		
	void			KeyDecrypt(LPBYTE Data);
	BOOL			CompareKey(LPBYTE Key);
	void			InitializeBlowfish(LPBYTE k, DWORD len = KEYBYTES);	
	void			InitializeBlowfishWithBaseKey();

private:
	// for UUE
	void			UUPadAndEncode(const BYTE* pSrc, BYTE* pDes, UINT nPaddingSize);
	void			UUPartialEncode(const BYTE* pSrc, BYTE* pDes);
	void			UUPartialDecode(const BYTE* pSrc, BYTE* pDes);
	
	// for XXE
	void			XXPaddAndEncode(const BYTE* pSrc, BYTE* pDes, UINT nPaddingSize);
	void			XXPartialEncode(const BYTE* pSrc, BYTE* pDes);
	void			XXPartialDecode(const BYTE* pSrc, BYTE* pDes);

	// BLOWFISH Encode/Decode Methods
	void			BlowfishEncode(LPVOID pSrc, DWORD dwSrcSize);
	void			BlowfishDecode(LPVOID pSrc, DWORD dwSrcSize);

	// blowfish helpers
	unsigned long	F(unsigned long x);
	void			blowfish_encipher(unsigned long* xl, unsigned long* xr);
	void			blowfish_decipher(unsigned long* xl, unsigned long* xr);
	void			blf_enc(unsigned long* data, unsigned long blocks);
	void			blf_dec(unsigned long* data, unsigned long blocks);
	void			blf_key();

	// ctx members!
	blf_ctx			m_blf_ctx;
	int				m_nKeySize;
	BYTE			m_Key[MAXKEYBYTES]; // ¹Ì¸® ÃÖ´ëÅ©±â¸¸Å­ÀÇ key ¹öÆÛ¸¦ °®Ãá´Ù.
};

#endif 