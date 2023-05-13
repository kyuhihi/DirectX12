#include "stdafx.h"
#include "GraphicsPipeline.h"
#include "RotatingObject.h"



CRotatingObject::CRotatingObject()
{
}

CRotatingObject::~CRotatingObject()
{
}

void CRotatingObject::Animate(float fElapsedTime)
{
	if (m_fRotationSpeed != 0.0f) 
		Rotate(m_xmf3RotationAxis, m_fRotationSpeed * fElapsedTime);

	CGameObject::Animate(fElapsedTime);
}