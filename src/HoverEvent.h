#pragma once

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

	cHoverEvent(Action a_Action, cChatMessageBuilder a_ChatMessageBuilder) :
		m_Action(a_Action),
		m_ChatMessageBuilder(cpp14::make_unique<cChatMessageBuilder>(a_ChatMessageBuilder))
	{
		
	}

	~cHoverEvent() {}

	AString CreateText()
	{
		// m_ChatMessageBuilder->AddLastOne();
		// 
		// AString Ret;
		// for (auto & HoverPart : m_ChatMessageBuilder->GetParts())
		// {
		// 	Ret.append(HoverPart->CreateMessage());
		// }
		return "";
	}

private:
	std::unique_ptr<cChatMessageBuilder> m_ChatMessageBuilder;
};
