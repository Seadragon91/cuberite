
// TradeWindow.h

// Representing the UI window for trading


#pragma once

#include "Window.h"





class cTradeWindow:
	public cWindow
{
	typedef cWindow Super;
public:
	/** Creates a trade window. The window title is optional, default Trading */
	cTradeWindow(const AString & a_WindowTitle = "");

	virtual void DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply) override;

	bool ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse) override;

	/** Sets callback function to call when the window is about to close */
	void SetOnClosing(std::function<void(cPlayer & a_Player)> && a_OnClosing) { m_OnClosing = std::move(a_OnClosing); }

protected:
	cSlotAreaTrade * m_TradeSlotArea;
private:
	std::function<void(cPlayer & a_Player)> m_OnClosing;
};
