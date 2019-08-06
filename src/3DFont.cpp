// 3D Font 사용법
// 기능 : 3D 로 초기화시켜준곳에다가 2D 글씨를 찍어준다. 
//        현재 영문, 한글가능
// 사용법 : HRESULT Font3D::FontCreate() 이걸로 초기화를 시켜준후
// Font.m_pStatsFont->DrawText( 10, 20, D3DCOLOR_ARGB(255,255,0,0), _T("Data Loding...") ); 같이 찍어준다.
// CleanUp 로 객체를 해제시킨다.
//  한글 출력..루프문에서 :
//		LPTSTR lptstrBuf;
//		lptstrBuf = new TCHAR[1024];
//		wsprintf(lptstrBuf, "- 3D 슈팅게임 -");
//		RECT drawRect;
//		SetRect(&drawRect, 265, 30, 450, 60);
//		Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 255, 255));
//		SAFE_DELETE(lptstrBuf);

#include "3DFont.h"

extern Font3D Font;

HRESULT Font3D::CreateD3DXTextMesh()
{
    HRESULT hr;
    HDC hdc = CreateCompatibleDC( NULL );
    HFONT hFont;
    HFONT hFontOld;
    LOGFONT lf;

    SAFE_RELEASE(m_pMesh3DText);
    ZeroMemory(&lf, sizeof(lf));
    lf.lfHeight = m_dwFontSize;
    lf.lfStrikeOut = 1;
    lstrcpy(lf.lfFaceName, m_strFont);
    hFont = CreateFontIndirect(&lf);
    hFontOld = (HFONT)SelectObject(hdc, hFont); 
    if( FAILED( hr = D3DXCreateText(g_pd3dDevice, hdc, _T("D3DXCreateText"), 
        0.001f, 0.4f, &m_pMesh3DText, NULL) ) )
    {
        SelectObject(hdc, hFontOld);
        return hr;
    }
    SelectObject(hdc, hFontOld);

    return S_OK;
}
HRESULT Font3D::FontCreate()
{
    m_pMesh3DText       = NULL;

   // Create fonts
    lstrcpy( m_strFont, _T("Arial") );
    m_dwFontSize  = 18;
    m_pFont       = new CD3DFont( m_strFont, m_dwFontSize );
    m_pStatsFont  = new CD3DFont( _T("Arial"), 12, D3DFONT_BOLD );

     // Restore the fonts
    m_pFont->InitDeviceObjects( g_pd3dDevice );
    m_pStatsFont->InitDeviceObjects( g_pd3dDevice );
    // Restore the fonts
    m_pFont->RestoreDeviceObjects();
    m_pStatsFont->RestoreDeviceObjects();

    // Restore the textures
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );

    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    g_pd3dDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
    g_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
    g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x80808080);
    D3DLIGHT8 light;
    D3DUtil_InitLight(light, D3DLIGHT_DIRECTIONAL, 10.0f, -10.0f, 10.0f);
    g_pd3dDevice->SetLight(0, &light );
    g_pd3dDevice->LightEnable(0, TRUE);

    // Set the transform matrices
    D3DXVECTOR3 vEyePt    = D3DXVECTOR3( 0.0f,-5.0f,-10.0f );
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3( 0.0f, 0.0f,  0.0f );
    D3DXVECTOR3 vUpVec    = D3DXVECTOR3( 0.0f, 1.0f,  0.0f );
    D3DXMATRIX  matWorld, matView, matProj;

    D3DXMatrixIdentity( &matWorld );
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    FLOAT fAspect = 640 / (FLOAT)480;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, fAspect, 1.0f, 100.0f );

    g_pd3dDevice->SetTransform( D3DTS_WORLD,      &matWorld );
    g_pd3dDevice->SetTransform( D3DTS_VIEW,       &matView );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

    if( FAILED( CreateD3DXTextMesh() ) )
        return E_FAIL;

	return S_OK;
}

HRESULT Font3D::HangelFontCreate()
{
	// 믁돿
	LOGFONT lFont;
	
	ZeroMemory(&lFont, sizeof(LOGFONT));
	lFont.lfHeight = 12;
	lFont.lfWidth = 0;
	lFont.lfWeight = FW_BOLD; 
	lFont.lfCharSet = SHIFTJIS_CHARSET;
	lFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lFont.lfQuality = PROOF_QUALITY;
	lFont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
	strcpy(lFont.lfFaceName, "돋움체");
	
	D3DXCreateFontIndirect(g_pd3dDevice, &lFont, &m_lpFont);
	return S_OK;
}
VOID Font3D::FontCleanUp()
{
    SAFE_RELEASE( m_pMesh3DText );
    SAFE_DELETE( m_pFont );
    SAFE_DELETE( m_pStatsFont );
	SAFE_RELEASE( m_lpFont );

}

