#pragma once

#include "Mesh.h"
#include "Camera.h"

class CGameObject
{
public:
	CGameObject() { }
	virtual ~CGameObject();

public:
	TCHAR						m_pstrFrameName[256];

	bool						m_bActive = true;

	CMesh*						m_pMesh = NULL;
	XMFLOAT4X4					m_xmf4x4Transform = Matrix4x4::Identity();
	XMFLOAT4X4					m_xmf4x4World = Matrix4x4::Identity();

	BoundingOrientedBox			m_xmOOBB = BoundingOrientedBox();

	CGameObject*				m_pObjectCollided = NULL;
	DWORD						m_dwDefaultColor = RGB(255, 0, 0);
	DWORD						m_dwColor = RGB(255, 0, 0);

	XMFLOAT3					m_xmf3MovingDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	float						m_fMovingSpeed = 0.0f;
	float						m_fMovingRange = 0.0f;

	CGameObject*				m_pParent = NULL;
	CGameObject*				m_pChild = NULL;
	CGameObject*				m_pSibling = NULL;
	string	m_strTag;

public:
	void SetActive(bool bActive) { m_bActive = bActive; }
	void SetMesh(CMesh *pMesh) { m_pMesh = pMesh; if (pMesh) pMesh->AddRef(); }

	void SetDefaultColor(DWORD dwColor) { m_dwColor = m_dwDefaultColor = dwColor; }
	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }

	void SetRotationTransform(XMFLOAT4X4 *pmxf4x4Transform);

	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3& xmf3Position);

	void SetMovingDirection(XMFLOAT3& xmf3MovingDirection) { m_xmf3MovingDirection = Vector3::Normalize(xmf3MovingDirection); }
	void SetMovingSpeed(float fSpeed) { m_fMovingSpeed = fSpeed; }
	void SetMovingRange(float fRange) { m_fMovingRange = fRange; }

	void SetChild(CGameObject* pChild);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();
	CGameObject* GetParent() { return(m_pParent); }
	string GetTag() { return m_strTag; }

public:

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);
	void Move(XMFLOAT3& vDirection, float fSpeed);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(XMFLOAT3& xmf3Axis, float fAngle);

	void LookTo(XMFLOAT3& xmf3LookTo, XMFLOAT3& xmf3Up);
	void LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);

	void UpdateBoundingBox();

	void Render(HDC hDCFrameBuffer, XMFLOAT4X4* pxmf4x4World, CMesh* pMesh,_float3* pCullingPosition = nullptr);

	virtual void OnCollision(CGameObject* pOtherObject) {};
	virtual void OnUpdateTransform() { }
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera, bool bSibling = false);

	void GenerateRayForPicking(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection);
	int PickObjectByRayIntersection(XMVECTOR& xmPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance);

	void ComputeWorldTransform(XMFLOAT4X4* pxmf4x4Parent = NULL);
	CGameObject* FindFrame(_TCHAR* pstrFrameName);
};

static float RandF(float fMin, float fMax);
static XMVECTOR RandomUnitVectorOnSphere();
static _vector RandomUnitVectorOnSphere(XMFLOAT2 vX, XMFLOAT2 vY, XMFLOAT2 vZ);

static float RandF(float fMin, float fMax)
{
	return(fMin + ((float)rand() / (float)RAND_MAX) * (fMax - fMin));
}

static XMVECTOR RandomUnitVectorOnSphere()
{
	XMVECTOR xmvOne = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR xmvZero = XMVectorZero();

	while (true)
	{
		XMVECTOR v = XMVectorSet(RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), 0.0f);
		if (!XMVector3Greater(XMVector3LengthSq(v), xmvOne))
			return(XMVector3Normalize(v));
	}
}

static XMVECTOR RandomUnitVectorOnSphere(XMFLOAT2 vX, XMFLOAT2 vY, XMFLOAT2 vZ)
{
	XMVECTOR xmvOne = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR xmvZero = XMVectorZero();

	while (true)
	{
		XMVECTOR v = XMVectorSet(RandF(vX.x, vX.y), RandF(vY.x, vY.y), RandF(vZ.x, vZ.y), 0.0f);
		//if (!XMVector3Greater(XMVector3LengthSq(v), xmvOne))
		return(XMVector3Normalize(v));
	}
}