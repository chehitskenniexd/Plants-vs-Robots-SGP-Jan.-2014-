#pragma once
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

struct CharInfo
{
	CharInfo(): m_nX(0), m_nY(0), m_nXOffset(0), m_nYOffset(0), m_nWidth(0), m_nHeight(0), m_nXAdv(0), m_nPage(0) {}

	int m_nX, m_nY;
	int m_nWidth, m_nHeight;
	int m_nXOffset, m_nYOffset;
	int m_nXAdv;
	int m_nPage; 
};

struct CharKerning
{
	CharKerning() : m_nFirst(0), m_nSecond(0), m_nAmount(0) {}
	int m_nFirst, m_nSecond, m_nAmount;
};

class CBMFont
{
public:
	CBMFont(void);
	~CBMFont(void);

	void Initialize(const char * szFilename = "Resources/XML/BMFont.fnt");
	void Terminate(void);

	void Write(const char* strOut, int nX, int nY, float fScale, DWORD dwColor) const;
	bool LoadInfo(const char * szFontPath);

	int GetLineHeight(void) const {return m_nLineHeight;}

private:
	int m_nImageID;

	int m_nLineHeight;
	int m_nBase;
	int m_nWidth, m_nHeight;
	int m_nNumPages;
	CharInfo m_sCharInfos[256];

	std::vector<CharKerning> m_vKernings;

	//Return a rect from the texture
	RECT GlyphAlgorithm(unsigned char nID) const; 
};