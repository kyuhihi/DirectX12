#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "GameObject.h"

class CFloorObject : public CGameObject
{
public:
	CFloorObject();
	virtual ~CFloorObject();

	void SetPlayer(CGameObject* pPlayer) { m_pPlayer = pPlayer; }
private:
	CGameObject* m_pPlayer = nullptr;

public:
	BoundingOrientedBox			m_xmOOBBPlayerMoveCheck = BoundingOrientedBox();
	XMFLOAT4					m_pxmf4FloorPlane;
	


	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
	
};