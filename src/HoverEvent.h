#pragma once

#include "ChatMessageBuilder.h"

class cChatMessageBuilder;

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

	cHoverEvent(Action a_Action, std::unique_ptr<cChatMessageBuilder> a_ChatMessageBuilder) :
		m_Action(a_Action),
		m_HoverMessageBuilder(std::move(a_ChatMessageBuilder))
	{}

	~cHoverEvent() {}

	AString CreateText()
	{
		m_HoverMessageBuilder->m_Parts.push_back(m_HoverMessageBuilder->m_Current.release());

		AString Ret;
		for each(auto & HoverPart in m_HoverMessageBuilder->m_Parts)
		{
			Ret.append(HoverPart->CreateMessage());
		}
		return Ret;
	}

	std::unique_ptr<cChatMessageBuilder> m_HoverMessageBuilder = cpp14::make_unique<cChatMessageBuilder>();
};
