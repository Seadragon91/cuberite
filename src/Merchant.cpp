
// Merchant.cpp

#include "Globals.h"
#include "Merchant.h"


std::map<cPlayer *, cMerchant *> cMerchant::m_PlayerToMerchant;

cMerchant::cMerchant(const AString & a_WindowTitle):
	m_IsTrading(false),
	m_WindowTitle(a_WindowTitle)
{
}





cMerchant::~cMerchant()
{
	LOG("Destroy cMerchant...");
	TradeWindowClosed();

	m_TradeRecipes.clear();
}





void cMerchant::StartTrade(cPlayer & a_Player)
{
	if (m_IsTrading)
	{
		// A player is currently trading
		return;
	}

	m_IsTrading = true;

	auto & pos = m_PlayerToMerchant.find(&a_Player);
	if (pos != m_PlayerToMerchant.end())
	{
		// This should not happen
		m_PlayerToMerchant.erase(&a_Player);
		ASSERT(!"cMerchant::StartTrade: Player is in list m_PlayerToMerchant, this should not happen!");
	}

	// Add player and current merchant class
	m_PlayerToMerchant.insert(std::make_pair(&a_Player, this));

	// Reset selected trade window
	m_SelectedTradeWindow = 0;

	cTradeWindow * Window = new cTradeWindow(m_WindowTitle);
	Window->SetOnClosing(
		[=](cPlayer & a_Player) -> void
		{
			// Player closed trade window
			TradeWindowClosed();

			// Remove player from list
			m_PlayerToMerchant.erase(&a_Player);
		}
	);
	a_Player.OpenWindow(*Window);
	a_Player.GetClientHandle()->SendTradeList(Window->GetWindowID(), *this);
}





bool cMerchant::IsValidInputItem(const cItem & a_ItemStack)
{
	// Check if the item is a valid input item
	const auto & tradeRecipe = m_TradeRecipes.at(m_SelectedTradeWindow);

	if (tradeRecipe->m_InputItem2 == nullptr)
	{
		if (!CheckIfItemsAreEquals(*tradeRecipe->m_InputItem1.get(), a_ItemStack))
		{
			return false;
		}
	}
	else
	{
		if (
			!CheckIfItemsAreEquals(*tradeRecipe->m_InputItem1.get(), a_ItemStack) ||
			(!CheckIfItemsAreEquals(*tradeRecipe->m_InputItem2.get(), a_ItemStack))
		)
		{
			return false;
		}
	}
	return true;
}





/** Check if the item type and the item damage are equal */
bool cMerchant::CheckIfItemsAreEquals(const cItem & a_Item1, const cItem & a_Item2)
{
	if (
		(a_Item1.m_ItemType == a_Item2.m_ItemType) &&
		(a_Item1.m_ItemDamage == a_Item2.m_ItemDamage)
	)
	{
		return true;
	}
	return false;
}
