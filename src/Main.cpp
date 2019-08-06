#include "Main.h"

Engine3DG Eng3D; //상속
MusicEngine MEng;
SoundEngine SoEng;
C_H3DCOLL cndehf;
Font3D Font;
FadeEng FADE;

BYTE *Ptr;
int Fadecount = 0;
int FadeCheck = 0;
int Fadeout = 0;

FLOAT STAGE1CLEAR = 0.0f;
/// 초기값 셋팅	
/// Game = Main.!!
HRESULT InitGeometry2(){
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );			// Turn on the zbuffer
	g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );	// Turn off culling	
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );			// Turn on Light	
	
	cASE[0].ReadASE("air3.ase"); //주인공
	cASE[0].Animate(0);
	cASE[1].ReadASE("aaa.ase"); //행성들
	cASE[1].Animate(0);
	cASE[2].ReadASE("aaa2.ase"); //행성들
	cASE[2].Animate(0);
	cASE[3].ReadASE("aaa3.ase");
	cASE[3].Animate(0);

	cASE[4].ReadASE("air4.ase"); //적비행기
	cASE[4].Animate(0);

	cASE[5].ReadASE("air5.ase"); //적비행기
	cASE[5].Animate(0);

	cASE[6].ReadASE("altkdlf.ase");  //미사일
	cASE[6].Animate(0);

	cASE[7].ReadASE("tank.ASE");  //탱크 & 배경.
	cASE[7].Animate(0);

	cASE[8].ReadASE("wjraltkdlf.ase");  //적미사일
	cASE[8].Animate(0);

	g_pd3dDevice->Clear( 0, NULL,  D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,Eng3D.g_dwFogColor, 1.0f, 0 );
    Font.m_pStatsFont->DrawText( 10, 20, D3DCOLOR_ARGB(255,255,0,0), _T("Data Loding...") );
    Font.m_pStatsFont->DrawText( 280, 300, D3DCOLOR_ARGB(255,255,255,0), _T("- Wait Please -") );
    Font.m_pStatsFont->DrawText( 10, 40, D3DCOLOR_ARGB(255,0,255,0), _T("Character load success") );
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	D3DXMatrixIdentity( &matMove );
	D3DXMatrixIdentity( &matMove2 );
	D3DXMatrixIdentity( &matMove3 );
	D3DXMatrixIdentity( &matMove4 );
	D3DXMatrixIdentity( &matMove5 );
	
	D3DXMatrixIdentity( &matMove6 );
	D3DXMatrixIdentity( &matMove7 );
	D3DXMatrixIdentity( &matMove8 );
	D3DXMatrixIdentity( &matMove9 );
	D3DXMatrixIdentity( &matMove10 );

	D3DXMatrixIdentity( &matMove11 );
	D3DXMatrixIdentity( &matMove12 );
	D3DXMatrixIdentity( &matMove13 );
	for(int i = 0; i < 10; i++){
	D3DXMatrixIdentity( &matMove14[i] );
	D3DXMatrixIdentity( &matMove15[i] );
	D3DXMatrixIdentity( &matMove29[i] );
	D3DXMatrixIdentity( &matMove30[i] );
	D3DXMatrixIdentity( &matMove33[i] );  //적 미사일.
	}
	D3DXMatrixIdentity( &matMove16 );
	D3DXMatrixIdentity( &matMove17 );

	D3DXMatrixIdentity( &matMove18 );
	D3DXMatrixIdentity( &matMove19 );
	D3DXMatrixIdentity( &matMove20 );
	D3DXMatrixIdentity( &matMove21 );

	D3DXMatrixIdentity( &matMove22 );//총알 바운딩박스 주인공
	D3DXMatrixIdentity( &matMove23 );//총알 바운딩박스 적비행기
	D3DXMatrixIdentity( &matMove24 );//총알 바운딩박스 적비행기
	D3DXMatrixIdentity( &matMove25 );//총알 바운딩박스 적비행기
	D3DXMatrixIdentity( &matMove26 );//총알 바운딩박스 적비행기2
	D3DXMatrixIdentity( &matMove27 );//총알 바운딩박스 적비행기2
	D3DXMatrixIdentity( &matMove28 );//총알 바운딩박스 적비행기2

	D3DXMatrixIdentity( &matMove31 );//텍스쳐..충돌시 임펙트
	
	D3DXMatrixIdentity( &matMove32 );//탱크..

	D3DXMatrixIdentity( &matMove36 );//gameover
	D3DXMatrixIdentity( &matMove37 );//적비행기 충돌체크

	D3DXMatrixIdentity( &matMove38 );//프롤로그 
	matMove2._43 = -2000.0f;
	matMove3._43 = -3000.0f;
	matMove4._43 = -2500.0f;

	matMove10._43 = -2500.0f;
	matMove6._43 = -3000.0f;
	matMove7._43 = -2500.0f;
	matMove8._43 = -2400.0f;
	matMove9._43 = -3000.0f;

	matMove10._42 = -50.0f;
	matMove6._42 = -80.0f;
	matMove7._42 = -35.0f;
	matMove8._42 = 60.0f;
	matMove9._42 = 30.0f;

	matMove10._41 = -100.0f;
	matMove6._41 = 60.0f;
	matMove7._41 = -30.0f;
	matMove8._41 = 30.0f;
	matMove9._41 = -10.0f;

 	matMove11._43 = -2000.0f;
	matMove12._43 = -2500.0f;
	matMove13._43 = -1500.0f;
   
	matMove11._42 = -30.0f;
	matMove12._42 = -10.0f;
	matMove13._42 =  80.0f;

	matMove11._41 =  20.0f;
	matMove12._41 =  -65.0f;
	matMove13._41 =  95.0f;

	D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 5.0f, 2000.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	g_pd3dDevice->Clear( 0, NULL,  D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,Eng3D.g_dwFogColor, 1.0f, 0 );
    Font.m_pStatsFont->DrawText( 10, 20, D3DCOLOR_ARGB(255,255,0,0), _T("Data Loding...") );
    Font.m_pStatsFont->DrawText( 280, 300, D3DCOLOR_ARGB(255,255,255,0), _T("- Wait Please -") );
    Font.m_pStatsFont->DrawText( 10, 40, D3DCOLOR_ARGB(255,0,255,0), _T("Character load success") );
    Font.m_pStatsFont->DrawText( 10, 60, D3DCOLOR_ARGB(255,0,255,0), _T("Setting success") );
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	
	s3DVertex BillboardVerts[4] = {
        { -50.0f, -50.0f, 20.0f,0.0f,1.0f, },
        { -50.0f, -35.0f, 20.0f,0.0f,0.0f, },
        { 50.0f,  -50.0f, 20.0f,1.0f,1.0f, },
        { 50.0f,  -35.0f, 20.0f,1.0f,0.0f, }, //0xffffffff
    };

 if(FAILED(g_pd3dDevice->CreateVertexBuffer(                 \
            sizeof(BillboardVerts), 0, FVF3D,                 \
            D3DPOOL_DEFAULT, &g_pBillboardVB)))
    return FALSE;
  if(FAILED(g_pBillboardVB->Lock(0,0, (BYTE**)&Ptr, 0)))
    return FALSE;
  memcpy(Ptr, BillboardVerts, sizeof(BillboardVerts));
  g_pBillboardVB->Unlock();

	D3DUtil_CreateTexture( g_pd3dDevice, "aaa.tga", &g_pBillboardTexture);
	
  s3DVertex BillboardVerts2[4] = {
        { -500.0f, -100.0f, -500.0f,0.0f,1.0f, },
        { -500.0f, -100.0f, 500.0f,0.0f,0.0f, },
        { 500.0f,  -100.0f, -500.0f,1.0f,1.0f, },
        { 500.0f,  -100.0f, 500.0f,1.0f,0.0f, }, //0xffffffff
    };

  if(FAILED(g_pd3dDevice->CreateVertexBuffer(                 \
            sizeof(BillboardVerts2), 0, FVF3D,                 \
            D3DPOOL_DEFAULT, &g_pVB2)))
    return FALSE;
  if(FAILED(g_pVB2->Lock(0,0, (BYTE**)&Ptr, 0)))
    return FALSE;
  memcpy(Ptr, BillboardVerts2, sizeof(BillboardVerts2));
  g_pVB2->Unlock();

	D3DUtil_CreateTexture( g_pd3dDevice, "alfksgkxmzpd_10.jpg", &pTexture2[0]);


    s3DVertex BillboardVerts3[4] = {
        { -700.0f, -100.0f, -1100.0f,0.0f,1.0f, },
        { -700.0f, -100.0f, 1100.0f,0.0f,0.0f, },
        { 700.0f,  -100.0f, -1100.0f,1.0f,1.0f, },
        { 700.0f,  -100.0f, 1100.0f,1.0f,0.0f, }, //0xffffffff
    };

  if(FAILED(g_pd3dDevice->CreateVertexBuffer(                 \
            sizeof(BillboardVerts3), 0, FVF3D,                 \
            D3DPOOL_DEFAULT, &g_pVB3)))
    return FALSE;
  if(FAILED(g_pVB3->Lock(0,0, (BYTE**)&Ptr, 0)))
    return FALSE;
  memcpy(Ptr, BillboardVerts3, sizeof(BillboardVerts3));
  g_pVB3->Unlock();
	

     s3DVertex BillboardVerts4[4] = {
        {  -20.0f, -20.0f, 0.0f,0.0f,1.0f, },
        { -20.0f, 20.0f, 0.0f, 0.0f,0.0f, },
        {  20.0f, -20.0f, 0.0f,1.0f,1.0f, },  //0xffffffff
        {  20.0f, 20.0f, 0.0f, 1.0f,0.0f, },
  };
	D3DUtil_CreateTexture( g_pd3dDevice, "altkdlfeffer01.tga", &g_pBillboardTexture2[0]);
	D3DUtil_CreateTexture( g_pd3dDevice, "altkdlfeffer02.tga", &g_pBillboardTexture2[1]);
	D3DUtil_CreateTexture( g_pd3dDevice, "altkdlfeffer03.tga", &g_pBillboardTexture2[2]);
	D3DUtil_CreateTexture( g_pd3dDevice, "altkdlfeffer04.tga", &g_pBillboardTexture2[3]);
	D3DUtil_CreateTexture( g_pd3dDevice, "altkdlfeffer05.tga", &g_pBillboardTexture2[4]);
	D3DUtil_CreateTexture( g_pd3dDevice, "altkdlfeffer06.tga", &g_pBillboardTexture2[5]);
	D3DUtil_CreateTexture( g_pd3dDevice, "altkdlfeffer07.tga", &g_pBillboardTexture2[6]);

	if(FAILED(g_pd3dDevice->CreateVertexBuffer(                 \
            sizeof(BillboardVerts4), 0, FVF3D,                 \
            D3DPOOL_DEFAULT, &g_pVB4)))
    return FALSE;
  if(FAILED(g_pVB4->Lock(0,0, (BYTE**)&Ptr, 0)))
    return FALSE;
  memcpy(Ptr, BillboardVerts4, sizeof(BillboardVerts4));
  g_pVB4->Unlock();


     s3DVertex BillboardVerts7[4] = {
        {  -20.0f, -20.0f, 0.0f,0.0f,1.0f, },
        { -20.0f, 20.0f, 0.0f, 0.0f,0.0f, },
        {  20.0f, -20.0f, 0.0f,1.0f,1.0f, },  //0xffffffff
        {  20.0f, 20.0f, 0.0f, 1.0f,0.0f, },
  };

	D3DUtil_CreateTexture( g_pd3dDevice, "boom001.tga", &g_pBillboardTexture3[0]);
	D3DUtil_CreateTexture( g_pd3dDevice, "boom002.tga", &g_pBillboardTexture3[1]);
	D3DUtil_CreateTexture( g_pd3dDevice, "boom003.tga", &g_pBillboardTexture3[2]);
	D3DUtil_CreateTexture( g_pd3dDevice, "boom004.tga", &g_pBillboardTexture3[3]);
	D3DUtil_CreateTexture( g_pd3dDevice, "boom005.tga", &g_pBillboardTexture3[4]);
	D3DUtil_CreateTexture( g_pd3dDevice, "boom006.tga", &g_pBillboardTexture3[5]);
	D3DUtil_CreateTexture( g_pd3dDevice, "boom007.tga", &g_pBillboardTexture3[6]);

	if(FAILED(g_pd3dDevice->CreateVertexBuffer(                 \
            sizeof(BillboardVerts7), 0, FVF3D,                 \
            D3DPOOL_DEFAULT, &g_pVB7)))
    return FALSE;
  if(FAILED(g_pVB7->Lock(0,0, (BYTE**)&Ptr, 0)))
    return FALSE;
  memcpy(Ptr, BillboardVerts7, sizeof(BillboardVerts7));
  g_pVB7->Unlock();
  
 
	D3DUtil_CreateTexture( g_pd3dDevice, "gameover.tga", &g_pBillboardTexture5);

     s3DVertex BillboardVerts10[4] = {
        {  -200.0f, -200.0f, 0.0f,1.0f,1.0f, },
        { -200.0f, 200.0f, 0.0f, 1.0f,0.0f, },
        {  200.0f, -200.0f, 0.0f,0.0f,1.0f, },  //0xffffffff
        {  200.0f, 200.0f, 0.0f, 0.0f,0.0f, },
  };

	if(FAILED(g_pd3dDevice->CreateVertexBuffer(                 \
            sizeof(BillboardVerts10), 0, FVF3D,                 \
            D3DPOOL_DEFAULT, &g_pVB10)))
    return FALSE;
  if(FAILED(g_pVB10->Lock(0,0, (BYTE**)&Ptr, 0)))
    return FALSE;
  memcpy(Ptr, BillboardVerts10, sizeof(BillboardVerts10));
  g_pVB10->Unlock();

  
  Eng3D.FogSet(D3DFOG_LINEAR); //안개

   g_pd3dDevice->BeginStateBlock();
   g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   FALSE);//칼라키
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,  D3DTADDRESS_CLAMP );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,  D3DTADDRESS_CLAMP );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );	
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );			// Turn on Light	
   g_pd3dDevice->EndStateBlock(&h_SaveBlock);   
    
   g_pd3dDevice->BeginStateBlock();
   g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ); //칼라//
   g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
   g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1 );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSU,  D3DTADDRESS_CLAMP );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_ADDRESSV,  D3DTADDRESS_CLAMP );
   g_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );			// Turn on Light	
   g_pd3dDevice->EndStateBlock(&h_SaveBlock2);   
	
	matMove17._43 = -2200.0f;
	matMove._43 = 300.0f;

	matMove32._43 = -1000.0f;
	matMove32._42 = -50.0f;
	g_pd3dDevice->Clear( 0, NULL,  D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,Eng3D.g_dwFogColor, 1.0f, 0 );
    Font.m_pStatsFont->DrawText( 10, 20, D3DCOLOR_ARGB(255,255,0,0), _T("Data Loding...") );
    Font.m_pStatsFont->DrawText( 280, 300, D3DCOLOR_ARGB(255,255,255,0), _T("- Wait Please -") );
    Font.m_pStatsFont->DrawText( 10, 40, D3DCOLOR_ARGB(255,0,255,0), _T("Character load success") );
    Font.m_pStatsFont->DrawText( 10, 60, D3DCOLOR_ARGB(255,0,255,0), _T("Setting success") );
    Font.m_pStatsFont->DrawText( 10, 80, D3DCOLOR_ARGB(255,0,255,0), _T("Texture load success") );
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	sound_ids[1] = SoEng.DSound_Load_WAV("MONSTER.WAV"); //wav load
	sound_ids[0] = SoEng.DSound_Load_WAV("THUNDER2.WAV"); //wav load
    midi_ids[0] = MEng.DMusic_Load_MIDI("ending.mid"); //mid load
	MEng.DMusic_Play(midi_ids[0]);	//mid 재생

	cndehf.MAKE_BoundBox(30,15,30,Object[0]);
	cndehf.H3D_Up_Data(Object[0], matMove20);
	cndehf.MAKE_BoundBox(20,10,10,Object[1]);
	cndehf.H3D_Up_Data(Object[1], matMove21);
	cndehf.MAKE_BoundBox(20,10,10,Object[2]);
	cndehf.H3D_Up_Data(Object[2], matMove22);
	cndehf.MAKE_BoundBox(20,10,10,Object[3]);
	cndehf.H3D_Up_Data(Object[3], matMove23);
	cndehf.MAKE_BoundBox(20,10,10,Object[4]);
	cndehf.H3D_Up_Data(Object[4], matMove24);
	cndehf.MAKE_BoundBox(20,10,10,Object[5]);
	cndehf.H3D_Up_Data(Object[5], matMove25);

	cndehf.MAKE_BoundBox(60,20,30,Object[6]);
	cndehf.H3D_Up_Data(Object[6], matMove26);
	cndehf.MAKE_BoundBox(60,20,30,Object[7]);
	cndehf.H3D_Up_Data(Object[7], matMove27);
	cndehf.MAKE_BoundBox(60,20,30,Object[8]);
	cndehf.H3D_Up_Data(Object[8], matMove28);

	for( i = 0; i < 10; i++){
	cndehf.MAKE_BoundBox(10,10,10,Object[i + 9]);
	cndehf.H3D_Up_Data(Object[i + 9], matMove29[i]);
	cndehf.MAKE_BoundBox(10,10,10,Object[i + 19]);
	cndehf.H3D_Up_Data(Object[i + 19], matMove30[i]);
	}

	g_pd3dDevice->Clear( 0, NULL,  D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,Eng3D.g_dwFogColor, 1.0f, 0 );
    Font.m_pStatsFont->DrawText( 10, 20, D3DCOLOR_ARGB(255,255,0,0), _T("Data Loding...") );
    Font.m_pStatsFont->DrawText( 280, 300, D3DCOLOR_ARGB(255,255,255,0), _T("- Wait Please -") );
    Font.m_pStatsFont->DrawText( 10, 40, D3DCOLOR_ARGB(255,0,255,0), _T("Character load success") );
    Font.m_pStatsFont->DrawText( 10, 60, D3DCOLOR_ARGB(255,0,255,0), _T("Setting success") );
    Font.m_pStatsFont->DrawText( 10, 80, D3DCOLOR_ARGB(255,0,255,0), _T("Texture load success") );
    Font.m_pStatsFont->DrawText( 10, 100, D3DCOLOR_ARGB(255,0,255,0), _T("Sound & Bound success. OK") );
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	
/*    s3DVertex2 BillboardVerts5[8] = {
        {  Object[10].Poi[0].x , Object[10].Poi[0].y, Object[10].Poi[0].z,0xffff0000, },
        {  Object[10].Poi[1].x , Object[10].Poi[1].y, Object[10].Poi[1].z,0xffff0000, },
        {  Object[10].Poi[2].x , Object[10].Poi[2].y, Object[10].Poi[2].z,0xffff0000, },
        {  Object[10].Poi[3].x , Object[10].Poi[3].y, Object[10].Poi[3].z,0xffff0000, },
        {  Object[10].Poi[4].x , Object[10].Poi[4].y, Object[10].Poi[4].z,0xffff0000, },
        {  Object[10].Poi[5].x , Object[10].Poi[5].y, Object[10].Poi[5].z,0xffff0000, },
        {  Object[10].Poi[6].x , Object[10].Poi[6].y, Object[10].Poi[6].z,0xffff0000, },
        {  Object[10].Poi[7].x , Object[10].Poi[7].y, Object[10].Poi[7].z,0xffff0000, },
  };
  if(FAILED(g_pd3dDevice->CreateVertexBuffer(                 \
            sizeof(BillboardVerts5), 0, FVF3D2,                 \
            D3DPOOL_DEFAULT, &g_pVB5)))
    return FALSE;
  if(FAILED(g_pVB5->Lock(0,0, (BYTE**)&Ptr, 0)))
    return FALSE;
  memcpy(Ptr, BillboardVerts5, sizeof(BillboardVerts5));
  g_pVB5->Unlock();
	
     s3DVertex2 BillboardVerts6[8] = {
        {  Object[8].Poi[0].x , Object[8].Poi[0].y, Object[8].Poi[0].z,0xff00ff00, },
        {  Object[8].Poi[1].x , Object[8].Poi[1].y, Object[8].Poi[1].z,0xff00ff00, },
        {  Object[8].Poi[2].x , Object[8].Poi[2].y, Object[8].Poi[2].z,0xff00ff00, },
        {  Object[8].Poi[3].x , Object[8].Poi[3].y, Object[8].Poi[3].z,0xff00ff00, },
        {  Object[8].Poi[4].x , Object[8].Poi[4].y, Object[8].Poi[4].z,0xff00ff00, },
        {  Object[8].Poi[5].x , Object[8].Poi[5].y, Object[8].Poi[5].z,0xff00ff00, },
        {  Object[8].Poi[6].x , Object[8].Poi[6].y, Object[8].Poi[6].z,0xff00ff00, },
        {  Object[8].Poi[7].x , Object[8].Poi[7].y, Object[8].Poi[7].z,0xff00ff00, },
  };
	
  if(FAILED(g_pd3dDevice->CreateVertexBuffer(                 \
            sizeof(BillboardVerts6), 0, FVF3D2,                 \
            D3DPOOL_DEFAULT, &g_pVB6)))
    return FALSE;
  if(FAILED(g_pVB6->Lock(0,0, (BYTE**)&Ptr, 0)))
    return FALSE;
  memcpy(Ptr, BillboardVerts6, sizeof(BillboardVerts6));
  g_pVB6->Unlock();
*/	
	GameMode = Main;

	return S_OK;
}
HRESULT InitGeometry()
{
	ShowCursor(FALSE);

	BillboardVerts8[0].x =-10.0f; BillboardVerts8[0].y =-10.0f; BillboardVerts8[0].z = 0.0f; BillboardVerts8[0].Color = 0x00000000;
	BillboardVerts8[1].x =-10.0f; BillboardVerts8[1].y = 10.0f; BillboardVerts8[1].z = 0.0f; BillboardVerts8[1].Color = 0x00000000;
	BillboardVerts8[2].x = 10.0f; BillboardVerts8[2].y =-10.0f; BillboardVerts8[2].z = 0.0f; BillboardVerts8[2].Color = 0x00000000;
	BillboardVerts8[3].x = 10.0f; BillboardVerts8[3].y = 10.0f; BillboardVerts8[3].z = 0.0f; BillboardVerts8[3].Color = 0x00000000;
 
 	if(FAILED(g_pd3dDevice->CreateVertexBuffer(                 \
            sizeof(BillboardVerts8), 0, FVF3D2,                 \
            D3DPOOL_DEFAULT, &g_pVB8)))
    return FALSE;
  if(FAILED(g_pVB8->Lock(0,0, (BYTE**)&Ptr, 0)))
    return FALSE;
  memcpy(Ptr, BillboardVerts8, sizeof(BillboardVerts8));
  g_pVB8->Unlock();

	D3DUtil_CreateTexture( g_pd3dDevice, "cgi24_1.jpg", &g_pBillboardTexture4);

    s3DVertex BillboardVerts9[4] = {
        { -10.0f, -10.0f, 0.0f,0.0f,1.0f, },
        { -10.0f,  10.0f, 0.0f,0.0f,0.0f, },
        { 10.0f,  -10.0f, 0.0f,1.0f,1.0f, },
        { 10.0f,   10.0f, 0.0f,1.0f,0.0f, },
    };

  if(FAILED(g_pd3dDevice->CreateVertexBuffer(                 \
            sizeof(BillboardVerts9), 0, FVF3D,                 \
            D3DPOOL_DEFAULT, &g_pVB9)))
    return FALSE;
  if(FAILED(g_pVB9->Lock(0,0, (BYTE**)&Ptr, 0)))
    return FALSE;
  memcpy(Ptr, BillboardVerts9, sizeof(BillboardVerts9));
  g_pVB9->Unlock();
  
   D3DXMatrixIdentity( &matMove34 );//프롤로그 배경 스크롤..
   D3DXMatrixIdentity( &matMove35 );//프롤로그 배경 스크롤..
   MEng.DMusic_Init(); //mid create
	SoEng.DSound_Init(); //sound create
	sound_ids[1] = SoEng.DSound_Load_WAV("BOOM.WAV"); //wav load
	sound_ids[0] = SoEng.DSound_Load_WAV("OBAKEDOA.WAV"); //wav load
    midi_ids[0] = MEng.DMusic_Load_MIDI("Title.mid"); //mid load
	MEng.DMusic_Play(midi_ids[0]);	//mid 재생
	Font.FontCreate();
	Font.HangelFontCreate();

  
  return S_OK;
}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------

VOID Cleanup()
{
    if( g_pVB != NULL )
        g_pVB->Release();

    if( g_pVB2 != NULL )
        g_pVB2->Release();

    if( g_pVB3 != NULL )
        g_pVB3->Release();

    if( g_pVB4 != NULL )
        g_pVB4->Release();

    if( g_pVB5 != NULL )
        g_pVB5->Release();

    if( g_pVB6 != NULL )
        g_pVB6->Release();

    if( g_pVB7 != NULL )
        g_pVB7->Release();
	
    if( g_pVB8 != NULL )
        g_pVB8->Release();

    if( g_pVB9 != NULL )
        g_pVB9->Release();

	Font.FontCleanUp();
	ShowCursor(TRUE);
 
	Eng3D.Cleanup();
}

void ASERender(int j)
{
	g_pd3dDevice->CaptureStateBlock(h_SaveBlock4);

	g_pd3dDevice->ApplyStateBlock(h_SaveBlock);

	int id;
	static int a = 0;
		if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
		{
	if(bRead) //애니메이션 재생부분
	{	
		if(AnimationCheck1 == 1)
        AnimationNumber(j);
		if(AnimationCheck2 == 1)
        AnimationNumber(j);
	}
	else{ //아닐때는... 자동적으로 돌아오도록 애니메이션..
		if(indexRate != 0 && indexRate != 8000 && indexRate != 16000){
			if(indexRate > 0 && indexRate < 4000){
				bbb = indexRate;
				indexRate = 8000 - bbb;
			}
			if(indexRate > 8000 && indexRate < 12000){
				bbb = 16000 - indexRate;
				indexRate = 8000 +  bbb;
			}
			if(indexRate != 4000 && indexRate != 12000)
				AnimationNumber(0);
			else
				indexRate-=160;
		}
		else{
		if(AnimationCheck1 == 1)
			AnimationCheck1 = 0;
		if(AnimationCheck2 == 1)
			AnimationCheck2 = 0;
		
		if(indexRate >= 16000)
			indexRate = 0;
		}
	}
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove );
		for( DWORD i=0; i<cASE[j].m_dwTotalGObject ; i++)
		{
			id = cASE[j].m_pGObject[i].m_dwMaterialREF;
			if( id != -1 )	// 지정된Tjt매터리얼이 없나?
			{
				g_pd3dDevice->SetMaterial( & cASE[j].m_pMaterial[id].m_material );
			}

			g_pd3dDevice->SetVertexShader(D3DFVF_CUSTOMVERTEX);	
			g_pd3dDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 
												   0,
												   cASE[j].m_pGObject[i].m_dwVertexCount,
												   cASE[j].m_pGObject[i].m_dwFaceCount,
												   (void *)cASE[j].m_pGObject[i].m_pFaceList,
												   D3DFMT_INDEX16,
												   (void *)cASE[j].m_pGObject[i].m_pVertexCashe,
												   sizeof(Vertex) );			
		}
			g_pd3dDevice->EndScene();
			a = 0;
	}

	g_pd3dDevice->ApplyStateBlock(h_SaveBlock4);
}

void ASERender2(int j, int flag)

{
	int id;
		if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
		{
		if(flag == 0)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove2 );
		if(flag == 1)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove3 );
		if(flag == 2)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove4 );
		if(flag == 3)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove10 );
		if(flag == 4)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove6 );
		if(flag == 5)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove7 );
		if(flag == 6)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove8 );
		if(flag == 7)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove9 );
		if(flag == 8)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove11 );
		if(flag == 9)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove12 );
		if(flag == 10)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove13 );
		if(flag == 11)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove14[0]);
		if(flag == 12)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove14[1]);
		if(flag == 13)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove14[2]);
		if(flag == 14)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove14[3]);
		if(flag == 15)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove14[4]);
		if(flag == 16)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove14[5]);
		if(flag == 17)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove14[6]);
		if(flag == 18)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove14[7]);
		if(flag == 19)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove14[8]);
		if(flag == 20)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove14[9]);
		if(flag == 21)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove14[10]);

		if(flag == 22)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove15[0]);
		if(flag == 23)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove15[1]);
		if(flag == 24)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove15[2]);
		if(flag == 25)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove15[3]);
		if(flag == 26)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove15[4]);
		if(flag == 27)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove15[5]);
		if(flag == 28)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove15[6]);
		if(flag == 29)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove15[7]);
		if(flag == 30)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove15[8]);
		if(flag == 31)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove15[9]);
		if(flag == 32)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove15[10]);
		if(flag == 33)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove32);
		if(flag == 34)
		g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove33[0]);
	
		for( DWORD i=0; i<cASE[j].m_dwTotalGObject ; i++)
		{
			id = cASE[j].m_pGObject[i].m_dwMaterialREF;
			if( id != -1 )	// 지정된 매터리얼이 없나?
			{
				g_pd3dDevice->SetMaterial( & cASE[j].m_pMaterial[id].m_material );
			}

			g_pd3dDevice->SetVertexShader(D3DFVF_CUSTOMVERTEX);	
			g_pd3dDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 
												   0,
												   cASE[j].m_pGObject[i].m_dwVertexCount,
												   cASE[j].m_pGObject[i].m_dwFaceCount,
												   (void *)cASE[j].m_pGObject[i].m_pFaceList,
												   D3DFMT_INDEX16,
												   (void *)cASE[j].m_pGObject[i].m_pVertexCashe,
												   sizeof(Vertex) );			
		}
		
		g_pd3dDevice->EndScene();
	}

}
void GameOverRender()
{
	static int count = 0;

	count++;
	Eng3D.SetupMatrices();
	for(int i = 0; i < 10; i++){
	cndehfwndlsrhd[i] = 0;
	}

	g_pd3dDevice->CaptureStateBlock(h_SaveBlock5);

	g_pd3dDevice->ApplyStateBlock(h_SaveBlock2);

	g_pd3dDevice->BeginScene();
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove36 );
    g_pd3dDevice->SetVertexShader(FVF3D);
    g_pd3dDevice->SetStreamSource(0, g_pVB10, sizeof(s3DVertex));
    g_pd3dDevice->SetTexture(0, g_pBillboardTexture5);

    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	g_pd3dDevice->EndScene();
    g_pd3dDevice->SetTexture(0, NULL);
	
	g_pd3dDevice->ApplyStateBlock(h_SaveBlock5);

	if(count > allcount)
		PostQuitMessage(0);
}
void backRender(int j)
{
    D3DXMATRIX matView;
 
 /*    D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3( 0.0f, 0.0f, 300.0f ),
                                  &D3DXVECTOR3( 0.0f, 0.0f, -100.0f ),
                                  &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
  g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
   g_pd3dDevice->BeginScene();
    // Draw the billboards
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove16 );
    g_pd3dDevice->SetVertexShader(FVF3D);
    g_pd3dDevice->SetStreamSource(0, g_pVB2, sizeof(s3DVertex));
    g_pd3dDevice->SetTexture(0, pTexture2[0]);

 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	g_pd3dDevice->EndScene();

   */
     D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3( 0.0f, 500.0f, 400.0f ),
                                  &D3DXVECTOR3( 0.0f, 0.0f, -100.0f ),
                                  &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
	 
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
   g_pd3dDevice->BeginScene();
   if(j == 0)
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove16 );
   if(j == 1)
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove17 );

   g_pd3dDevice->SetVertexShader(FVF3D);
    g_pd3dDevice->SetStreamSource(0, g_pVB3, sizeof(s3DVertex));
    g_pd3dDevice->SetTexture(0, pTexture2[0]);

 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	g_pd3dDevice->EndScene();
 	
	g_pd3dDevice->SetTexture(0, NULL);
}
void fierRender(int i)
{
	Eng3D.SetupMatrices();

	g_pd3dDevice->CaptureStateBlock(h_SaveBlock5);

	g_pd3dDevice->ApplyStateBlock(h_SaveBlock2);
	
	matMove18._41 = cASE[0].m_pGObject->m_pVertexCashe[314].x - 2.0f + matMove._41;
	matMove18._42 = cASE[0].m_pGObject->m_pVertexCashe[314].y - 3.0f + matMove._42;
	matMove18._43 = cASE[0].m_pGObject->m_pVertexCashe[314].z - 2.0f + matMove._43;
	matMove19._41 = cASE[0].m_pGObject->m_pVertexCashe[224].x - 1.0f + matMove._41;
	matMove19._42 = cASE[0].m_pGObject->m_pVertexCashe[224].y - 3.0f + matMove._42;
	matMove19._43 = cASE[0].m_pGObject->m_pVertexCashe[224].z - 2.0f + matMove._43;

	g_pd3dDevice->BeginScene();

	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove18 );
    g_pd3dDevice->SetVertexShader(FVF3D);
    g_pd3dDevice->SetStreamSource(0, g_pVB4, sizeof(s3DVertex));
    g_pd3dDevice->SetTexture(0, g_pBillboardTexture2[i]);

    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	g_pd3dDevice->EndScene();

	g_pd3dDevice->BeginScene();
		// Draw the billboards
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove19 );
    g_pd3dDevice->SetVertexShader(FVF3D);
    g_pd3dDevice->SetStreamSource(0, g_pVB4, sizeof(s3DVertex));
    g_pd3dDevice->SetTexture(0, g_pBillboardTexture2[i]);

    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);


	g_pd3dDevice->EndScene();
    g_pd3dDevice->SetTexture(0, NULL);
	
	g_pd3dDevice->ApplyStateBlock(h_SaveBlock5);
	if(eee == 6){
		eee = 0;
	}
}
void effectRender(int i)
{
	Eng3D.SetupMatrices();
	g_pd3dDevice->CaptureStateBlock(h_SaveBlock5);

	g_pd3dDevice->ApplyStateBlock(h_SaveBlock2);

	g_pd3dDevice->BeginScene();
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove31 );
    g_pd3dDevice->SetVertexShader(FVF3D);
    g_pd3dDevice->SetStreamSource(0, g_pVB7, sizeof(s3DVertex));

    g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE ); //칼라//
    g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
    g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );

    g_pd3dDevice->SetTexture(0, g_pBillboardTexture3[i]);
 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	g_pd3dDevice->EndScene();

	g_pd3dDevice->ApplyStateBlock(h_SaveBlock5);

    g_pd3dDevice->SetTexture(0, NULL);
	if(ddd == 6){
	ddd =0;
	effectcheck = 0;
	}
}
void bbdRender()
{
	g_pd3dDevice->CaptureStateBlock(h_SaveBlock5);

	g_pd3dDevice->ApplyStateBlock(h_SaveBlock2);

     D3DXMATRIX matView;
    D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3( 0.0f, 0.0f, -101.0f ),
                                  &D3DXVECTOR3( 0.0f, 0.0f, -100.0f ),
                                  &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
   g_pd3dDevice->BeginScene();
    // Draw the billboards
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove5 );
    g_pd3dDevice->SetVertexShader(FVF3D);
    g_pd3dDevice->SetStreamSource(0, g_pBillboardVB, sizeof(s3DVertex));
    g_pd3dDevice->SetTexture(0, g_pBillboardTexture);

    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//    g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    g_pd3dDevice->SetTexture(0, NULL);

	g_pd3dDevice->EndScene();

	g_pd3dDevice->ApplyStateBlock(h_SaveBlock5);
}

void cndehfbox2()
{
	matMove20._41 = cASE[0].m_pGObject->m_pVertexCashe[150].x - 2.0f + matMove._41;
	matMove20._42 = cASE[0].m_pGObject->m_pVertexCashe[150].y - 3.0f + matMove._42;
	matMove20._43 = cASE[0].m_pGObject->m_pVertexCashe[150].z - 2.0f + matMove._43;
	matMove21._41 = cASE[4].m_pGObject->m_pVertexCashe[10].x + matMove6._41;
	matMove21._42 = cASE[4].m_pGObject->m_pVertexCashe[10].y + matMove6._42;
	matMove21._43 = cASE[4].m_pGObject->m_pVertexCashe[10].z + matMove6._43;
	matMove22._41 = cASE[6].m_pGObject->m_pVertexCashe[10].x + matMove7._41;
	matMove22._42 = cASE[6].m_pGObject->m_pVertexCashe[10].y + matMove7._42;
	matMove22._43 = cASE[6].m_pGObject->m_pVertexCashe[10].z + matMove7._43;
	matMove23._41 = cASE[4].m_pGObject->m_pVertexCashe[10].x + matMove8._41;
	matMove23._42 = cASE[4].m_pGObject->m_pVertexCashe[10].y + matMove8._42;
	matMove23._43 = cASE[4].m_pGObject->m_pVertexCashe[10].z + matMove8._43;
	matMove24._41 = cASE[4].m_pGObject->m_pVertexCashe[10].x + matMove9._41;
	matMove24._42 = cASE[4].m_pGObject->m_pVertexCashe[10].y + matMove9._42;
	matMove24._43 = cASE[4].m_pGObject->m_pVertexCashe[10].z + matMove9._43;
	matMove25._41 = cASE[4].m_pGObject->m_pVertexCashe[10].x + matMove10._41;
	matMove25._42 = cASE[4].m_pGObject->m_pVertexCashe[10].y + matMove10._42;
	matMove25._43 = cASE[4].m_pGObject->m_pVertexCashe[10].z + matMove10._43;
	matMove26._41 = cASE[5].m_pGObject->m_pVertexCashe[75].x + matMove11._41;
	matMove26._42 = cASE[5].m_pGObject->m_pVertexCashe[75].y + matMove11._42;
	matMove26._43 = cASE[5].m_pGObject->m_pVertexCashe[75].z + matMove11._43;
	matMove27._41 = cASE[5].m_pGObject->m_pVertexCashe[75].x + matMove12._41;
	matMove27._42 = cASE[5].m_pGObject->m_pVertexCashe[75].y + matMove12._42;
	matMove27._43 = cASE[5].m_pGObject->m_pVertexCashe[75].z + matMove12._43;
	matMove28._41 = cASE[5].m_pGObject->m_pVertexCashe[75].x + matMove13._41;
	matMove28._42 = cASE[5].m_pGObject->m_pVertexCashe[75].y + matMove13._42;
	matMove28._43 = cASE[5].m_pGObject->m_pVertexCashe[75].z + matMove13._43;
	
	cndehf.MAKE_BoundBox(30,15,30,Object[0]);
	cndehf.H3D_Up_Data(Object[0], matMove20);
	cndehf.MAKE_BoundBox(20,10,10,Object[1]);
	cndehf.H3D_Up_Data(Object[1], matMove21);
	cndehf.MAKE_BoundBox(20,10,10,Object[2]);
	cndehf.H3D_Up_Data(Object[2], matMove22);
	cndehf.MAKE_BoundBox(20,10,10,Object[3]);
	cndehf.H3D_Up_Data(Object[3], matMove23);
	cndehf.MAKE_BoundBox(20,10,10,Object[4]);
	cndehf.H3D_Up_Data(Object[4], matMove24);
	cndehf.MAKE_BoundBox(20,10,10,Object[5]);
	cndehf.H3D_Up_Data(Object[5], matMove25);

	cndehf.MAKE_BoundBox(60,20,30,Object[6]);
	cndehf.H3D_Up_Data(Object[6], matMove26);
	cndehf.MAKE_BoundBox(60,20,30,Object[7]);
	cndehf.H3D_Up_Data(Object[7], matMove27);
	cndehf.MAKE_BoundBox(60,20,30,Object[8]);
	cndehf.H3D_Up_Data(Object[8], matMove28);


	for(int i = 0; i < 10; i++){
		if(altkdlfcheck[i]==1){
	cndehf.MAKE_BoundBox(10,10,10,Object[i + 9]);
	cndehf.H3D_Up_Data(Object[i + 9], matMove29[i]);
	matMove29[i]._41 = cASE[5].m_pGObject->m_pVertexCashe[0].x + matMove15[i]._41;
	matMove29[i]._42 = cASE[5].m_pGObject->m_pVertexCashe[0].y + matMove15[i]._42;
	matMove29[i]._43 = cASE[5].m_pGObject->m_pVertexCashe[0].z + matMove15[i]._43;
	if(cndehf1[i] == 0)
	cndehf1[i] = cndehf.H3D_Collision(Object[i + 9], Object[1]);
	if(cndehf1[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 1; 
	if(cndehf1[i] == 0)
	cndehf1[i] = cndehf.H3D_Collision(Object[i + 9], Object[2]);
	if(cndehf1[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 2;
	if(cndehf1[i] == 0)
	cndehf1[i] = cndehf.H3D_Collision(Object[i + 9], Object[3]);
	if(cndehf1[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 3;
	if(cndehf1[i] == 0)
	cndehf1[i] = cndehf.H3D_Collision(Object[i + 9], Object[4]);
	if(cndehf1[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 4;
	if(cndehf1[i] == 0)
	cndehf1[i] = cndehf.H3D_Collision(Object[i + 9], Object[5]);
	if(cndehf1[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 5;
	if(cndehf1[i] == 0)
	cndehf1[i] = cndehf.H3D_Collision(Object[i + 9], Object[6]);
	if(cndehf1[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 6;
	if(cndehf1[i] == 0)
	cndehf1[i] = cndehf.H3D_Collision(Object[i + 9], Object[7]);
	if(cndehf1[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 7;
	if(cndehf1[i] == 0)
	cndehf1[i] = cndehf.H3D_Collision(Object[i + 9], Object[8]);
	if(cndehf1[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 8;
	cndehf.MAKE_BoundBox(10,10,10,Object[i + 19]);
	cndehf.H3D_Up_Data(Object[i + 19], matMove30[i]);
	if(cndehf2[i] == 0)
	matMove30[i]._41 = cASE[5].m_pGObject->m_pVertexCashe[0].x + matMove14[i]._41;
	if(cndehf2[i] == 0)
	matMove30[i]._42 = cASE[5].m_pGObject->m_pVertexCashe[0].y + matMove14[i]._42;
	if(cndehf2[i] == 0)
	matMove30[i]._43 = cASE[5].m_pGObject->m_pVertexCashe[0].z + matMove14[i]._43;
	if(cndehf2[i] == 0)
	cndehf2[i] = cndehf.H3D_Collision(Object[i + 19], Object[1]);
	if(cndehf2[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 1;
	if(cndehf2[i] == 0)
	cndehf2[i] = cndehf.H3D_Collision(Object[i + 19], Object[2]);
	if(cndehf2[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 2;
	if(cndehf2[i] == 0)
	cndehf2[i] = cndehf.H3D_Collision(Object[i + 19], Object[3]);
	if(cndehf2[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 3;
	if(cndehf2[i] == 0)
	cndehf2[i] = cndehf.H3D_Collision(Object[i + 19], Object[4]);
	if(cndehf2[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 4;
	if(cndehf2[i] == 0)
	cndehf2[i] = cndehf.H3D_Collision(Object[i + 19], Object[5]);
	if(cndehf2[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 5;
	if(cndehf2[i] == 0)
	cndehf2[i] = cndehf.H3D_Collision(Object[i + 19], Object[6]);
	if(cndehf2[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 6;
	if(cndehf2[i] == 0)
	cndehf2[i] = cndehf.H3D_Collision(Object[i + 19], Object[7]);
	if(cndehf2[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 7;
	if(cndehf2[i] == 0)
	cndehf2[i] = cndehf.H3D_Collision(Object[i + 19], Object[8]);
	if(cndehf2[i] == 1 && cndehf11[i] == 0)
	cndehf11[i] = 8;
		}
	}

	for(i = 1; i < 9; i++){ //주인공과 적 비행기들간의 충돌체크
	cndehfwndlsrhd[i] = cndehf.H3D_Collision(Object[0], Object[i]);
	}
	//총알과 주인공
	matMove37._41 = cASE[8].m_pGObject->m_pVertexCashe[0].x + matMove33[0]._41;
	matMove37._42 = cASE[8].m_pGObject->m_pVertexCashe[0].y + matMove33[0]._42;
	matMove37._43 = cASE[8].m_pGObject->m_pVertexCashe[0].z + matMove33[0]._43;
	cndehf.MAKE_BoundBox(10,10,10,Object[30]);
	cndehf.H3D_Up_Data(Object[30], matMove37);
	cndehfwndlsrhd[9] = cndehf.H3D_Collision(Object[30], Object[0]);

}

VOID Render()
{
if(GameMode == Log){
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
	LogKeyDown();
	g_pd3dDevice->Clear( 0, NULL,  D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
					RGB(0,0,0), 1.0f, 0 );
	if(fontcheck == 1){
    Font.m_pFont->DrawText( 250, 100, D3DCOLOR_ARGB(255,0,255,0), _T("Game Start") );
    Font.m_pFont->DrawText( 270, 200, D3DCOLOR_ARGB(255,255,0,0), _T("Option") );
    Font.m_pFont->DrawText( 280, 300, D3DCOLOR_ARGB(255,255,0,0), _T("Exit") );
	}
	if(fontcheck == 2){
    Font.m_pFont->DrawText( 250, 100, D3DCOLOR_ARGB(255,255,0,0), _T("Game Start") );
    Font.m_pFont->DrawText( 270, 200, D3DCOLOR_ARGB(255,0,255,0), _T("Option") );
    Font.m_pFont->DrawText( 280, 300, D3DCOLOR_ARGB(255,255,0,0), _T("Exit") );
	}
	if(fontcheck == 3){
    Font.m_pFont->DrawText( 250, 100, D3DCOLOR_ARGB(255,255,0,0), _T("Game Start") );
    Font.m_pFont->DrawText( 270, 200, D3DCOLOR_ARGB(255,255,0,0), _T("Option") );
    Font.m_pFont->DrawText( 280, 300, D3DCOLOR_ARGB(255,0,255,0), _T("Exit") );
	}
		LPTSTR lptstrBuf;
		lptstrBuf = new TCHAR[1024];
		wsprintf(lptstrBuf, "- 3D 슈팅게임 -");
		RECT drawRect;
		SetRect(&drawRect, 265, 30, 450, 60);
		Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 255, 255));
		SAFE_DELETE(lptstrBuf);
		
	FadeOut();
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );			// Turn on Light	
	g_pd3dDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pd3dDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	g_pd3dDevice->SetVertexShader(FVF3D2);
    g_pd3dDevice->SetStreamSource(0, g_pVB8, sizeof(s3DVertex2));
 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	g_pd3dDevice->EndScene();
	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	if(Fadeout == 3)
		GameMode = Prollog;
	}
}
else if(GameMode == Loding)
{
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
	g_pd3dDevice->Clear( 0, NULL,  D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
					Eng3D.g_dwFogColor, 1.0f, 0 );
    Font.m_pStatsFont->DrawText( 10, 20, D3DCOLOR_ARGB(255,255,0,0), _T("Data Loding...") );
    Font.m_pStatsFont->DrawText( 280, 300, D3DCOLOR_ARGB(255,255,255,0), _T("- Wait Please -") );

	FadeOut();
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );			// Turn on Light	
	g_pd3dDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pd3dDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	g_pd3dDevice->SetVertexShader(FVF3D2);
    g_pd3dDevice->SetStreamSource(0, g_pVB8, sizeof(s3DVertex2));
 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	if(Fadeout == 4)
	InitGeometry2();
    g_pd3dDevice->EndScene();
	}
}
else if(GameMode == Loding2)
{
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
	g_pd3dDevice->Clear( 0, NULL,  D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
					Eng3D.g_dwFogColor, 1.0f, 0 );
    Font.m_pStatsFont->DrawText( 10, 20, D3DCOLOR_ARGB(255,255,0,0), _T("Data Loding...") );
    Font.m_pStatsFont->DrawText( 280, 300, D3DCOLOR_ARGB(255,255,255,0), _T("- Wait Please -") );

	FadeOut();
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );			// Turn on Light	
	g_pd3dDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pd3dDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	g_pd3dDevice->SetVertexShader(FVF3D2);
    g_pd3dDevice->SetStreamSource(0, g_pVB8, sizeof(s3DVertex2));
 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	if(Fadeout == 4){
//	cASE[4].ReadASE("air7.ase"); //적비행기
//	cASE[4].Animate(0);
	D3DUtil_CreateTexture( g_pd3dDevice, "skt6025_63.jpg", &pTexture2[0]);
	GameMode = Main;
	}
    g_pd3dDevice->EndScene();
	}
}
else if(GameMode == Prollog)
{
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
	g_pd3dDevice->Clear( 0, NULL,  D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
					Eng3D.g_dwFogColor, 1.0f, 0 );

	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove34 );

   g_pd3dDevice->SetVertexShader(FVF3D);
    g_pd3dDevice->SetStreamSource(0, g_pVB9, sizeof(s3DVertex));
    g_pd3dDevice->SetTexture(0, g_pBillboardTexture4);

 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
 	
	g_pd3dDevice->SetTexture(0, NULL);

	LPTSTR lptstrBuf;
	LPTSTR lptstrBuf2;
	LPTSTR lptstrBuf3;
	lptstrBuf = new TCHAR[1024];
	lptstrBuf2 = new TCHAR[1024];
	lptstrBuf3 = new TCHAR[1024];
	RECT drawRect, drawRect2, drawRect3;
	SetRect(&drawRect, 100, 100, 600, 160);
	SetRect(&drawRect2, 100, 230, 600, 300);
	SetRect(&drawRect3, 340, 420, 660, 480);

	if(prol == 0){
	wsprintf(lptstrBuf, "본부 : K1 호기 나와라.");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 1){
	wsprintf(lptstrBuf, "본부 : K1 호기 나와라.");
	wsprintf(lptstrBuf2, "K1 : 예. K1 여기 있습니다.");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
 	Font.m_lpFont->DrawText(lptstrBuf2, -1, &drawRect2, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(135, 91, 166, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 2){
	wsprintf(lptstrBuf, "본부 : 그쪽 상황은 어떤가?");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 3){
	wsprintf(lptstrBuf, "본부 : 그쪽 상황은 어떤가?");
 	wsprintf(lptstrBuf2, "K1 : 네. 아직은 순조롭습니다..앗..?");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
 	Font.m_lpFont->DrawText(lptstrBuf2, -1, &drawRect2, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(135, 91, 166, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 4){
	wsprintf(lptstrBuf, "본부 : 왜그런가?.무슨일인가?. K1?..K1?");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 5){
	wsprintf(lptstrBuf, "본부 : 왜그런가?.무슨일인가?. K1?..K1?");
 	wsprintf(lptstrBuf2, "K1 : 근처에 적기들이 포착 되고있습니다. 그 수는 점점....");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
 	Font.m_lpFont->DrawText(lptstrBuf2, -1, &drawRect2, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(135, 91, 166, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 6){
	wsprintf(lptstrBuf, "본부 : 그래?. 그렇군..그들의 계획은..음..빨리 탈출하도록 하게!");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 7){
	wsprintf(lptstrBuf, "본부 : 그래?. 그렇군..그들의 계획은..음..빨리 탈출하도록 하게!");
 	wsprintf(lptstrBuf2, "K1 : 알겠습니다. 탈출하겠습니다. ");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
 	Font.m_lpFont->DrawText(lptstrBuf2, -1, &drawRect2, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(135, 91, 166, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	Fadecount = 0;
	FadeCheck = 1;
	Fadeout = 0;
	}
	else if(prol == 8){
	wsprintf(lptstrBuf, "                         - 스테이지 1 -");
 	wsprintf(lptstrBuf2, "                           - Start -");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(151, 179, 87, 255));
 	Font.m_lpFont->DrawText(lptstrBuf2, -1, &drawRect2, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(151, 179,87, 255));
	if(Fadeout == 3)
		prol = 9;
	}
	else if(prol == 9){
	prol = 0;
	GameMode = Loding;
	}
	SAFE_DELETE(lptstrBuf);
	SAFE_DELETE(lptstrBuf2);
	SAFE_DELETE(lptstrBuf3);
	
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove35 );
	FadeOut();
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );			// Turn on Light	
	g_pd3dDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pd3dDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	g_pd3dDevice->SetVertexShader(FVF3D2);
    g_pd3dDevice->SetStreamSource(0, g_pVB8, sizeof(s3DVertex2));
 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	g_pd3dDevice->EndScene();
	}
}
else if(GameMode == Prollog2)
{
	if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
	g_pd3dDevice->Clear( 0, NULL,  D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
					Eng3D.g_dwFogColor, 1.0f, 0 );

	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove38 );

     D3DXMATRIX matView;
    D3DXMatrixLookAtLH( &matView, &D3DXVECTOR3( 0.0f, 0.0f, 10.0f ),
                                  &D3DXVECTOR3( 0.0f, 0.0f,  0.0f ),
                                  &D3DXVECTOR3( 0.0f, 1.0f, 0.0f ) );
	 
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );
	
	g_pd3dDevice->SetVertexShader(FVF3D);
    g_pd3dDevice->SetStreamSource(0, g_pVB9, sizeof(s3DVertex));
    g_pd3dDevice->SetTexture(0, g_pBillboardTexture4);

 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
 	
	g_pd3dDevice->SetTexture(0, NULL);

	LPTSTR lptstrBuf;
	LPTSTR lptstrBuf2;
	LPTSTR lptstrBuf3;
	lptstrBuf = new TCHAR[1024];
	lptstrBuf2 = new TCHAR[1024];
	lptstrBuf3 = new TCHAR[1024];
	RECT drawRect, drawRect2, drawRect3;
	SetRect(&drawRect, 100, 100, 600, 160);
	SetRect(&drawRect2, 100, 230, 600, 300);
	SetRect(&drawRect3, 340, 420, 660, 480);

	if(prol == 0){
	wsprintf(lptstrBuf, "본부 : K1 호기, 응답하라.");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 1){
	wsprintf(lptstrBuf, "본부 : K1 호기, 응답하라.");
	wsprintf(lptstrBuf2, "K1 : 예. K1 호기 무사히 탈출에 성공하였습니다.");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
 	Font.m_lpFont->DrawText(lptstrBuf2, -1, &drawRect2, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(135, 91, 166, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 2){
	wsprintf(lptstrBuf, "본부 : 좋아. 잘했네. 그건 그렇다 치고 다친데는 없나?.");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 3){
	wsprintf(lptstrBuf, "본부 : 좋아. 잘했네. 그건 그렇다 치고 다친데는 없나?.");
	wsprintf(lptstrBuf2, "K1 : 약간의 손상이 있지만. 상관없습니다.");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
 	Font.m_lpFont->DrawText(lptstrBuf2, -1, &drawRect2, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(135, 91, 166, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 4){
	wsprintf(lptstrBuf, "본부 : 알다시피 지금 상황이 긴박하네 지금 바로 출동 할 수 있겠나?");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 5){
	wsprintf(lptstrBuf, "본부 : 알다시피 지금 상황이 긴박하네 지금 바로 출동 할 수 있겠나?");
	wsprintf(lptstrBuf2, "K1 : 예!. 하지만 약 1시간 정도의 시간이 필요합니다.");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
 	Font.m_lpFont->DrawText(lptstrBuf2, -1, &drawRect2, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(135, 91, 166, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 6){
	wsprintf(lptstrBuf, "본부 : 그정도는 괜찮아. 그럼 최대한빨리 G-242 지역으로 지동해주게.");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 7){
	wsprintf(lptstrBuf, "본부 : 그정도는 괜찮아. 그럼 최대한빨리 G-242 지역으로 지동해주게.");
	wsprintf(lptstrBuf2, "K1 : 알겠습니다.");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(0, 153, 75, 255));
 	Font.m_lpFont->DrawText(lptstrBuf2, -1, &drawRect2, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(135, 91, 166, 255));
	wsprintf(lptstrBuf3, "왼쪽 마우스버튼을 눌러주세요.");
	Font.m_lpFont->DrawText(lptstrBuf3, -1, &drawRect3, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(255, 255, 0, 255));
	}
	else if(prol == 8){
	wsprintf(lptstrBuf, "                         - 스테이지 2 -");
 	wsprintf(lptstrBuf2, "                           - Start -");
	Font.m_lpFont->DrawText(lptstrBuf, -1, &drawRect, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(151, 179, 87, 255));
 	Font.m_lpFont->DrawText(lptstrBuf2, -1, &drawRect2, DT_WORDBREAK | DT_LEFT | DT_TOP, D3DCOLOR_RGBA(151, 179,87, 255));
	}
	else if(prol == 9){
	GameMode = Loding2;
	prol = 0;
	}
	SAFE_DELETE(lptstrBuf);
	SAFE_DELETE(lptstrBuf2);
	SAFE_DELETE(lptstrBuf3);

	g_pd3dDevice->SetTransform( D3DTS_WORLD, &matMove38 );
	FadeOut();
	g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );			// Turn on Light	
	g_pd3dDevice->SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pd3dDevice->SetRenderState (D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState (D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState (D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	g_pd3dDevice->SetVertexShader(FVF3D2);
    g_pd3dDevice->SetStreamSource(0, g_pVB8, sizeof(s3DVertex2));
 	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
	g_pd3dDevice->EndScene();
	}
}
////////////////// 1stage ///////////////////////////
else if(GameMode == Main){
/*	static int adsd = 0;
	if(adsd == 0 || adsd == 1 || adsd == 2){
	adsd++;
	if(adsd == 1)
	resent = GetTickCount();
	if(adsd == 2){
	old = GetTickCount();
	allcount = old - resent;
	adsd = 0;
	}
	}
*/
	static int framecount = 0;
	static int afh = 0;
    static DWORD                lastTickCount = 0;
    DWORD                       delay = 1000;
    DWORD                       thisTickCount;
	if(afh == 0 || afh == 1 || afh == 2){
		thisTickCount = GetTickCount();
		framecount++;
        if ((thisTickCount - lastTickCount) > delay)
        {
			allcount = 2500 / framecount;
			framecount = 0;
            lastTickCount = thisTickCount;
			afh++;
 		}
	}
	if(allcount != 2500){
	movealtkdlf(); //미사일 나갈수있나 없나 체크
	KeyDownCheck(); //키체크
	g_pd3dDevice->Clear( 0, NULL,  D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
								Eng3D.g_dwFogColor, 1.0f, 0 );
	Eng3D.SetupMatrices();
	Eng3D.SetupLights();			

	ASERender(0); //cASE 0 번째 랜더링
	ASERender2(1 ,0); //cASE 1 번째 랜더링
	ASERender2(2 , 1); //cASE 2 번째 랜더링

	ASERender2(4 , 3); 
	ASERender2(4 , 4); 
	ASERender2(4 , 5); 
	ASERender2(4 , 6); 
	ASERender2(4 , 7); 

	for(int i = 0; i < 10; i++){
		if(altkdlfcheck[i]==1){
			if(cndehf1[i] == 1 || cndehf2[i] == 1){
				if(cndehf1[i] == 1) //임펙트 텍스쳐 메트릭스에다가 총알값을 대입시킴
				matMove31 = matMove15[i];
				if(cndehf2[i] == 1)
				matMove31 = matMove14[i];
				if(effectcheck == 0){
				ddd =0;
				effectcheck = 1; //임펙트 텍스쳐 작동
				}
				cndehf1[i] = 0;
				matMove15[i]._41 = -1000.0f; //총알 다시 원위치로
				matMove14[i]._41 = -1000.0f;
				cndehf2[i] = 0;
				altkdlfcheck[i] = 0;
				if(cndehf11[i]==1)
					matMove6._43 = -2500.0f;
				else if(cndehf11[i]==2)
					matMove7._43 = -2500.0f;
				else if(cndehf11[i]==3)
					matMove8._43 = -2500.0f;
				else if(cndehf11[i]==4)
					matMove9._43 = -2500.0f;
				else if(cndehf11[i]==5)
					matMove10._43 = -2500.0f;
				else if(cndehf11[i]==6)
					matMove11._43 = -2500.0f;
				else if(cndehf11[i]==7)
					matMove12._43 = -2500.0f;
				else if(cndehf11[i]==8)
					matMove13._43 = -2500.0f;

					cndehf11[i] = 0;
			}
		}
	}
	
	for( i = 1; i < 10; i++){
	if(cndehfwndlsrhd[i] == 1)
	{
		matMove31 = matMove;
		matMove31._43 = -2.0f;
		if(effectcheck == 0){
		ddd =0;
		effectcheck = 1; //임펙트 텍스쳐 작동
		}
		cndehfwndlsrhd[i] = 0;
		cndehfwndlsrhd2 = 1;
		matMove._43 = -5000.0f;
		if(i == 1)
			matMove6._43 = -6000.0f;
		else if(i == 2)
			matMove7._43 = -6000.0f;
		else if(i == 3)
			matMove8._43 = -6000.0f;
		else if(i == 4)
			matMove9._43 = -6000.0f;
		else if(i == 5)
			matMove10._43 = -6000.0f;
		else if(i == 6)
			matMove11._43 = -6000.0f;
		else if(i == 7)
			matMove12._43 = -6000.0f;
		else if(i == 8)
			matMove13._43 = -6000.0f;
		else if(i == 9)
			matMove33[0]._43 = -6000.0f;
	}
	}
	ASERender2(5 , 8);
	ASERender2(5 , 9); 
	ASERender2(5 , 10); 
	if(altkdlfcheck[0] == 1){
	ASERender2(6 , 11); 
	ASERender2(6 , 22); 
	}
	if(altkdlfcheck[1] == 1){
	ASERender2(6 , 12); //cASE 2 번째 랜더링
	ASERender2(6 , 23); //cASE 2 번째 랜더링
	}
	if(altkdlfcheck[2] == 1){
	ASERender2(6 , 13); //cASE 2 번째 랜더링
	ASERender2(6 , 24); //cASE 2 번째 랜더링
	}
	if(altkdlfcheck[3] == 1){
	ASERender2(6 , 14); //cASE 2 번째 랜더링
	ASERender2(6 , 25); //cASE 2 번째 랜더링
	}
	if(altkdlfcheck[4] == 1){
	ASERender2(6 , 15); 
	ASERender2(6 , 26); 
	}
	if(altkdlfcheck[5] == 1){
	ASERender2(6 , 16); 
	ASERender2(6 , 27); 
	}
	if(altkdlfcheck[6] == 1){
	ASERender2(6 , 17); 
	ASERender2(6 , 28); 
	}
	if(altkdlfcheck[7] == 1){
	ASERender2(6 , 18); 
	ASERender2(6 , 29); 
	}
	if(altkdlfcheck[8] == 1){
	ASERender2(6 , 19); 
	ASERender2(6 , 30); 
	}
	if(altkdlfcheck[9] == 1){
	ASERender2(6 , 20); 
	ASERender2(6 , 31); 
	}
	if(altkdlfcheck[10] == 1){
	ASERender2(6 , 21); 
	ASERender2(6 , 32); 
	}
	
	ASERender2(7 , 33);  //탱크
	if(matMove33check == 1){
	matMove33[0]._43 += 0.5f * allcount;
	ASERender2(8 , 34);  //적 비행기 미사일
	}
//	cndehfbox();
	cndehfbox2();
//	ASERender2(3 , 2); 
	backRender(0);
	backRender(1);
	if(movealtkdlfcheck == 0 && movealtkdlfcheck2 == 1){
	fierRender(eee);
	eee++;
		movealtkdlfcheck2 = 0;
	}
	if(effectcheck == 1){
	effectRender(ddd);
	ddd++;
	}
//	bbdRender();
	MoveCheck();
	if(cndehfwndlsrhd2 == 1)
		GameOverRender();

	STAGE1CLEAR+= 0.1f * allcount;

	if(STAGE1CLEAR > 1000.0f){
		STAGE1CLEAR = 0.0f;
		D3DUtil_CreateTexture( g_pd3dDevice, "wildmystery_5.jpg", &g_pBillboardTexture4);
		GameMode = Prollog2; // stage 2로.
	}

	g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}
}
}
void MoveCheck()
{
	static int movecheck = 1;
	static int movecheck2 = 1;
	matMove2._43 += 0.02f * allcount;
	matMove3._43 += 0.1f * allcount; //배경...
	matMove4._43 += 0.06f * allcount;
	if(matMove6check == 0)
	matMove6._43 += 0.20f * allcount;
	else
	moveplane();
	matMove7._43 += 0.20f * allcount;
	matMove8._43 += 0.30f * allcount;
	matMove9._43 += 0.10f * allcount;
	matMove10._43 +=0.10f * allcount;

	matMove11._43 += 0.07f * allcount;
	matMove12._43 += 0.15f * allcount;
	matMove13._43 += 0.08f * allcount;

	matMove32._43 += 0.10f * allcount;

	if(movecheck == 1)
		matMove13._41 += 0.08f * allcount;
	if(movecheck == 2)
		matMove13._41 -= 0.08f * allcount;

	if(movecheck2 == 1)
		matMove12._42 += 0.08f * allcount;
	if(movecheck2 == 2)
		matMove12._42 -= 0.08f * allcount;

	if(matMove2._43 < -500.0f && matMove2._43 > -550.0f)
		SoEng.DSound_Play(sound_ids[1],0,0,0,0); //wav 재생
	if(matMove13._41 >= 200)
		movecheck = 2;
	if(matMove13._41 <= -200)
		movecheck = 1;

	if(matMove12._42 >= 200)
		movecheck2 = 2;
	if(matMove12._42 <= -200)
		movecheck2 = 1;

	if(matMove6._43 > 1000.0f)
		matMove6._43 = -2500.0f;
	else if(matMove6._43 > -490.0f && matMove6._43 < -450.0f){
		matMove6check = 1;
		if(matMove33check == 0){
		matMove33check = 1;
		matMove33[0]._41 = matMove6._41;
		matMove33[0]._42 = matMove6._42;
		matMove33[0]._43 = matMove6._43;
		}
	}
	if(matMove33[0]._43 > 1000.0f){
		matMove33[0]._43 = -2500.0f;
		matMove33check = 0;
	}

	if(matMove7._43 > 1000.0f)
		matMove7._43 = -2500.0f;
	if(matMove8._43 > 1000.0f)
		matMove8._43 = -2500.0f;
	if(matMove9._43 > 1000.0f)
		matMove9._43 = -2500.0f;
	if(matMove10._43 > 1000.0f)
		matMove10._43 = -2500.0f;

	if(matMove11._43 > 1000.0f)
		matMove11._43 = -2500.0f;
	if(matMove12._43 > 1000.0f)
		matMove12._43 = -2500.0f;
	if(matMove13._43 > 1000.0f)
		matMove13._43 = -2500.0f;

	for(int i = 0; i < 10; i++) //미사일 move, 범위체크
	{
		if(altkdlfcheck[i] == 1)
			matMove15[i]._43 -= 0.3f * allcount;

		if(altkdlfcheck[i] == 1){
		matMove14[i]._43 -= 0.3f * allcount;

		if(matMove14[i]._43 < -800.0f){
			altkdlfcheck[i] = 0;
			matMove14[i]._43 = -5000.0f;
			matMove15[i]._43 = -5000.0f;
			}
		}
	}

	if(matMove3._43 > 1000.0f)
	matMove3._43 = -3000.0f;
	if(matMove2._43 > 1000.0f)
	matMove2._43 = -2000.0f;
	if(matMove4._43 > 1000.0f)
	matMove4._43 = -2500.0f;

	matMove16._43+=0.08f * allcount;
	matMove17._43+=0.08f * allcount;

	if(matMove16._43 > 2200.0f)
		matMove16._43 = -2180.0f;
	if(matMove17._43 > 2200.0f)
		matMove17._43 = -2180.0f;


	if(matMove32._43 > 1400.0f)
		matMove32._43 = -1500.0f;
}
//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
		case WM_LBUTTONDOWN:
			if(GameMode == Prollog){
				if(prol >= 0 && prol < 8)
				prol++;
			}
			else if(GameMode == Prollog2){
				if(prol >= 0 && prol < 9)
				prol++;
			}

			return 0;
        case WM_DESTROY:
            PostQuitMessage( 0 );
            return 0;
	}

	
	return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE, LPSTR, INT )
{
	WNDCLASS wc;
	wc.lpszClassName =		 "3D GAME";
	wc.lpfnWndProc =		 MsgProc;
	wc.style =				 CS_VREDRAW | CS_HREDRAW;
	wc.hInstance =			 hInstance;
	wc.hIcon =				 LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor =			 LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = 		(HBRUSH) (COLOR_WINDOW + 1);
	wc.lpszMenuName = 		NULL;
	wc.cbClsExtra=			0;
	wc.cbWndExtra =			0;

    RegisterClass( &wc );

    // Create the application's window
    HWND hWnd = CreateWindowEx(WS_EX_TOPMOST, "3D GAME", "3D GAME",
                              WS_POPUP|WS_BORDER, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                              NULL, NULL, hInstance, NULL );

	main_window_handle = hWnd;
	// Initialize Direct3D
    if( SUCCEEDED( Eng3D.InitD3DFullscreen( hWnd, 0) ) )
    {
        // Create the scene geometry
        if( SUCCEEDED( InitGeometry() ) )
        {
            // Show the window
            ShowWindow( hWnd, SW_SHOWDEFAULT );
            UpdateWindow( hWnd );

            // Enter the message loop
            MSG msg;
            ZeroMemory( &msg, sizeof(msg) );
            while( msg.message!=WM_QUIT )
            {
                if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
                else
                    Render();
            }
        }
    }

    // Clean up everything and exit the app
    Cleanup();
    UnregisterClass( "3D GAME", wc.hInstance );
    return 0;
}

void movea(int flag, int a) //오브젝트 이동
{
	switch( flag ) {
		case 0 :  
			if(matMove._43 < 350.0f)
			matMove._43 += 0.06f * allcount; 
			break;
		case 1 :  
			if(matMove._43 > 30.0f)
			matMove._43 -= 0.06f * allcount; 
			break;
		case 2 : 
			if(matMove._41 < 200.0f)
			matMove._41 += 0.06f * allcount;
			break;
		case 3 :
			if(matMove._41 > -200.0f)
			matMove._41 -= 0.06f * allcount;
			break;
		case 4 :
			if(matMove._42 < 80.0f)
			matMove._42 += 0.06f * allcount;
			break;
		case 5 :  
			if(matMove._42 > -80.0f)
			matMove._42 -= 0.06f * allcount; 
			break;
	}
}
void LogKeyDown()
{
    static DWORD                lastTickCount = 0;
    DWORD                       delay = 40;
    DWORD                       thisTickCount;
	thisTickCount = GetTickCount();
    if ((thisTickCount - lastTickCount) > delay)
    {
	if(_KeyDown(0x53)){
			if(fontcheck == 1){
				SoEng.DSound_Play(sound_ids[1],0,0,0,0); //wav 재생
				fontcheck = 2;
			}
			else if(fontcheck == 2){
				SoEng.DSound_Play(sound_ids[1],0,0,0,0); //wav 재생
				fontcheck = 3;
			}

	}
	if(_KeyDown(0x57)){
			if(fontcheck == 3){
				SoEng.DSound_Play(sound_ids[1],0,0,0,0); //wav 재생
				fontcheck = 2;
			}
			else if(fontcheck == 2){
				SoEng.DSound_Play(sound_ids[1],0,0,0,0); //wav 재생
				fontcheck = 1;
			}
	
		}
	if(_KeyDown(VK_SPACE)){
		if(fontcheck == 1)
		{
			if(FadeCheck == 0){
			FadeCheck = 1;
			Fadecount = 0;
			Fadeout = 0;
			SoEng.DSound_Play(sound_ids[0],0,0,0,0); //wav 재생
		}
			MEng.DMusic_Stop(midi_ids[0]);	//mid 정지
		}
		if(fontcheck == 3)
            PostQuitMessage( 0 );
	}
          lastTickCount = thisTickCount;
	}
}
void KeyDownCheck()
{
	static int aa = 0;
	if(_KeyDown(0x41)){
		AnimationCheck2 = 0;
		if(AnimationCheck1 == 0){
			if(indexRate >= 4000 && indexRate < 8000){
				bbb = 8000 - indexRate;
				indexRate = bbb;
			}
			if(indexRate >= 8000 && indexRate < 12000){
				bbb = 16000 - indexRate;
				indexRate = 8000 +  bbb;
			}
			AnimationCheck1 = 1;
		}
		if(indexRate != 4000){
		bRead = TRUE;
		}
		if(indexRate >= 16000)
			indexRate = 0;
	}

	if(_KeyDown(0x44)){
		AnimationCheck1 = 0;
		if(AnimationCheck2 == 0){ //애니메이션이 되고있는지, 않되고있는지. 않되고있다면 초기화 시켜준다
		if(indexRate >= 0 && indexRate < 4000){
			bbb = indexRate;
			indexRate = 8000 - bbb;
		}
		if(indexRate >= 12000 && indexRate < 16000){
				bbb = 16000 - indexRate;
				indexRate = 8000 + bbb;
		}
		AnimationCheck2 = 1;
		}
		if(indexRate != 12000){
		bRead = TRUE;
		}
		if(indexRate >= 16000)
			indexRate = 0;

		}
	if(_KeyDown(0x57))
		movea(1,0);
	if(_KeyDown(0x53))
		movea(0,0);
	if(_KeyDown(0x41))
		movea(2,0);
	if(_KeyDown(0x44))
		movea(3,0);
	if(_KeyDown(0x51))
		movea(4,0);
	if(_KeyDown(0x45))
		movea(5,0);
	if(_KeyDown(0x20)){
		if(movealtkdlfcheck == 1){
			for(int i = 0; i < 10; i++){
			if(altkdlfcheck[i]==0){
				matMove14[i]._41 = cASE[0].m_pGObject->m_pVertexCashe[224].x - 1.0f + matMove._41;
				matMove14[i]._42 = cASE[0].m_pGObject->m_pVertexCashe[224].y - 3.0f + matMove._42;
				matMove14[i]._43 = cASE[0].m_pGObject->m_pVertexCashe[224].z - 2.0f + matMove._43;

				matMove15[i]._41 = cASE[0].m_pGObject->m_pVertexCashe[314].x - 2.0f + matMove._41;
				matMove15[i]._42 = cASE[0].m_pGObject->m_pVertexCashe[314].y - 3.0f + matMove._42;
				matMove15[i]._43 = cASE[0].m_pGObject->m_pVertexCashe[314].z - 2.0f + matMove._43;
			

				SoEng.DSound_Play(sound_ids[0],0,0,0,0); //wav 재생

				altkdlfcheck[i] = 1;
				movealtkdlfcheck = 0;
				break;
				}
			}
		}
	}

	if(_KeyDown(VK_ESCAPE))
		PostQuitMessage(0);
}
void movealtkdlf()
{
    static DWORD                lastTickCount = 0;
    DWORD                       delay = 500;
    DWORD                       thisTickCount;
	
		thisTickCount = GetTickCount();
        if ((thisTickCount - lastTickCount) > delay)
        {
			movealtkdlfcheck2 = 1;
			movealtkdlfcheck = 1;
            lastTickCount = thisTickCount;
 		}
}
void moveplane()
{
    static DWORD                lastTickCount = 0;
    DWORD                       delay = 1000;
    DWORD                       thisTickCount;

		thisTickCount = GetTickCount();
        if ((thisTickCount - lastTickCount) > delay)
        {
			matMove6check = 0;
     		matMove6._43 += 0.20f * allcount;
            lastTickCount = thisTickCount;
 		}
}

void AnimationNumber( int j)
{
	 		cASE[j].Animate(indexRate);
			indexRate+=160;
			bRead = FALSE;
}

HRESULT FadeOut()
{
	if(FadeCheck == 1){
	DWORD color;
	if(Fadecount == 0) {color = 0x00000000; if(Fadeout == 3){ FadeCheck =0; Fadeout = 4;} else Fadeout= 1;}  	else if(Fadecount == 1) color = 0x01000000; 	else if(Fadecount == 2) color = 0x02000000; else if(Fadecount == 3) color = 0x03000000; else if(Fadecount == 4) color = 0x04000000; else if(Fadecount == 5) color = 0x05000000; else if(Fadecount == 6) color = 0x06000000; else if(Fadecount == 7) color = 0x07000000; else if(Fadecount == 8) color = 0x08000000; else if(Fadecount == 9) color = 0x09000000;
else if(Fadecount == 10) color = 0x10000000; 	else if(Fadecount == 11) color = 0x11000000; 	else if(Fadecount == 12) color = 0x12000000; else if(Fadecount == 13) color = 0x13000000; else if(Fadecount == 14) color = 0x14000000; else if(Fadecount == 15) color = 0x15000000; else if(Fadecount == 16) color = 0x16000000; else if(Fadecount == 17) color = 0x17000000; else if(Fadecount == 18) color = 0x18000000; else if(Fadecount == 19) color = 0x19000000;
else if(Fadecount == 20) color = 0x20000000; 	else if(Fadecount == 21) color = 0x21000000; 	else if(Fadecount == 22) color = 0x22000000; else if(Fadecount == 23) color = 0x23000000; else if(Fadecount == 24) color = 0x24000000; else if(Fadecount == 25) color = 0x25000000; else if(Fadecount == 26) color = 0x26000000; else if(Fadecount == 27) color = 0x27000000; else if(Fadecount == 28) color = 0x28000000; else if(Fadecount == 29) color = 0x29000000;
else if(Fadecount == 30) color = 0x30000000; 	else if(Fadecount == 31) color = 0x31000000; 	else if(Fadecount == 32) color = 0x32000000; else if(Fadecount == 33) color = 0x33000000; else if(Fadecount == 34) color = 0x34000000; else if(Fadecount == 35) color = 0x35000000; else if(Fadecount == 36) color = 0x36000000; else if(Fadecount == 37) color = 0x37000000; else if(Fadecount == 38) color = 0x38000000; else if(Fadecount == 39) color = 0x39000000;
else if(Fadecount == 40) color = 0x40000000; 	else if(Fadecount == 41) color = 0x41000000; 	else if(Fadecount == 42) color = 0x42000000; else if(Fadecount == 43) color = 0x43000000; else if(Fadecount == 44) color = 0x44000000; else if(Fadecount == 45) color = 0x45000000; else if(Fadecount == 46) color = 0x46000000; else if(Fadecount == 47) color = 0x47000000; else if(Fadecount == 48) color = 0x48000000; else if(Fadecount == 49) color = 0x49000000;
else if(Fadecount == 50) color = 0x50000000; 	else if(Fadecount == 51) color = 0x51000000; 	else if(Fadecount == 52) color = 0x52000000; else if(Fadecount == 53) color = 0x53000000; else if(Fadecount == 54) color = 0x54000000; else if(Fadecount == 55) color = 0x55000000; else if(Fadecount == 56) color = 0x56000000; else if(Fadecount == 57) color = 0x57000000; else if(Fadecount == 58) color = 0x58000000; else if(Fadecount == 59) color = 0x59000000;
else if(Fadecount == 60) color = 0x60000000; 	else if(Fadecount == 61) color = 0x61000000; 	else if(Fadecount == 62) color = 0x62000000; else if(Fadecount == 63) color = 0x63000000; else if(Fadecount == 64) color = 0x64000000; else if(Fadecount == 65) color = 0x65000000; else if(Fadecount == 66) color = 0x66000000; else if(Fadecount == 67) color = 0x67000000; else if(Fadecount == 68) color = 0x68000000; else if(Fadecount == 69) color = 0x69000000;
else if(Fadecount == 70) color = 0x70000000; 	else if(Fadecount == 71) color = 0x71000000; 	else if(Fadecount == 72) color = 0x72000000; else if(Fadecount == 73) color = 0x73000000; else if(Fadecount == 74) color = 0x74000000; else if(Fadecount == 75) color = 0x75000000; else if(Fadecount == 76) color = 0x76000000; else if(Fadecount == 77) color = 0x77000000; else if(Fadecount == 78) color = 0x78000000; else if(Fadecount == 79) color = 0x79000000;
else if(Fadecount == 80) color = 0x80000000; 	else if(Fadecount == 81) color = 0x81000000; 	else if(Fadecount == 82) color = 0x82000000; else if(Fadecount == 83) color = 0x83000000; else if(Fadecount == 84) color = 0x84000000; else if(Fadecount == 85) color = 0x85000000; else if(Fadecount == 86) color = 0x86000000; else if(Fadecount == 87) color = 0x87000000; else if(Fadecount == 88) color = 0x88000000; else if(Fadecount == 89) color = 0x89000000;
else if(Fadecount == 90) color = 0x90000000; 	else if(Fadecount == 91) color = 0x91000000; 	else if(Fadecount == 92) color = 0x92000000; else if(Fadecount == 93) color = 0x93000000; else if(Fadecount == 94) color = 0x94000000; else if(Fadecount == 95) color = 0x95000000; else if(Fadecount == 96) color = 0x96000000; else if(Fadecount == 97) color = 0x97000000; else if(Fadecount == 98) color = 0x98000000; else if(Fadecount == 99) color = 0x99000000;
if(Fadecount == 100) color = 0xa0000000; 	else if(Fadecount == 101) color = 0xa1000000; 	else if(Fadecount == 102) color = 0xa2000000; else if(Fadecount == 103) color = 0xa3000000; else if(Fadecount == 104) color = 0xa4000000; else if(Fadecount == 105) color = 0xa5000000; else if(Fadecount == 106) color = 0xa6000000; else if(Fadecount == 107) color = 0xa7000000; else if(Fadecount == 108) color = 0xa8000000; else if(Fadecount == 109) color = 0xa9000000;
else if(Fadecount == 110) color = 0xb0000000; 	else if(Fadecount == 111) color = 0xb1000000; 	else if(Fadecount == 112) color = 0xb2000000; else if(Fadecount == 113) color = 0xb3000000; else if(Fadecount == 114) color = 0xb4000000; else if(Fadecount == 115) color = 0xb5000000; else if(Fadecount == 116) color = 0xb6000000; else if(Fadecount == 117) color = 0xb7000000; else if(Fadecount == 118) color = 0xb8000000; else if(Fadecount == 119) color = 0xb9000000;
else if(Fadecount == 120) color = 0xc0000000; 	else if(Fadecount == 121) color = 0xc1000000; 	else if(Fadecount == 122) color = 0xc2000000; else if(Fadecount == 123) color = 0xc3000000; else if(Fadecount == 124) color = 0xc4000000; else if(Fadecount == 125) color = 0xc5000000; else if(Fadecount == 126) color = 0xc6000000; else if(Fadecount == 127) color = 0xc7000000; else if(Fadecount == 128) color = 0xc8000000; else if(Fadecount == 129) color = 0xc9000000;
else if(Fadecount == 130) color = 0xd0000000; 	else if(Fadecount == 131) color = 0xd1000000; 	else if(Fadecount == 132) color = 0xd2000000; else if(Fadecount == 133) color = 0xd3000000; else if(Fadecount == 134) color = 0xd4000000; else if(Fadecount == 135) color = 0xd5000000; else if(Fadecount == 136) color = 0xd6000000; else if(Fadecount == 137) color = 0xd7000000; else if(Fadecount == 138) color = 0xd8000000; else if(Fadecount == 139) color = 0xd9000000;
else if(Fadecount == 140) color = 0xe0000000; 	else if(Fadecount == 141) color = 0xe1000000; 	else if(Fadecount == 142) color = 0xe2000000; else if(Fadecount == 143) color = 0xe3000000; else if(Fadecount == 144) color = 0xe4000000; else if(Fadecount == 145) color = 0xe5000000; else if(Fadecount == 146) color = 0xe6000000; else if(Fadecount == 147) color = 0xe7000000; else if(Fadecount == 148) color = 0xe8000000; else if(Fadecount == 149) color = 0xe9000000;
else if(Fadecount == 150) color = 0xf0000000; 	else if(Fadecount == 151) color = 0xf1000000; 	else if(Fadecount == 152) color = 0xf2000000; else if(Fadecount == 153) color = 0xf3000000; else if(Fadecount == 154) color = 0xf4000000; else if(Fadecount == 155) color = 0xf5000000; else if(Fadecount == 156) color = 0xf6000000; else if(Fadecount == 157) color = 0xf7000000; else if(Fadecount == 158) color = 0xf8000000; else if(Fadecount == 159) { color = 0xf9000000;  if(Fadeout == 2) Fadeout = 3;} 
else if(Fadecount == 160) {color = 0xff000000; Fadeout = 2;}
	BillboardVerts8[0].Color = color;
	BillboardVerts8[1].Color = color;
	BillboardVerts8[2].Color = color;
	BillboardVerts8[3].Color = color;
 
	if(FAILED(g_pd3dDevice->CreateVertexBuffer(                 \
            sizeof(BillboardVerts8), 0, FVF3D2,                 \
            D3DPOOL_DEFAULT, &g_pVB8)))
    return FALSE;
  if(FAILED(g_pVB8->Lock(0,0, (BYTE**)&Ptr, 0)))
    return FALSE;
  memcpy(Ptr, BillboardVerts8, sizeof(BillboardVerts8));
  g_pVB8->Unlock();
	}
	if(Fadeout == 0 || Fadeout == 1)
	Fadecount++;
	if(Fadeout == 2 || Fadeout == 3)
	Fadecount--;

	return S_OK;
}
