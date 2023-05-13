#include "stdafx.h"
#include "TextManager.h"
CTextManager* CTextManager::m_pInstance = nullptr;

CTextManager::CTextManager()
{
}

CTextManager::~CTextManager()
{
}

void CTextManager::UpdateText(float fTimeDelta)
{//Y값 100에서 30까지.
	vector<int> EraseIndexVec;
	for (int i =0; i< m_TextVec.size(); ++i) 
	{
		float fMakeMaxY{ 30.f + 15.f * i };
		if (m_TextVec[i].vGeometry.y <= fMakeMaxY) {
			if (i == 0) {
				m_TextVec[i].fLifeTime += fTimeDelta * 0.1f;
				if (m_TextVec[i].fLifeTime > 0.15f)
					EraseIndexVec.push_back(i);
			}
			continue;
		}

		m_TextVec[i].fLifeTime += fTimeDelta * 0.1f;
		m_TextVec[i].vGeometry.y = Lerp(m_TextVec[i].vGeometry.y, fMakeMaxY, m_TextVec[i].fLifeTime);

		if (m_TextVec[i].fLifeTime > 1.3f)
			EraseIndexVec.push_back(i);
	}
	
	int iCount = 0;
	for (auto& iter = m_TextVec.begin(); iter != m_TextVec.end(); ++iter)
	{
		bool bEraseTiming{ false };
		for(auto& Eraseiter :EraseIndexVec)
		{
			if ((Eraseiter) == iCount)
			{
				bEraseTiming = true;
				break;
			}
		}
		if (bEraseTiming) {
			iter = m_TextVec.erase(iter);
			if (iter == m_TextVec.end())
				break;
		}
		++iCount;
	}

	if (!IsText("Score")) {
		TextInfo tInfo;
		tInfo.strFindTag = "Score";//000000이 여섯개.
		tInfo.vGeometry = XMFLOAT2(500.f, 30.f);
		string strGameScore = to_string(m_iGameScore);

		int iRestZeroCount{ 6 - (int)strGameScore.size() };

		char cAppendData = (char)(0 + 48);
		string strNewData;
		for (int i = 0; i < iRestZeroCount; ++i) 
			strNewData.push_back(cAppendData);
		strNewData += strGameScore;

		tInfo.strText = "Score: " + strNewData;
		m_TextVec.push_back(tInfo);
	}
}

int CTextManager::FindText(string strTag)
{
	int iIndexCount = 0;
	for (auto& iter : m_TextVec)
	{
		if (iter.strFindTag == strTag)
			return iIndexCount;
		++iIndexCount;
	}
	return -1;
}

void CTextManager::UpdateScore()
{
	int iFindIndex{ FindText("Score") };
	if (iFindIndex > 0) {
		string strGameScore = to_string(m_iGameScore);

		int iRestZeroCount{ 6 - (int)strGameScore.size() };

		char cAppendData = (char)(0 + 48);
		string strNewData;
		for (int i = 0; i < iRestZeroCount; ++i)
			strNewData.push_back(cAppendData);
		strNewData += strGameScore;

		m_TextVec[iFindIndex].strText = "Score: " + strNewData;
	}
}

bool CTextManager::IsText(string strTag)
{
	if (m_TextVec.empty())
		return false;

	for (auto& iter : m_TextVec)
	{
		if ((iter).strFindTag == strTag)
			return true;
	}

	return false;
}