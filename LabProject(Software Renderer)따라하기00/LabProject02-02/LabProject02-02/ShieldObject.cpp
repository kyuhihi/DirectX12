#include "stdafx.h"
#include "ShieldObject.h"
#include "GameObject.h"



CShieldObject::CShieldObject()
{
	SetColor(RGB(250.f,253.f,15.f));
	m_bActive = false;
}

CShieldObject::~CShieldObject()
{
}


void CShieldObject::Initialize()
{
	
}

void CShieldObject::Animate(float fElapsedTime)
{
	_float3 vUpdatePosition{ m_pPlayer->GetPosition() };
	_matrix RotationMat = XMMatrixRotationY(XMConvertToRadians(m_fShieldDegree));
	XMStoreFloat3(&vUpdatePosition,XMLoadFloat3(&vUpdatePosition) + RotationMat.r[2] * m_fHalfRadius);
	SetPosition(vUpdatePosition);
	m_fShieldDegree += fElapsedTime * 90.f;

	CGameObject::Animate(fElapsedTime);
	ComputeWorldTransform(NULL);

}

void CShieldObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGameObject::Render(hDCFrameBuffer, pCamera);
}

void CShieldObject::SetShield(int iNumShield, CGameObject* pTargetObject)
{
	CTextManager* pTextManager = CTextManager::GetInstance();
	CTextManager::TextInfo tMessage;
	tMessage.strText = "Get Shield!!!!";
	tMessage.strFindTag = "Shield";
	pTextManager->AddText(tMessage);

	if (!m_pPlayer)
		m_pPlayer = dynamic_cast<CPlayer*>(pTargetObject);

	m_fShieldDegree = (iNumShield % 3 * 3) * (360.f / SHIELD_MAX);
	m_bActive = true;
}

void CShieldObject::OnCollision(CGameObject* pOtherObject)
{
	m_bActive = false;
	CTextManager* pTextManager = CTextManager::GetInstance();
	CTextManager::TextInfo tMessage;
	tMessage.strText = "Shield Safe You!!!!!";
	tMessage.strFindTag = "Shield Safe";
	pTextManager->AddText(tMessage);
}
