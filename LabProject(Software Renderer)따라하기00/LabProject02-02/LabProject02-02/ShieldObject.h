#pragma once
#include "Camera.h"
#include "Player.h"

#define SHIELD_MAX 9
class CShieldObject : public CGameObject
{
public:
	CShieldObject();
	virtual ~CShieldObject();
	
public:
	void Initialize();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	void SetShield(int iNumShield, CGameObject* pTargetObject);
	virtual void OnCollision(CGameObject* pOtherObject) override;

private:
	CPlayer* m_pPlayer = nullptr;
	float m_fShieldDegree = 0.f;
	float m_fHalfRadius = 5.f;
};
