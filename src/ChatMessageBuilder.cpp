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


cChatMessageBuilder * cChatMessageBuilder::Click(cClickEvent::Action a_Action, const AString & a_Text)
{
	m_Current->m_ClickEvent.reset(new cClickEvent(a_Action, a_Text));
	return this;
}


cChatMessageBuilder * cChatMessageBuilder::Hover(cHoverEvent::Action a_Action, cChatMessageBuilder * a_ChatMessageBuilder)
{
	m_Current->m_HoverEvent.reset(new cHoverEvent(a_Action, a_ChatMessageBuilder));
	return this;
}



void cChatMessageBuilder::AddClickPart(cChatMessagePart & a_ChatMessagePart, Json::Value & a_Value) const
{
	Json::Value Click;
	switch (a_ChatMessagePart.m_ClickEvent->m_Action)
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
	Click["value"] = a_ChatMessagePart.m_ClickEvent->m_Text;
	a_Value["clickEvent"] = Click;
}



void cChatMessageBuilder::AddHoverPart(cChatMessagePart & a_ChatMessagePart, Json::Value & a_Value) const
{
	Json::Value Hover;
	switch (a_ChatMessagePart.m_HoverEvent->m_Action)
	{
		case cHoverEvent::Action::SHOW_TEXT:
			Hover["action"] = "show_text";
			break;
		case cHoverEvent::Action::SHOW_ITEM:
			Hover["action"] = "show_item";
			break;
	}
	Hover["value"] = a_ChatMessagePart.m_HoverEvent->CreateText();
	a_Value["hoverEvent"] = Hover;
}




void cChatMessageBuilder::CreateCurrentPart(cChatMessagePart & a_ChatMessagePart, Json::Value & a_Value) const
{
	// Set text
	a_Value["text"] = a_ChatMessagePart.m_Text;

	// Set color
	a_Value["color"] = a_ChatMessagePart.GetColor();

	// Set style
	a_Value["bold"] = a_ChatMessagePart.m_Bold;
	a_Value["italic"] = a_ChatMessagePart.m_Italic;
	a_Value["underlined"] = a_ChatMessagePart.m_Underlined;
	a_Value["strikethrough"] = a_ChatMessagePart.m_Strikethrough;
	a_Value["obfuscated"] = a_ChatMessagePart.m_Obfuscated;

	// Add click part
	if (a_ChatMessagePart.m_ClickEvent->m_Action != cClickEvent::Action::NONE)
	{
		AddClickPart(a_ChatMessagePart, a_Value);
	}

	// Add hover part
	if (a_ChatMessagePart.m_HoverEvent->m_Action != cHoverEvent::Action::NONE)
	{
		AddHoverPart(a_ChatMessagePart, a_Value);
	}

	// Set insertion text
	if (!a_ChatMessagePart.m_Insertion.empty())
	{
		a_Value["insertion"] = a_ChatMessagePart.m_Insertion;
	}
}




const AString cChatMessageBuilder::CreateJsonString(bool a_ShouldUseChatPrefixes) const
{
	Json::Value Messages;
	Messages[""] = Json::Value(Json::arrayValue);

	for (auto & ChatMessagePart : m_Parts)
	{
		Json::Value Part;
		CreateCurrentPart(*ChatMessagePart.get(), Part);
		Messages[""].append(Part);
	}

	// Add the current part
	Json::Value Part;
	CreateCurrentPart(*m_Current.get(), Part);
	Messages[""].append(Part);

	return Messages.toStyledString();
}
