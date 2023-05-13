#include "stdafx.h"
#include "MortarMonster.h"
#include "GameObject.h"

void CMortarMonster::OnCollision(CGameObject* pOtherObject)
{
	string strTag = pOtherObject->GetTag();
	if (strTag == "Player") {
		m_bActive = false;
		CTextManager* pTextManager = CTextManager::GetInstance();
		CTextManager::TextInfo tMessage;
		tMessage.strText = "MortarMonster " + to_string(m_iMortarMonsterNum) + " Down!!!";
		tMessage.strFindTag = "MortarMonster" + to_string(m_iMortarMonsterNum);
		pTextManager->AddText(tMessage);
		pTextManager->AddScore(500);
	}
}

void CMortarMonster::OnUpdateTransform()
{
	m_xmf4x4Transform._11 = m_xmf3Right.x;
	m_xmf4x4Transform._12 = m_xmf3Right.y;
	m_xmf4x4Transform._13 = m_xmf3Right.z;

	m_xmf4x4Transform._21 = m_xmf3Up.x;
	m_xmf4x4Transform._22 = m_xmf3Up.y;
	m_xmf4x4Transform._23 = m_xmf3Up.z;

	m_xmf4x4Transform._31 = m_xmf3Look.x;
	m_xmf4x4Transform._32 = m_xmf3Look.y;
	m_xmf4x4Transform._33 = m_xmf3Look.z;

	m_xmf4x4Transform._41 = m_xmf3Position.x;
	m_xmf4x4Transform._42 = m_xmf3Position.y;
	m_xmf4x4Transform._43 = m_xmf3Position.z;
}

void CMortarMonster::CheckShootingBulletTiming(float fTimeDelta)
{
	m_fBulletFireTimer += fTimeDelta;
	if (m_fBulletFireTimer > m_fBulletFireMaxTimer) {
		m_fBulletFireTimer = 0.f;
		FireBullet(nullptr);
	}
}

void CMortarMonster::InitializePreViewCube()
{
	CCubeMesh* pCubeMesh = new CCubeMesh(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < PREVIEW_CUBES; ++i) {
		CGameObject* pCube = new CGameObject();
		pCube->SetMesh(pCubeMesh);
		pCube->SetColor(RGB(0.f, 0.f, 0.f));
		m_pPreViewCubeVec.push_back(pCube);
	}
}

void CMortarMonster::MovePreviewCube(float fTimeDelta)
{
	_vector vPlayerPos = XMLoadFloat3(&m_pPlayer->GetPosition());
	
	_vector vMyPos = XMLoadFloat3(&GetFirePosition());
	_vector vMiddlePos = (vMyPos + vPlayerPos) * 0.5f;
	vMiddlePos.m128_f32[1] += 20.f;

	_vector vPoint0{ vMyPos }, vPoint1{ vMiddlePos }, vPoint2{ vPlayerPos };
	float fDist = XMVector3Length(vPoint1).m128_f32[0];
	_float3 vTargetPositions[3];
	XMStoreFloat3(&vTargetPositions[0], vPoint0);
	XMStoreFloat3(&vTargetPositions[1], vPoint1);
	XMStoreFloat3(&vTargetPositions[2], vPoint2);

	for (int i = 0; i < MON_BULLETS; ++i) {
		if(!m_ppBullets[i]->m_bActive)
			m_ppBullets[i]->SetMortarThreePoint(vTargetPositions[0], vTargetPositions[1], vTargetPositions[2]);
	}

	if (fDist > 1.f) {
		for (int i = 0; i < PREVIEW_CUBES; ++i) {
			float fMakeOneMinusT{ 1.f - (1.f / PREVIEW_CUBES)* i };
			_vector vTargetPosition;

			vTargetPosition = (fMakeOneMinusT * fMakeOneMinusT * vMyPos) + (2 * fMakeOneMinusT * (1.f / PREVIEW_CUBES)*i * vPoint1) + 
				((1.f / PREVIEW_CUBES)*i * (1.f / PREVIEW_CUBES)*i) * vPoint2;
			_float3 vConvertf3;
			XMStoreFloat3(&vConvertf3, vTargetPosition);

			if (i == 0)
			{
				_float3 vLookat;
				XMStoreFloat3(&vLookat,vMiddlePos);
				m_pGun->LookAt(vLookat, m_pGun->GetUp());
			}
			m_pPreViewCubeVec[i]->SetPosition(vConvertf3);
			m_pPreViewCubeVec[i]->ComputeWorldTransform(NULL);

		}

	}
		/*m_pTurret->LookAt(m_pPlayer->GetPosition(), m_pPlayer->GetUp());*/

}

void CMortarMonster::Initialize(int iMortarMonsterNum, int iMaxMortarMonsterNum)
{
	m_iMortarMonsterNum = iMortarMonsterNum;
	float fRate = 1.f / iMaxMortarMonsterNum * 360.f * iMortarMonsterNum;
	_matrix RotationMat = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fRate));
	_float3 vPlayerPos = m_pPlayer->GetPosition();
	m_xmf3Position.x = vPlayerPos.x + RotationMat.r[2].m128_f32[0] * 50.f;
	m_xmf3Position.y = vPlayerPos.y;
	m_xmf3Position.z = vPlayerPos.z + RotationMat.r[2].m128_f32[2] * 50.f;
	//OnUpdateTransform();
	m_fBulletFireMaxTimer = RandF(5.f, 8.f);
}

void CMortarMonster::RespawnMortarMonster()
{
	Initialize(m_iMortarMonsterNum, 10);
	m_xmf3Position.y = 10.f;
	m_bActive = true;
	m_fRespawnTimer = 0.f;
}

void CMortarMonster::UpdateRespawnTimer(float fTimeDelta)
{
	m_fRespawnTimer += fTimeDelta;
	if (m_fRespawnTimer > 3.f) {
		RespawnMortarMonster();
	}
}

CMortarMonster::CMortarMonster()
{
	m_strTag = "Monster";
	CCubeMesh* pBodyMesh = new CCubeMesh(6.0f, 2.0f, 6.0f);
	SetMesh(pBodyMesh);
	SetColor(RGB(255, 0, 0));

	m_pTurret = new CGameObject();
	CCubeMesh* pTurretMesh = new CCubeMesh(4.0f, 1.5f, 4.0f);
	m_pTurret->SetMesh(pTurretMesh);
	m_pTurret->SetColor(RGB(255, 0, 0));
	m_pTurret->SetPosition(0.0f, 1.75f, 0.0f);
	SetChild(m_pTurret);

	m_pGun = new CGameObject();
	CCubeMesh* pGunMesh = new CCubeMesh(1.0f, 1.0f, 10.0f);
	m_pGun->SetMesh(pGunMesh);
	m_pGun->SetColor(RGB(255, 0, 0));
	m_pGun->SetPosition(0.0f, 1.25f, 4.0f);
	m_pTurret->SetChild(m_pGun);

	CCubeMesh* pBulletMesh = new CCubeMesh(1.0f, 1.0f, 4.0f);
	for (int i = 0; i < MON_BULLETS; i++)
	{
		m_ppBullets[i] = new CBulletObject(m_fBulletEffectiveRange);
		m_ppBullets[i]->SetMesh(pBulletMesh);
		m_ppBullets[i]->SetRotationAxis(XMFLOAT3(0.0f, 0.0f, 1.0f));
		m_ppBullets[i]->SetRotationSpeed(360.0f);
		m_ppBullets[i]->SetMovingSpeed(120.0f);
		m_ppBullets[i]->SetActive(false);

		m_ppBullets[i]->SetMortarBullet();// 박격포탄으로 변경.
	}

	InitializePreViewCube();
}

CMortarMonster::~CMortarMonster()
{
	for (int i = 0; i < MON_BULLETS; i++)
		if (m_ppBullets[i])
			delete m_ppBullets[i];
	m_pPlayer = nullptr;
}

void CMortarMonster::FireBullet(CGameObject* pLockedObject)
{
	CBulletObject* pBulletObject = NULL;
	for (int i = 0; i < MON_BULLETS; i++)
	{
		if (!m_ppBullets[i]->m_bActive)
		{
			pBulletObject = m_ppBullets[i];
			break;
		}
	}

	if (pBulletObject)
	{
		XMFLOAT3 xmf3Position = m_pGun->GetPosition();
		XMFLOAT3 xmf3Direction = m_pGun->GetLook();
		XMFLOAT3 xmf3FirePosition = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Direction, 6.0f, false));

		pBulletObject->m_xmf4x4Transform = m_pGun->m_xmf4x4World;

		pBulletObject->SetFirePosition(xmf3FirePosition);
		pBulletObject->SetMovingDirection(xmf3Direction);
		pBulletObject->SetColor(RGB(255, 0, 0));
		pBulletObject->SetActive(true);

		if (pLockedObject)
		{
			pBulletObject->m_pLockedObject = pLockedObject;
			pBulletObject->SetColor(RGB(0, 0, 255));
		}
	}
}

void CMortarMonster::Animate(float fElapsedTime)
{
	CheckShootingBulletTiming(fElapsedTime);
	
	MovePreviewCube(fElapsedTime);
	
	OnUpdateTransform();

	CGameObject::Animate(fElapsedTime);
	ComputeWorldTransform(NULL);

	for (int i = 0; i < MON_BULLETS; i++)
	{
		if (m_ppBullets[i]->m_bActive)
		{
			m_ppBullets[i]->Animate(fElapsedTime * 0.1f);
			m_ppBullets[i]->ComputeWorldTransform(NULL);
		}
	}
}

void CMortarMonster::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGameObject::Render(hDCFrameBuffer, pCamera, true);

	for (int i = 0; i < MON_BULLETS; i++)
		if (m_ppBullets[i]->m_bActive)
			m_ppBullets[i]->Render(hDCFrameBuffer, pCamera, true);

	for (int i = 0; i < PREVIEW_CUBES; ++i) {
		if(abs(m_fBulletFireTimer - m_fBulletFireMaxTimer) < 1.f)
		m_pPreViewCubeVec[i]->Render(hDCFrameBuffer, pCamera, true);
	}
}

_float3 CMortarMonster::GetFirePosition()
{
	XMFLOAT3 xmf3Position = m_pGun->GetPosition();
	XMFLOAT3 xmf3Direction = m_pGun->GetLook();
	XMFLOAT3 xmf3FirePosition = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Direction, 6.0f, false));
	return xmf3FirePosition;
}
