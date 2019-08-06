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
	DestroyTree(m_pRoot);	// ������ Ʈ���� ����
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
// Desc: ASEȭ���� �ε��Ѵ�.
//-----------------------------------------------------------------------------
BOOL CASE::ReadASE( LPSTR fileName )
{	
	FILE	*fp;
	char	buffer[MAX_BUFFER], lineHead[MAX_LINE], linethree[MAX_BUFFER];
	int		index=0;
	
	DestroyObject();	// ���� ��ü ����
	
	fp = fopen(fileName, "rb");
	if( fp == NULL )
	{
		MessageBox(NULL, "Cannot open File", "ASEViewer", MB_OK|MB_ICONERROR);
		return FALSE;	
	}
	
	ScanFile(fp);		// ȭ���� �̸� �ѹ� �д´�. pass 1�� �ǹ�
	
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
		// ��Ÿ �ٸ� HIGH LEVEL���� ����� �߰�
		
		fgets(buffer, MAX_BUFFER, fp);
		sscanf(buffer, "%s", lineHead);
	}

	CreateTree();	// ������Ʈ���� Ʈ���� �����Ѵ�.

	// �� �������� ���� ���� TM�� �����.
	Node *pNode = NULL;
	for(DWORD i=0; i<m_dwTotalGObject ; i++)
	{
		pNode = FindParent(i);

		if(pNode == NULL)	//�θ� ���� ��Ʈ���
		{
			m_pGObject[i].m_LocalTM = m_pGObject[i].m_TM;
			continue;
		}
		// LocalTM = �ڱ�TM * �θ�InverseTM		
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
	
	if( strcmp( ASEFILE, lineHead) )	// ASE �������� üũ
		return FALSE;
	
	while( strcmp( END_PARA, lineHead ) )	// SCENE�κ��� ��ŵ�Ѵ�.
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
	fseek(fp, 0, SEEK_SET);	// �ٽ� ó������ ������.
}

//-----------------------------------------------------------------------------
// Name: CASE::ReadMaterialList()
// Desc: ASEȭ���� ���͸��� ������ �д´�.
//-----------------------------------------------------------------------------

void CASE::ReadMaterialList(FILE *fp)
{
	char buffer[MAX_BUFFER], lineHead[MAX_LINE], lineSecond[MAX_LINE];
	int index;
	int iBlockCnt = 1;	// *MATERIAL_LIST { ���� �ϳ� ���Ʊ� ������	
	
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
			iBlockCnt++;	// MATERIAL index { �̼����̱� ������..
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
			// �ؽ��� �ε��� ���⼭
		}
		//��Ÿ �ٸ� ���͸��� ���� �߰������� ���⿡
	}
}
//-----------------------------------------------------------------------------
// Name: CASE::ReadGeomObject()
// Desc: ASEȭ�Ͽ��� GEOMOBJECT�� �д´�
//-----------------------------------------------------------------------------

void CASE::ReadGeomObject(FILE *fp, int index)
{	
	char		buffer[MAX_BUFFER], lineHead[MAX_LINE], lineSecond[MAX_LINE];	
	int			iBlockCnt = 1;		// Ż�� ������ ����
	int			a, b, c;			// Face�� ����
	DWORD		i;					// for������ ���� ����
	CGObject	*pObject;			// ���� ������Ʈ�� ������ 

	D3DVECTOR	*pTVertex = NULL;	// �ؽ�����ǥ�� ���� ����
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

		if( !strcmp( TM_ROW0, lineHead ) )		// y�� z��ü..
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
				fgets(buffer, MAX_BUFFER, fp);		// y�� z�� �ٲپ���.
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
				pObject->m_pFaceList[i].a = a;	// �̻��� �κ�, ���� ������ delete�Ҷ� ������
				pObject->m_pFaceList[i].b = b;	// WORD���� �׷�������...2����Ʈ�ϱ�..
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
				
				pTFace[i].a = a;	//���� FaceList�� ���� ���� ���� ����
				pTFace[i].b = b;
				pTFace[i].c = c;
			}
			// Texture ��ǥ(tu, tv) ����
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
			iBlockCnt--;		// ReadTMAnimation()���� �Ѱ��� �� �б� �����̴�.
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

	pObject->ComputeNormal();	//������Ʈ�� �븻���͸� ����Ѵ�.
	pObject->MultiplyInvese();

	delete[] pTVertex;
	pTVertex = NULL;
	delete[] pTFace;
	pTFace = NULL;
}	// end of function



//-----------------------------------------------------------------------------
// Name: CASE::ReadTMAnimation()
// Desc: ASEȭ�Ͽ��� pos���� rot���� �о���δ�.
//		 �̸� TM_ANIMATION���� POS�� ROT�� ������ �̸� �ѹ� �а� ���� ���� �����Ѵ�.
//-----------------------------------------------------------------------------

void CASE::ReadTMAnimation(FILE *fp, int index)
{
	char		buffer[MAX_BUFFER], lineHead[MAX_LINE], lineSecond[MAX_LINE];
	CGObject	*pObject;
	int			iBlockCnt = 1;	// TM_ANIMAITON ���� �Ѱ� �о����Ƿ�
	int			iFilePos, iPosCnt, iRotCnt;
	D3DXVECTOR3	v;
	float		w;

	iPosCnt = iRotCnt = 0;
	iFilePos = ftell(fp);

	pObject = &m_pGObject[index];

	while(iBlockCnt)		// pos�� rot�� ������ ����
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
	fseek(fp, iFilePos, SEEK_SET);	// ���� �ڸ��� fp�� �̵���Ų��.

	if(iPosCnt != 0)
		pObject->m_pAniPos = new Pos[iPosCnt];
	if(iRotCnt != 0)
		pObject->m_pAniQuater = new AniQuater[iRotCnt];

	iPosCnt = iRotCnt = 0;	// �������� �ε����� ����Ѵ�.

	while(iBlockCnt)		// ���� �о���̴� �κ�
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
		{							// y�� z�� ��ü�Ѵ�.
			sscanf(buffer, "%s%d%f%f%f", lineHead, &pObject->m_pAniPos[iPosCnt].index,
												   &pObject->m_pAniPos[iPosCnt].x,
												   &pObject->m_pAniPos[iPosCnt].z,
												   &pObject->m_pAniPos[iPosCnt].y);
			AniPosIndex = pObject->m_pAniPos[1].index; //Render �Ҷ� �ִϸ��̼� ����..
			iPosCnt++;
		}
		else if( !strcmp( ROT_SAMPLE, lineHead ) )
		{	
			if(index == 2)
			{
				index = 2;
			}
			
			// y�� z�� ��ü�Ѵ�.
			sscanf(buffer, "%s%d%f%f%f%f", lineHead, &pObject->m_pAniQuater[iRotCnt].index,
													 &v.x, &v.z, &v.y, &w);
			// ���ʹϾ� ����
			D3DXQuaternionRotationAxis( &pObject->m_pAniQuater[iRotCnt].q, &v, w);
//			m_pGObject[index].a[iRotCnt].x = v.x; //cik ������ �����Ű�� ����..
//			m_pGObject[index].a[iRotCnt].y = v.y;
//			m_pGObject[index].a[iRotCnt].z = v.z;
//			m_pGObject[index].b[iRotCnt] = w;
			iRotCnt++;
			AniPosIndex = pObject->m_pAniQuater[1].index; //Render �Ҷ� �ִϸ��̼� ����..
		}			
	}
}


//-----------------------------------------------------------------------------
// Name: CASE::Animate()
// Desc: ĳ���͸� index�� ���� �ִϸ��̼��� �ϵ����Ѵ�.
//		 ���߿� �и��� ���ľ� �� �Լ� (������ ���� �׽�Ʈ..)
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
// Desc: Ʈ���� ���󰡸� �� ������Ʈ�� AniTM�� �����Ѵ�.
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
	
	for( i=0 ; i<pObject->m_iPosCnt ; i++)	// ���� �ִϿ� �̿��� pos���� ã�´�.
	{
		if( pObject->m_pAniPos[i].index == indexRate)
		{
			position = pObject->m_pAniPos[i];
			break;
		}
	}

	for( i=0 ; i<pObject->m_iRotCnt ; i++)	// ���� �ִϿ� �̿��� rot���� ã�´�.
	{
		if( pObject->m_pAniQuater[i].index == indexRate )		
		{
			bIsRot = TRUE;
			q = pObject->m_pAniQuater[i].q;
			break;
		}
	}	

	if(position.index == -1)	// pos���� ���� ���
	{
		position.x = pObject->m_LocalTM._41;
		position.y = pObject->m_LocalTM._42;
		position.z = pObject->m_LocalTM._43;
	}

	if(!bIsRot)		//rot���� ���ٸ�		
		D3DXQuaternionRotationMatrix(&q, &pObject->m_LocalTM);	

	// �ڽ��� AniTM ���
	if( indexRate == 0 )	
	{		
		pObject->m_PreQuater = q;
		D3DXMatrixRotationQuaternion(&myAniTM, &pObject->m_PreQuater);
	}		
	else								// ���� ���ʹϾ� ����
	{
		// ������ �ݵ�� �ʿ��� �κ� 
		// ���߿� ���� ���鶧 ó�� aseȭ�� �ε��ϴ� �κп��� ���ļ� ���� �����ؾ���
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
	
	// �ڱ� AniTM * �θ��� AniTM
	pObject->m_AniTM = myAniTM * parentTM;

	if(pRoot->pChild != NULL)			// �ڽĿ��� �ڽ��� ����AniTM�� �Ѱ��ش�.
		ComputeAniTM(pRoot->pChild, pObject->m_AniTM, indexRate);
}

//-----------------------------------------------------------------------------
// Name: CASE::CreateTree()
// Desc: GObject�� Ʈ���� �����Ѵ�.
//       ���� ó�� ������ ���� �θ� ���� Ʈ������ �Ѵ�.
//       ��� ������Ʈ�� ������ �θ� ���� ���´ٰ� �����Ѵ�.
//       �� �θ� �ִ� �ڽ��̶�� �θ𺸴� ���� ASE���Ͽ� ������ �ʴ´ٴ� �����̴�.
//		 �θ� �ִ� ����� �ݵ�� �θ� Ʈ�� �ȿ� �ִٰ� �����ϰ� �����Ѵ�.
//       �̷� �������� Ʈ���� ����ٸ�..�켱 ��� ������Ʈ�� ���� ��带 �̸� ����� ����
//		 �ϸ� ������ ������ �θ� ���� ��尡 ���� ������ �������� ������?
//-----------------------------------------------------------------------------
void CASE::CreateTree()
{	
	Node		*pNode;

	for(DWORD i=0 ; i < m_dwTotalGObject ; i++)
	{
		if(m_pGObject[i].m_pNodeParent == NULL)	// �θ� ����.
		{
			if(m_pRoot == NULL)		// �θ� ���� ��尡 ó���ΰ�?
			{
				pNode = new Node;
				pNode->ID = i;
				m_pRoot = pNode;
			}
			else	// ������ �θ� ���� ��尡 �־����Ƿ� ��Ʈ�� ������ ����
			{
				pNode = m_pRoot;

				while(pNode->pSibling != NULL)
					pNode = pNode->pSibling;

				pNode->pSibling = new Node;
				pNode->pSibling->ID = i;
			}
		}
		else					// �θ� �ִ� ����̴�.
		{
			pNode = FindParent(i);

			if(pNode->pChild == NULL)	// ã�� �θ𿡰� �ڽ��� �Ѱ��� ���ٸ�..
			{
				pNode->pChild = new Node;
				pNode->pChild->ID = i;
			}
			else			//�θ𿡰Դ� �̹� �ٸ� �ڽĵ��� �����Ƿ� ������� �����Ų��.
			{
				pNode = pNode->pChild;	//ù��° ������ ���
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
// Desc: ���� �־��� ������Ʈ�� �ε����� ������Ʈ�� �θ� ã�´�.
//-----------------------------------------------------------------------------

Node* CASE::FindParent(int id)
{
	return SearchNode(m_pRoot, m_pGObject[id].m_pNodeParent);
}

//-----------------------------------------------------------------------------
// Name: CASE::SearchNode()
// Desc: �־��� ����̸��� �´� ��带 Ʈ������ ã�� �� �����͸� �����Ѵ�.
//		 ������ �Ǿ��ٰ�� ������ �ᱹ�� Binary Tree�̹Ƿ� �Ϲ� Binary Tree��
//		 Search ��� �״�� In Order�� ��ġ�Ѵ�.
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
		return pRoot;		//���� ��尡 ã�� ����̴�.
		
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
// Desc: ������ Ʈ���� �����Ѵ�.
//		 �ؿ������� �����ؾ� �޸𸮸��� �߻����ϹǷ�
//		 Post Order������ Ž���ϸ� �����Ѵ�.
//-----------------------------------------------------------------------------

void CASE::DestroyTree(Node *pRoot)
{
	if(pRoot == NULL)	//Ʈ���� �����Ǿ� ���� �ʴ�.
		return;	

	if( pRoot->pChild != NULL )
		DestroyTree(pRoot->pChild);
	if( pRoot->pSibling != NULL )
		DestroyTree(pRoot->pSibling);

	delete pRoot;	// �ڽ��� �ƹ��� �����Ƿ� �ڱ� �ڽ��� �����Ѵ�.
}

//***************************************************************
//*	 CGObject Class												*
//***************************************************************

//-----------------------------------------------------------------
// Name: CGObject::AnimateObject()
// Desc: ���� aniTM�� �ڱ� TM�� ������ ������Ʈ�� �����δ�.
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
// Desc: ������Ʈ�� ���� �� ���ؽ��� �븻���͸� ���Ѵ�.
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
	m_dwMaterialREF = -1;	// ���� ���͸����� ������ -1 �̴�.

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
// Desc: " "�� �����ϰ� ���ϴ� ���븸���� ������ش�.
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