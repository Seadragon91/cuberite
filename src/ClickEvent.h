#pragma once

class cClickEvent
{
public:
	enum Action {
		OPEN_URL,
		OPEN_FILE,
		RUN_COMMAND,
		SUGGEST_COMMAND,
		NONE,
	};

	Action m_Action = Action::NONE;

	AString m_Text;

	cClickEvent(Action a_Action, const AString & a_Text):
		m_Action(a_Action),
		m_Text(a_Text)
	{}

	~cClickEvent() {}
};

