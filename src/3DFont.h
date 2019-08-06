#ifndef __3D_FONT_H__
#define __3D_FONT_H__

#include "3D Engine [G].h"
#include "D3DApp.h"
#include "D3DFont.h"
#include "D3DUtil.h"
#include "DXUtil.h"

class Font3D
{
	public:
 // font
   CD3DFont*     m_pFont;
   CD3DFont*     m_pStatsFont;
   LPD3DXMESH    m_pMesh3DText;

   TCHAR         m_strFont[100];
   DWORD         m_dwFontSize;

   D3DXMATRIX    m_matObj1;
   D3DXMATRIX    m_matObj2;
	// ÇÑ±Û
   LPD3DXFONT		m_lpFont;

   HRESULT CreateD3DXTextMesh();
   HRESULT FontCreate();
   HRESULT HangelFontCreate();
	
   VOID FontCleanUp();
};

#endif