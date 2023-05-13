#include "stdafx.h"
#include "GameObject.h"
#include "GraphicsPipeline.h"

CGameObject::~CGameObject(void)
{
	if (m_pMesh) 
		m_pMesh->Release();

	if (m_pSibling) 
		delete m_pSibling;
	if (m_pChild) 
		delete m_pChild;
}

void CGameObject::SetChild(CGameObject* pChild)
{
	if (m_pChild)
	{
		if (pChild) 
			pChild->m_pSibling = m_pChild->m_pSibling;
		m_pChild->m_pSibling = pChild;
	}
	else
	{
		m_pChild = pChild;
	}

	if (pChild) {
		if(!pChild->GetParent())
			pChild->m_pParent = this;
	}
}

void CGameObject::SetPosition(float x, float y, float z)
{
	m_xmf4x4Transform._41 = x;
	m_xmf4x4Transform._42 = y;
	m_xmf4x4Transform._43 = z;

	ComputeWorldTransform(NULL);
}

void CGameObject::SetPosition(XMFLOAT3& xmf3Position)
{
	SetPosition(xmf3Position.x, xmf3Position.y, xmf3Position.z);
}

XMFLOAT3 CGameObject::GetPosition()
{
	return(XMFLOAT3(m_xmf4x4World._41, m_xmf4x4World._42, m_xmf4x4World._43));
}

void CGameObject::SetRotationTransform(XMFLOAT4X4* pmxf4x4Transform)
{
	m_xmf4x4Transform._11 = pmxf4x4Transform->_11; m_xmf4x4Transform._12 = pmxf4x4Transform->_12; m_xmf4x4Transform._13 = pmxf4x4Transform->_13;
	m_xmf4x4Transform._21 = pmxf4x4Transform->_21; m_xmf4x4Transform._22 = pmxf4x4Transform->_22; m_xmf4x4Transform._23 = pmxf4x4Transform->_23;
	m_xmf4x4Transform._31 = pmxf4x4Transform->_31; m_xmf4x4Transform._32 = pmxf4x4Transform->_32; m_xmf4x4Transform._33 = pmxf4x4Transform->_33;

	ComputeWorldTransform(NULL);
}

XMFLOAT3 CGameObject::GetLook()
{
	XMFLOAT3 xmf3LookAt(m_xmf4x4World._31, m_xmf4x4World._32, m_xmf4x4World._33);
	xmf3LookAt = Vector3::Normalize(xmf3LookAt);
	return(xmf3LookAt);
}

XMFLOAT3 CGameObject::GetUp()
{
	XMFLOAT3 xmf3Up(m_xmf4x4World._21, m_xmf4x4World._22, m_xmf4x4World._23);
	xmf3Up = Vector3::Normalize(xmf3Up);
	return(xmf3Up);
}

XMFLOAT3 CGameObject::GetRight()
{
	XMFLOAT3 xmf3Right(m_xmf4x4World._11, m_xmf4x4World._12, m_xmf4x4World._13);
	xmf3Right = Vector3::Normalize(xmf3Right);
	return(xmf3Right);
}

void CGameObject::MoveStrafe(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Right = GetRight();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Right, fDistance));

	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveUp(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3Up = GetUp();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Up, fDistance));

	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::MoveForward(float fDistance)
{
	XMFLOAT3 xmf3Position = GetPosition();
	XMFLOAT3 xmf3LookAt = GetLook();
	xmf3Position = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3LookAt, fDistance));

	CGameObject::SetPosition(xmf3Position);
}

void CGameObject::Rotate(float fPitch, float fYaw, float fRoll)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationYawPitchRoll(fPitch, fYaw, fRoll);
	m_xmf4x4Transform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4Transform);

	ComputeWorldTransform(NULL);
}

void CGameObject::Rotate(XMFLOAT3& xmf3RotationAxis, float fAngle)
{
	XMFLOAT4X4 mtxRotate = Matrix4x4::RotationAxis(xmf3RotationAxis, fAngle);
	m_xmf4x4Transform = Matrix4x4::Multiply(mtxRotate, m_xmf4x4Transform);

	ComputeWorldTransform(NULL);
}

void CGameObject::Move(XMFLOAT3& vDirection, float fSpeed)
{
	SetPosition(m_xmf4x4World._41 + vDirection.x * fSpeed, m_xmf4x4World._42 + vDirection.y * fSpeed, m_xmf4x4World._43 + vDirection.z * fSpeed);
}

void CGameObject::LookTo(XMFLOAT3& xmf3LookTo, XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookToLH(GetPosition(), xmf3LookTo, xmf3Up);
	m_xmf4x4Transform._11 = xmf4x4View._11; m_xmf4x4Transform._12 = xmf4x4View._21; m_xmf4x4Transform._13 = xmf4x4View._31;
	m_xmf4x4Transform._21 = xmf4x4View._12; m_xmf4x4Transform._22 = xmf4x4View._22; m_xmf4x4Transform._23 = xmf4x4View._32;
	m_xmf4x4Transform._31 = xmf4x4View._13; m_xmf4x4Transform._32 = xmf4x4View._23; m_xmf4x4Transform._33 = xmf4x4View._33;

	ComputeWorldTransform(NULL);
}

void CGameObject::LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookAtLH(GetPosition(), xmf3LookAt, xmf3Up);
	m_xmf4x4Transform._11 = xmf4x4View._11; m_xmf4x4Transform._12 = xmf4x4View._21; m_xmf4x4Transform._13 = xmf4x4View._31;
	m_xmf4x4Transform._21 = xmf4x4View._12; m_xmf4x4Transform._22 = xmf4x4View._22; m_xmf4x4Transform._23 = xmf4x4View._32;
	m_xmf4x4Transform._31 = xmf4x4View._13; m_xmf4x4Transform._32 = xmf4x4View._23; m_xmf4x4Transform._33 = xmf4x4View._33;

	ComputeWorldTransform(NULL);
}

void CGameObject::UpdateBoundingBox()
{
	if (m_pMesh)
	{
		m_pMesh->m_xmOOBB.Transform(m_xmOOBB, XMLoadFloat4x4(&m_xmf4x4World));
		XMStoreFloat4(&m_xmOOBB.Orientation, XMQuaternionNormalize(XMLoadFloat4(&m_xmOOBB.Orientation)));
	}
}

void CGameObject::Animate(float fElapsedTime)
{
	if (m_fMovingSpeed != 0.0f) Move(m_xmf3MovingDirection, m_fMovingSpeed * fElapsedTime);

	if (m_pSibling) 
		m_pSibling->Animate(fElapsedTime);
	if (m_pChild) 
		m_pChild->Animate(fElapsedTime);

	UpdateBoundingBox();
}

CGameObject* CGameObject::FindFrame(_TCHAR* pstrFrameName)
{
	CGameObject* pFrameObject = NULL;
	if (!_tcsncmp(m_pstrFrameName, pstrFrameName, _tcslen(pstrFrameName))) 
		return(this);

	if (m_pSibling) 
		if (pFrameObject = m_pSibling->FindFrame(pstrFrameName)) 
			return(pFrameObject);
	if (m_pChild) 
		if (pFrameObject = m_pChild->FindFrame(pstrFrameName)) 
			return(pFrameObject);

	return(NULL);
}

void CGameObject::Render(HDC hDCFrameBuffer, XMFLOAT4X4* pxmf4x4World, CMesh* pMesh,_float3* pCullingPosition)
{
	if (pMesh)
	{
		CGraphicsPipeline::SetWorldTransform(pxmf4x4World);

		HPEN hPen = ::CreatePen(PS_SOLID, 0, m_dwColor);
		HPEN hOldPen = (HPEN)::SelectObject(hDCFrameBuffer, hPen);
		
		if (pCullingPosition)
			pMesh->Render(hDCFrameBuffer, pCullingPosition, 100.f);
		else
			pMesh->Render(hDCFrameBuffer);

		::SelectObject(hDCFrameBuffer, hOldPen);
		::DeleteObject(hPen);
	}
}

void CGameObject::Render(HDC hDCFrameBuffer, CCamera* pCamera, bool bSibling)
{
	if(bSibling){
		CGameObject::Render(hDCFrameBuffer, &m_xmf4x4World, m_pMesh);
	}
	else {
		if (pCamera->IsInFrustum(m_xmOOBB))
			CGameObject::Render(hDCFrameBuffer, &m_xmf4x4World, m_pMesh);
	}
	if (m_pSibling) 
		m_pSibling->Render(hDCFrameBuffer, pCamera, true);

	if (m_pChild) 
		m_pChild->Render(hDCFrameBuffer, pCamera, true);
}

void CGameObject::ComputeWorldTransform(XMFLOAT4X4* pxmf4x4Parent)
{
	m_xmf4x4World = (pxmf4x4Parent) ? Matrix4x4::Multiply(m_xmf4x4Transform, *pxmf4x4Parent) : m_xmf4x4Transform;

	if (m_pSibling) 
		m_pSibling->ComputeWorldTransform(pxmf4x4Parent);
	if (m_pChild) 
		m_pChild->ComputeWorldTransform(&m_xmf4x4World);
}

void CGameObject::GenerateRayForPicking(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection)
{
	XMMATRIX xmmtxToModel = XMMatrixInverse(NULL, XMLoadFloat4x4(&m_xmf4x4World) * xmmtxView);

	XMFLOAT3 xmf3CameraOrigin(0.0f, 0.0f, 0.0f);
	xmvPickRayOrigin = XMVector3TransformCoord(XMLoadFloat3(&xmf3CameraOrigin), xmmtxToModel);
	xmvPickRayDirection = XMVector3TransformCoord(xmvPickPosition, xmmtxToModel);
	xmvPickRayDirection = XMVector3Normalize(xmvPickRayDirection - xmvPickRayOrigin);
}

int CGameObject::PickObjectByRayIntersection(XMVECTOR& xmvPickPosition, XMMATRIX& xmmtxView, float* pfHitDistance)
{
	int nIntersected = 0;
	if (m_pMesh)
	{
		XMVECTOR xmvPickRayOrigin, xmvPickRayDirection;
		GenerateRayForPicking(xmvPickPosition, xmmtxView, xmvPickRayOrigin, xmvPickRayDirection);
		nIntersected = m_pMesh->CheckRayIntersection(xmvPickRayOrigin, xmvPickRayDirection, pfHitDistance);
	}
	return(nIntersected);
}

