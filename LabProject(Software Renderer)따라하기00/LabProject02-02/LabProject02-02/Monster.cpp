#include "stdafx.h"
#include "Monster.h"
#include "GameObject.h"

void CMonster::OnCollision(CGameObject* pOtherObject)
{
	string strTag = pOtherObject->GetTag();
	if (strTag == "Player") {
		m_bActive = false;
		CTextManager* pTextManager = CTextManager::GetInstance();
		CTextManager::TextInfo tMessage;
		tMessage.strText = "Monster " + to_string(m_iMonsterNum) + " Down!!!";
		tMessage.strFindTag = "Monster" + to_string(m_iMonsterNum);
		pTextManager->AddText(tMessage);
		pTextManager->AddScore(100);
	}
}

void CMonster::OnUpdateTransform()
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

void CMonster::CheckShootingBulletTiming(float fTimeDelta)
{
	if (!m_bLand)
		return;
	m_fBulletFireTimer += fTimeDelta;
	if (m_fBulletFireTimer > m_fBulletFireMaxTimer) {
		m_fBulletFireTimer = 0.f;
		FireBullet(nullptr);
	}
}

void CMonster::Initialize(int iMonsterNum, int iMaxMonsterNum)
{
	m_iMonsterNum = iMonsterNum;
	float fRate = 1.f / iMaxMonsterNum * 360.f * iMonsterNum;
	_matrix RotationMat = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fRate));
	_float3 vPlayerPos = m_pPlayer->GetPosition();
	m_xmf3Position.x = vPlayerPos.x + RotationMat.r[2].m128_f32[0] * 50.f;
	m_xmf3Position.y = vPlayerPos.y;
	m_xmf3Position.z = vPlayerPos.z + RotationMat.r[2].m128_f32[2] * 50.f;
	//OnUpdateTransform();
	m_fBulletFireMaxTimer = RandF(3.f, 5.f);
}

void CMonster::RespawnMonster()
{
	int iCurScore{ CTextManager::GetInstance()->GetScore() };
	iCurScore /= 100;
	Initialize(iCurScore + 10, 10);
	m_xmf3Position.y = 10.f;
	m_bActive = true;
	m_bLand = false;
	m_fRespawnTimer = 0.f;
}

void CMonster::UpdateRespawnTimer(float fTimeDelta)
{
	m_fRespawnTimer += fTimeDelta;
	if (m_fRespawnTimer > 3.f) {
		RespawnMonster();
	}
}

CMonster::CMonster()
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
	}
}

CMonster::~CMonster()
{
	for (int i = 0; i < MON_BULLETS; i++)
		if (m_ppBullets[i])
			delete m_ppBullets[i];
	m_pPlayer = nullptr;
}

void CMonster::FireBullet(CGameObject* pLockedObject)
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

void CMonster::Animate(float fElapsedTime)
{
	if (!m_bLand)
	{
		m_xmf3Position.y -= fElapsedTime * 5.f;
		if (m_xmf3Position.y <= 1.f) {
			m_xmf3Position.y = 1.f;
			m_bLand = true;
		}
	}
	CheckShootingBulletTiming(fElapsedTime);
	_vector vDirectional = XMLoadFloat3(&m_pPlayer->GetPosition()) - XMLoadFloat3(&m_xmf3Position);

	float fDist = XMVector3Length(vDirectional).m128_f32[0];
	if (fDist > 1.f)
		m_pTurret->LookAt(m_pPlayer->GetPosition(), m_pPlayer->GetUp());

	XMStoreFloat3(&m_xmf3Position , XMLoadFloat3(&m_xmf3Position) + XMVector3Normalize(vDirectional) * 3.f * fElapsedTime);
	
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

void CMonster::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CGameObject::Render(hDCFrameBuffer, pCamera);
	
	for (int i = 0; i < MON_BULLETS; i++)
		if (m_ppBullets[i]->m_bActive)
			m_ppBullets[i]->Render(hDCFrameBuffer, pCamera);
}
