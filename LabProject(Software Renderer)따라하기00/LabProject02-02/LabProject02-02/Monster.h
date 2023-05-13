#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "RotatingObject.h"
#include "BulletObject.h"
#include "TextManager.h"

#define MON_BULLETS					3
class CMonster : public CGameObject
{
private:
	CGameObject* m_pTurret = NULL;
	CGameObject* m_pGun = NULL;
	CGameObject* m_pPlayer = NULL;

	float m_fBulletFireTimer = 0.f;
	float m_fBulletFireMaxTimer = 3.f;
	float m_fRespawnTimer = 0.f;
	
	bool m_bLand = true;
	int m_iMonsterNum = 0;

	XMFLOAT3					m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3					m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);


public:
	virtual void OnCollision(CGameObject* pOtherObject) override;

private:
	virtual void OnUpdateTransform();
	void CheckShootingBulletTiming(float fTimeDelta);

public:
	void Initialize(int iMonsterNum, int iMaxMonsterNum);
	void RespawnMonster();
	void SetPlayer(CGameObject* pPlayer) { m_pPlayer = pPlayer; }
	void UpdateRespawnTimer(float fTimeDelta); 
	CBulletObject** GetBullet() { return m_ppBullets; }

public:
	CMonster();
	virtual ~CMonster();

	float						m_fBulletEffectiveRange = 150.0f;
	CBulletObject* m_ppBullets[MON_BULLETS];

	void RotateTurret(float fAngle) { m_pTurret->Rotate(0.0f, fAngle, 0.0f); }
	void RotateGun(float fAngle) { m_pGun->Rotate(fAngle, 0.0f, 0.0f); }

	void FireBullet(CGameObject* pLockedObject);
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
};

