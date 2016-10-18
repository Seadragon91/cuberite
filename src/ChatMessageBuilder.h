#pragma once

#include "ChatColor.h"
#include "ChatMessagePart.h"
#include "ClickEvent.h"
#include "HoverEvent.h"
#include "json/json.h"

class cChatMessageBuilder
{
public:
	cChatMessageBuilder(const AString & a_Text);

	~cChatMessageBuilder() {}

	cChatMessageBuilder * Append(const AString & a_Text);

	cChatMessageBuilder * Color(const char * a_Color);

	cChatMessageBuilder * Bold(bool a_Bold);

	cChatMessageBuilder * Italic(bool a_Italic);

	cChatMessageBuilder * Underlined(bool a_Underlined);

	cChatMessageBuilder * Striketrough(bool a_Strikethrough);

	cChatMessageBuilder * Obfuscated(bool a_Obfuscated);

	cChatMessageBuilder * Insertion(const AString & a_Insertion);

	cChatMessageBuilder * Click(cClickEvent::Action a_Action, const AString & a_Text);

	cChatMessageBuilder * Hover(cHoverEvent::Action a_Action, cChatMessageBuilder * a_ChatMessageBuilder);

	const AString CreateJsonString(bool a_ShouldUseChatPrefixes) const;

	typedef std::unique_ptr<cChatMessagePart> ChatMessagePartPtr;

	std::vector<ChatMessagePartPtr> & GetParts(void)
	{
		return m_Parts;
	}

private:
	void CreateCurrentPart(cChatMessagePart & a_ChatMessagePart, Json::Value & a_Value) const;

	void AddClickPart(cChatMessagePart & a_ChatMessagePart, Json::Value & a_Value) const;

	void AddHoverPart(cChatMessagePart & a_ChatMessagePart, Json::Value & a_Value) const;

	/** Contains all parts for the chat message. */
	std::vector<ChatMessagePartPtr> m_Parts;

	/** Current chat message part */
	ChatMessagePartPtr m_Current;
};

