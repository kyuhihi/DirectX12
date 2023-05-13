#include "stdafx.h"
#include "GameObject.h"
#include "GraphicsPipeline.h"
#include "BulletObject.h"


CBulletObject::CBulletObject(float fEffectiveRange)
{
	m_fBulletEffectiveRange = fEffectiveRange;
}

CBulletObject::~CBulletObject()
{
}

void CBulletObject::SetFirePosition(XMFLOAT3 xmf3FirePosition)
{
	m_xmf3FirePosition = xmf3FirePosition;
	SetPosition(xmf3FirePosition);
}

void CBulletObject::Reset()
{
	m_pLockedObject = NULL;
	m_fElapsedTimeAfterFire = 0;
	m_fMovingDistance = 0;
	m_fRotationAngle = 0.0f;

	m_bActive = false;
	m_fBazierTimer = 0.f;
}

void CBulletObject::SetMortarThreePoint(_float3 vFirst, _float3 vSecond, _float3 vFinal)
{
	m_vTargetPositions[0] = vFirst;
	m_vTargetPositions[1] = vSecond;
	m_vTargetPositions[2] = vFinal;
}

void CBulletObject::Animate(float fElapsedTime)
{

	if (!m_bMortarBullet) {
		m_fElapsedTimeAfterFire += fElapsedTime;

		float fDistance = m_fMovingSpeed * fElapsedTime;

		XMFLOAT3 xmf3Position = GetPosition();
		if ((m_fElapsedTimeAfterFire > m_fLockingDelayTime) && m_pLockedObject)
		{
			XMVECTOR xmvPosition = XMLoadFloat3(&xmf3Position);

			XMFLOAT3 xmf3LockedObjectPosition = m_pLockedObject->GetPosition();
			XMVECTOR xmvLockedObjectPosition = XMLoadFloat3(&xmf3LockedObjectPosition);
			XMVECTOR xmvToLockedObject = xmvLockedObjectPosition - xmvPosition;
			xmvToLockedObject = XMVector3Normalize(xmvToLockedObject);

			XMVECTOR xmvMovingDirection = XMLoadFloat3(&m_xmf3MovingDirection);
			xmvMovingDirection = XMVector3Normalize(XMVectorLerp(xmvMovingDirection, xmvToLockedObject, 0.125f));
			XMStoreFloat3(&m_xmf3MovingDirection, xmvMovingDirection);
		}
		XMMATRIX xmRotate = XMMatrixRotationRollPitchYaw(XMConvertToRadians(0.0f), XMConvertToRadians(0.0f), XMConvertToRadians(m_fRotationSpeed * fElapsedTime));
		XMStoreFloat4x4(&m_xmf4x4World, xmRotate * XMLoadFloat4x4(&m_xmf4x4World));
		XMStoreFloat3(&xmf3Position, XMLoadFloat3(&xmf3Position) + (XMLoadFloat3(&m_xmf3MovingDirection) * fDistance));
		m_xmf4x4World._41 = xmf3Position.x; m_xmf4x4World._42 = xmf3Position.y; m_xmf4x4World._43 = xmf3Position.z;
		m_fMovingDistance += fDistance;

		CGameObject::Animate(fElapsedTime);

		if ((m_fMovingDistance > m_fBulletEffectiveRange) || (m_fElapsedTimeAfterFire > m_fLockingTime)) Reset();
	}
	else {
		float fMakeTime{ fElapsedTime * 10.f };
		_float3 vFixPosition,vLookAtPosition;
		_vector vPoint0{XMLoadFloat3(&m_vTargetPositions[0]) }, vPoint1{ XMLoadFloat3(&m_vTargetPositions[1]) }, vPoint2{ XMLoadFloat3(&m_vTargetPositions[2]) };

		float fMakeMinusOne{ 1.f - m_fBazierTimer };
		_vector vMakePosition = (fMakeMinusOne * fMakeMinusOne * vPoint0) + (2 * fMakeMinusOne * m_fBazierTimer * vPoint1)
			+ (m_fBazierTimer * m_fBazierTimer * vPoint2);
		
		float fMakeMinusOneLook{ 1.f - m_fBazierTimer + fMakeTime };
		_vector vMakeLookAtPos= (fMakeMinusOneLook * fMakeMinusOneLook * vPoint0) + (2 * fMakeMinusOneLook * (m_fBazierTimer + fMakeTime) * vPoint1)
			+ ((m_fBazierTimer + fMakeTime) * (m_fBazierTimer+ fMakeTime) * vPoint2);

		XMStoreFloat3(&vFixPosition, vMakePosition);
		XMStoreFloat3(&vLookAtPosition, vMakeLookAtPos);

		if(m_fBazierTimer < 1.f)
			LookAt(vLookAtPosition, GetUp());
		SetPosition(vFixPosition);

		m_fBazierTimer += fMakeTime;
		if (m_fBazierTimer > 1.f)
			Reset();
	}
}
