#include "stdafx.h"
#include "ExplosiveObject.h"
#include "GameObject.h"

XMFLOAT3 CExplosiveObject::m_pxmf3SphereVectors[EXPLOSION_DEBRISES];
CMesh* CExplosiveObject::m_pExplosionMesh = NULL;

CExplosiveObject::CExplosiveObject()
{
	m_strTag = "ItemObject";
	m_bActive = false;
}

CExplosiveObject::~CExplosiveObject()
{
}

void CExplosiveObject::PrepareExplosion()
{
	for (int i = 0; i < EXPLOSION_DEBRISES; i++) 
		XMStoreFloat3(&m_pxmf3SphereVectors[i], RandomUnitVectorOnSphere());

	m_pExplosionMesh = new CCubeMesh(0.5f, 0.5f, 0.5f);
}

void CExplosiveObject::Animate(float fElapsedTime)
{
	if (m_bBlowingUp)
	{
		m_fElapsedTimes += fElapsedTime;
		if (m_fElapsedTimes <= m_fDuration)
		{
			XMFLOAT3 xmf3Position = GetPosition();
			for (int i = 0; i < EXPLOSION_DEBRISES; i++)
			{
				m_pxmf4x4Transforms[i] = Matrix4x4::Identity();
				m_pxmf4x4Transforms[i]._41 = xmf3Position.x + m_pxmf3SphereVectors[i].x * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i]._42 = xmf3Position.y + m_pxmf3SphereVectors[i].y * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i]._43 = xmf3Position.z + m_pxmf3SphereVectors[i].z * m_fExplosionSpeed * m_fElapsedTimes;
				m_pxmf4x4Transforms[i] = Matrix4x4::Multiply(Matrix4x4::RotationAxis(m_pxmf3SphereVectors[i], m_fExplosionRotation * m_fElapsedTimes), m_pxmf4x4Transforms[i]);
			}
		}
		else
		{
			m_bActive = false;
			m_bBlowingUp = false;
			m_fElapsedTimes = 0.0f;
			SetColor(m_dwColor);
		}
	}
	else
	{
		if (!m_bLand) {
			_float3 vFixPosition = GetPosition();
			vFixPosition.y -= fElapsedTime * 5.f;
			if (vFixPosition.y < 0.f) {
				m_bLand = true;
				vFixPosition.y = 0.f;
				m_pExcuteCallBack();
			}
			SetPosition(vFixPosition);
			
		}
		CRotatingObject::Animate(fElapsedTime);
	}
}

void CExplosiveObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	if (m_bBlowingUp)
	{
		for (int i = 0; i < EXPLOSION_DEBRISES; i++)
		{
			CGameObject::Render(hDCFrameBuffer , &m_pxmf4x4Transforms[i], m_pExplosionMesh);
		}
	}
	else
	{
		CGameObject::Render(hDCFrameBuffer, pCamera);
	}
}

void CExplosiveObject::OnCollision(CGameObject* pOtherObject)
{//여기서 플레이어가 인자로 전달됨.
	m_bBlowingUp = true;
	m_bLand = false;
	m_fElapsedTimes = 0.0f;

	CTextManager* pTextManager = CTextManager::GetInstance();
	CTextManager::TextInfo tMessage;
	tMessage.strText = "Get Item!!!!!!";
	tMessage.strFindTag = "Item";
	pTextManager->AddText(tMessage);

}

void CExplosiveObject::SupplyFall(_float3 vTargetPosition, function<void()>pEndCall)
{
	CTextManager* pTextManager = CTextManager::GetInstance();
	pTextManager->ClearTextVec();
	CTextManager::TextInfo tMessage;
	tMessage.strText = "Supply Box Falling!!!!!!!!!!";
	tMessage.strFindTag = "Supply";
	pTextManager->AddText(tMessage);

	m_bActive = true;
	m_bBlowingUp = false;
	m_bLand = false;
	m_fElapsedTimes = 0.0f;
	m_pExcuteCallBack = pEndCall;
	vTargetPosition.y = 30.f;
	SetPosition(vTargetPosition);
}
