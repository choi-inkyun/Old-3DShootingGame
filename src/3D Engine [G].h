#ifndef __3D_ENGINE_H__
#define __3D_ENGINE_H__

#define WIN32_LEAN_AND_MEAN 
#define NODEFAULTLIB 

#include<d3d8.h>
#include<d3d8types.h>
#include<d3dx8.h>
#include<windows.h>
#include<mmsystem.h>
#include "ASE.h"

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d8.lib")
#pragma comment(lib, "d3dx8.lib")
#pragma comment(lib, "d3dx8dt.lib")
#pragma comment(lib, "d3dxof.lib")

extern LPDIRECT3D8             g_pD3D;
extern LPDIRECT3DDEVICE8       g_pd3dDevice; 
extern LPDIRECT3DVERTEXBUFFER8 g_pVB;   
extern LPDIRECT3DTEXTURE8	   pTexture[8];		 // �ؽ���..8���� �ְ�
extern HWND main_window_handle; 
class Engine3DG
{
	public:
		HRESULT InitD3DWindow(HWND hWnd);
		HRESULT InitD3DFullscreen(HWND hWnd , int flag); 
		VOID Cleanup();
		void textureadd();
		void SetupLights();
		VOID SetupMatrices();
		void SetRenderStateSet(int flag);
		void FogSet(DWORD Flag);
		DWORD   g_dwFogColor; 
		Engine3DG();
	private:
		// ���� ///
		FLOAT   g_fFogStartValue;
		FLOAT   g_fFogEndValue;
		DWORD   g_dwFogMode;
		FLOAT   g_fFogDensity;
		D3DPRESENT_PARAMETERS	d3dpp;	// device������ ���� ����ü
		D3DDISPLAYMODE			d3ddm;	// ���� ���÷����̸�带 ������ ���� ����ü
		VOID SettingMatrices();

};

#endif

