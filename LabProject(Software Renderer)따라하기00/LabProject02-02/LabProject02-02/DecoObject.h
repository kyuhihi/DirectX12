#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "RotatingObject.h"

class CDecoObject : public CRotatingObject
{
public:
	CDecoObject();
	virtual ~CDecoObject();
	XMFLOAT4X4					m_pxmf4x4Transforms[EXPLOSION_DEBRISES];

	float						m_fExplosionRotation = 720.0f;


	void Initialize();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	void SetPlayer(CGameObject* pPlayer) { m_pPlayer = pPlayer; }
private:
	CGameObject* m_pPlayer = nullptr;

public:
	static CMesh*				m_pExplosionMesh;
	static XMFLOAT3				m_pxmf3SphereVectors[EXPLOSION_DEBRISES];

	static void PrepareExplosion();
};
