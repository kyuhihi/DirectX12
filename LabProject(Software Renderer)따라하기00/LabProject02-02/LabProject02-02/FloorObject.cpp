#include "stdafx.h"
#include "FloorObject.h"
#include "GraphicsPipeline.h"

CFloorObject::CFloorObject()
{
}

CFloorObject::~CFloorObject()
{
	m_pPlayer = nullptr;
}

void CFloorObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	_float3 vPlayerPos = m_pPlayer->GetPosition();
	CGameObject::Render(hDCFrameBuffer, &m_xmf4x4World, m_pMesh, &vPlayerPos);
}

