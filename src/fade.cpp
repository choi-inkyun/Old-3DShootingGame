#include "fade.h"

extern FadeEng FADE;

LPDIRECT3DVERTEXBUFFER8 FadeVB = NULL;
VOID FadeEng::SetupMatrices() 
{
    D3DXMATRIX matWorld, matX, matY;

    D3DXMATRIX matView;
    D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
                                  &D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),
                                  &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

HRESULT FadeEng::Create()
{
    FadeVerts[0].x = 0.0f; FadeVerts[0].y = 0.0f; FadeVerts[0].z = 0.0f; FadeVerts[0].Color = 0xffff0000;
	FadeVerts[1].x = 0.0f; FadeVerts[1].y = 480.0f; FadeVerts[1].z = 0.0f; FadeVerts[1].Color = 0xffff0000;
	FadeVerts[2].x = 640.0f; FadeVerts[2].y = 0.0f; FadeVerts[2].z = 0.0f; FadeVerts[2].Color = 0xffff0000;
	FadeVerts[3].x = 640.0f; FadeVerts[3].y = 480.0f; FadeVerts[3].z = 0.0f; FadeVerts[3].Color = 0xffff0000;

	if(FAILED(g_pd3dDevice->CreateVertexBuffer(                 \
            sizeof(FadeVerts), 0, FVF3DFADE,                 \
            D3DPOOL_DEFAULT, &FadeVB)))
    return FALSE;
  if(FAILED(FadeVB->Lock(0,0, (BYTE**)&Ptr, 0)))
    return FALSE;
  memcpy(Ptr, FadeVerts, sizeof(FadeVerts));
  FadeVB->Unlock();
  return S_OK;
}

void FadeEng::FadeRender()
{
    g_pd3dDevice->BeginScene();	
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );			// Turn on Light	
	g_pd3dDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pd3dDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pd3dDevice->SetVertexShader(FVF3DFADE);
    g_pd3dDevice->SetStreamSource(0, FadeVB, sizeof(FadeVerts));
 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
    g_pd3dDevice->EndScene();	
}

HRESULT FadeEng::FadeOut()
{
	FadeVerts[0].Color = 0x55000000;
	FadeVerts[1].Color = 0x55000000;
	FadeVerts[2].Color = 0x55000000;
	FadeVerts[3].Color = 0x55000000;

	if(FAILED(g_pd3dDevice->CreateVertexBuffer(                 \
            sizeof(FadeVerts), 0, FVF3DFADE,                 \
            D3DPOOL_DEFAULT, &FadeVB)))
    return FALSE;
  if(FAILED(FadeVB->Lock(0,0, (BYTE**)&Ptr, 0)))
    return FALSE;
  memcpy(Ptr, FadeVerts, sizeof(FadeVerts));
  FadeVB->Unlock();
	return S_OK;
}