#pragma once

class CTextManager
{
public:
	struct TextInfo {
		string strText;
		XMFLOAT2 vGeometry = { 30.f, 130.f };
		float fLifeTime = 0.f;
		string strFindTag;
	};

public:
	static CTextManager* GetInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CTextManager;

		return m_pInstance;
	}
	static void DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:

	wstring s2ws(const string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}

	bool IsText(string strTag);
	int FindText(string strTag);
	void AddText(TextInfo tText) { m_TextVec.push_back(tText); }
	void AddScore(int iAddScore) { m_iGameScore += iAddScore; UpdateScore(); }
	int GetScore() { return m_iGameScore; }
	
	void ClearTextVec() { m_TextVec.clear(); }
	void UpdateText(float fTimeDelta);

	const vector<TextInfo> GetTextVector() { return m_TextVec; }

private:
	float Lerp(float A, float B, float Alpha)
	{
		return A * (1 - Alpha) + B * Alpha;
	}
	void UpdateScore();

private:
	CTextManager();
	~CTextManager();

private:
	static CTextManager* m_pInstance;
	vector<TextInfo> m_TextVec;

	int m_iGameScore = 0;
};
