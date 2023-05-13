#include "stdafx.h"
#include "Camera.h"
#include "Mesh.h"
#include "Player.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
void CViewport::SetViewport(int nLeft, int nTop, int nWidth, int nHeight)
{
	m_nLeft = nLeft; 
	m_nTop = nTop; 
	m_nWidth = nWidth; 
	m_nHeight = nHeight; 
}

CCamera::CCamera()
{
}

CCamera::~CCamera()
{
}

void CCamera::GenerateViewMatrix()
{
	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Up, m_xmf3Look));
	m_xmf3Up = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Look, m_xmf3Right));

	m_xmf4x4View._11 = m_xmf3Right.x; m_xmf4x4View._12 = m_xmf3Up.x; m_xmf4x4View._13 = m_xmf3Look.x;
	m_xmf4x4View._21 = m_xmf3Right.y; m_xmf4x4View._22 = m_xmf3Up.y; m_xmf4x4View._23 = m_xmf3Look.y;
	m_xmf4x4View._31 = m_xmf3Right.z; m_xmf4x4View._32 = m_xmf3Up.z; m_xmf4x4View._33 = m_xmf3Look.z;
	
	m_xmf4x4View._41 = -Vector3::DotProduct(m_xmf3Position, m_xmf3Right);
	m_xmf4x4View._42 = -Vector3::DotProduct(m_xmf3Position, m_xmf3Up);
	m_xmf4x4View._43 = -Vector3::DotProduct(m_xmf3Position, m_xmf3Look);

	m_xmf4x4ViewPerspectiveProject = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4PerspectiveProject);
	m_xmf4x4OrthographicProject = Matrix4x4::Multiply(m_xmf4x4View, m_xmf4x4OrthographicProject);

	m_xmf4x4InverseView._11 = m_xmf3Right.x; m_xmf4x4InverseView._12 = m_xmf3Right.y; m_xmf4x4InverseView._13 = m_xmf3Right.z;
	m_xmf4x4InverseView._21 = m_xmf3Up.x; m_xmf4x4InverseView._22 = m_xmf3Up.y; m_xmf4x4InverseView._23 = m_xmf3Up.z;
	m_xmf4x4InverseView._31 = m_xmf3Look.x; m_xmf4x4InverseView._32 = m_xmf3Look.y; m_xmf4x4InverseView._33 = m_xmf3Look.z;
	m_xmf4x4InverseView._41 = m_xmf3Position.x; m_xmf4x4InverseView._42 = m_xmf3Position.y; m_xmf4x4InverseView._43 = m_xmf3Position.z;

	m_xmFrustumView.Transform(m_xmFrustumWorld, XMLoadFloat4x4(&m_xmf4x4InverseView));
}

void CCamera::SetLookAt(XMFLOAT3& xmf3Position, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	m_xmf3Position = xmf3Position;
	m_xmf4x4View = Matrix4x4::LookAtLH(m_xmf3Position, xmf3LookAt, xmf3Up);
	m_xmf3Right = Vector3::Normalize(XMFLOAT3(m_xmf4x4View._11, m_xmf4x4View._21, m_xmf4x4View._31));
	m_xmf3Up = Vector3::Normalize(XMFLOAT3(m_xmf4x4View._12, m_xmf4x4View._22, m_xmf4x4View._32));
	m_xmf3Look = Vector3::Normalize(XMFLOAT3(m_xmf4x4View._13, m_xmf4x4View._23, m_xmf4x4View._33));
}

void CCamera::SetCamMode()
{
	if (m_fSetterTimer != 0.f)
		return;
	int iMode = m_eCamMode;
	++iMode;
	if (iMode == MODE_END)
		iMode = MODE_DEFAULT;
	m_eCamMode = (CAMERA_MODE)iMode;
}

void CCamera::SetLookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookAtLH(m_xmf3Position, xmf3LookAt, xmf3Up);
	m_xmf3Right = Vector3::Normalize(XMFLOAT3(xmf4x4View._11, xmf4x4View._21, xmf4x4View._31));
	m_xmf3Up = Vector3::Normalize(XMFLOAT3(xmf4x4View._12, xmf4x4View._22, xmf4x4View._32));
	m_xmf3Look = Vector3::Normalize(XMFLOAT3(xmf4x4View._13, xmf4x4View._23, xmf4x4View._33));
}

void CCamera::SetViewport(int nLeft, int nTop, int nWidth, int nHeight)
{
	m_Viewport.SetViewport(nLeft, nTop, nWidth, nHeight);
	m_fAspectRatio = float(m_Viewport.m_nWidth) / float(m_Viewport.m_nHeight);
}

void CCamera::LerpPosition(_fvector vTargetPosition, float fTimeDelta)
{
	_vector vTargetPos = XMVectorLerp(XMLoadFloat3(&m_xmf3Position), vTargetPosition, fTimeDelta * 5.f);
	XMStoreFloat3(&m_xmf3Position, vTargetPos);
}

void CCamera::SetFOVAngle(float fFOVAngle)
{
	m_fFOVAngle = fFOVAngle;
	m_fProjectRectDistance = float(1.0f / tan(DegreeToRadian(fFOVAngle * 0.5f)));
}

void CCamera::GeneratePerspectiveProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fFOVAngle)
{
	float fAspectRatio = (float(m_Viewport.m_nWidth) / float(m_Viewport.m_nHeight));
	XMMATRIX xmmtxProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(fFOVAngle), fAspectRatio, fNearPlaneDistance, fFarPlaneDistance);
	XMStoreFloat4x4(&m_xmf4x4PerspectiveProject, xmmtxProjection);

	BoundingFrustum::CreateFromMatrix(m_xmFrustumView, xmmtxProjection);
}

void CCamera::GenerateOrthographicProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fWidth, float hHeight)
{
	XMMATRIX xmmtxProjection = XMMatrixOrthographicLH(fWidth, hHeight, fNearPlaneDistance, fFarPlaneDistance);
	XMStoreFloat4x4(&m_xmf4x4OrthographicProject, xmmtxProjection);
}

void CCamera::RunShaking()
{
	if (m_bShaking || m_eCamMode == MODE_EVENT)
		return;

	m_fShakingTimer = 0.f;
	
	m_vPreCamPosition=  m_xmf3Position;
	m_bShaking = true;
}

void CCamera::UpdateShaking(float fTimeDelta)
{
	if (!m_bShaking)
		return;

	_vector vPosition = XMLoadFloat3(&m_vPreCamPosition);
	XMVectorSetW(vPosition,1.f);
	m_fShakingTimer+= fTimeDelta;

	_float3 updatePosition;
	XMStoreFloat3(&updatePosition, vPosition);

	// sinf(m_fShakingTimer * 50.f) 반복 속도
	// powf(0.01f, m_fShakingTimer) 얼마나 크게 흔들리냐
	updatePosition.y += (sinf(m_fShakingTimer * m_fShakingSpeed) * powf(m_fShakingPower, m_fShakingTimer)) * m_fMagnitude;
	updatePosition.x += (cosf(m_fShakingTimer * m_fShakingSpeed) * powf(m_fShakingPower, m_fShakingTimer)) * m_fMagnitude;
	if (m_fShakingTimer > m_fMaxShakingTime)
	{
		m_bShaking = false;
	}
	else
	{
		m_xmf3Position = updatePosition;
	}
}

bool CCamera::IsInFrustum(BoundingOrientedBox& xmBoundingBox)
{
	return(m_xmFrustumWorld.Intersects(xmBoundingBox));
}

_float3 CCamera::GetState(STATE eState)
{
	_float3 vRetState = { 0.f,0.f,0.f };
	switch (eState)
	{
	case CCamera::STATE_RIGHT:
		vRetState = m_xmf3Right;
		break;
	case CCamera::STATE_UP:
		vRetState = m_xmf3Up;
		break;
	case CCamera::STATE_LOOK:
		vRetState = m_xmf3Look;
		break;
	case CCamera::STATE_POSITION:
		vRetState = m_xmf3Position;
		break;
	}
	return vRetState;
}

void CCamera::Move(XMFLOAT3& xmf3Shift)
{
	if (m_bShaking)
		return;
	m_xmf3Position = Vector3::Add(m_xmf3Position, xmf3Shift);
}

void CCamera::Move(float x, float y, float z)
{
	if (m_bShaking)
		return;
	Move(XMFLOAT3(x, y, z));
}

void CCamera::Move(MOVE_DIRECTION eDirection)
{
	if (m_bShaking)
		return;
	switch (eDirection)
	{
	case CCamera::MOVE_STRAIGHT:
		m_xmf3Position = Vector3::Add(m_xmf3Position, m_xmf3Look);
		break;
	case CCamera::MOVE_LEFT: {
		_vector vMakeLeft = XMLoadFloat3(&m_xmf3Right);
		vMakeLeft *= -1.f;
		_float3 vMakeDirection = { 0.f,0.f,0.f };
		XMStoreFloat3(&vMakeDirection, vMakeLeft);
		m_xmf3Position = Vector3::Add(m_xmf3Position, vMakeDirection);
		break;
	}
	case CCamera::MOVE_BACKWARD: {
		_vector vMakeLeft = XMLoadFloat3(&m_xmf3Look);
		vMakeLeft *= -1.f;
		_float3 vMakeDirection = { 0.f,0.f,0.f };
		XMStoreFloat3(&vMakeDirection, vMakeLeft);
		m_xmf3Position = Vector3::Add(m_xmf3Position, vMakeDirection);
		break;
	}
	case CCamera::MOVE_RIGHT:
		m_xmf3Position = Vector3::Add(m_xmf3Position, m_xmf3Right);
		break;
	}
}

void CCamera::Rotate(float fPitch, float fYaw, float fRoll)
{
	if (fPitch != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
	}
	if (fYaw != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(fYaw));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}
	if (fRoll != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}
}

void CCamera::Turn(_float3 vAxis, float fTimeDelta)
{
	_matrix		RotationMatrix = XMMatrixRotationAxis(XMLoadFloat3(&vAxis), fTimeDelta);

	XMStoreFloat3(&m_xmf3Right,XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Right), RotationMatrix));
	XMStoreFloat3(&m_xmf3Up, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Up), RotationMatrix));
	XMStoreFloat3(&m_xmf3Look, XMVector3TransformNormal(XMLoadFloat3(&m_xmf3Look), RotationMatrix));

}

void CCamera::Update(CPlayer* pPlayer, XMFLOAT3& xmf3LookAt, float fTimeElapsed)
{//여기서 들어오는 LookAt값은 PlayerPosition이다.
	if (m_fSetterTimer != 0.f)
	{
		m_fSetterTimer += fTimeElapsed;
		if (m_fSetterTimer > 1.5f)
			m_fSetterTimer = 0.f;
	}


	if (m_eCamMode == MODE_FREE)
		return;

	XMFLOAT4X4 mtxRotate = Matrix4x4::Identity();
	_float3 vTurretRight = pPlayer->GetTurret()->GetRight();
	_float3 vTurretUp = pPlayer->GetTurret()->GetUp();
	_float3 vTurretLook = pPlayer->GetTurret()->GetLook();

	
	mtxRotate._11 = vTurretRight.x;
	mtxRotate._12 = vTurretRight.y;
	mtxRotate._13 = vTurretRight.z;

	mtxRotate._21 = vTurretUp.x;
	mtxRotate._22 = vTurretUp.y;
	mtxRotate._23 = vTurretUp.z;

	mtxRotate._31 = vTurretLook.x;
	mtxRotate._32 = vTurretLook.y;
	mtxRotate._33 = vTurretLook.z;

	XMFLOAT3 xmf3Offset = Vector3::TransformCoord(pPlayer->m_xmf3CameraOffset, mtxRotate);
	XMFLOAT3 xmf3Position = Vector3::Add(pPlayer->m_xmf3Position, xmf3Offset);
	XMFLOAT3 xmf3Direction = Vector3::Subtract(xmf3Position, m_xmf3Position);

	float fLength = Vector3::Length(xmf3Direction);
	xmf3Direction = Vector3::Normalize(xmf3Direction);

	float fTimeLagScale = fTimeElapsed * (1.0f / 0.25f);
	float fDistance = fLength * fTimeLagScale;
	
	if (fDistance > fLength) 
		fDistance = fLength;
	if (fLength < 0.01f) 
		fDistance = fLength;
	
	if (fDistance > 0)
	{
		if (m_eCamMode == MODE_DEFAULT) {
			_matrix RotationMat = XMLoadFloat4x4(&mtxRotate);
			_vector MakeLerpPosition = XMLoadFloat3(&xmf3LookAt);
			MakeLerpPosition += RotationMat.r[2] * (-20.f);
			MakeLerpPosition += RotationMat.r[1] * (8.f);
			LerpPosition(MakeLerpPosition, fTimeElapsed);

			SetLookAt(pPlayer->m_xmf3Position, pPlayer->GetTurret()->GetUp());
		}
		else if (m_eCamMode == MODE_EVENT) {
			_float3 vTargetPosition = m_pTargetObject->GetPosition();
			vTargetPosition.y = 2.5f;
			vTargetPosition.x -= 15.f;
			vTargetPosition.z -= 15.f;
			LerpPosition(XMLoadFloat3(&vTargetPosition), fTimeElapsed);
			SetLookAt(m_pTargetObject->GetPosition(), _float3(0.f,1.f,0.f));

		}
		
	}
	UpdateShaking(fTimeElapsed);

}
