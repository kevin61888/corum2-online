#pragma once


#ifndef GLOBAL_FUNC_DLL
#define GLOBAL_FUNC_DLL extern "C" __declspec(dllimport) 
#endif


#include "../4DyuchiGRX_common/typedef.h"
#include "type.h"


#define IN
#define OUT


#ifndef _MAX_EXP
	GLOBAL_FUNC_DLL float	__stdcall Sin(float x);
	GLOBAL_FUNC_DLL float	__stdcall Cos(float x);
	GLOBAL_FUNC_DLL float	__stdcall ACos(float x);
	GLOBAL_FUNC_DLL float	__stdcall ASin(float x);
#endif


GLOBAL_FUNC_DLL BOOL	__stdcall ClipRenderObjectWithViewVolume(VIEW_VOLUME* pVolume,COLLISION_MESH_OBJECT_DESC* pColMeshObjDesc,DWORD dwFlag);
GLOBAL_FUNC_DLL DWORD	__stdcall ClipLightWithRS(LIGHT_DESC* pLightDescResult,COLLISION_MESH_OBJECT_DESC* pColMeshDesc,LIGHT_DESC* pLightDescIn,DWORD dwLightNum);
GLOBAL_FUNC_DLL BOOL	__stdcall CalcIntersectPointLineAndPlane(VECTOR3* pv3Result,PLANE* pPlane,VECTOR3* pv3From,VECTOR3* pv3To, float* pft);
GLOBAL_FUNC_DLL void	__stdcall SET_VECTOR3(VECTOR3* pv3,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_ADD_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_SUB_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_MUL_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_DIV_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1,VECTOR3* pv3Arg2);

GLOBAL_FUNC_DLL void	__stdcall VECTOR3_ADDEQU_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_SUBEQU_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_MULEQU_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_DIVEQU_VECTOR3(VECTOR3* pv3Result,VECTOR3* pv3Arg1);

GLOBAL_FUNC_DLL void	__stdcall VECTOR3_ADDEQU_FLOAT(VECTOR3* pv3Result,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_SUBEQU_FLOAT(VECTOR3* pv3Result,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_MULEQU_FLOAT(VECTOR3* pv3Result,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_DIVEQU_FLOAT(VECTOR3* pv3Result,float fVal);

GLOBAL_FUNC_DLL void	__stdcall VECTOR3_ADD_FLOAT(VECTOR3* pv3Result,VECTOR3* pv3Arg1,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_SUB_FLOAT(VECTOR3* pv3Result,VECTOR3* pv3Arg1,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_MUL_FLOAT(VECTOR3* pv3Result,VECTOR3* pv3Arg1,float fVal);
GLOBAL_FUNC_DLL void	__stdcall VECTOR3_DIV_FLOAT(VECTOR3* pv3Result,VECTOR3* pv3Arg1,float fVal);

GLOBAL_FUNC_DLL void	__stdcall FindNearestVertexOnLine2(VECTOR3* OUT pv3Result ,float* OUT pT, VECTOR3* IN pv3From, VECTOR3* IN pv3To, VECTOR3* IN pv3Point);
GLOBAL_FUNC_DLL float	__stdcall VECTOR3Length(VECTOR3* pv3);
GLOBAL_FUNC_DLL void	__stdcall MakeViewVolume(VIEW_VOLUME* pViewVolume,VECTOR3* pv3From,VECTOR3* pv3To,VECTOR3* pv3Up, float fov, float fHVRatio, float fFar);
GLOBAL_FUNC_DLL void	__stdcall MakeViewVolumeWithOrtho(VIEW_VOLUME* pViewVolume,VECTOR3* pv3From,VECTOR3* pv3To,VECTOR3* pv3Up, float width, float fHVRatio, float fFar);

GLOBAL_FUNC_DLL void	__stdcall CrossProduct(VECTOR3* r,VECTOR3* u,VECTOR3* v);
GLOBAL_FUNC_DLL float	__stdcall DotProduct(VECTOR3* pv3_0,VECTOR3* pv3_1);
GLOBAL_FUNC_DLL void	__stdcall Normalize(VECTOR3* OUT vn,VECTOR3* IN v);
GLOBAL_FUNC_DLL float	__stdcall CalcAngle(VECTOR3* vec1, VECTOR3* vec2);
GLOBAL_FUNC_DLL float	__stdcall CalcCosAngle(VECTOR3* vec1, VECTOR3* vec2);
GLOBAL_FUNC_DLL float	__stdcall CalcDistance(VECTOR3* pv3Start,VECTOR3* pv3End);
GLOBAL_FUNC_DLL BOOL	__stdcall IsCollisionSpherAndSphere(VECTOR3* pv3PointTarget,float fRsTarget,VECTOR3* pv3PointSrc,float fRsSrc);
GLOBAL_FUNC_DLL BOOL	__stdcall SetIdentityMatrix(MATRIX4* pMat);
GLOBAL_FUNC_DLL BOOL	__stdcall SetInverseMatrix(MATRIX4* q, MATRIX4* a );

GLOBAL_FUNC_DLL void	__stdcall TransformVector3_VPTR1(VECTOR3* pv3SrcDest, MATRIX4* pMat,DWORD dwNum);
GLOBAL_FUNC_DLL void	__stdcall TransformVector3_VPTR2(VECTOR3* pv3Dest,VECTOR3* pv3Src, MATRIX4* pMat,DWORD dwNum);
GLOBAL_FUNC_DLL void	__stdcall TransformVector4_VPTR2(VECTOR4* pv3Dest,VECTOR4* pv3Src, MATRIX4* pMat,DWORD dwNum);
GLOBAL_FUNC_DLL void	__stdcall TransformV3TOV4(VECTOR4* pv3Dest,VECTOR3* pv3Src, MATRIX4* pMat,DWORD dwNum);
GLOBAL_FUNC_DLL void	__stdcall TransformNSizeVector3_VPTR1(char* pv3SrcDest,DWORD dwSize,MATRIX4* pMat,DWORD dwNum);

GLOBAL_FUNC_DLL void	__stdcall SetRotationMatrix(MATRIX4* mat, VECTOR3* vDir, FLOAT fRads );
GLOBAL_FUNC_DLL void	__stdcall SetRotationXMatrix(MATRIX4* pMat, float fRad);
GLOBAL_FUNC_DLL void	__stdcall SetRotationYMatrix(MATRIX4* pMat, float fRad);
GLOBAL_FUNC_DLL void	__stdcall SetRotationZMatrix(MATRIX4* pMat, float fRad);
GLOBAL_FUNC_DLL void	__stdcall MatrixMultiply3(MATRIX4* pResult,MATRIX4* mat1, MATRIX4* mat2, MATRIX4* mat3);
GLOBAL_FUNC_DLL void	__stdcall MatrixMultiply2(MATRIX4* pResult, MATRIX4* mat1, MATRIX4* mat2);
GLOBAL_FUNC_DLL void	__stdcall TranslateMatrix(MATRIX4* m, VECTOR3* vec);
GLOBAL_FUNC_DLL void	__stdcall SetScaleMatrix(MATRIX4* m, VECTOR3* v );
GLOBAL_FUNC_DLL void	__stdcall TransposeMatrix(MATRIX4* pMat);
GLOBAL_FUNC_DLL void	__stdcall SetLightTexMatrix(MATRIX4* pMat,VECTOR3* pv3Pos,float fRs);
GLOBAL_FUNC_DLL VOID	__stdcall MatrixFromQuaternion(MATRIX4* mat, QUARTERNION* q);
GLOBAL_FUNC_DLL VOID	__stdcall QuaternionSlerp(QUARTERNION* qQ,QUARTERNION* aQ,QUARTERNION* bQ,float fAlpha);
GLOBAL_FUNC_DLL BOOL	__stdcall CalcPlaneEquation(PLANE* pPlane,VECTOR3* pv3Tri);
GLOBAL_FUNC_DLL BOOL	__stdcall ComparePlane(PLANE* pTarget,PLANE* pSrc);
GLOBAL_FUNC_DLL BOOL	__stdcall CalcBoundingMesh(VECTOR3* pv3,DWORD dwVertexNum,COLLISION_MESH_OBJECT_DESC* pDesc);
GLOBAL_FUNC_DLL void	__stdcall CreateVertexListWithBox(char* pv3Array36,DWORD dwVertexSize,VECTOR3* pv3Oct);
GLOBAL_FUNC_DLL BOOL	__stdcall IsCollisionSphereAndPlane(VECTOR3* pv3Point,float fRs,PLANE* pPlane);
GLOBAL_FUNC_DLL void	__stdcall CalcXZ(float* px,float* pz,DWORD dwPosX,DWORD dwPosY,float top,float left,float bottom,float right,float width,float height,DWORD dwFacesNumX);

GLOBAL_FUNC_DLL void	__stdcall Set2PowValueLess(DWORD* pdwOut,DWORD dwIn);
GLOBAL_FUNC_DLL BOOL	__stdcall TransformBoundingSphere(BOUNDING_SPHERE* pDest,BOUNDING_SPHERE* pSrc,DWORD dwFlag);
GLOBAL_FUNC_DLL DWORD	__stdcall COLORtoDWORD(float r,float g,float b,float a);
GLOBAL_FUNC_DLL void	__stdcall SetViewAlignMatrix(MATRIX4* pMat,VECTOR3* pv3Dir,VECTOR3* pv3Eye,VECTOR3* pv3At);
GLOBAL_FUNC_DLL void	__stdcall QuaternionFromRotation(QUARTERNION* pQ,VECTOR3* v, float fTheta );
GLOBAL_FUNC_DLL void	__stdcall QuaternionMultiply(QUARTERNION* pQResult,QUARTERNION* pQA,QUARTERNION* pQB);

GLOBAL_FUNC_DLL BOOL	__stdcall AddBoundingMesh(COLLISION_MESH_OBJECT_DESC* pDesc,COLLISION_MESH_OBJECT_DESC** ppDesc,DWORD dwNum);
GLOBAL_FUNC_DLL void	__stdcall DWORDtoCOLOR(DWORD color,float* r,float* g,float* b,float* a);
GLOBAL_FUNC_DLL	void	__stdcall PhysiqueTransform(BYTE* pVertexResult,PHYSIQUE_VERTEX* pPhyVertex,DWORD dwVertexNum,DWORD dwSize,MATRIX4* pMatrixEntry);

//GLOBAL_FUNC_DLL void	__stdcall TransformPhysiqueVertex(PHYSIQUE_VERTEX* pPhy, MATRIX4* mat,DWORD num);

GLOBAL_FUNC_DLL BOOL	__stdcall IntersectTriangle(VECTOR3* orig,VECTOR3* dir,VECTOR3* v0,VECTOR3* v1, VECTOR3* v2,float* t, float* u, float* v );
GLOBAL_FUNC_DLL void	__stdcall CalcRay(VECTOR3* pv3Pos,VECTOR3* pv3Dir,DWORD dwPosX,DWORD dwPosY,DWORD dwWidth,DWORD dwHeight,MATRIX4* pProj,MATRIX4* pView);
GLOBAL_FUNC_DLL BOOL	__stdcall CalcIntersectPointRayAndTri(VECTOR3* pv3IntersectPoint,VECTOR3* pv3Orig,VECTOR3* pv3Dir,VECTOR3* pv3Tri);
GLOBAL_FUNC_DLL BOOL	__stdcall IsCollisionMeshAndRay(VECTOR3* pv3IntersectTri,float* pfDist,COLLISION_MESH_OBJECT_DESC* pColMeshDesc,VECTOR3* pv3Pos,VECTOR3* pv3Dir);
GLOBAL_FUNC_DLL void	__stdcall SetTransformBoundingMesh(COLLISION_MESH_OBJECT_DESC* pDest,COLLISION_MESH_OBJECT_DESC* pSrc,MATRIX4* pMat);
GLOBAL_FUNC_DLL void	__stdcall ResetTM(NODE_TM* pTM);
GLOBAL_FUNC_DLL BOOL	__stdcall SetViewMatrix(MATRIX4* pMat,VECTOR3* pv3From,VECTOR3* pv3To,VECTOR3* pv3Up);
GLOBAL_FUNC_DLL BOOL	__stdcall SetProjectionMatrix(MATRIX4* pMat,float fFOV,float fAspect,float fNearPlane,float fFarPlane);
GLOBAL_FUNC_DLL void	__stdcall SetBitampFromTexturePlane(char* pBits,DWORD dwPitch,TEXTURE_PLANE* pTexPlane,DWORD dwTexPlaneNum);
GLOBAL_FUNC_DLL void	__stdcall SetBitampShadeLightTexture(char* pBits,DWORD dwPitch,TEXTURE_PLANE* pTexPlane,DWORD dwTexPlaneNum,LIGHT_DESC* pLight,DWORD dwLightNum);
GLOBAL_FUNC_DLL BOOL	__stdcall WriteTGA(char* szFileName,char* pSrc,DWORD dwWidth,DWORD dwHeight,DWORD bpp);

GLOBAL_FUNC_DLL BOOL	__stdcall ResizeImage(char* pDest,DWORD dwDestWidth,DWORD dwDestHeight,char* pSrc,DWORD dwSrcWidth,DWORD dwSrcHeight,DWORD dwBytesPerPixel);

// ItemQueue


GLOBAL_FUNC_DLL ITEMQ_HANDLE	__stdcall	CreateItemQueue();
GLOBAL_FUNC_DLL void			__stdcall	ReleaseItemQueue(ITEMQ_HANDLE hQ);

GLOBAL_FUNC_DLL BOOL			__stdcall	BeginInitialize(ITEMQ_HANDLE pQ,DWORD dwMaxTypeNum);
GLOBAL_FUNC_DLL BOOL			__stdcall	SetType(ITEMQ_HANDLE pQ,DWORD dwTypeID,DWORD dwItemSize,void* pFunc,DWORD dwMaxNum);
GLOBAL_FUNC_DLL void			__stdcall	EndInitialize(ITEMQ_HANDLE pQ);

GLOBAL_FUNC_DLL BOOL			__stdcall	PushItem(ITEMQ_HANDLE pQ,DWORD dwTypeID,char* pItem);
GLOBAL_FUNC_DLL char*			__stdcall	GetItemSeq(ITEMQ_HANDLE pQ,DWORD* pdwTypeID,void** ppFunc);
GLOBAL_FUNC_DLL char*			__stdcall	GetItem(ITEMQ_HANDLE pQ,void** ppFunc,DWORD dwTypeID,DWORD dwSeqIndex);

GLOBAL_FUNC_DLL void			__stdcall	Clear(ITEMQ_HANDLE pQ);


GLOBAL_FUNC_DLL DWORD			__stdcall 	GetItemNum(ITEMQ_HANDLE pQ);
GLOBAL_FUNC_DLL DWORD			__stdcall	GetItemNumWithType(ITEMQ_HANDLE pQ,DWORD dwTypeID);
GLOBAL_FUNC_DLL void			__stdcall	ResetReadPosition(ITEMQ_HANDLE pQ);
	


// LookAsideList

GLOBAL_FUNC_DLL STMPOOL_HANDLE	__stdcall	CreateStaticMemoryPool();
GLOBAL_FUNC_DLL void			__stdcall	ReleaseStaticMemoryPool(STMPOOL_HANDLE pool);
GLOBAL_FUNC_DLL BOOL			__stdcall	InitializeStaticMemoryPool(STMPOOL_HANDLE pool,DWORD dwUnitSize,DWORD dwDefaultCommitNum,DWORD dwMaxNum);

GLOBAL_FUNC_DLL void*			__stdcall	LALAlloc(STMPOOL_HANDLE pool);
GLOBAL_FUNC_DLL void			__stdcall	LALFree(STMPOOL_HANDLE pool,void* pMemory);


// Various Bytes HashTable
GLOBAL_FUNC_DLL	DWORD			__stdcall	VBHSelect(VBHASH_HANDLE pHash,DWORD OUT* pItems,DWORD dwMaxItemNum,void* pKeyData,DWORD dwSize);
GLOBAL_FUNC_DLL	void*			__stdcall	VBHInsert(VBHASH_HANDLE pHash,DWORD dwItem,void* pKeyData,DWORD dwSize);
GLOBAL_FUNC_DLL	BOOL			__stdcall	VBHDelete(VBHASH_HANDLE pHash,void* pBucket);

GLOBAL_FUNC_DLL	VBHASH_HANDLE	__stdcall	VBHCreate();
GLOBAL_FUNC_DLL	BOOL			__stdcall	VBHInitialize(VBHASH_HANDLE pHash,DWORD dwMaxBucketNum,DWORD dwMaxSize,DWORD dwMaxItemNum);
GLOBAL_FUNC_DLL	void			__stdcall	VBHRelease(VBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL	DWORD			__stdcall	VBHGetMaxBucketNum(VBHASH_HANDLE pHash);
GLOBAL_FUNC_DLL void			__stdcall	VBHDeleteAll(VBHASH_HANDLE pHash);


// IndexCreattor
GLOBAL_FUNC_DLL		INDEXCR_HANDLE		__stdcall	ICCreate();
GLOBAL_FUNC_DLL		void				__stdcall	ICRelease(INDEXCR_HANDLE pIC);
GLOBAL_FUNC_DLL		DWORD				__stdcall	ICAllocIndex(INDEXCR_HANDLE pIC);
GLOBAL_FUNC_DLL		void				__stdcall  	ICFreeIndex(INDEXCR_HANDLE pIC,DWORD dwIndex);
GLOBAL_FUNC_DLL		BOOL				__stdcall 	ICInitialize(INDEXCR_HANDLE pIC,DWORD dwMaxNum);
GLOBAL_FUNC_DLL		DWORD				__stdcall	ICGetIndexNum(INDEXCR_HANDLE pIC);

// ItemIndexTable

GLOBAL_FUNC_DLL		ITEMTABLE_HANDLE	__stdcall	ITCreate();
GLOBAL_FUNC_DLL		void				__stdcall	ITRelease(ITEMTABLE_HANDLE pIT);

GLOBAL_FUNC_DLL		BOOL				__stdcall	ITInitialize(ITEMTABLE_HANDLE pIT,DWORD dwMaxItemNum);
GLOBAL_FUNC_DLL		DWORD				__stdcall	ITAddItem(ITEMTABLE_HANDLE pIT,void* pVoid);
GLOBAL_FUNC_DLL		BOOL				__stdcall	ITDeleteItem(ITEMTABLE_HANDLE pIT,DWORD dwItemIndex);
GLOBAL_FUNC_DLL		DWORD				__stdcall	ITGetItemNum(ITEMTABLE_HANDLE pIT);
GLOBAL_FUNC_DLL		void*				__stdcall	ITGetItem(ITEMTABLE_HANDLE pIT,DWORD dwItemIndex);
GLOBAL_FUNC_DLL		void*				__stdcall	ITGetItemSequential(ITEMTABLE_HANDLE pIT,DWORD dwSeqIndex);

GLOBAL_FUNC_DLL		DWORD				__stdcall	RemoveExt(char* szResultName,char* szFileName);
GLOBAL_FUNC_DLL		DWORD				__stdcall	GetNameRemovePath(char* dest,char* src);

//#endif

