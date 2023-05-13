#include "stdafx.h"
#include "DecoObject.h"
#include "GameObject.h"

XMFLOAT3 CDecoObject::m_pxmf3SphereVectors[EXPLOSION_DEBRISES];
CMesh* CDecoObject::m_pExplosionMesh = NULL;

CDecoObject::CDecoObject()
{
	SetColor(RGB(0.f,0.f,255.f));
	PrepareExplosion();
	for (int i = 0; i < EXPLOSION_DEBRISES; i++) {
		m_pxmf4x4Transforms[i] = Matrix4x4::Identity();

	}
}

CDecoObject::~CDecoObject()
{
}

void CDecoObject::PrepareExplosion()
{
	XMFLOAT2 fDirection{ -100.f, 100.f };
	for (int i = 0; i < EXPLOSION_DEBRISES; i++) {
		XMStoreFloat3(&m_pxmf3SphereVectors[i], RandomUnitVectorOnSphere(fDirection, fDirection, fDirection));
	}
	m_pExplosionMesh = new CCubeMesh(0.5f, 0.5f, 0.5f);
	
}

void CDecoObject::Initialize()
{
	for (int i = 0; i < EXPLOSION_DEBRISES; ++i) {
		//m_pxmf4x4Transforms[i] = m_pPlayer->GetPosition();
		_float3 vInitPosition = m_pPlayer->GetPosition();
		m_pxmf4x4Transforms[i]._41 = vInitPosition.x+m_pxmf3SphereVectors[i].x;
		m_pxmf4x4Transforms[i]._42 = vInitPosition.y+m_pxmf3SphereVectors[i].y;
		m_pxmf4x4Transforms[i]._43 = vInitPosition.z+m_pxmf3SphereVectors[i].z;
	}
}

void CDecoObject::Animate(float fElapsedTime)
{
	_float3 vPlayerPos = m_pPlayer->GetPosition();
	for (int i = 0; i < EXPLOSION_DEBRISES; i++)
	{
		_float3 xmf3Position = vPlayerPos;
		XMStoreFloat3(&xmf3Position, XMLoadFloat3(&xmf3Position) + /*50.f **/ XMLoadFloat3(&m_pxmf3SphereVectors[i]));

		//m_pxmf4x4Transforms[i]._41 -= xmf3Position.x /*+ m_pxmf3SphereVectors[i].x */* fElapsedTime /** 3.f */;
		//m_pxmf4x4Transforms[i]._42 -= /*xmf3Position.y + m_pxmf3SphereVectors[i].y * */fElapsedTime * 3.f ;
		//m_pxmf4x4Transforms[i]._43 -= xmf3Position.z /*+ m_pxmf3SphereVectors[i].z */* fElapsedTime /** 3.f */;

		m_pxmf4x4Transforms[i]._41 +=  fElapsedTime* m_pxmf3SphereVectors[i].x /** 3.f */;
		m_pxmf4x4Transforms[i]._42 += fElapsedTime * 3.f;
		m_pxmf4x4Transforms[i]._43 += fElapsedTime* m_pxmf3SphereVectors[i].z  /** 3.f */;
																							  
		if (m_pxmf4x4Transforms[i]._42 > 10.f) {
			_float3 vResetPosition = { 0.f,0.f,0.f };
			XMStoreFloat3(&vResetPosition, XMLoadFloat3(&m_pxmf3SphereVectors[i]) + XMLoadFloat3(&vPlayerPos));
			m_pxmf4x4Transforms[i]._41 = vResetPosition.x + m_pxmf3SphereVectors[i].x * 3.f/** 10.f*/;
			m_pxmf4x4Transforms[i]._42 = 0.f + m_pxmf3SphereVectors[i].y;
			m_pxmf4x4Transforms[i]._43 = vResetPosition.z + m_pxmf3SphereVectors[i].z * 3.f/** 10.f*/;
		}

		//m_pxmf4x4Transforms[i] = Matrix4x4::Multiply(Matrix4x4::RotationAxis(m_pxmf3SphereVectors[i], m_fExplosionRotation * m_fElapsedTimes), m_pxmf4x4Transforms[i]);
	}

}

void CDecoObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{

	for (int i = 0; i < EXPLOSION_DEBRISES; i++)
	{
		CGameObject::Render(hDCFrameBuffer, &m_pxmf4x4Transforms[i], m_pExplosionMesh);
	}


}