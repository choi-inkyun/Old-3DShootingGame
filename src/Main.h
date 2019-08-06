
#ifndef _Main_H_
#define _Main_H_

#include "3D Engine [G].h"
#include "D3DApp.h"
#include "D3DFont.h"
#include "D3DUtil.h"
#include "DXUtil.h"
#include "MusicEngine.h"
#include "3DFont.h"
#include "H3DCOLL.h"
#include "fade.h"
#define _KeyDown( vkey ) HIBYTE(GetAsyncKeyState( vkey ))
#define _KeyPush( vkey )  LOBYTE(GetAsyncKeyState( vkey ))
#define Log 0
#define Loding 1
#define Prollog 2
#define Main 3
#define Prollog2 4
#define Loding2 5

IDirect3DVertexBuffer8 *g_pBillboardVB      = NULL;
IDirect3DTexture8      *g_pBillboardTexture = NULL;
IDirect3DTexture8      *g_pBillboardTexture2[7];
IDirect3DTexture8      *g_pBillboardTexture3[7];
IDirect3DTexture8      *g_pBillboardTexture4 = NULL;
IDirect3DTexture8      *g_pBillboardTexture5 = NULL;

LPDIRECT3DVERTEXBUFFER8 g_pVB2        = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER8 g_pVB3        = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER8 g_pVB4        = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER8 g_pVB5        = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER8 g_pVB6       = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER8 g_pVB7       = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER8 g_pVB8       = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER8 g_pVB9       = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER8 g_pVB10       = NULL; // Buffer to hold vertices

LPDIRECT3DTEXTURE8		pTexture2[5];		 // �ؽ���..8���� �ְ�


typedef struct {
  FLOAT x, y, z;
  FLOAT u, v;   
} s3DVertex;


typedef struct {
  FLOAT x, y, z;
  DWORD Color;
} s3DVertex2;


static int bbb = 0;
#define FVF3D (D3DFVF_XYZ | D3DFVF_TEX1)
#define FVF3D2 (D3DFVF_XYZ | D3DFVF_DIFFUSE)

BOOL altkdlfcheck[20];
BOOL movealtkdlfcheck = 0;
BOOL movealtkdlfcheck2 = 0;
// The billboard vertex buffer and texture

CASE cASE[15];
int indexRate = 0;
bool bRead = FALSE,bRead2 = FALSE;
HINSTANCE hInstance;
HWND hWnd;
bool AnimationCheck1 = 0, AnimationCheck2 = 0;
DWORD h_SaveBlock, h_SaveBlock2, h_SaveBlock3, h_SaveBlock4, h_SaveBlock5, h_SaveBlock6;//����

void ASERender(int j);
void movea(int flag, int a); //������Ʈ �̵�
void KeyDownCheck();
void AnimationNumber( int j);
void ASERender2(int j, int flag);
void bbdRender();
void MoveCheck();
void movealtkdlf();
void fierRender(int i);
void effectRender(int i);
VOID Render();

int midi_ids[10]; // mid
int sound_ids[10]; // wav


D3DXMATRIX		matMove;
D3DXMATRIX		matMove2;
D3DXMATRIX		matMove3;
D3DXMATRIX		matMove4;
D3DXMATRIX		matMove5;

D3DXMATRIX		matMove6;
D3DXMATRIX		matMove7;
D3DXMATRIX		matMove8;
D3DXMATRIX		matMove9;
D3DXMATRIX		matMove10;

D3DXMATRIX		matMove11;
D3DXMATRIX		matMove12;
D3DXMATRIX		matMove13;

D3DXMATRIX		matMove14[10];
D3DXMATRIX		matMove15[10];

D3DXMATRIX		matMove16; //back
D3DXMATRIX		matMove17; //back

D3DXMATRIX		matMove18; //�Ҳ�
D3DXMATRIX		matMove19; //�Ҳ�

D3DXMATRIX		matMove20; //�ٿ���ڽ�
D3DXMATRIX		matMove21; //�ٿ���ڽ�

D3DXMATRIX		matMove22; //�ٿ���ڽ�
D3DXMATRIX		matMove23; //�ٿ���ڽ�
D3DXMATRIX		matMove24; //�ٿ���ڽ�
D3DXMATRIX		matMove25; //�ٿ���ڽ�
D3DXMATRIX		matMove26; //�ٿ���ڽ�
D3DXMATRIX		matMove27; //�ٿ���ڽ�
D3DXMATRIX		matMove28; //�ٿ���ڽ�

D3DXMATRIX		matMove29[20]; //�ٿ���ڽ�
D3DXMATRIX		matMove30[20]; //�ٿ���ڽ�

D3DXMATRIX		matMove31; //�浹�� �ؽ���

D3DXMATRIX		matMove32; //��ũ..

D3DXMATRIX		matMove33[10]; //���̻���..

D3DXMATRIX		matMove34; //���ѷα� ��� ��ũ��..
D3DXMATRIX		matMove35; //���ѷα� ��� ��ũ��..

D3DXMATRIX		matMove36; //���ӿ���..

D3DXMATRIX		matMove37; //�� �̻��� �浹üũ..

D3DXMATRIX		matMove38; //���ѷα�..
s3DVertex2 BillboardVerts8[4];

int GameMode = 0; //�߿�..���Ӹ��

void LogKeyDown();

int cndehf1[20]; //�浹
int cndehf11[20];
int cndehf2[20];
int cndehf21[20];
int cndehfwndlsrhd[10],cndehfwndlsrhd2 = 0;//���ΰ� �浹üũ
int ddd = 0, eee = 0;//�浹 ���� �ִϸ��̼�
int fontcheck = 1;
bool effectcheck = 0;
Coll_Info Object[50]; //�浹 ����ü..�ٿ�� �ڽ� ����ü
int resent = 0, old = 0, allcount = 0;
bool matMove6check = 0;
bool matMove33check = 0;
HRESULT FadeOut();
void moveplane();
void bbdRender2();
void GameOverRender();
int prol = 0;
#endif
