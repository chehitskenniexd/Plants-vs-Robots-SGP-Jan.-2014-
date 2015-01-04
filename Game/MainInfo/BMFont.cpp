#include "BMFont.h"

#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/tinyxml.h"

#include <sstream>
CBMFont::CBMFont(void)
{
	m_nImageID = -1;
}


CBMFont::~CBMFont(void)
{

}

void CBMFont::Initialize(const char* szFilename)
{
	LoadInfo(szFilename);

}

void CBMFont::Terminate(void)
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nImageID);
	m_nImageID = -1;
}

bool CBMFont::LoadInfo(const char * szFontPath)
{
	TiXmlDocument fontDoc;

	if (!fontDoc.LoadFile(szFontPath))
		return false;

	TiXmlElement* pRoot = fontDoc.RootElement();

	if (!pRoot)
		return false;

	TiXmlElement* pCommon = pRoot->FirstChildElement("common");
	pCommon->Attribute("lineHeight", &m_nLineHeight);
	pCommon->Attribute("base", &m_nBase);
	pCommon->Attribute("scaleW", &m_nWidth);
	pCommon->Attribute("scaleH", &m_nHeight);
	pCommon->Attribute("pages", &m_nNumPages);
	
	TOSTRINGSTREAM sImg;
	sImg << "Resources/" << pRoot->FirstChildElement("pages")->FirstChildElement("page")->Attribute("file");

	m_nImageID = CSGD_TextureManager::GetInstance()->LoadTexture(sImg.str().c_str());

	TiXmlElement* pChar = pRoot->FirstChildElement("chars")->FirstChildElement("char");
	int i = 0;

	while (pChar)
	{
		pChar->Attribute("id", &i);

		pChar->Attribute("x", &m_sCharInfos[i].m_nX);
		pChar->Attribute("y", &m_sCharInfos[i].m_nY);
		pChar->Attribute("width", &m_sCharInfos[i].m_nWidth);
		pChar->Attribute("height", &m_sCharInfos[i].m_nHeight);
		pChar->Attribute("xoffset", &m_sCharInfos[i].m_nXOffset);
		pChar->Attribute("yoffset", &m_sCharInfos[i].m_nYOffset);
		pChar->Attribute("xadvance", &m_sCharInfos[i].m_nXAdv);

		pChar = pChar->NextSiblingElement("char");
	}

	TiXmlElement* pKerning = pRoot->FirstChildElement("kernings")->FirstChildElement("kerning");
	CharKerning tempKerning;

	while (pKerning)
	{
		pKerning->Attribute("first", &tempKerning.m_nFirst);
		pKerning->Attribute("second", &tempKerning.m_nSecond);
		pKerning->Attribute("amount", &tempKerning.m_nAmount);

		m_vKernings.push_back(tempKerning);
		pKerning = pKerning->NextSiblingElement("kerning");
	}

	return true;
}

//This function takes in a subscript for the array m_sCharInfos and returns a RECT encompassing the position of character nID within the texture m_nImgID.

RECT CBMFont::GlyphAlgorithm(unsigned char nID) const
{
	CharInfo c = m_sCharInfos[nID];
	RECT glyph = {c.m_nX, c.m_nY, c.m_nX + c.m_nWidth, c.m_nY + c.m_nHeight};
	return glyph;
}

void CBMFont::Write(const char* strOut, int nX, int nY, float fScale, DWORD dwColor) const
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance(); //Convenience pointer
	
	unsigned char currChar;

	int nCursX = nX, nCursY = nY; //Cursor position
	int nCount = 0; //For tab
	for (unsigned int i = 0; strOut[i] != '\0'; i++)
	{
		currChar = static_cast<unsigned char>(strOut[i]);

		switch (currChar)
		{
		case ' ':
			nCursX += m_sCharInfos[' '].m_nWidth;
			break;
		case '\n':
			nCursX = nX;
			nCursY += static_cast<int>(m_nLineHeight * fScale);
			nCount = 0;
			break;
		case '\t':
			nCursX += (4 - nCount % 4) * m_sCharInfos[' '].m_nWidth;
			nCount += 4 - nCount % 4;
			break;
		default:
			int nTempY = nCursY +  static_cast<int>(fScale * m_sCharInfos[currChar].m_nYOffset);
			pTM->Draw(m_nImageID, nCursX, nTempY, fScale, fScale, &GlyphAlgorithm(currChar), 0, 0, 0, dwColor);

			nCursX += static_cast<int>(fScale * m_sCharInfos[currChar].m_nWidth);
			nCount++;
			break;
		}
	}

}
