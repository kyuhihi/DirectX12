#include "stdafx.h"
#include "Scene.h"
#include "GraphicsPipeline.h"
#include "DecoObject.h"

CScene::CScene(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

CScene::~CScene()
{
}

void CScene::BuildObjects()
{
	CExplosiveObject::PrepareExplosion();

	float fHalfWidth = 360.0f, fHalfHeight = 45.0f, fHalfDepth = 360.0f;
	CFloorMesh* pFloorMesh = new CFloorMesh(fHalfWidth * 2.0f, fHalfDepth * 2.0f, 100);

	m_pFloorObject = new CFloorObject();
	m_pFloorObject->SetPosition(0.f, 0.0f, 0.0f);
	m_pFloorObject->SetMesh(pFloorMesh);
	m_pFloorObject->SetColor(RGB(0, 0, 0));
	m_pFloorObject->m_pxmf4FloorPlane = XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);

	m_pFloorObject->SetPlayer(m_pPlayer);// 플레이어 위치로부터 일정거리 이상인 삼각형 렌더중지

	CCubeMesh* pCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);

	m_nObjects = 10;
	m_ppObjects = new CExplosiveObject * [m_nObjects];

	CExplosiveObject *pExplosiveObject = new CExplosiveObject();
	pExplosiveObject->SetMesh(pCubeMesh);
	pExplosiveObject->SetColor(RGB(255, 0, 0));
	pExplosiveObject->SetPosition(-43.5f, 0.0f, -44.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pExplosiveObject->SetRotationSpeed(90.0f);
	m_ppObjects[0] = pExplosiveObject;

	pExplosiveObject = new CExplosiveObject();
	pExplosiveObject->SetMesh(pCubeMesh);
	pExplosiveObject->SetColor(RGB(0, 0, 255));
	pExplosiveObject->SetPosition(+43.5f, 0.0f, -44.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pExplosiveObject->SetRotationSpeed(180.0f);
	m_ppObjects[1] = pExplosiveObject;

	pExplosiveObject = new CExplosiveObject();
	pExplosiveObject->SetMesh(pCubeMesh);
	pExplosiveObject->SetColor(RGB(0, 255, 0));
	pExplosiveObject->SetPosition(-43.5f, 0.0f, +44.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pExplosiveObject->SetRotationSpeed(30.15f);
	m_ppObjects[2] = pExplosiveObject;

	pExplosiveObject = new CExplosiveObject();
	pExplosiveObject->SetMesh(pCubeMesh);
	pExplosiveObject->SetColor(RGB(0, 255, 255));
	pExplosiveObject->SetPosition(+43.5f, 0.0f, +44.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pExplosiveObject->SetRotationSpeed(40.6f);
	m_ppObjects[3] = pExplosiveObject;

	pExplosiveObject = new CExplosiveObject();
	pExplosiveObject->SetMesh(pCubeMesh);
	pExplosiveObject->SetColor(RGB(128, 0, 255));
	pExplosiveObject->SetPosition(0.0f, 0.0f, 40.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pExplosiveObject->SetRotationSpeed(50.06f);
	m_ppObjects[4] = pExplosiveObject;


	pExplosiveObject = new CExplosiveObject();
	pExplosiveObject->SetMesh(pCubeMesh);
	pExplosiveObject->SetColor(RGB(255, 0, 255));
	pExplosiveObject->SetPosition(-20.0f, 0.0f, -20.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pExplosiveObject->SetRotationSpeed(60.06f);
	m_ppObjects[5] = pExplosiveObject;

	pExplosiveObject = new CExplosiveObject();
	pExplosiveObject->SetMesh(pCubeMesh);
	pExplosiveObject->SetColor(RGB(255, 0, 255));
	pExplosiveObject->SetPosition(-20.0f, 0.0f, +20.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pExplosiveObject->SetRotationSpeed(60.06f);
	m_ppObjects[6] = pExplosiveObject;

	pExplosiveObject = new CExplosiveObject();
	pExplosiveObject->SetMesh(pCubeMesh);
	pExplosiveObject->SetColor(RGB(255, 0, 128));
	pExplosiveObject->SetPosition(+20.0f, 0.0f, -20.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pExplosiveObject->SetRotationSpeed(70.06f);
	m_ppObjects[7] = pExplosiveObject;

	pExplosiveObject = new CExplosiveObject();
	pExplosiveObject->SetMesh(pCubeMesh);
	pExplosiveObject->SetColor(RGB(128, 0, 255));
	pExplosiveObject->SetPosition(+20.0f, 0.0f, +20.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(1.0f, 0.0f, 0.0f));
	pExplosiveObject->SetRotationSpeed(90.06f);
	m_ppObjects[8] = pExplosiveObject;

	pExplosiveObject = new CExplosiveObject();
	pExplosiveObject->SetMesh(pCubeMesh);
	pExplosiveObject->SetColor(RGB(255, 64, 64));
	pExplosiveObject->SetPosition(+15.0f, 0.0f, 0.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(1.0f, 0.0f, 0.0f));
	pExplosiveObject->SetRotationSpeed(90.06f);
	m_ppObjects[9] = pExplosiveObject;


	m_pPlayer->SetPosition(-180.f, 1.f, 0.f);

	for (int i = 0; i < 10; ++i) {
		CMonster* pMonster = new CMonster();
		pMonster->SetPlayer(m_pPlayer);
		m_pMonsterVec.push_back(pMonster);
		pMonster->Initialize(i, 10);
	}

	for (int i = 0; i < 10; ++i) {
		CBlowUpObject* pBlowUpObject =new CBlowUpObject();
		m_pBlowUpEffectVec.push_back(pBlowUpObject);
	}

	for (int i = 0; i < 9; ++i) {
		CShieldObject* pShieldObject = new CShieldObject;
		pShieldObject->SetMesh(pCubeMesh);
		m_pShieldObjectVec.push_back(pShieldObject);
	}
	for (int i = 0; i < 4; ++i) {
		CMortarMonster* pMonsterMortal= new CMortarMonster;
		pMonsterMortal->SetPlayer(m_pPlayer);
		pMonsterMortal->Initialize(i, 4);
		m_pMortarMonsterVec.push_back(pMonsterMortal);
	}
	
	m_pDecoObject = new CDecoObject;
	CDecoObject* pDecoObject = dynamic_cast<CDecoObject*>(m_pDecoObject);
	pDecoObject->SetPlayer(m_pPlayer);
	pDecoObject->Initialize();

#ifdef _WITH_DRAW_AXIS
	m_pWorldAxis = new CGameObject();
	CAxisMesh* pAxisMesh = new CAxisMesh(0.5f, 0.5f, 0.5f);
	m_pWorldAxis->SetMesh(pAxisMesh);
#endif
}

void CScene::ReleaseObjects()
{
	if (CExplosiveObject::m_pExplosionMesh) 
		CExplosiveObject::m_pExplosionMesh->Release();

	if (CDecoObject::m_pExplosionMesh)
		CDecoObject::m_pExplosionMesh->Release();

	for (int i = 0; i < m_nObjects; i++) 
		if (m_ppObjects[i]) 
			delete m_ppObjects[i];

	if (m_ppObjects) 
		delete[] m_ppObjects;

	if (m_pFloorObject) 
		delete m_pFloorObject;

	if (m_pDecoObject)
		delete m_pDecoObject;

	for (int i = 0; i < m_pMonsterVec.size();++i) 
		delete m_pMonsterVec[i];

	for (int i = 0; i < m_pShieldObjectVec.size(); ++i)
		delete m_pShieldObjectVec[i];
	
	for (auto& iter : m_pBlowUpEffectVec) 
		delete iter;

	for (auto& iter : m_pMortarMonsterVec)
		delete iter;
		
	m_pMonsterVec.clear();
	m_pBlowUpEffectVec.clear();
	m_pShieldObjectVec.clear();
	m_pMortarMonsterVec.clear();

#ifdef _WITH_DRAW_AXIS
	if (m_pWorldAxis) 
		delete m_pWorldAxis;
#endif
}

void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[int(wParam - '1')];
			pExplosiveObject->m_bBlowingUp = true;
			break;
		}
		/*case 'A':
			for (int i = 0; i < m_nObjects; i++)
			{
				CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[i];
				pExplosiveObject->m_bBlowingUp = true;
			}
			break;*/
		
		default:
			break;
		}
		break;
	default:
		break;
	}
	
}

CGameObject* CScene::PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera)
{
	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / (float)pCamera->m_Viewport.m_nWidth) - 1) / pCamera->m_xmf4x4PerspectiveProject._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / (float)pCamera->m_Viewport.m_nHeight) - 1) / pCamera->m_xmf4x4PerspectiveProject._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmmtxView = XMLoadFloat4x4(&pCamera->m_xmf4x4View);

	int nIntersected = 0;
	float fNearestHitDistance = FLT_MAX;
	CGameObject* pNearestObject = NULL;
	for (int i = 0; i < m_nObjects; i++)
	{
		float fHitDistance = FLT_MAX;
		nIntersected = m_ppObjects[i]->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, &fHitDistance);
		if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance))
		{
			fNearestHitDistance = fHitDistance;
			pNearestObject = m_ppObjects[i];
		}
	}
	return(pNearestObject);
}

void CScene::CheckObjectByObjectCollisions()
{
	for (int i = 0; i < m_nObjects; i++) 
		m_ppObjects[i]->m_pObjectCollided = NULL;

	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = (i + 1); j < m_nObjects; j++)
		{
			if (m_ppObjects[i]->m_xmOOBB.Intersects(m_ppObjects[j]->m_xmOOBB))
			{
				m_ppObjects[i]->m_pObjectCollided = m_ppObjects[j];
				m_ppObjects[j]->m_pObjectCollided = m_ppObjects[i];
			}
		}
	}
	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_ppObjects[i]->m_pObjectCollided)
		{
			XMFLOAT3 xmf3MovingDirection = m_ppObjects[i]->m_xmf3MovingDirection;
			float fMovingSpeed = m_ppObjects[i]->m_fMovingSpeed;
			m_ppObjects[i]->m_xmf3MovingDirection = m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection;
			m_ppObjects[i]->m_fMovingSpeed = m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
			m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed = fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_pObjectCollided = NULL;
			m_ppObjects[i]->m_pObjectCollided = NULL;
		}
	}
}

void CScene::PlayEvent(EVENT eEvent, const _float3 vTargetPosition)
{
	switch (eEvent)
	{
	case CScene::EVENT_BLOWUP: {//펑
		auto& iter = m_pBlowUpEffectVec.begin();
		for (iter; iter != m_pBlowUpEffectVec.end(); ++iter)
		{
			if (!(*iter)->m_bActive)
				break;
		}
		if (iter != m_pBlowUpEffectVec.end()) {
			(*iter)->SetTargetPosition(vTargetPosition);
		}
		break;
	}
	case CScene::EVENT_SUPPLY:
		for (int i = 0; i < m_nObjects; ++i) {
			if (!m_ppObjects[i]->m_bActive) {
				m_fTimeMultiply = 0.1f;

				m_ppObjects[i]->SupplyFall(vTargetPosition, [&] {
					m_fTimeMultiply = 1.f;
					CCamera* pCamera = m_pPlayer->GetCamera();
					pCamera->SetCamMode(CCamera::MODE_DEFAULT);
					pCamera->SetTarget(nullptr);
				});
				CCamera* pCamera = m_pPlayer->GetCamera();
				pCamera->SetCamMode(CCamera::MODE_EVENT);
				pCamera->SetTarget(m_ppObjects[i]);
				break;
			}
		}
		m_iSupplyCount = 0;
		break;
	default:
		break;
	}
}

void CScene::CheckObjectByBulletCollisions()
{
	CBulletObject** ppBullets = ((CTankPlayer*)m_pPlayer)->m_ppBullets;
	for (int i = 0; i < m_nObjects; i++)
	{//플레이어가 아이템을 맞췄을때
		for (int j = 0; j < BULLETS; j++)
		{
			if (ppBullets[j]->m_bActive && m_ppObjects[i]->m_bActive && m_ppObjects[i]->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
			{
				CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[i];
				pExplosiveObject->OnCollision(m_pPlayer);

				MakeShield();

				ppBullets[j]->Reset();
			}
		}
	}

	for (int i = 0; i < m_pMonsterVec.size(); ++i) {
		//플레이어가 몬스터를 맞췄을때.
		for (int j = 0; j < BULLETS; j++)
		{
			if (ppBullets[j]->m_bActive && m_pMonsterVec[i]->m_bActive && m_pMonsterVec[i]->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
			{
				m_pMonsterVec[i]->OnCollision(m_pPlayer);
				PlayEvent(EVENT_BLOWUP, m_pMonsterVec[i]->GetPosition());
				++m_iSupplyCount;

				if (m_iSupplyCount > 8)//만약 8마리 이상 잡으면 보급상자가 드랍된다.
					PlayEvent(EVENT_SUPPLY, m_pMonsterVec[i]->GetPosition());
				ppBullets[j]->Reset();
			}
		}

	}

	


	for (int j = 0; j < m_pMonsterVec.size(); ++j) {
		ppBullets = m_pMonsterVec[j]->GetBullet();
		for (int j = 0; j < MON_BULLETS; ++j) {
			if (ppBullets[j]->m_bActive) {
				bool bCollision{ false };
				for (int k = 0; k < SHIELD_MAX; ++k) {
					if (m_pShieldObjectVec[k]->m_bActive && m_pShieldObjectVec[k]->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
					{//만약 몬스터의 불렛이 쉴드에 맞았다면?
						m_pShieldObjectVec[k]->OnCollision(ppBullets[j]);
						bCollision = true;
						break;
					}
				}

				if (bCollision) {
					ppBullets[j]->Reset();
					continue;
				}

				if (m_pPlayer->m_bActive && m_pPlayer->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
				{
					if (m_pPlayer->GetCamera()->GetCamMode() != CCamera::MODE_FREE)
					{//몬스터가 플레이어 맞췄을때
						m_pPlayer->OnCollision(m_pMonsterVec[j]);
						PlayEvent(EVENT_BLOWUP, m_pPlayer->GetPosition());
					}
					ppBullets[j]->Reset();
				}
			}
		}
	}

	for (int j = 0; j < m_pMortarMonsterVec.size(); ++j) {
		ppBullets = m_pMortarMonsterVec[j]->GetBullet();
		for (int j = 0; j < MON_BULLETS; ++j) {
			if (ppBullets[j]->m_bActive) {
				bool bCollision{ false };
				for (int k = 0; k < SHIELD_MAX; ++k) {
					if (m_pShieldObjectVec[k]->m_bActive && m_pShieldObjectVec[k]->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
					{//만약 몬스터의 불렛이 쉴드에 맞았다면?
						m_pShieldObjectVec[k]->OnCollision(ppBullets[j]);
						bCollision = true;
						break;
					}
				}

				if (bCollision) {
					ppBullets[j]->Reset();
					continue;
				}

				if (m_pPlayer->m_bActive && m_pPlayer->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
				{
					if (m_pPlayer->GetCamera()->GetCamMode() != CCamera::MODE_FREE)
					{//몬스터가 플레이어 맞췄을때
						m_pPlayer->OnCollision(m_pMortarMonsterVec[j]);
						PlayEvent(EVENT_BLOWUP, m_pPlayer->GetPosition());
					}
					ppBullets[j]->Reset();
				}
			}
		}
	}

}

void CScene::Animate(float fElapsedTime)
{
	float fFixAnimatingTime = fElapsedTime;
	
	fFixAnimatingTime *= m_fTimeMultiply;
	for (int i = 0; i < m_pMonsterVec.size(); ++i) {
		if(m_pMonsterVec[i]->m_bActive)
			m_pMonsterVec[i]->Animate(fFixAnimatingTime);
		else{
			m_pMonsterVec[i]->UpdateRespawnTimer(fFixAnimatingTime);
		}
	}

	for (int i = 0; i < m_pMortarMonsterVec.size(); ++i) {
		if (m_pMortarMonsterVec[i]->m_bActive)
			m_pMortarMonsterVec[i]->Animate(fFixAnimatingTime);
		
	}

	for (int i = 0; i < m_pBlowUpEffectVec.size(); ++i) {
		if (m_pBlowUpEffectVec[i]->m_bActive)
			m_pBlowUpEffectVec[i]->Animate(fFixAnimatingTime);
	}

	for (auto& iter : m_pShieldObjectVec)
	{
		if (iter->m_bActive)
			iter->Animate(fFixAnimatingTime);
	}

	m_pFloorObject->Animate(fElapsedTime);
	m_pFloorObject->ComputeWorldTransform(NULL);

	m_pDecoObject->Animate(fFixAnimatingTime);

	for (int i = 0; i < m_nObjects; i++) {
		if(m_ppObjects[i]->m_bActive)
			m_ppObjects[i]->Animate(fElapsedTime);
		
	}
	for (int i = 0; i < m_nObjects; i++) {
		if (m_ppObjects[i]->m_bActive)
			m_ppObjects[i]->ComputeWorldTransform(NULL);
	}
	CheckObjectByObjectCollisions();

	CheckObjectByBulletCollisions();
}

void CScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);
	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);

	m_pFloorObject->Render(hDCFrameBuffer, pCamera);

	for (int i = 0; i < m_nObjects; i++) {
		if(m_ppObjects[i]->m_bActive)
		m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);
	}

	if (m_pPlayer) 
		m_pPlayer->Render(hDCFrameBuffer, pCamera);

	for (auto& iter : m_pShieldObjectVec)
	{
		if (iter->m_bActive)
			iter->Render(hDCFrameBuffer, pCamera);
	}

	for (int i = 0; i < m_pMonsterVec.size(); ++i) {
		if(m_pMonsterVec[i]->m_bActive)
			m_pMonsterVec[i]->Render(hDCFrameBuffer, pCamera);
	}

	for (int i = 0; i < m_pMortarMonsterVec.size(); ++i) {
		if (m_pMortarMonsterVec[i]->m_bActive)
			m_pMortarMonsterVec[i]->Render(hDCFrameBuffer, pCamera);

	}

	for (int i = 0; i < m_pBlowUpEffectVec.size(); ++i) {
		if (m_pBlowUpEffectVec[i]->m_bActive)
			m_pBlowUpEffectVec[i]->Render(hDCFrameBuffer, pCamera);
	}
	
	if (m_pDecoObject)
		((CDecoObject*)m_pDecoObject)->Render(hDCFrameBuffer, pCamera);
	
//UI
#ifdef _WITH_DRAW_AXIS
	CGraphicsPipeline::SetViewOrthographicProjectTransform(&pCamera->m_xmf4x4ViewOrthographicProject);
	

	_float4x4 PlayerF4x4 = m_pPlayer->m_xmf4x4World;

	_matrix PlayerMat = XMLoadFloat4x4(&PlayerF4x4);
	for (int i = 0; i < 3; ++i)
		PlayerMat.r[i] = XMVector3Normalize(PlayerMat.r[i]);

	XMStoreFloat4x4(&PlayerF4x4, PlayerMat);
	m_pWorldAxis->SetRotationTransform(&PlayerF4x4);
	m_pWorldAxis->Render(hDCFrameBuffer, pCamera);
#endif
}

void CScene::MakeShield()
{
	int iMakeShieldCount{ 3 };
	for (int k = 0; k < SHIELD_MAX; ++k)
	{
		if (iMakeShieldCount < 1)
			break;
		if (!m_pShieldObjectVec[k]->m_bActive) {
			m_pShieldObjectVec[k]->SetShield(k, m_pPlayer);
			--iMakeShieldCount;
		}
	}
}
