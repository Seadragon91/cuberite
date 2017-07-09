
// TradeWindow.cpp

#include "Globals.h"
#include "TradeWindow.h"
#include "SlotArea.h"


cTradeWindow::cTradeWindow(const AString & a_WindowTitle):
	cWindow(wtNPCTrade, a_WindowTitle.empty() ? "Trading": a_WindowTitle)
{
	m_TradeSlotArea = new cSlotAreaTrade(*this);
	m_SlotAreas.push_back(m_TradeSlotArea);
	m_SlotAreas.push_back(new cSlotAreaInventory(*this));
	m_SlotAreas.push_back(new cSlotAreaHotBar(*this));
}





void cTradeWindow::DistributeStack(cItem & a_ItemStack, int a_Slot, cPlayer & a_Player, cSlotArea * a_ClickedArea, bool a_ShouldApply)
{
	cSlotAreas AreasInOrder;

	if (a_ClickedArea == m_SlotAreas[0])
	{
		if (a_Slot == 2)
		{
			// Result slot
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
		}
		else
		{
			AreasInOrder.push_back(m_SlotAreas[1]);  /* Inventory */
			AreasInOrder.push_back(m_SlotAreas[2]);  /* Hotbar    */
		}
	}
	else
	{
		// Inventory or Hotbar
		AreasInOrder.push_back(m_SlotAreas[0]);  /* Trade */
	}
	Super::DistributeStackToAreas(a_ItemStack, a_Player, AreasInOrder, a_ShouldApply, false);
}






bool cTradeWindow::ClosedByPlayer(cPlayer & a_Player, bool a_CanRefuse)
{
	m_OnClosing(a_Player);
	return Super::ClosedByPlayer(a_Player, false);
}
