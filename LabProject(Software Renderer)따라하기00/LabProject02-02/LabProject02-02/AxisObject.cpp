#include "stdafx.h"
#include "GraphicsPipeline.h"
#include "AxisObject.h"

void CAxisObject::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGraphicsPipeline::SetWorldTransform(&m_xmf4x4World);

	m_pMesh->Render(hDCFrameBuffer);
}

