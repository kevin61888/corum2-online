#ifndef __DFXFORM_H__
#define __DFXFORM_H__


void DFOutputDebugString(LPCTSTR pFormat, ...);

class DFSimpleXForm
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

class DFXform
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

	void DFXform::UUEncode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize);
	void DFXform::UUDecode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize);

	void DFXform::XXEncode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize);
	void DFXform::XXDecode(BYTE* pSrc, UINT nSrcSize, BYTE* pDes, UINT* pDesSize);

	// Transform Methods that are Cominations of Protected Methods. 
	virtual void	Encode(LPBYTE pSrc, DWORD dwSrcSize, LPBYTE pDes, LPDWORD pdwDesSize);
	virtual void	Decode(LPBYTE pSrc, DWORD dwSrcSize, LPBYTE pDes, LPDWORD pdwDesSize);

	// word size version.
	virtual void	Encode(LPBYTE pSrc, WORD wdSrcSize, LPBYTE pDes, LPWORD pDesSize);
	virtual void	Decode(LPBYTE pSrc, WORD wdSrcSize, LPBYTE pDes, LPWORD pwdDesSize);

	virtual void	KeyEncrypt(LPBYTE Data);		
	virtual void	KeyDecrypt(LPBYTE Data);
	virtual BOOL	CompareKey(LPBYTE Key);
	void			InitializeBlowfish(LPBYTE k, DWORD len = KEYBYTES);	
	void			InitializeBlowfishWithBaseKey();

private:
	// for UUE
	void			UUPadAndEncode(BYTE* pSrc, BYTE* pDes, UINT nPaddingSize);
	void			UUPartialEncode(BYTE* pSrc, BYTE* pDes);
	void			UUPartialDecode(BYTE* pSrc, BYTE* pDes);
	
	// for XXE
	void			XXPaddAndEncode(BYTE* pSrc, BYTE* pDes, UINT nPaddingSize);
	void			XXPartialEncode(BYTE* pSrc, BYTE* pDes);
	void			XXPartialDecode(BYTE* pSrc, BYTE* pDes);

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

#endif // __DFXFORM_H__