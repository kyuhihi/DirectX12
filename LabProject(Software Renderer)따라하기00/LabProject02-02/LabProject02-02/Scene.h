#pragma once

#include "GameObject.h"
#include "Camera.h"

#include "ExplosiveObject.h"
#include "RotatingObject.h"
#include "MortarMonster.h"
#include "BlowUpObject.h"
#include "BulletObject.h"
#include "ShieldObject.h"
#include "FloorObject.h"
#include "AxisObject.h"
#include "Monster.h"
#include "Player.h"

class CScene
{
public:
	enum EVENT { EVENT_BLOWUP, EVENT_SUPPLY, EVENT_END};
public:
	CScene(CPlayer *pPlayer);
	virtual ~CScene();

private:
	int							m_nObjects = 0;
	int m_iSupplyCount = 0;
	CExplosiveObject** m_ppObjects = NULL;
	CGameObject* m_pDecoObject = NULL;

	CFloorObject*				m_pFloorObject = NULL;

	CPlayer*					m_pPlayer = NULL;
	vector<CMonster*>		m_pMonsterVec;
	vector<CBlowUpObject*>	m_pBlowUpEffectVec;
	vector<CShieldObject*>	m_pShieldObjectVec;
	
	vector<CMortarMonster*>	m_pMortarMonsterVec;
	float m_fTimeMultiply = 1.f;
		

#ifdef _WITH_DRAW_AXIS
	CGameObject*				m_pWorldAxis = NULL;
#endif
private:
	void PlayEvent(EVENT eEvent,const _float3 vTargetPosition);
	void MakeShield();

public:
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	void CheckObjectByObjectCollisions();
	void CheckObjectByBulletCollisions();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	CGameObject* PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera);
};

