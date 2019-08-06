#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "3D Engine [G].h"

extern Engine3DG Eng3D;

LPDIRECT3D8             g_pD3D       = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE8       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER8 g_pVB        = NULL; // Buffer to hold vertices
LPDIRECT3DTEXTURE8		pTexture[8];		 // �ؽ���..8���� �ְ�

D3DXMATRIX matViewa ,matProja ; //ī�޶�

inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); } //fog
HWND main_window_handle;
//-----------------------------------------------------------------------------
// �̸�: InitD3DWindow()
// ���: Direct3D �� Create!. �׸��� ������.. (������ ��� ����)
//-----------------------------------------------------------------------------
HRESULT Engine3DG::InitD3DWindow(HWND hWnd) 
{
 	HRESULT		hr;
	main_window_handle = hWnd;
	// Direct3D8 create
	g_pD3D = Direct3DCreate8(D3D_SDK_VERSION);

	if( g_pD3D == NULL)		
			return E_FAIL;	

	// Retreive current Display Mode
	hr = g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if(FAILED(hr))
		return E_FAIL;
	
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed   = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3ddm.Format;		
	d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	hr =  g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                  &d3dpp, &g_pd3dDevice );
	

	if(FAILED(hr))
		return E_FAIL;

	SettingMatrices();//ī�޶� ����

    return S_OK;
}
//-----------------------------------------------------------------------------
// �̸�: InitD3D()
// ���: Direct3D �� Create!. �׸��� ������.. (Ǯ��ũ�� ���)
//       flag = 0 : 640 * 480
//       flag = 1 : 800 * 600
//       flag = 2 : 1024 * 768
//-----------------------------------------------------------------------------
HRESULT Engine3DG::InitD3DFullscreen(HWND hWnd , int flag) 
{
 	HRESULT		hr;
	// Direct3D8 create
	g_pD3D = Direct3DCreate8(D3D_SDK_VERSION);

	if( g_pD3D == NULL)		
			return E_FAIL;	

	// Retreive current Display Mode
	hr = g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	if(FAILED(hr))
		return E_FAIL;
	
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed   = FALSE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3ddm.Format;		
	d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	if(flag == 0){
	d3dpp.BackBufferWidth = 640;
	d3dpp.BackBufferHeight = 480; 	}

	else if(flag == 1){
	d3dpp.BackBufferWidth = 800;
	d3dpp.BackBufferHeight = 600; 	}

	else if(flag == 2){
	d3dpp.BackBufferWidth = 1024;
	d3dpp.BackBufferHeight = 768; 	}

	hr =  g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                  &d3dpp, &g_pd3dDevice );
	if(FAILED(hr))
		return E_FAIL;

	SettingMatrices();//ī�޶����
    return S_OK;
}


//-----------------------------------------------------------------------------
// �̸�: Cleanup()
// ���: 3d�� Release ���ش�.
//-----------------------------------------------------------------------------
VOID Engine3DG::Cleanup()
{
    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}



//-----------------------------------------------------------------------------
// �̸�: SettingMatrices()
// ���: ī�޶� ������..ó���� �ѹ��� �ҷ����ָ� ��..
//-----------------------------------------------------------------------------
VOID Engine3DG::SettingMatrices(){//ī�޶� ����
    D3DXMatrixLookAtLH( &matViewa, &D3DXVECTOR3( 0.0f, 500.0f, 500.0f ),
                                  &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
                                  &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );

    D3DXMatrixPerspectiveFovLH( &matProja, D3DX_PI/4, 1.0f, 3.0f, 2000.0f );

}
//-----------------------------------------------------------------------------
// �̸�: SetupMatrices()
// ���: �⺻���� Matrices
//-----------------------------------------------------------------------------
VOID Engine3DG::SetupMatrices() //3��Ī
{

   g_pd3dDevice->SetTransform( D3DTS_VIEW, &matViewa );
	
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProja );
}
//-----------------------------------------------------------------------------
// �̸�: SetupLights()
// ���: �⺻���� Lights
//-----------------------------------------------------------------------------
void Engine3DG::SetupLights()
{
	D3DXVECTOR3		vecDir;	// Direction of Light
    D3DLIGHT8		light;
	D3DMATERIAL8	mtrl;

    ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;	

    g_pd3dDevice->SetMaterial( &mtrl );
	
    // Set up a white, directional light, with an oscillating direction.
    ZeroMemory( &light, sizeof(D3DLIGHT8) );
    light.Type       = D3DLIGHT_DIRECTIONAL;
    light.Diffuse.r  = 1.0f;
    light.Diffuse.g  = 1.0f;
    light.Diffuse.b  = 1.0f;	

	vecDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

    D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecDir );
    light.Range       = 1000.0f;
    g_pd3dDevice->SetLight( 0, &light );	// 0�� ����Ʈ ����
    g_pd3dDevice->LightEnable( 0, TRUE );	// 0�� ����Ʈ Ȱ��ȭ  
	
	g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0x00606060 );
}
//-----------------------------------------------------------------------------
// �̸�: SetRenderStateSet()
// ���: SetRenderState �� �����Ѵ�.
// flag = 0 : zbuffer true, light true
// flag = 1 : zbuffer true, light fase
// flag = 2 : zbuffer fase, light true
// flag = 3 : zbuffer fase, light fase
// flag = 4 : Tool ���� ���� ����..
//-----------------------------------------------------------------------------
void Engine3DG::SetRenderStateSet(int flag)
{
	if(flag == 0){
	    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
		g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	}
	if(flag == 1){
	    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
		g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );		
	}
	if(flag == 2){
	    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );		
		g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );		
	}
	if(flag == 3){
	    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );		
		g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );		
	}
	if(flag == 4){
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_BLENDTEXTUREALPHA );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2 );
		g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
		
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		g_pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
		
		g_pd3dDevice->SetTextureStageState( 2, D3DTSS_COLOROP,   D3DTOP_DISABLE );
		g_pd3dDevice->SetTextureStageState( 2, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	}
}
void Engine3DG::FogSet(DWORD Flag)
{
	g_dwFogMode = Flag;
	// ���� /////////////////////
	g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_FOGCOLOR,  g_dwFogColor );

	g_pd3dDevice->SetRenderState( D3DRS_FOGSTART,   FtoDW(g_fFogStartValue) );
	g_pd3dDevice->SetRenderState( D3DRS_FOGEND,     FtoDW(g_fFogEndValue) );
	g_pd3dDevice->SetRenderState( D3DRS_FOGDENSITY, FtoDW(g_fFogDensity) );

	g_pd3dDevice->SetRenderState( D3DRS_FOGVERTEXMODE,  D3DFOG_NONE );
	g_pd3dDevice->SetRenderState( D3DRS_FOGTABLEMODE,   g_dwFogMode );
	////////////////////////////
}


Engine3DG::Engine3DG()
{
		g_fFogStartValue = 250.0f; //���� ��ŸƮ ;
		g_fFogEndValue  = 1300.0f;  //���� ��;
//		g_dwFogColor  = 0x00b5b5ff;  //���� Į��; 
		g_dwFogColor  = 0xf0000000;  //���� Į��; 
		g_dwFogMode  = D3DFOG_LINEAR; //���� ���;
		g_fFogDensity = 0.0025f; //���� ��;
}

