#ifndef __3D_FADE_H__
#define __3D_FADE_H__

#include "3D Engine [G].h"
#include "D3DApp.h"
#include "D3DUtil.h"
#include "DXUtil.h"

typedef struct {
  FLOAT x, y, z;
  DWORD Color;
} FadeVertex;

#define FVF3DFADE (D3DFVF_XYZ | D3DFVF_DIFFUSE)

extern LPDIRECT3DVERTEXBUFFER8 FadeVB; // Buffer to hold vertices



class FadeEng
{
	public:
		FadeVertex FadeVerts[4];
		HRESULT Create();
		void FadeRender();
		HRESULT FadeOut();
		void SetupMatrices();
	private:
		BYTE *Ptr;

};

#endif