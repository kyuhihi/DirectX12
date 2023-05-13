#pragma once

#include "Mesh.h"

class CViewport
{
public:
	CViewport() { }
	virtual ~CViewport() { }

	int							m_nLeft = 0;
	int							m_nTop = 0;
	int							m_nWidth = 0;
	int							m_nHeight = 0;

	void SetViewport(int nLeft, int nTop, int nWidth, int nHeight);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CPlayer;
class CTankPlayer;
class CCamera
{
public:
	enum CAMERA_MODE { MODE_DEFAULT, MODE_FREE, MODE_EVENT, MODE_END };
	enum MOVE_DIRECTION { MOVE_STRAIGHT, MOVE_LEFT, MOVE_BACKWARD, MOVE_RIGHT };
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION };

public:
	CCamera();
	virtual ~CCamera();

private:
	XMFLOAT3					m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3					m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	float						m_fFOVAngle = 90.0f;
	float						m_fProjectRectDistance = 1.0f;
	float						m_fSetterTimer = 0.f;

	float						m_fShakingTimer = 0.f;
	float						m_fShakingSpeed = 15.f;
	float						m_fShakingPower = 0.01f;
	float						m_fMaxShakingTime = 1.3f;
	float						m_fMagnitude = 1.f;
	
	bool						m_bShaking = false;
	_float3						m_vPreCamPosition = _float3(0.f, 0.f, 0.f);

	float						m_fAspectRatio = float(FRAMEBUFFER_WIDTH) / float(FRAMEBUFFER_HEIGHT);
	CAMERA_MODE 				m_eCamMode = MODE_DEFAULT;

	BoundingFrustum				m_xmFrustumView = BoundingFrustum();
	BoundingFrustum				m_xmFrustumWorld = BoundingFrustum();
	XMFLOAT4X4					m_xmf4x4InverseView = Matrix4x4::Identity();

	class CGameObject*			m_pTargetObject = nullptr;

public:
	XMFLOAT4X4					m_xmf4x4View = Matrix4x4::Identity();
	XMFLOAT4X4					m_xmf4x4PerspectiveProject = Matrix4x4::Identity();
	XMFLOAT4X4					m_xmf4x4ViewPerspectiveProject = Matrix4x4::Identity();

	XMFLOAT4X4					m_xmf4x4OrthographicProject = Matrix4x4::Identity();
	XMFLOAT4X4					m_xmf4x4ViewOrthographicProject = Matrix4x4::Identity();

	CViewport					m_Viewport;

private:
	void LerpPosition(_fvector vTargetPosition, float fTimeDelta);

public:
	void SetFOVAngle(float fFOVAngle);

	void GenerateViewMatrix();
	void GeneratePerspectiveProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fFOVAngle);
	void GenerateOrthographicProjectionMatrix(float fNearPlaneDistance, float fFarPlaneDistance, float fWidth, float hHeight);
	void SetViewport(int xTopLeft, int yTopLeft, int nWidth, int nHeight);

	void SetLookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);
	void SetLookAt(XMFLOAT3& vPosition, XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);
	void SetCamMode();
	void SetCamMode(CAMERA_MODE eMode) { m_eCamMode = eMode; };
	void SetTarget(CGameObject* pTargetObject) { m_pTargetObject = pTargetObject; }

	CAMERA_MODE GetCamMode() { return m_eCamMode; }
	float& GetModeSetterTimer() { return m_fSetterTimer; }
	_float3 GetState(STATE eState);

	void Move(XMFLOAT3& xmf3Shift);
	void Move(float x, float y, float z);
	void Move(MOVE_DIRECTION eDirection);

	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	void Turn(_float3 vAxis, float fTimeDelta);
	void Update(CPlayer* pPlayer, XMFLOAT3& xmf3LookAt, float fTimeElapsed = 0.016f);

	void ResetShaking() {
		m_fShakingTimer = 0.0f;
		m_bShaking = false;
	};
	void RunShaking();
	void UpdateShaking(float fTimeDelta);

	bool IsInFrustum(BoundingOrientedBox& xmBoundingBox);
};
