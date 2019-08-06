
#ifndef _ASE_H_
#define _ASE_H_

#include<stdio.h>

//***************************************************************
//*	 Structure													*
//***************************************************************

typedef struct tagVertex
{
	float x,y,z;
	float nx, ny, nz;
	float tu, tv;

	tagVertex()
	{
		x = y = z = 0;
		nx = ny = nz = 0;
		tu = tv = 0;
	}
}Vertex;

typedef struct tagFace
{
	WORD a, b, c;

	tagFace()
	{
		a = b = c = 0;
	}
}Face;

// A structure for our custom vertex type
typedef struct tagNode
{
	int ID;	
	tagNode *pChild;
	tagNode *pSibling;

	tagNode()
	{
		ID = -1;		
		pChild = NULL;
		pSibling = NULL;
	}
}Node;

typedef struct tagPos
{
	int index;
	float x, y, z;

	tagPos()
	{
		index = -1;
		x = y = z = 0.0f;
	}
}Pos;

typedef struct tagAniQuater
{
	int index;
	D3DXQUATERNION	q;
	tagAniQuater()
	{
		index = -1;
		q.x = q.y = q.z = 0;
		q.w = 1;
	}		
}AniQuater;




#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

struct CUSTOMVERTEX
{
    FLOAT x, y, z; // The transformed position for the vertex
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX2 (D3DFVF_XYZ)

struct CUSTOMVERTEX3
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
	FLOAT u,v;
};
#define D3DFVF_CUSTOMVERTEX3 (D3DFVF_XYZ| D3DFVF_TEX1)
//***************************************************************
//*	 Class														*
//***************************************************************

class CMaterial
{
	public:
		D3DMATERIAL8		m_material;					// ���͸���		
		BOOL				m_bIsTex;					// �ؽ����� ����
		char				m_pTextureName[MAX_PATH];	// �ؽ��� ȭ�ϸ�		
		
	public:		
		CMaterial();
		~CMaterial();
};

class CGObject	
{
	public:
		DWORD		m_dwVertexCount;	// ���ؽ� ����
		DWORD		m_dwFaceCount;		// ���̽� ����
		int			m_dwMaterialREF;	// ���͸��� ID
		D3DXMATRIX	m_TM;				// TM
		Vertex		*m_pVertexList;		// ���ؽ�(Vertex, Normal, tu, tv) ����Ʈ
		Vertex		*m_pVertexCashe;	// ���ؽ� ĳ��
		Face		*m_pFaceList;		// ���̽� ����Ʈ		

		D3DXMATRIX	m_InverseTM;		// TM�� �����
		D3DXMATRIX	m_AniTM;			// �� �����Ӹ����� �ִ�TM�� ������ ����
		D3DXMATRIX	m_LocalTM;			// �� �������� ä�� TM

		int			m_iPosCnt;			// Position ����
		int			m_iRotCnt;			// Rot ����
		Pos			*m_pAniPos;			// ani�� Pos��
		AniQuater	*m_pAniQuater;		// ani�� ���ʹϾ�
		Pos			m_sPrePos;			// ������ ������Ʈ Pos��(������ �ջ��Ű�� �ʱ����� �ӽ�)
		AniQuater	m_sPreQuater;		// ������ ������Ʈ Quater��

		D3DXQUATERNION	m_PreQuater;	// ���� �ִϸ��̼Ǳ��� ������ ���ʹϾ�

		char		*m_pNodeName;		// NodeName
		char		*m_pNodeParent;		// NodeParent

		BOOL		m_bTexture_Flags;	// �ؽ��� ��1 ��0

		
		int        AnimationCheck;
	
		D3DXVECTOR3	*a;
		float		*b;

	public:
		void MultiplyInvese();
		void ComputeNormal();		
		void AnimateObject();

		CGObject();
		~CGObject();
};

class CASE
{
	public:
		CMaterial	*m_pMaterial;		// ���͸��� ����Ʈ
		CGObject	*m_pGObject;			// �޽� ����Ʈ
		DWORD		m_dwTotalMaterial;	// �� ���͸��� ��
		DWORD		m_dwTotalGObject;		// �� �޽� ��

		Node		*m_pRoot;				// ������Ʈ���� �������� Ʈ��	
		
		FILE	*fp;
		
		char       SoundName[260];              //����
		DWORD		m_dwOldTime;				// �ð��� �����ϱ����� ����

		// Middle Level
		void ReadTMAnimation(FILE *fp, int index);

		// About Tree
		void CreateTree();
		Node* FindParent(int id);
		Node* SearchNode(Node *pRoot, char *pNodeName);
		void DestroyTree(Node *pRoot);

		// About Animation
		void ComputeAniTM(Node* pRoot, D3DXMATRIX parentTM, int indexRate);
		void Animate(int indexRate);

	public:

		// Read ASE File
		BOOL ReadASE(LPSTR fileName);
		void ScanFile(FILE *fp);

		// High Level
		BOOL ReadGlobalInfo(FILE *fp);
		void ReadMaterialList(FILE *fp);
		void ReadGeomObject(FILE *fp, int index);
		
		void DestroyObject();

		int AniPosIndex;
		CASE();
		~CASE();
		

};
//***************************************************************
//*	 Token														*
//***************************************************************

#define ASEFILE				"*3DSMAX_ASCIIEXPORT"

// About MATERIAL
#define MAT_LIST			"*MATERIAL_LIST"

#define MAT_COUNT			"*MATERIAL_COUNT"
#define MAT_MATERIAL		"*MATERIAL"

#define MAT_AMBIENT			"*MATERIAL_AMBIENT"
#define MAT_DIFFUSE			"*MATERIAL_DIFFUSE"
#define MAT_SPECULAR		"*MATERIAL_SPECULAR"

#define MAT_NUMSUBMTL		"*NUMSUBMTLS"
#define MAT_SUBMATERIAL		"*SUBMATERIAL"


#define TEXTURE				"*BITMAP"
#define UTILE				"*UVW_U_TILING"
#define VTILE				"*UVW_V_TILING"
#define UOFFSET				"*UVW_U_OFFSET"
#define VOFFSET				"*UVW_V_OFFSET"

// About GEOMOBJECT
#define GEOMOBJECT			"*GEOMOBJECT"

#define NODENAME			"*NODE_NAME"
#define NODEPARENT			"*NODE_PARENT"

#define NODE_TM				"*NODE_TM"
#define TM_ROW0				"*TM_ROW0"
#define TM_ROW1				"*TM_ROW1"
#define TM_ROW2				"*TM_ROW2"
#define TM_ROW3				"*TM_ROW3"

// About MESH
#define MESH				"*MESH"

#define NUM_VERTEX			"*MESH_NUMVERTEX"
#define NUM_FACE			"*MESH_NUMFACES"

#define VERTEX_LIST			"*MESH_VERTEX_LIST"
#define VERTEX				"*MESH_VERTEX"

#define FACE_LIST			"*MESH_FACE_LIST"
#define FACE				"*MESH_FACE"

//About TEXTURE
#define NUM_TVERTEX			"*MESH_NUMTVERTEX"
#define NUM_TFACE			"*MESH_NUMTVFACES"

#define TVERT_LIST			"*MESH_TVERTLIST"
#define TVERTEX				"*MESH_TVERT"

#define TFACE_LIST			"*MESH_TFACELIST"
#define TFACE				"*MESH_TFACE"

// About NORMAL
#define NORMALS				"*MESH_NORMALS"
#define FACE_NORMAL			"*MESH_FACENORMAL"
#define NVERTEX				"*MESH_VERTEXNORMAL"

#define MAT_REF				"*MATERIAL_REF"

// About TM Animation
#define TM_ANI				"*TM_ANIMATION"
#define POSITION			"*CONTROL_POS_TRACK"
#define ROTATE				"*CONTROL_ROT_TRACK"
#define POS_SAMPLE			"*CONTROL_POS_SAMPLE"
#define ROT_SAMPLE			"*CONTROL_ROT_SAMPLE"

// ETC
#define WIREFRAMECOLOR		"*WIREFRAME_COLOR"
#define GROUP				"*GROUP"

#define MESH_MTLID			"*MESH_MTLID"
#define MAT_REF				"*MATERIAL_REF"

#define START_PARA			"{"
#define END_PARA			"}"

#define Sound               "*Sound"

#endif

void DeleteMark(char *src);
//**************************************************************************************