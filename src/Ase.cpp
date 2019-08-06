#include<d3d8.h>
#include<d3d8types.h>
#include<d3dx8.h>
#include<windows.h>
#include"ASE.H"
#include"math.h"


#define MAX_BUFFER 250
#define MAX_LINE 40

extern CASE cASE;
//extern CASE cAse;

//***************************************************************
//*	 Global Variable											*
//***************************************************************


//***************************************************************
//*	 CASE Class													*
//***************************************************************

CASE::CASE()
{	
	m_dwTotalMaterial	= 0;
	m_dwTotalGObject	= 0;

	m_pMaterial		= NULL;	
	m_pGObject		= NULL;		

	m_pRoot = NULL;

}

CASE::~CASE()
{
	DestroyObject();
}

void CASE::DestroyObject()
{
	DestroyTree(m_pRoot);	// 생성된 트리를 제거
	m_pRoot = NULL;

	if(m_pMaterial)
	{
		delete []m_pMaterial;
		m_pMaterial = NULL;
	}
	
	if(m_pGObject)
	{
		delete []m_pGObject;
		m_pGObject = NULL;
	}
//	m_dwTotalGObject = 0;
//	m_dwTotalMaterial = 0;
}


//-----------------------------------------------------------------------------
// Name: CASE::ReadASE()
// Desc: ASE화일을 로드한다.
//-----------------------------------------------------------------------------
BOOL CASE::ReadASE( LPSTR fileName )
{	
	FILE	*fp;
	char	buffer[MAX_BUFFER], lineHead[MAX_LINE], linethree[MAX_BUFFER];
	int		index=0;
	
	DestroyObject();	// 이전 객체 제거
	
	fp = fopen(fileName, "rb");
	if( fp == NULL )
	{
		MessageBox(NULL, "Cannot open File", "ASEViewer", MB_OK|MB_ICONERROR);
		return FALSE;	
	}
	
	ScanFile(fp);		// 화일을 미리 한번 읽는다. pass 1의 의미
	
	if( !ReadGlobalInfo(fp) )
	{
		fclose(fp);
		MessageBox(NULL, "This is not ASE file!", "ASEViewer", MB_OK|MB_ICONERROR);
		return FALSE;
	}	
	
	m_pMaterial = new CMaterial[m_dwTotalMaterial];
	m_pGObject = new CGObject[m_dwTotalGObject];	

	fgets(buffer, MAX_BUFFER, fp);
	sscanf(buffer, "%s", lineHead);
	
	while( !feof(fp) )
	{
		if( !strcmp( MAT_LIST, lineHead) )
			ReadMaterialList(fp);					
		else if( !strcmp( GEOMOBJECT, lineHead ) )					
		{
			ReadGeomObject(fp, index);
			index++;
		}
		else if( !strcmp( Sound, lineHead )){
		sscanf(buffer, "%s%s", lineHead, linethree);
		strcpy(SoundName, linethree);
		}
		else ;	
		// 기타 다른 HIGH LEVEL들은 여기다 추가
		
		fgets(buffer, MAX_BUFFER, fp);
		sscanf(buffer, "%s", lineHead);
	}

	CreateTree();	// 오브젝트들의 트리를 구성한다.

	// 빈 프레임을 위한 예비 TM을 만든다.
	Node *pNode = NULL;
	for(DWORD i=0; i<m_dwTotalGObject ; i++)
	{
		pNode = FindParent(i);

		if(pNode == NULL)	//부모가 없는 루트노드
		{
			m_pGObject[i].m_LocalTM = m_pGObject[i].m_TM;
			continue;
		}
		// LocalTM = 자기TM * 부모InverseTM		
		m_pGObject[i].m_LocalTM = m_pGObject[i].m_TM * m_pGObject[pNode->ID].m_InverseTM;		
	}
	
	fclose(fp);
	return TRUE;
}

BOOL CASE::ReadGlobalInfo(FILE *fp)
{
	char buffer[MAX_BUFFER], lineHead[MAX_LINE];
	
	fgets(buffer, MAX_BUFFER, fp);
	sscanf(buffer, "%s", lineHead);
	
	if( strcmp( ASEFILE, lineHead) )	// ASE 파일인지 체크
		return FALSE;
	
	while( strcmp( END_PARA, lineHead ) )	// SCENE부분을 스킵한다.
	{
		fgets(buffer, MAX_BUFFER, fp);
		sscanf(buffer, "%s", lineHead);
	}
	
	return TRUE;
}

void CASE::ScanFile(FILE *fp)
{
	char buffer[MAX_BUFFER], lineHead[MAX_LINE];
	while( !feof(fp) )
	{
		fgets(buffer, MAX_BUFFER, fp);
		sscanf(buffer, "%s", lineHead);
	
		if( !strcmp( MAT_MATERIAL , lineHead ) )
			m_dwTotalMaterial++;
	
		else if( !strcmp( GEOMOBJECT, lineHead ) )
			m_dwTotalGObject++;
		
	}
	fseek(fp, 0, SEEK_SET);	// 다시 처음으로 돌린다.
}

//-----------------------------------------------------------------------------
// Name: CASE::ReadMaterialList()
// Desc: ASE화일중 매터리얼 정보를 읽는다.
//-----------------------------------------------------------------------------

void CASE::ReadMaterialList(FILE *fp)
{
	char buffer[MAX_BUFFER], lineHead[MAX_LINE], lineSecond[MAX_LINE];
	int index;
	int iBlockCnt = 1;	// *MATERIAL_LIST { 에서 하나 거쳤기 때문에	
	
	while(iBlockCnt)
	{
		fgets(buffer, MAX_BUFFER, fp);
		sscanf(buffer, "%s%s", lineHead, lineSecond);
		
		if( !strcmp( START_PARA, lineSecond ) )		
			iBlockCnt++;
		if( !strcmp( END_PARA, lineHead ) )
			iBlockCnt--;
		
		if( !strcmp( MAT_MATERIAL, lineHead ) )
		{
			sscanf(buffer, "%s%d", lineHead, &index);
			iBlockCnt++;	// MATERIAL index { 이순서이기 때문에..
		}		
		else if( !strcmp( MAT_AMBIENT, lineHead ) )
		{
			sscanf(buffer, "%s%f%f%f", lineHead, &m_pMaterial[index].m_material.Ambient.r,
											 	 &m_pMaterial[index].m_material.Ambient.g,
												 &m_pMaterial[index].m_material.Ambient.b);
		}		
		else if( !strcmp( MAT_DIFFUSE, lineHead ) )
		{
			sscanf(buffer, "%s%f%f%f", lineHead, &m_pMaterial[index].m_material.Diffuse.r,
												 &m_pMaterial[index].m_material.Diffuse.g,
												 &m_pMaterial[index].m_material.Diffuse.b);
		}		
		else if( !strcmp( MAT_SPECULAR, lineHead ) )
		{
			sscanf(buffer, "%s%f%f%f", lineHead, &m_pMaterial[index].m_material.Specular.r,
												 &m_pMaterial[index].m_material.Specular.g,
												 &m_pMaterial[index].m_material.Specular.b);
		}	
		else if( !strcmp( TEXTURE, lineHead ) )
		{
			sscanf(buffer, "%s%s", lineHead, lineSecond);
			DeleteMark(buffer);
			strcpy(m_pMaterial[index].m_pTextureName, buffer);
			m_pMaterial[index].m_pTextureName[ strlen(buffer) ] = NULL;
			m_pMaterial[index].m_bIsTex = TRUE;
			// 텍스쳐 로딩을 여기서
		}
		//기타 다른 매터리얼에 관한 추가사항은 여기에
	}
}
//-----------------------------------------------------------------------------
// Name: CASE::ReadGeomObject()
// Desc: ASE화일에서 GEOMOBJECT를 읽는다
//-----------------------------------------------------------------------------

void CASE::ReadGeomObject(FILE *fp, int index)
{	
	char		buffer[MAX_BUFFER], lineHead[MAX_LINE], lineSecond[MAX_LINE];	
	int			iBlockCnt = 1;		// 탈출 조건을 위한
	int			a, b, c;			// Face를 위한
	DWORD		i;					// for루프를 위한 변수
	CGObject	*pObject;			// 현재 오브젝트의 포인터 

	D3DVECTOR	*pTVertex = NULL;	// 텍스쳐좌표를 위한 변수
	Face		*pTFace = NULL;
	DWORD		iNumTVertex, iNumTFace;		
	
	pObject = &m_pGObject[index];	
	
	while( iBlockCnt )
	{
		fgets(buffer, MAX_BUFFER, fp);
		sscanf(buffer, "%s%s", lineHead, lineSecond);
		
		if( !strcmp( START_PARA, lineSecond ) )
		{
			iBlockCnt++;
			lineSecond[0] = NULL;
		}
		if( !strcmp( END_PARA, lineHead ) )
			iBlockCnt--;

		if( !strcmp( TM_ROW0, lineHead ) )		// y와 z교체..
		{
			sscanf(buffer, "%s%f%f%f", lineHead, &pObject->m_TM._11,
										 		 &pObject->m_TM._13,
												 &pObject->m_TM._12);
		}
		else if( !strcmp( TM_ROW1, lineHead ) )		
		{
			sscanf(buffer, "%s%f%f%f", lineHead, &pObject->m_TM._31,
												 &pObject->m_TM._33, 
												 &pObject->m_TM._32);
		}
		else if( !strcmp( TM_ROW2, lineHead ) )
		{
			sscanf(buffer, "%s%f%f%f", lineHead, &pObject->m_TM._21,
												 &pObject->m_TM._23,
												 &pObject->m_TM._22);
		}
		else if( !strcmp( TM_ROW3, lineHead ) )
		{
			sscanf(buffer, "%s%f%f%f", lineHead, &pObject->m_TM._41,
												 &pObject->m_TM._43,
												 &pObject->m_TM._42);
		}
		else if( !strcmp( NUM_VERTEX, lineHead ) )
		{
			sscanf(buffer, "%s%d", lineHead, &pObject->m_dwVertexCount);
			pObject->m_pVertexList = new Vertex[pObject->m_dwVertexCount];
			pObject->m_pVertexCashe = new Vertex[pObject->m_dwVertexCount];
		}
		else if( !strcmp( NUM_FACE, lineHead ) )
		{
			sscanf(buffer, "%s%d", lineHead, &pObject->m_dwFaceCount);
			pObject->m_pFaceList = new Face[pObject->m_dwFaceCount];
		}
		else if( !strcmp( NODENAME, lineHead ) )
		{			
			DeleteMark(buffer);
			pObject->m_pNodeName = new char[strlen(buffer) + 1];
			strcpy(pObject->m_pNodeName, buffer);
			pObject->m_pNodeName[strlen(buffer)] = NULL;
		}
		else if( !strcmp( NODEPARENT, lineHead ) )
		{			
			DeleteMark(buffer);
			pObject->m_pNodeParent = new char[strlen(buffer) + 1];
			strcpy(pObject->m_pNodeParent, buffer);
			pObject->m_pNodeParent[strlen(buffer)] = NULL;
		}
		else if( !strcmp( VERTEX_LIST, lineHead ) )
		{
			for( i=0; i< pObject->m_dwVertexCount ; i++ )
			{
				fgets(buffer, MAX_BUFFER, fp);		// y와 z를 바꾸었다.
				sscanf(buffer, "%s%s%f%f%f", lineHead, lineSecond, &pObject->m_pVertexList[i].x,
																   &pObject->m_pVertexList[i].z,
																   &pObject->m_pVertexList[i].y);
			}
		}
		else if( !strcmp( FACE_LIST, lineHead ) )
		{			
			for( i=0 ; i<pObject->m_dwFaceCount ; i++)
			{
				fgets(buffer, MAX_BUFFER, fp);		
				sscanf(buffer, "%s%s%s%d%s%d%s%d", lineHead, lineHead, lineHead, &a,
																	   lineHead, &b,
																	   lineHead, &c);
				pObject->m_pFaceList[i].a = a;	// 이상한 부분, 직접 넣으면 delete할때 에러가
				pObject->m_pFaceList[i].b = b;	// WORD여서 그런걸지도...2바이트니까..
				pObject->m_pFaceList[i].c = c;
			}
		}
		else if( !strcmp( NUM_TVERTEX, lineHead ) )
		{
			sscanf(buffer, "%s%d", lineHead, &iNumTVertex);
			pTVertex = new D3DVECTOR[iNumTVertex];
		}
		else if( !strcmp( NUM_TFACE, lineHead ) )
		{
			sscanf(buffer, "%s%d", lineHead, &iNumTFace);
			pTFace = new Face[iNumTFace];
		}
		else if( !strcmp( TVERT_LIST, lineHead ) )
		{
			for( i=0 ; i<iNumTVertex ; i++)
			{
				fgets(buffer, MAX_BUFFER, fp);
				sscanf(buffer, "%s%s%f%f", lineHead, lineHead, &pTVertex[i].x, &pTVertex[i].y);
			}
		}
		else if( !strcmp( TFACE_LIST, lineHead ) )
		{
			for( i=0 ; i<iNumTFace ; i++)
			{
				fgets(buffer, MAX_BUFFER, fp);		
				sscanf(buffer, "%s%s%d%d%d", lineHead, lineHead, &a, &b, &c);
				
				pTFace[i].a = a;	//위의 FaceList를 읽을 때와 같은 이유
				pTFace[i].b = b;
				pTFace[i].c = c;
			}
			// Texture 좌표(tu, tv) 정리
			for( i=0 ; i< pObject->m_dwFaceCount ; i++)
			{
				pObject->m_pVertexList[ pObject->m_pFaceList[i].a ].tu = pTVertex[ pTFace[i].a ].x;
				pObject->m_pVertexList[ pObject->m_pFaceList[i].a ].tv = pTVertex[ pTFace[i].a ].y * -1;
		
				pObject->m_pVertexList[ pObject->m_pFaceList[i].b ].tu = pTVertex[ pTFace[i].b ].x;
				pObject->m_pVertexList[ pObject->m_pFaceList[i].b ].tv = pTVertex[ pTFace[i].b ].y * -1;
		
				pObject->m_pVertexList[ pObject->m_pFaceList[i].c ].tu = pTVertex[ pTFace[i].c ].x;
				pObject->m_pVertexList[ pObject->m_pFaceList[i].c ].tv = pTVertex[ pTFace[i].c ].y * -1;
			}
		}
		else if( !strcmp( TM_ANI, lineHead ) )
		{
			ReadTMAnimation(fp, index);
			m_pGObject[index].AnimationCheck = 1;
			iBlockCnt--;		// ReadTMAnimation()에서 한개를 더 읽기 때문이다.
		}
		else if( !strcmp( MAT_REF, lineHead ) ){
			sscanf(buffer, "%s%d", lineHead, &pObject->m_dwMaterialREF );
			if(m_pMaterial[pObject->m_dwMaterialREF].m_bIsTex == TRUE)
				m_pGObject[index].m_bTexture_Flags = 1;
			else
				m_pGObject[index].m_bTexture_Flags = 0;
		}
	}	// end of while	

	D3DXMatrixInverse(&pObject->m_InverseTM, NULL, &pObject->m_TM);

	pObject->ComputeNormal();	//오브젝트의 노말벡터를 계산한다.
	pObject->MultiplyInvese();

	delete[] pTVertex;
	pTVertex = NULL;
	delete[] pTFace;
	pTFace = NULL;
}	// end of function



//-----------------------------------------------------------------------------
// Name: CASE::ReadTMAnimation()
// Desc: ASE화일에서 pos값과 rot값을 읽어들인다.
//		 미리 TM_ANIMATION에서 POS와 ROT의 개수를 미리 한번 읽고 실제 값을 저장한다.
//-----------------------------------------------------------------------------

void CASE::ReadTMAnimation(FILE *fp, int index)
{
	char		buffer[MAX_BUFFER], lineHead[MAX_LINE], lineSecond[MAX_LINE];
	CGObject	*pObject;
	int			iBlockCnt = 1;	// TM_ANIMAITON 에서 한개 읽었으므로
	int			iFilePos, iPosCnt, iRotCnt;
	D3DXVECTOR3	v;
	float		w;

	iPosCnt = iRotCnt = 0;
	iFilePos = ftell(fp);

	pObject = &m_pGObject[index];

	while(iBlockCnt)		// pos와 rot의 갯수를 센다
	{
		fgets(buffer, MAX_BUFFER, fp);
		sscanf(buffer, "%s%s", lineHead, lineSecond);

		if( !strcmp( START_PARA, lineSecond ) )
		{
			iBlockCnt++;
			lineSecond[0] = NULL;
		}
		if( !strcmp( END_PARA, lineHead ) )
			iBlockCnt--;
		if( !strcmp( POS_SAMPLE, lineHead ) )
			iPosCnt++;
		else if( !strcmp( ROT_SAMPLE, lineHead ) )
			iRotCnt++;
	}

	pObject->m_iPosCnt = iPosCnt;
	pObject->m_iRotCnt = iRotCnt;

	iBlockCnt = 1;
	fseek(fp, iFilePos, SEEK_SET);	// 원래 자리로 fp를 이동시킨다.

	if(iPosCnt != 0)
		pObject->m_pAniPos = new Pos[iPosCnt];
	if(iRotCnt != 0)
		pObject->m_pAniQuater = new AniQuater[iRotCnt];

	iPosCnt = iRotCnt = 0;	// 이제부터 인덱스로 사용한다.

	while(iBlockCnt)		// 실제 읽어들이는 부분
	{
		fgets(buffer, MAX_BUFFER, fp);
		sscanf(buffer, "%s%s", lineHead, lineSecond);

		if( !strcmp( START_PARA, lineSecond ) )
		{
			iBlockCnt++;
			lineSecond[0] = NULL;
		}
		if( !strcmp( END_PARA, lineHead ) )
			iBlockCnt--;

		if( !strcmp( POS_SAMPLE, lineHead ) )
		{							// y와 z를 교체한다.
			sscanf(buffer, "%s%d%f%f%f", lineHead, &pObject->m_pAniPos[iPosCnt].index,
												   &pObject->m_pAniPos[iPosCnt].x,
												   &pObject->m_pAniPos[iPosCnt].z,
												   &pObject->m_pAniPos[iPosCnt].y);
			AniPosIndex = pObject->m_pAniPos[1].index; //Render 할때 애니메이션 정도..
			iPosCnt++;
		}
		else if( !strcmp( ROT_SAMPLE, lineHead ) )
		{	
			if(index == 2)
			{
				index = 2;
			}
			
			// y와 z를 교체한다.
			sscanf(buffer, "%s%d%f%f%f%f", lineHead, &pObject->m_pAniQuater[iRotCnt].index,
													 &v.x, &v.z, &v.y, &w);
			// 쿼터니언 생성
			D3DXQuaternionRotationAxis( &pObject->m_pAniQuater[iRotCnt].q, &v, w);
//			m_pGObject[index].a[iRotCnt].x = v.x; //cik 포멧중 적용시키기 위해..
//			m_pGObject[index].a[iRotCnt].y = v.y;
//			m_pGObject[index].a[iRotCnt].z = v.z;
//			m_pGObject[index].b[iRotCnt] = w;
			iRotCnt++;
			AniPosIndex = pObject->m_pAniQuater[1].index; //Render 할때 애니메이션 정도..
		}			
	}
}


//-----------------------------------------------------------------------------
// Name: CASE::Animate()
// Desc: 캐릭터를 index에 따라 애니메이션을 하도록한다.
//		 나중에 분명히 고쳐야 할 함수 (지금은 단지 테스트..)
//-----------------------------------------------------------------------------

void CASE::Animate(int indexRate)
{
	DWORD i;
	D3DXMATRIX mat;

	D3DXMatrixIdentity(&mat);

	ComputeAniTM(m_pRoot, mat, indexRate);	

	for( i=0 ; i<m_dwTotalGObject ; i++)
	{
		m_pGObject[i].AnimateObject();
	}
}

//-----------------------------------------------------------------------------
// Name: CASE::ComputeAniTM
// Desc: 트리를 따라가며 각 오브젝트의 AniTM을 생성한다.
//-----------------------------------------------------------------------------

void CASE::ComputeAniTM(Node* pRoot, D3DXMATRIX parentTM, int indexRate)
{	
	CGObject		*pObject;
	D3DXMATRIX		myAniTM;
	Pos				position;
	D3DXQUATERNION	q;
	BOOL			bIsRot = FALSE;	
	int				i;

	if(pRoot->pSibling != NULL)
		ComputeAniTM(pRoot->pSibling, parentTM, indexRate);
	
	pObject = &m_pGObject[ pRoot->ID ];	
	
	for( i=0 ; i<pObject->m_iPosCnt ; i++)	// 현재 애니에 이용할 pos값을 찾는다.
	{
		if( pObject->m_pAniPos[i].index == indexRate)
		{
			position = pObject->m_pAniPos[i];
			break;
		}
	}

	for( i=0 ; i<pObject->m_iRotCnt ; i++)	// 현재 애니에 이용할 rot값을 찾는다.
	{
		if( pObject->m_pAniQuater[i].index == indexRate )		
		{
			bIsRot = TRUE;
			q = pObject->m_pAniQuater[i].q;
			break;
		}
	}	

	if(position.index == -1)	// pos값이 없는 경우
	{
		position.x = pObject->m_LocalTM._41;
		position.y = pObject->m_LocalTM._42;
		position.z = pObject->m_LocalTM._43;
	}

	if(!bIsRot)		//rot값이 없다면		
		D3DXQuaternionRotationMatrix(&q, &pObject->m_LocalTM);	

	// 자신의 AniTM 계산
	if( indexRate == 0 )	
	{		
		pObject->m_PreQuater = q;
		D3DXMatrixRotationQuaternion(&myAniTM, &pObject->m_PreQuater);
	}		
	else								// 이전 쿼터니언에 누적
	{
		// 수정이 반드시 필요한 부분 
		// 나중에 새로 만들때 처음 ase화일 로드하는 부분에서 고쳐서 새로 수정해야함
		if(!bIsRot)	
			myAniTM = pObject->m_LocalTM;
		else
		{
			D3DXQuaternionMultiply( &pObject->m_PreQuater, &pObject->m_PreQuater, &q);		
			D3DXMatrixRotationQuaternion(&myAniTM, &pObject->m_PreQuater);	
		}
	}
	myAniTM._41 = position.x;
	myAniTM._42 = position.y;
	myAniTM._43 = position.z;
	
	// 자기 AniTM * 부모의 AniTM
	pObject->m_AniTM = myAniTM * parentTM;

	if(pRoot->pChild != NULL)			// 자식에게 자신의 최종AniTM을 넘겨준다.
		ComputeAniTM(pRoot->pChild, pObject->m_AniTM, indexRate);
}

//-----------------------------------------------------------------------------
// Name: CASE::CreateTree()
// Desc: GObject의 트리를 생성한다.
//       제일 처음 들어오는 노드는 부모가 없는 트리여야 한다.
//       모든 오브젝트의 순서는 부모가 먼저 나온다고 가정한다.
//       즉 부모가 있는 자식이라면 부모보다 먼저 ASE파일에 나오지 않는다는 가정이다.
//		 부모가 있는 노드라면 반드시 부모가 트리 안에 있다고 가정하고 시작한다.
//       이런 가정없이 트리를 만든다면..우선 모든 오브젝트에 대한 노드를 미리 만들고 시작
//		 하면 될지도 모르지만 부모가 없는 노드가 많지 않으면 삽질이지 않을까?
//-----------------------------------------------------------------------------
void CASE::CreateTree()
{	
	Node		*pNode;

	for(DWORD i=0 ; i < m_dwTotalGObject ; i++)
	{
		if(m_pGObject[i].m_pNodeParent == NULL)	// 부모가 없다.
		{
			if(m_pRoot == NULL)		// 부모가 없는 노드가 처음인가?
			{
				pNode = new Node;
				pNode->ID = i;
				m_pRoot = pNode;
			}
			else	// 이전에 부모가 없는 노드가 있었으므로 루트의 형제로 연결
			{
				pNode = m_pRoot;

				while(pNode->pSibling != NULL)
					pNode = pNode->pSibling;

				pNode->pSibling = new Node;
				pNode->pSibling->ID = i;
			}
		}
		else					// 부모가 있는 노드이다.
		{
			pNode = FindParent(i);

			if(pNode->pChild == NULL)	// 찾은 부모에게 자식이 한개도 없다면..
			{
				pNode->pChild = new Node;
				pNode->pChild->ID = i;
			}
			else			//부모에게는 이미 다른 자식들이 있으므로 형제들과 연결시킨다.
			{
				pNode = pNode->pChild;	//첫번째 형제의 노드
				while(pNode->pSibling != NULL)
					pNode = pNode->pSibling;

				pNode->pSibling = new Node;
				pNode->pSibling->ID = i;
			}		
		}
	}

}

//-----------------------------------------------------------------------------
// Name: CASE::FindParent()
// Desc: 현재 주어진 오브젝트의 인덱스로 오브젝트의 부모를 찾는다.
//-----------------------------------------------------------------------------

Node* CASE::FindParent(int id)
{
	return SearchNode(m_pRoot, m_pGObject[id].m_pNodeParent);
}

//-----------------------------------------------------------------------------
// Name: CASE::SearchNode()
// Desc: 주어진 노드이름에 맞는 노드를 트리에서 찾아 그 포인터를 리턴한다.
//		 변형이 되었다고는 하지만 결국은 Binary Tree이므로 일반 Binary Tree의
//		 Search 방법 그대로 In Order로 서치한다.
//-----------------------------------------------------------------------------

Node* CASE::SearchNode(Node *pRoot, char *pNodeName)
{
	Node	*p = NULL;

	if(pNodeName == NULL)
		return NULL;

	if(pRoot->pChild != NULL)
	{
		p = SearchNode(pRoot->pChild, pNodeName);	
		if(p)
			return p;
	}
	if( !strcmp(m_pGObject[pRoot->ID].m_pNodeName, pNodeName) )
		return pRoot;		//현재 노드가 찾는 노드이다.
		
	if(pRoot->pSibling != NULL)	
	{
		p = SearchNode(pRoot->pSibling, pNodeName);
		if(p)
			return p;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Name: CASE::DestroyTree()
// Desc: 구성된 트리를 제거한다.
//		 밑에서부터 제거해야 메모리릭이 발생안하므로
//		 Post Order순으로 탐색하며 제거한다.
//-----------------------------------------------------------------------------

void CASE::DestroyTree(Node *pRoot)
{
	if(pRoot == NULL)	//트리가 구성되어 있지 않다.
		return;	

	if( pRoot->pChild != NULL )
		DestroyTree(pRoot->pChild);
	if( pRoot->pSibling != NULL )
		DestroyTree(pRoot->pSibling);

	delete pRoot;	// 자식이 아무도 없으므로 자기 자신을 제거한다.
}

//***************************************************************
//*	 CGObject Class												*
//***************************************************************

//-----------------------------------------------------------------
// Name: CGObject::AnimateObject()
// Desc: 계산된 aniTM과 자기 TM을 가지고 오브젝트를 움직인다.
//-----------------------------------------------------------------

void CGObject::AnimateObject()
{
	D3DXMATRIX	mat;
	float		x, y, z;
	
	memcpy( m_pVertexCashe, m_pVertexList, sizeof(Vertex) * m_dwVertexCount );
	
	mat = m_AniTM; 

	for( DWORD i=0 ; i < m_dwVertexCount ; i++)
	{		
		x = m_pVertexList[i].x * mat._11 + m_pVertexList[i].y * mat._21 + 
			m_pVertexList[i].z * mat._31 + mat._41;

		y = m_pVertexList[i].x * mat._12 + m_pVertexList[i].y * mat._22 +
			m_pVertexList[i].z * mat._32 + mat._42;

		z = m_pVertexList[i].x * mat._13 + m_pVertexList[i].y * mat._23 +
			m_pVertexList[i].z * mat._33 + mat._43;

		m_pVertexCashe[i].x = x;
		m_pVertexCashe[i].y = y;
		m_pVertexCashe[i].z = z;
	}	
}


void CGObject::MultiplyInvese()
{
	D3DXMATRIX	mat;
	float		x, y, z;

	mat = m_InverseTM;

	for( DWORD i=0 ; i < m_dwVertexCount ; i++)
	{		
		x = m_pVertexList[i].x * mat._11 + m_pVertexList[i].y * mat._21 + 
			m_pVertexList[i].z * mat._31 + mat._41;

		y = m_pVertexList[i].x * mat._12 + m_pVertexList[i].y * mat._22 +
			m_pVertexList[i].z * mat._32 + mat._42;

		z = m_pVertexList[i].x * mat._13 + m_pVertexList[i].y * mat._23 +
			m_pVertexList[i].z * mat._33 + mat._43;

		m_pVertexList[i].x = x;
		m_pVertexList[i].y = y;
		m_pVertexList[i].z = z;
	}
}

//-----------------------------------------------------------------------------
// Name: CGObject::ComputeNormal()
// Desc: 오브젝트가 가진 각 버텍스의 노말벡터를 구한다.
//-----------------------------------------------------------------------------

void CGObject::ComputeNormal()
{
	D3DXVECTOR3	n, n1, n2 ;							
	
	for( DWORD i=0 ; i<m_dwFaceCount ; i++)
	{
		n1.x = m_pVertexList[ m_pFaceList[i].c ].x - m_pVertexList[ m_pFaceList[i].b ].x;
		n1.y = m_pVertexList[ m_pFaceList[i].c ].y - m_pVertexList[ m_pFaceList[i].b ].y;
		n1.z = m_pVertexList[ m_pFaceList[i].c ].z - m_pVertexList[ m_pFaceList[i].b ].z;
		
		n2.x = m_pVertexList[ m_pFaceList[i].c ].x - m_pVertexList[ m_pFaceList[i].a ].x;
		n2.y = m_pVertexList[ m_pFaceList[i].c ].y - m_pVertexList[ m_pFaceList[i].a ].y;
		n2.z = m_pVertexList[ m_pFaceList[i].c ].z - m_pVertexList[ m_pFaceList[i].a ].z;
		
		D3DXVec3Cross(&n, &n1, &n2);		
		D3DXVec3Normalize(&n, &n);
		
		m_pVertexList[ m_pFaceList[i].a ].nx += n.x;
		m_pVertexList[ m_pFaceList[i].a ].ny += n.y;
		m_pVertexList[ m_pFaceList[i].a ].nz += n.z;
		
		m_pVertexList[ m_pFaceList[i].b ].nx += n.x;
		m_pVertexList[ m_pFaceList[i].b ].ny += n.y;
		m_pVertexList[ m_pFaceList[i].b ].nz += n.z;
		
		m_pVertexList[ m_pFaceList[i].c ].nx += n.x;
		m_pVertexList[ m_pFaceList[i].c ].ny += n.y;
		m_pVertexList[ m_pFaceList[i].c ].nz += n.z;
	}
	
	for( i=0 ; i< m_dwVertexCount ; i++)
	{
		n.x = m_pVertexList[i].nx;
		n.y = m_pVertexList[i].ny;
		n.z = m_pVertexList[i].nz;
		
		D3DXVec3Normalize(&n, &n);
		
		m_pVertexList[i].nx = n.x;
		m_pVertexList[i].ny = n.y;
		m_pVertexList[i].nz = n.z;
	}
}

CGObject::CGObject()
{
	m_dwVertexCount = 0;
	m_dwFaceCount = 0;
	m_dwMaterialREF = -1;	// 참조 메터리얼이 없으면 -1 이다.

	D3DXMatrixIdentity(&m_TM);
	D3DXMatrixIdentity(&m_InverseTM);
	D3DXMatrixIdentity(&m_LocalTM);
	D3DXMatrixIdentity(&m_AniTM);

	m_pVertexList = NULL;
	m_pVertexCashe = NULL;
	m_pFaceList = NULL;	
	
	m_pAniPos = NULL;
	m_pAniQuater = NULL;
	m_pNodeName = NULL;
	m_pNodeParent = NULL;
	
}

CGObject::~CGObject()
{
	if(m_pVertexCashe)
	{
		delete[] m_pVertexCashe;
		m_pVertexCashe = NULL;
	}
	if(m_pAniPos)
	{
		delete[] m_pAniPos;
		m_pAniPos = NULL;
	}
	if(m_pAniQuater)
	{
		delete[] m_pAniQuater;
		m_pAniQuater = NULL;
	}
	if(m_pNodeName)
	{
		delete[] m_pNodeName;
		m_pNodeName = NULL;
	}
	if(m_pNodeParent)
	{
		delete[] m_pNodeParent;
		m_pNodeParent = NULL;
	}	
	if(m_pVertexList)
	{
		delete[] m_pVertexList;
		m_pVertexList = NULL;
	}
	if(m_pFaceList)
	{
		delete[] m_pFaceList;
		m_pFaceList = NULL;
	}
}


//***************************************************************
//*	 CMaterial Class											*
//***************************************************************

CMaterial::CMaterial()
{
/*	m_material.Diffuse.r = m_material.Ambient.r = 1.0f;
	m_material.Diffuse.g = m_material.Ambient.g = 1.0f;
	m_material.Diffuse.b = m_material.Ambient.b = 1.0f;
	m_material.Diffuse.a = m_material.Ambient.a = 1.0f;
*/
	ZeroMemory( &m_material, sizeof(D3DMATERIAL8));

	m_bIsTex = FALSE;
	m_pTextureName[0] = NULL;

}

CMaterial::~CMaterial()
{
}
//-----------------------------------------------------------------------------
// Name: DeleteMark()
// Desc: " "를 제거하고 원하는 내용만으로 만들어준다.
//-----------------------------------------------------------------------------
void DeleteMark(char *src)
{
	int length, i=0;
	char *pTemp;

	while(src[i++] != '"');

	length = strlen(&src[i]);	
	
	pTemp = new char[length-1];

	for(int j=0 ; src[i]!='"'; i++)
		pTemp[j++] = src[i];
	pTemp[j] = NULL;
	strcpy(src, pTemp);
	delete []pTemp;
}