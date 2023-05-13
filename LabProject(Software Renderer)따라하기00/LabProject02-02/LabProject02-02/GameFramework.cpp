//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "GameFramework.h"



void CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	::srand(timeGetTime());

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	BuildFrameBuffer(); 

	BuildObjects(); 

	_tcscpy_s(m_pszFrameRate, _T("LabProject ("));
}

void CGameFramework::OnDestroy()
{
	ReleaseObjects();
	CTextManager::GetInstance()->DestroyInstance();

	if (m_hBitmapFrameBuffer) ::DeleteObject(m_hBitmapFrameBuffer);
	if (m_hDCFrameBuffer) ::DeleteDC(m_hDCFrameBuffer);
}

void CGameFramework::BuildFrameBuffer()
{
	::GetClientRect(m_hWnd, &m_rcClient);

	HDC hDC = ::GetDC(m_hWnd);

    m_hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top);
    ::SelectObject(m_hDCFrameBuffer, m_hBitmapFrameBuffer);

	::ReleaseDC(m_hWnd, hDC);
    ::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
}

void CGameFramework::ClearFrameBuffer(DWORD dwColor)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 0, dwColor);
	HPEN hOldPen = (HPEN)::SelectObject(m_hDCFrameBuffer, hPen);
	HBRUSH hBrush = ::CreateSolidBrush(dwColor);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDCFrameBuffer, hBrush);
	::Rectangle(m_hDCFrameBuffer, m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom);
	::SelectObject(m_hDCFrameBuffer, hOldBrush);
	::SelectObject(m_hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);
	::DeleteObject(hBrush);
}

void CGameFramework::PresentFrameBuffer()
{    
    HDC hDC = ::GetDC(m_hWnd);
    ::BitBlt(hDC, m_rcClient.left, m_rcClient.top, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top, m_hDCFrameBuffer, m_rcClient.left, m_rcClient.top, SRCCOPY);
	SetBkMode(hDC, TRANSPARENT); // 매개변수 DC의 글자배경을 투명하게 한다. 
	
	CTextManager* pTextManager = CTextManager::GetInstance();
	const vector<CTextManager::TextInfo> tTextVector = pTextManager->GetTextVector();

	for (auto& iter : tTextVector) {
		TextOut(hDC, (int)iter.vGeometry.x, (int)iter.vGeometry.y, pTextManager->s2ws(iter.strText).c_str(), (int)iter.strText.size());
	}

	::ReleaseDC(m_hWnd, hDC);
	//pTextManager->ClearTextVec();
}

void CGameFramework::BuildObjects()
{
	CCamera* pCamera = new CCamera();
	pCamera->SetViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	pCamera->GeneratePerspectiveProjectionMatrix(1.01f, 500.0f, 60.0f);
	pCamera->SetFOVAngle(60.0f);

	pCamera->GenerateOrthographicProjectionMatrix(1.01f, 50.0f, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

	m_pPlayer = new CTankPlayer();
	m_pPlayer->SetPosition(0.0f, 1.0f, 0.0f);
	m_pPlayer->SetCamera(pCamera);
	m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -15.0f));

	m_pScene = new CScene(m_pPlayer);
	m_pScene->BuildObjects();
}

void CGameFramework::ReleaseObjects()
{
	if (m_pScene)
	{
		m_pScene->ReleaseObjects();
		delete m_pScene;
	}

	if (m_pPlayer) 
		delete m_pPlayer;
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pScene) 
		m_pScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{
	case WM_RBUTTONDOWN:
		break;
	case WM_LBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&m_ptOldCursorPos);
	/*	if (nMessageID == WM_RBUTTONDOWN)
		{
			m_pLockedObject = m_pScene->PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam), m_pPlayer->m_pCamera);

			if (m_pLockedObject) 
				m_pLockedObject->SetColor(RGB(0, 0, 0));
		}*/
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pScene) 
		m_pScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			break;
	
		case VK_SPACE:
			((CTankPlayer*)m_pPlayer)->FireBullet(m_pLockedObject);
			m_pLockedObject = NULL;
			break;
		default:
			m_pScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
			m_GameTimer.Stop();
		else
			m_GameTimer.Start();
		break;
	}
	case WM_SIZE:
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

void CGameFramework::ProcessInput()
{
	static UCHAR pKeyBuffer[256];
	if (GetKeyboardState(pKeyBuffer))
	{
		DWORD dwDirection = 0;
		if (pKeyBuffer[VK_UP] & 0xF0) 
			dwDirection |= DIR_FORWARD;

		if (pKeyBuffer[VK_DOWN] & 0xF0) 
			dwDirection |= DIR_BACKWARD;
		
		if (pKeyBuffer[VK_LEFT] & 0xF0) 
			dwDirection |= DIR_LEFT;
		
		if (pKeyBuffer[VK_RIGHT] & 0xF0) 
			dwDirection |= DIR_RIGHT;
		
		if (pKeyBuffer[VK_PRIOR] & 0xF0) 
			dwDirection |= DIR_UP;
		
		if (pKeyBuffer[VK_NEXT] & 0xF0) 
			dwDirection |= DIR_DOWN;

		if (pKeyBuffer[VK_F1] & 0xF0) {
			CCamera* pCamera = m_pPlayer->GetCamera();
			float& fSetterTime = pCamera->GetModeSetterTimer();
			if (fSetterTime == 0.f) {
				CCamera::CAMERA_MODE eMode = pCamera->GetCamMode();
				if (CCamera::CAMERA_MODE::MODE_FREE == eMode) {
					pCamera->SetCamMode(CCamera::MODE_DEFAULT);
					CTextManager* pTextManager = CTextManager::GetInstance();

					CTextManager::TextInfo tMessage;
					tMessage.strText = "CAMMODE: Default!!!!";
					tMessage.strFindTag = "CAMMODE";
					pTextManager->AddText(tMessage);
				}
				else if (CCamera::CAMERA_MODE::MODE_DEFAULT == eMode) {
					pCamera->SetCamMode(CCamera::MODE_FREE);

					CTextManager* pTextManager = CTextManager::GetInstance();

					CTextManager::TextInfo tMessage;
					tMessage.strText = "CAMMODE: FREE!!!!";
					tMessage.strFindTag = "CAMMODE";
					pTextManager->AddText(tMessage);
				}
				fSetterTime = 1.f;
			}
		}
	
		if (pKeyBuffer['P'] & 0xF0)
			m_pPlayer->SetScale(XMFLOAT3(0.9f, 0.9f, 0.9f));

		if (pKeyBuffer['O'] & 0xF0)
			m_pPlayer->SetScale(XMFLOAT3(1.1f, 1.1f, 1.1f));

		if (dwDirection) 
			m_pPlayer->Move(dwDirection, 0.5f);

		CCamera::CAMERA_MODE eMode = m_pPlayer->GetCamera()->GetCamMode();
		if (pKeyBuffer['W'] & 0xF0) {
			if(eMode == CCamera::MODE_FREE)
				m_pPlayer->GetCamera()->Move(CCamera::MOVE_STRAIGHT);
			else
				((CTankPlayer*)m_pPlayer)->RotateGun(1.0f);
		}
		if (pKeyBuffer['A'] & 0xF0) {
			if (eMode == CCamera::MODE_FREE)
				m_pPlayer->GetCamera()->Move(CCamera::MOVE_LEFT);
			else
				((CTankPlayer*)m_pPlayer)->RotateTurret(-5.0f);
		}
		if (pKeyBuffer['S'] & 0xF0) {
			if (eMode == CCamera::MODE_FREE)
				m_pPlayer->GetCamera()->Move(CCamera::MOVE_BACKWARD);
			else
				((CTankPlayer*)m_pPlayer)->RotateGun(-1.0f);
		}
		if (pKeyBuffer['D'] & 0xF0) {
			if (eMode == CCamera::MODE_FREE)
				m_pPlayer->GetCamera()->Move(CCamera::MOVE_RIGHT);
			else
				((CTankPlayer*)m_pPlayer)->RotateTurret(+5.0f);
		}
	}

	if (GetCapture() == m_hWnd)
	{
		//SetCursor(NULL);
		POINT ptCursorPos;
		GetCursorPos(&ptCursorPos);
		float cxMouseDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) ;
		float cyMouseDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) ;
		SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		//if (cxMouseDelta || cyMouseDelta)
		//{
		//	if (pKeyBuffer[VK_RBUTTON] & 0xF0)
		//		//m_pPlayer->Rotate(0.0f, 0.0f, -cxMouseDelta);
		//		m_pPlayer->Rotate(0.0f, -cxMouseDelta,0.0f);
		//	else
		//		m_pPlayer->Rotate(0.0f, cxMouseDelta, 0.0f);
		//}
		
		MouseProccessing(cxMouseDelta, cyMouseDelta);

	}

	m_pPlayer->Update(m_GameTimer.GetTimeElapsed());
}

void CGameFramework::MouseProccessing(float fX, float fY)
{
	CCamera* pCamera = m_pPlayer->GetCamera();
	CCamera::CAMERA_MODE eCamMode = pCamera->GetCamMode();

	switch (eCamMode)
	{
	case CCamera::MODE_DEFAULT:
		//fX /= 3.0f;
		//fY /= 1.0f;
		////m_pPlayer->Rotate(0.0f, fX, 0.0f);
		//((CTankPlayer*)m_pPlayer)->RotateTurret(fY);



		/*case 'R':
		((CTankPlayer*)m_pPlayer)->RotateTurret(+5.0f);
		break;
	case 'E':
		((CTankPlayer*)m_pPlayer)->RotateTurret(-5.0f);
		break;
	case 'Q':
		((CTankPlayer*)m_pPlayer)->RotateGun(+1.0f);
		break;
	case 'W':
		((CTankPlayer*)m_pPlayer)->RotateGun(-1.0f);
		break;*/
		break;
	case CCamera::MODE_FREE:
		pCamera->Turn(_float3(0.f, 1.f, 0.f), fX* 0.005f);
		pCamera->Turn(pCamera->GetState(CCamera::STATE_RIGHT), fY * 0.005f);
		break;
	case CCamera::MODE_END:
		break;
	default:
		break;
	}

}

void CGameFramework::AnimateObjects()
{
	float fTimeElapsed = m_GameTimer.GetTimeElapsed();
	if (m_pPlayer) 
		m_pPlayer->Animate(fTimeElapsed);
	if (m_pScene) 
		m_pScene->Animate(fTimeElapsed);

	CTextManager* pTextManager = CTextManager::GetInstance();
	pTextManager->UpdateText(fTimeElapsed);
}

void CGameFramework::FrameAdvance()
{    
	m_GameTimer.Tick(60.0f);

	ProcessInput();

	AnimateObjects();

    ClearFrameBuffer(RGB(255, 255, 255));

	CCamera* pCamera = m_pPlayer->GetCamera();
	if (m_pScene) 
		m_pScene->Render(m_hDCFrameBuffer, pCamera);

	PresentFrameBuffer();

	m_GameTimer.GetFrameRate(m_pszFrameRate + 12, 37);
	::SetWindowText(m_hWnd, m_pszFrameRate);
	
}


