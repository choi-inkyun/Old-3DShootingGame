
//#define STRICT
#define D3D_OVERLOADS
#include <stdio.h>
#include <math.h>
#include <D3D8.h>
#include <d3dx8.h>

#include "h3dColl.h"

extern C_H3DCOLL cndehf;

int Index[36];

C_H3DCOLL::C_H3DCOLL()
{
	Index[0]  = 0,  Index[1] =  2,  Index[2]  = 3;
	Index[3]  = 3,  Index[4] =  1,  Index[5]  = 0;
	Index[6]  = 4,  Index[7] =  5,  Index[8]  = 7;
	Index[9]  = 7,  Index[10] = 6,  Index[11]  = 4;
	Index[12]  = 0, Index[13] =  1, Index[14]  = 5;
	Index[15]  = 5, Index[16] =  4, Index[17]  = 0;
	Index[18]  = 1, Index[19] =  3, Index[20]  = 7;
	Index[21]  = 7, Index[22] =  5, Index[23]  = 1;
	Index[24]  = 3, Index[25] =  2, Index[26]  = 6;
	Index[27]  = 6, Index[28] =  7, Index[29]  = 3;
	Index[30]  = 2, Index[31] =  0, Index[32]  = 4;
	Index[33]  = 4, Index[34] =  6, Index[35]  = 2;

}

int C_H3DCOLL::H3D_Collision( Coll_Info Coll_1, Coll_Info Coll_2, P_VEC &VEC )
{
	float Sign;
	int Count;

	for ( int j = 0; j < 8; j++ ) {
		Count = 0;
		for ( int i = 0; i < 6; i++ ) {
			Sign = Coll_1.Normal[i].x * Coll_2.Poi[j].x + 
				   Coll_1.Normal[i].y * Coll_2.Poi[j].y + 
				   Coll_1.Normal[i].z * Coll_2.Poi[j].z + Coll_1.D[i];
			if ( Sign <= 0.0f ) Count++;
		}
		if ( Count == 6 ) {
			VEC.VEC[0] = Coll_2.Poi[0];
			VEC.VEC[1] = Coll_2.Poi[1];
			VEC.VEC[2] = Coll_2.Poi[2];
			VEC.VEC[3] = Coll_2.Poi[3];

			return 1;
		}
	}

	for ( j = 0; j < 8; j++ ) {
		Count = 0;
		for ( int i = 0; i < 6; i++ ) {
			Sign = Coll_2.Normal[i].x * Coll_1.Poi[j].x + 
				   Coll_2.Normal[i].y * Coll_1.Poi[j].y + 
				   Coll_2.Normal[i].z * Coll_1.Poi[j].z + Coll_2.D[i];
			if ( Sign <= 0.0f ) Count++;
		}
		if ( Count == 6 ) {
			VEC.VEC[0] = Coll_2.Poi[0];
			VEC.VEC[1] = Coll_2.Poi[1];
			VEC.VEC[2] = Coll_2.Poi[2];
			VEC.VEC[3] = Coll_2.Poi[3];

			return 1;
		}
	}

	VEC.VEC[0] = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	VEC.VEC[1] = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	VEC.VEC[2] = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	VEC.VEC[3] = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );


	return 0;
}

int C_H3DCOLL::H3D_Collision( Coll_Info Coll_1, Coll_Info Coll_2 )
{
	float Sign;
	int Count;
 
	for ( int j = 0; j < 8; j++ ) {
		Count = 0;
		for ( int i = 0; i < 6; i++ ) {
			Sign = Coll_1.Normal[i].x * Coll_2.Poi[j].x + 
				   Coll_1.Normal[i].y * Coll_2.Poi[j].y + 
				   Coll_1.Normal[i].z * Coll_2.Poi[j].z + Coll_1.D[i];
			if ( Sign <= 0.0f ) Count++;
		}
		if ( Count == 6 ) return 1;
	}

	for ( j = 0; j < 8; j++ ) {
		Count = 0;
		for ( int i = 0; i < 6; i++ ) {
			Sign = Coll_2.Normal[i].x * Coll_1.Poi[j].x + 
				   Coll_2.Normal[i].y * Coll_1.Poi[j].y + 
				   Coll_2.Normal[i].z * Coll_1.Poi[j].z + Coll_2.D[i];
			if ( Sign <= 0.0f ) Count++;
		}
		if ( Count == 6 ) return 1;
	}

	return 0;
}


int C_H3DCOLL::H3D_Collision(Coll_Info Coll, D3DXVECTOR3 Point )
{
	float Sign;
	int Count = 0;

	for ( int i = 0; i < 6; i++ ) {
		Sign = Coll.Normal[i].x * Point.x + 
			   Coll.Normal[i].y * Point.y + 
			   Coll.Normal[i].z * Point.z + Coll.D[i];
		if ( Sign <= 0.0f ) Count++;
	}
	if ( Count == 6 ) return 1;

	return 0;
}

VOID C_H3DCOLL::H3D_Up_Data( Coll_Info &Coll, D3DXMATRIX  matWord )
{
	D3DXVECTOR3 Temp, U, V;

	// VECTOR
	for ( int i = 0; i < 8; i++ ) {
		Temp.x = Coll.VEC[i].x; 
		Temp.y = Coll.VEC[i].y; 
		Temp.z = Coll.VEC[i].z; 
		H3D_VectorMatrixMultiply( Coll.Poi[i], Temp, matWord );
	}

	// NORMAL & D
	for ( i = 0; i < 6; i++ ) {
		D3DXVec3Normalize( &U, &(Coll.Poi[ Index[(i*2)*3] ] - Coll.Poi[ Index[(i*2)*3+1] ]) );
		D3DXVec3Normalize( &V, &(Coll.Poi[ Index[(i*2)*3] ] - Coll.Poi[ Index[(i*2)*3+2] ]) );

		D3DXVec3Cross( &Coll.Normal[i], &U, &V );
		Coll.D[i] = -( Coll.Normal[i].x * Coll.Poi[ Index[(i*2)*3] ].x +
		               Coll.Normal[i].y * Coll.Poi[ Index[(i*2)*3] ].y +
			    	   Coll.Normal[i].z * Coll.Poi[ Index[(i*2)*3] ].z );
	}
}

VOID C_H3DCOLL::H3D_Up_Data( Coll_Info &Coll, D3DXMATRIX  matWord, float radian )
{
	D3DXVECTOR3 Temp, U, V;
	D3DXMATRIX  matRadian;

	D3DXMatrixIdentity( &matRadian );
	D3DXMatrixRotationY( &matRadian, radian );
	D3DXMatrixMultiply( &matWord, &matRadian, &matWord );


	// VECTOR
	for ( int i = 0; i < 8; i++ ) {
		Temp.x = Coll.VEC[i].x; 
		Temp.y = Coll.VEC[i].y; 
		Temp.z = Coll.VEC[i].z; 
		H3D_VectorMatrixMultiply( Coll.Poi[i], Temp, matWord );
	}

	// NORMAL & D
	for ( i = 0; i < 6; i++ ) {
		D3DXVec3Normalize( &U, &(Coll.Poi[ Index[(i*2)*3] ] - Coll.Poi[ Index[(i*2)*3+1] ]) );
		D3DXVec3Normalize( &V, &(Coll.Poi[ Index[(i*2)*3] ] - Coll.Poi[ Index[(i*2)*3+2] ]) );

		D3DXVec3Cross( &Coll.Normal[i], &U, &V );
		Coll.D[i] = -( Coll.Normal[i].x * Coll.Poi[ Index[(i*2)*3] ].x +
		               Coll.Normal[i].y * Coll.Poi[ Index[(i*2)*3] ].y +
			    	   Coll.Normal[i].z * Coll.Poi[ Index[(i*2)*3] ].z );
	}
}


HRESULT C_H3DCOLL::H3D_VectorMatrixMultiply( D3DXVECTOR3& vDest, D3DXVECTOR3& vSrc, const D3DXMATRIX& mat)
{
		FLOAT x = vSrc.x*mat._11 + vSrc.y*mat._21 + vSrc.z* mat._31 + mat._41;
		FLOAT y = vSrc.x*mat._12 + vSrc.y*mat._22 + vSrc.z* mat._32 + mat._42;
		FLOAT z = vSrc.x*mat._13 + vSrc.y*mat._23 + vSrc.z* mat._33 + mat._43;
		FLOAT w = vSrc.x*mat._14 + vSrc.y*mat._24 + vSrc.z* mat._34 + mat._44;

//		if( fabs( w ) < g_EPSILON )
//		return E_INVALIDARG;

		vDest.x = x/w;
		vDest.y = y/w;
		vDest.z = z/w;

		return S_OK;
}

VOID C_H3DCOLL::H3D_Up_Data( Coll_Info &Coll, D3DXVECTOR3 Pos, float rad )
{
	D3DXVECTOR3 Temp, U, V;
	D3DXMATRIX  matWord, matRadian;

	D3DXMatrixIdentity( &matWord );
	D3DXMatrixIdentity( &matRadian );

	matWord._41 = Pos.x;
	matWord._42 = Pos.y;
	matWord._43 = Pos.z;

	D3DXMatrixRotationY( &matRadian, rad );
	D3DXMatrixMultiply( &matWord, &matRadian, &matWord );
	// VECTOR
	for ( int i = 0; i < 8; i++ ) {
		Temp.x = Coll.VEC[i].x; 
		Temp.y = Coll.VEC[i].y; 
		Temp.z = Coll.VEC[i].z; 
		H3D_VectorMatrixMultiply( Coll.Poi[i], Temp, matWord );
	}

	// NORMAL & D
	for ( i = 0; i < 6; i++ ) {
		D3DXVec3Normalize( &U, &(Coll.Poi[ Index[(i*2)*3] ] - Coll.Poi[ Index[(i*2)*3+1] ]) );
		D3DXVec3Normalize( &V, &(Coll.Poi[ Index[(i*2)*3] ] - Coll.Poi[ Index[(i*2)*3+2] ]) );

		D3DXVec3Cross( &Coll.Normal[i], &U, &V );
		Coll.D[i] = -( Coll.Normal[i].x * Coll.Poi[ Index[(i*2)*3] ].x +
		               Coll.Normal[i].y * Coll.Poi[ Index[(i*2)*3] ].y +
			    	   Coll.Normal[i].z * Coll.Poi[ Index[(i*2)*3] ].z );
	}

}

////////////////////////¹Ú½º¸¦ ¸¸µë////////////////
HRESULT C_H3DCOLL::MAKE_BoundBox( float x, float y, float z, Coll_Info &Object )
{
// VECTOR
	Object.VEC[0] = D3DXVECTOR3( x*(-1.0f), 0.0f    , z );
	Object.VEC[1] = D3DXVECTOR3( x        , 0.0f    , z );
	Object.VEC[2] = D3DXVECTOR3( x*(-1.0f), 0.0f    , z*(-1.0f) );
	Object.VEC[3] = D3DXVECTOR3( x        , 0.0f    , z*(-1.0f) );

	Object.VEC[4] = D3DXVECTOR3( x*(-1.0f), y*(2.0f), z );
	Object.VEC[5] = D3DXVECTOR3( x        , y*(2.0f), z );
	Object.VEC[6] = D3DXVECTOR3( x*(-1.0f), y*(2.0f), z*(-1.0f) );
	Object.VEC[7] = D3DXVECTOR3( x        , y*(2.0f), z*(-1.0f) );

	return S_OK;
}
