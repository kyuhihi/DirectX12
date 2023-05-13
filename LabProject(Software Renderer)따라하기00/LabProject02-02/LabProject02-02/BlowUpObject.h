#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "RotatingObject.h"

class CBlowUpObject : public CRotatingObject
{
public:
	CBlowUpObject();
	virtual ~CBlowUpObject();
	XMFLOAT4X4					m_pxmf4x4Transforms[EXPLOSION_DEBRISES];

	void Initialize();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	void SetTargetPosition(_float3 vPosition); 

private:
	_float3 m_vTargetPosition{ 0.f,0.f,0.f };
	float m_fBlowUpTimer{ 0.f };

public:
	 CMesh*				m_pExplosionMesh;
	 XMFLOAT3				m_pxmf3SphereVectors[EXPLOSION_DEBRISES];
	 float				m_fFallDownMaxTimeVector[EXPLOSION_DEBRISES];

	 void PrepareExplosion();
};
