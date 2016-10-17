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

	const AString GetColor(void)
	{	
		if (m_Color == cChatColor::White)
		{
			return AString("white");
		}
		if (m_Color == cChatColor::Black)
		{
			return AString("black");
		}
		if (m_Color == cChatColor::Blue)
		{
			return AString("dark_blue");
		}
		if (m_Color == cChatColor::Green)
		{
			return AString("dark_green");
		}
		if (m_Color == cChatColor::DarkPurple)
		{
			return AString("dark_aqua");
		}
		if (m_Color == cChatColor::Gold)
		{
			return AString("gold");
		}
		if (m_Color == cChatColor::LightGray)
		{
			return AString("gray");
		}
		if (m_Color == cChatColor::Gray)
		{
			return AString("dark_gray");
		}
		if (m_Color == cChatColor::LightBlue)
		{
			return AString("blue");
		}
		if (m_Color == cChatColor::LightGreen)
		{
			return AString("green");
		}
		if (m_Color == cChatColor::Navy)
		{
			return AString("aqua");
		}
		if (m_Color == cChatColor::Red)
		{
			return AString("red");
		}
		if (m_Color == cChatColor::LightPurple)
		{
			return AString("light_purple");
		}
		if (m_Color == cChatColor::Yellow)
		{
			return AString("yellow");
		}
		if (m_Color == cChatColor::Random)
		{
			return AString("obfuscated");
		}
		return AString("white");
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