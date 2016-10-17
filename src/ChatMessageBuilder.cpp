
#include "Globals.h"
#include "ChatMessageBuilder.h"

cChatMessageBuilder::cChatMessageBuilder(const AString & a_Text):
	m_Current(cpp14::make_unique<cChatMessagePart>(a_Text))
{
}


cChatMessageBuilder * cChatMessageBuilder::Append(const AString & a_Text = "")
{
	m_Parts.push_back(std::move(m_Current));
	m_Current.reset(new cChatMessagePart(a_Text));
	return this;
}


cChatMessageBuilder * cChatMessageBuilder::Color(const char * a_Color)
{
	m_Current->m_Color = a_Color;
	return this;
}


cChatMessageBuilder * cChatMessageBuilder::Bold(bool a_Bold)
{
	m_Current->m_Bold = a_Bold;
	return this;
}


cChatMessageBuilder * cChatMessageBuilder::Italic(bool a_Italic)
{
	m_Current->m_Italic = a_Italic;
	return this;
}


cChatMessageBuilder * cChatMessageBuilder::Underlined(bool a_Underlined)
{
	m_Current->m_Underlined = a_Underlined;
	return this;
}


cChatMessageBuilder * cChatMessageBuilder::Striketrough(bool a_Strikethrough)
{
	m_Current->m_Strikethrough = a_Strikethrough;
	return this;
}


cChatMessageBuilder * cChatMessageBuilder::Obfuscated(bool a_Obfuscated)
{
	m_Current->m_Obfuscated = a_Obfuscated;
	return this;
}


cChatMessageBuilder * cChatMessageBuilder::Insertion(const AString & a_Insertion)
{
	m_Current->m_Insertion = a_Insertion;
	return this;
}


// cChatMessageBuilder * cChatMessageBuilder::Click(cClickEvent::Action a_Action, const AString & a_Text)
// {
// 	m_Current->m_ClickEvent.reset(&cClickEvent(a_Action, a_Text));
// 	return this;
// }
// 
// 
// cChatMessageBuilder * cChatMessageBuilder::Hover(cHoverEvent::Action a_Action, cChatMessageBuilder * a_ChatMessageBuilder)
// {
// 	m_Current->m_HoverEvent.reset(&cHoverEvent(a_Action, a_ChatMessageBuilder));
// 	return this;
// }




AString cChatMessageBuilder::CreateJsonString(bool a_ShouldUseChatPrefixes) const
{
	Json::Value Msg;
	for (auto & ChatMessagePart : m_Parts)
	{
		// Append text
		Msg.append(ChatMessagePart->CreateMessage());

		// Set color
		Msg["color"] = ChatMessagePart->GetColor();

		// Set style
		Msg["bold"] = ChatMessagePart->m_Bold;
		Msg["italic"] = ChatMessagePart->m_Italic;
		Msg["underlined"] = ChatMessagePart->m_Underlined;
		Msg["strikethrough"] = ChatMessagePart->m_Strikethrough;
		Msg["obfuscated"] = ChatMessagePart->m_Obfuscated;

		/*if (ChatMessagePart->m_ClickEvent->m_Action != cClickEvent::Action::NONE)
		{
			Json::Value Click;
			switch (ChatMessagePart->m_ClickEvent->m_Action)
			{
				case cClickEvent::Action::OPEN_FILE:
					Click["action"] = "open_file";
					break;
				case cClickEvent::Action::OPEN_URL:
					Click["action"] = "open_url";
					break;
				case cClickEvent::Action::RUN_COMMAND:
					Click["action"] = "run_command";
					break;
				case cClickEvent::Action::SUGGEST_COMMAND:
					Click["action"] = "suggest_command";
					break;
			}
			Click["value"] = ChatMessagePart->m_ClickEvent->m_Text;
			Msg["clickEvent"] = Click;
		}

		if (ChatMessagePart->m_HoverEvent->m_Action != cHoverEvent::Action::NONE)
		{
			Json::Value Hover;
			switch (ChatMessagePart->m_HoverEvent->m_Action)
			{
				case cHoverEvent::Action::SHOW_TEXT:
					Hover["action"] = "show_text";
					break;

				case cHoverEvent::Action::SHOW_ITEM:
					Hover["action"] = "show_item";
					break;
			}
			Hover["value"] = ChatMessagePart->m_HoverEvent->CreateText();
			Msg["hoverEvent"] = Hover;
		}

		if (!ChatMessagePart->m_Insertion.empty())
		{
			Msg["insertion"] = ChatMessagePart->m_Insertion;
		}*/
	}

	// Add current part

	// Append text
	Msg.append(m_Current->CreateMessage());

	// Set color
	Msg["color"] = m_Current->GetColor();

	// Set style
	Msg["bold"] = m_Current->m_Bold;
	Msg["italic"] = m_Current->m_Italic;
	Msg["underlined"] = m_Current->m_Underlined;
	Msg["strikethrough"] = m_Current->m_Strikethrough;
	Msg["obfuscated"] = m_Current->m_Obfuscated;

	return Msg.toStyledString();
}
