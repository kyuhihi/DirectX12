#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "BulletObject.h"
#include "TextManager.h"

#define MON_BULLETS					3
#define PREVIEW_CUBES				20
class CMortarMonster : public CGameObject
{
private:
	CGameObject* m_pTurret = NULL;
	CGameObject* m_pGun = NULL;
	CGameObject* m_pPlayer = NULL;

	vector<CGameObject*> m_pPreViewCubeVec; 

	float m_fBulletFireTimer = 0.f;
	float m_fBulletFireMaxTimer = 5.f;
	float m_fRespawnTimer = 0.f;
	
	int m_iMortarMonsterNum = 0;

	XMFLOAT3					m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3					m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);


public:
	virtual void OnCollision(CGameObject* pOtherObject) override;

private:
	virtual void OnUpdateTransform();
	void CheckShootingBulletTiming(float fTimeDelta);
	void InitializePreViewCube();
	void MovePreviewCube(float fTimeDelta);

public:
	void Initialize(int iMortarMonsterNum, int iMaxMortarMonsterNum);
	void RespawnMortarMonster();
	void SetPlayer(CGameObject* pPlayer) { m_pPlayer = pPlayer; }
	void UpdateRespawnTimer(float fTimeDelta); 
	CBulletObject** GetBullet() { return m_ppBullets; }

public:
	CMortarMonster();
	virtual ~CMortarMonster();

	float						m_fBulletEffectiveRange = 150.0f;
	CBulletObject* m_ppBullets[MON_BULLETS];

	void RotateTurret(float fAngle) { m_pTurret->Rotate(0.0f, fAngle, 0.0f); }
	void RotateGun(float fAngle) { m_pGun->Rotate(fAngle, 0.0f, 0.0f); }

	void FireBullet(CGameObject* pLockedObject);
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
	_float3 GetFirePosition();
};

