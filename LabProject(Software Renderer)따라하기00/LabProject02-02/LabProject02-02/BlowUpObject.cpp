#include "stdafx.h"
#include "BlowUpObject.h"
#include "GameObject.h"


CBlowUpObject::CBlowUpObject()
{
	SetColor(m_dwDefaultColor);
	PrepareExplosion();
	for (int i = 0; i < EXPLOSION_DEBRISES; i++) {
		m_pxmf4x4Transforms[i] = Matrix4x4::Identity();
	}
	m_bActive = false;
}

CBlowUpObject::~CBlowUpObject()
{
	m_pExplosionMesh->Release();
}

void CBlowUpObject::PrepareExplosion()
{
	XMFLOAT2 fDirection{ -1.f, 1.f };
	for (int i = 0; i < EXPLOSION_DEBRISES; i++) {
		XMStoreFloat3(&m_pxmf3SphereVectors[i], RandomUnitVectorOnSphere(fDirection, XMFLOAT2(1.f, 1.f), fDirection));
		m_fFallDownMaxTimeVector[i] = RandF(2.f, 4.f);
	}
	m_pExplosionMesh = new CCubeMesh(0.5f, 0.5f, 0.5f);
	
}

void CBlowUpObject::Initialize()
{
	for (int i = 0; i < EXPLOSION_DEBRISES; ++i) {

		m_pxmf4x4Transforms[i]._41 = m_vTargetPosition.x + m_pxmf3SphereVectors[i].x;
		m_pxmf4x4Transforms[i]._42 = m_vTargetPosition.y + m_pxmf3SphereVectors[i].y * 0.5f;
		m_pxmf4x4Transforms[i]._43 = m_vTargetPosition.z + m_pxmf3SphereVectors[i].z;
	}
}

void CBlowUpObject::Animate(float fElapsedTime)
{
	m_fBlowUpTimer += fElapsedTime;
	for (int i = 0; i < EXPLOSION_DEBRISES; i++)
	{
		float fMakeDown{ 1.f };
		fMakeDown *= 50.f;

		if (m_fFallDownMaxTimeVector[i] * 0.5f < m_fBlowUpTimer)
			fMakeDown *= -1.f;

		_float3 vTransform = _float3(m_pxmf4x4Transforms[i]._41, m_pxmf4x4Transforms[i]._42, m_pxmf4x4Transforms[i]._43);;
		vTransform.x += fElapsedTime * m_pxmf3SphereVectors[i].x * 50.f;
		vTransform.y += fElapsedTime * m_pxmf3SphereVectors[i].y * fMakeDown;
		vTransform.z += fElapsedTime * m_pxmf3SphereVectors[i].z * 50.f;

		_vector vDist = XMLoadFloat3(&m_vTargetPosition) - XMLoadFloat3(&vTransform);
		float fDist = XMVectorGetX(XMVector3Length(vDist));

		m_pxmf4x4Transforms[i]._41 = vTransform.x;
		m_pxmf4x4Transforms[i]._42 = vTransform.y;
		m_pxmf4x4Transforms[i]._43 = vTransform.z;

	}

	if (m_fBlowUpTimer > 3.f)
		m_bActive = false;

}

void CBlowUpObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	for (int i = 0; i < EXPLOSION_DEBRISES; i++)
	{
		if (m_pxmf4x4Transforms[i]._42 < 0.f)
			continue;
		CGameObject::Render(hDCFrameBuffer, &m_pxmf4x4Transforms[i], m_pExplosionMesh);
	}

}

void CBlowUpObject::SetTargetPosition(_float3 vPosition)
{
	m_bActive = true;
	m_fBlowUpTimer = 0.f; 
	m_vTargetPosition = vPosition;
	Initialize();
	PrepareExplosion();
}
