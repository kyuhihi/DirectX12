#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "GameObject.h"

class CRotatingObject : public CGameObject
{
public:
	CRotatingObject();
	virtual ~CRotatingObject();

	XMFLOAT3					m_xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	float						m_fRotationSpeed = 0.0f;

	void SetRotationAxis(XMFLOAT3& xmf3RotationAxis) { m_xmf3RotationAxis = Vector3::Normalize(xmf3RotationAxis); }
	void SetRotationSpeed(float fSpeed) { m_fRotationSpeed = fSpeed; }
	virtual void OnCollision(CGameObject* pOtherObject) {};
	virtual void Animate(float fElapsedTime);
};

