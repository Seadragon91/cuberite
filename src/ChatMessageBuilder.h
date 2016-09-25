#pragma once

#include "ClickEvent.h"
#include "HoverEvent.h"
#include "ChatColor.h"
#include "json/json.h"

class cClickEvent;
class cHoverEvent;

class cChatMessageBuilder
{
public:
	cChatMessageBuilder(const AString & a_Text = "");

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

	cChatMessageBuilder * Hover(cHoverEvent::Action a_Action, std::unique_ptr<cChatMessageBuilder> a_ChatMessageBuilder);

	AString CreateMessage();

	AString CreateJsonString();

	class cChatMessagePart
	{
	public:
		cChatMessagePart(const AString & a_Text = "") :
			m_Text(a_Text),
			m_Color(cChatColor::White),
			m_Bold(false),
			m_Italic(false),
			m_Underlined(false),
			m_Strikethrough(false),
			m_Obfuscated(false)
		{}
		~cChatMessagePart() {}

		AString CreateMessage(void)
		{
			if (m_Text.empty())
				return "";

			AString Ret;
			if (m_Obfuscated)
				Ret.append("§k");

			if (m_Bold)
				Ret.append("§l");

			if (m_Strikethrough)
				Ret.append("§m");

			if (m_Underlined)
				Ret.append("§n");

			if (m_Italic)
				Ret.append("§o");

			Ret.append(m_Text);
		}

		/** The text of the message. */
		AString m_Text;

		/** Color of the text. */
		const char * m_Color;

		/** Text is bold. */
		bool m_Bold;

		/** Text is italic. */
		bool m_Italic;

		/** Text is underlined. */
		bool m_Underlined;

		/** Text is strikethrough. */
		bool m_Strikethrough;

		/** Text is obfuscated. */
		bool m_Obfuscated;

		/** Text to insert. */
		AString m_Insertion;

		/** Class for the click event. */
		std::unique_ptr<cClickEvent> m_ClickEvent;

		/** Class for the hover event. */
		std::unique_ptr<cHoverEvent> m_HoverEvent;
	};

	/** Contains all parts for the chat message. */
	std::vector<cChatMessagePart *> m_Parts;

	std::unique_ptr<cChatMessagePart> m_Current;

};

