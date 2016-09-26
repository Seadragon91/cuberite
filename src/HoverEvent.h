#pragma once

#include "Globals.h"
#include "ChatMessagePart.h"
#include "ChatMessageBuilder.h"

class cHoverEvent
{
public:
	enum Action {
		SHOW_TEXT,
		SHOW_ACHIEVEMENT,
		SHOW_ITEM,
		NONE,
	};

	Action m_Action = Action::NONE;

	cHoverEvent(Action a_Action, std::unique_ptr<cChatMessageBuilder> & a_ChatMessageBuilder) :
		m_Action(a_Action)
	{
		m_ChatMessageBuilder = std::move(a_ChatMessageBuilder);
	}

	~cHoverEvent() {}

	AString CreateText()
	{
		m_ChatMessageBuilder->m_Parts.push_back(m_ChatMessageBuilder->m_Current.release());

		AString Ret;
		for each(auto & HoverPart in m_ChatMessageBuilder->m_Parts)
		{
			Ret.append(HoverPart->CreateMessage());
		}
		return Ret;
	}

private:
	std::unique_ptr<cChatMessageBuilder> m_ChatMessageBuilder = cpp14::make_unique<cChatMessageBuilder>();
};
