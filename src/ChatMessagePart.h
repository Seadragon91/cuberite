#pragma once

// #include "ClickEvent.h"
// #include "HoverEvent.h"

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
		Ret.append(m_Text);
		return Ret;
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

	// /** Class for the click event. */
	// std::unique_ptr<cClickEvent> m_ClickEvent;
	// 
	// /** Class for the hover event. */
	// std::unique_ptr<cHoverEvent> m_HoverEvent;
};