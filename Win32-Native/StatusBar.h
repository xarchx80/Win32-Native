#pragma once

#include <Control.h>

struct StatusBarPart
{
	LPCSTR code;
	UINT width;
	DWORD flag;
};

class StatusBar : public Control
{
public:
	StatusBar(Wnd* parent);
	
	//bool SetParts(StatusBarPart* data, int len);
	bool SetParts(std::vector<StatusBarPart> data);

	void SetPartText(UINT index, LPCSTR code);
	const char* GetPartText(UINT index);

	virtual LRESULT NotifyReflectEvent(Event &e) override;
	virtual LRESULT DrawItemEvent(DRAWITEMSTRUCT* dip) override;
	virtual LRESULT EraseBkgndEvent(Event &e);

	std::string dummy_str;
	std::vector<const char*> m_parts_text;
};

