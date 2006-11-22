#ifndef YABWIDGET_H
#define YABWIDGET_H

#include <string>

class YabWidget
{
public:
	YabWidget(const char* id)
	{
		id_ = id;
	}
	const char* GetID()
	{
		return id_.c_str();
	}

private:
	std::string id_;
};

#endif
