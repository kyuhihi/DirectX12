#pragma once
#include "RotatingObject.h"

class CGameObject;
class CBulletObject : public CRotatingObject
{
public:
	CBulletObject(float fEffectiveRange);
	virtual ~CBulletObject();

private:
	bool m_bMortarBullet = false;
	_float3 m_vTargetPositions[3];
	float m_fBazierTimer = 0.f;

public:
	void SetMortarBullet() { m_bMortarBullet = true; }
	void SetMortarThreePoint(_float3 vFirst, _float3 vSecond, _float3 vFinal);

public:
	virtual void Animate(float fElapsedTime);

	float						m_fBulletEffectiveRange = 50.0f;
	float						m_fMovingDistance = 0.0f;
	float						m_fRotationAngle = 0.0f;
	XMFLOAT3					m_xmf3FirePosition = XMFLOAT3(0.0f, 0.0f, 1.0f);

	float						m_fElapsedTimeAfterFire = 0.0f;
	float						m_fLockingDelayTime = 0.3f;
	float						m_fLockingTime = 4.0f;
	CGameObject*				m_pLockedObject = NULL;

	void SetFirePosition(XMFLOAT3 xmf3FirePosition);
	void Reset();
};