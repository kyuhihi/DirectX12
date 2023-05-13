#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "RotatingObject.h"
#include "TextManager.h"

class CExplosiveObject : public CRotatingObject
{
public:
	CExplosiveObject();
	virtual ~CExplosiveObject();

	bool						m_bBlowingUp = false;
	bool						m_bLand = false;

	XMFLOAT4X4					m_pxmf4x4Transforms[EXPLOSION_DEBRISES];

	float						m_fElapsedTimes = 0.0f;
	float						m_fDuration = 2.0f;
	float						m_fExplosionSpeed = 10.0f;
	float						m_fExplosionRotation = 720.0f;
	function<void()>			m_pExcuteCallBack = NULL;


	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
	virtual void OnCollision(CGameObject* pOtherObject);
	
public:
	void SupplyFall(_float3 vTargetPosition, function<void()>pEndCall);

public:
	static CMesh*				m_pExplosionMesh;
	static XMFLOAT3				m_pxmf3SphereVectors[EXPLOSION_DEBRISES];
	

	static void PrepareExplosion();
};
