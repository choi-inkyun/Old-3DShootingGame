#ifndef H3DCOLL_H
#define H3DCOLL_H

struct Coll_Info {
	D3DXVECTOR3               Poi[8];
	D3DXVECTOR3               Normal[6];
	float                     D[6];
	int			    		  info;

    D3DXVECTOR3               VEC[8];
}; 

typedef struct {
    D3DXVECTOR3               VEC[4];
} P_VEC;

// Collision Class
class C_H3DCOLL
{
public:
	C_H3DCOLL();
	HRESULT H3D_VectorMatrixMultiply( D3DXVECTOR3& vDest, D3DXVECTOR3& vSrc, const D3DXMATRIX& mat);
	
	int  H3D_Collision( Coll_Info Coll_1, Coll_Info Coll_2 );
	int  H3D_Collision( Coll_Info Coll, D3DXVECTOR3 Point );
	int  H3D_Collision( Coll_Info Coll_1, Coll_Info Coll_2, P_VEC &VEC );

	VOID H3D_Up_Data( Coll_Info &Coll, D3DXMATRIX  matWord );
	VOID H3D_Up_Data( Coll_Info &Coll, D3DXMATRIX  matWord, float radian );
	VOID H3D_Up_Data( Coll_Info &Coll, D3DXVECTOR3 Pos, float rad );

	HRESULT MAKE_BoundBox( float x, float y, float z, Coll_Info &Object);

	int	 H3D_Collision_2D( Coll_Info Col, int x, int y, int Bx, int By, LPDIRECT3DDEVICE8 pd3dDevice );
};


#endif