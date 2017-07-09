#pragma once

#include "Item.h"
#include "UI/TradeWindow.h"
#include "Entities/Player.h"
#include "ClientHandle.h"
#include <iostream>
#include <map>

class cTradeWindow;

// tolua_begin
class cMerchant
{
public:
	// tolua_end

	struct cTradeRecipe
	{
		/** The first input item */
		std::unique_ptr<cItem> m_InputItem1 = cpp14::make_unique<cItem>();

		/** The second input item. By default empty */
		std::unique_ptr<cItem> m_InputItem2 = cpp14::make_unique<cItem>();

		/** The output item */
		std::unique_ptr<cItem> m_OutputItem = cpp14::make_unique<cItem>();

		/** The number of how many times this has been traded */
		size_t m_DoneTrades;

		/** The max amount this can be traded. The default for villager trades is 12 */
		size_t m_MaxTrades = 12;

		/** If true, this trade is disabled */
		bool m_IsDisabled = false;
	};

	typedef std::unique_ptr<cTradeRecipe> TradeRecipePtr;

	// tolua_begin

	/** Creates a instance. The window title is optional, default Trading */
	cMerchant(const AString & a_WindowTitle = "Trading");

	~cMerchant();

	void StartTrade(cPlayer & a_Player);

	bool cMerchant::IsValidInputItem(const cItem & a_ItemStack);

	static bool cMerchant::CheckIfItemsAreEquals(const cItem & a_Item1, const cItem & a_Item2);

	// tolua_end

	void AddTradeRecipe(TradeRecipePtr a_TradeRecipe)
	{
		m_TradeRecipes.emplace_back(std::move(a_TradeRecipe));
	}

	/** Player closed trade window */
	void TradeWindowClosed(void) { m_IsTrading = false; }

	/** Returns true if a player is trading */
	bool IsTrading(void) { return m_IsTrading; }

	/** Contains a list of trades */
	std::vector<TradeRecipePtr> m_TradeRecipes;

	/** Contains the player and the the merchant class */
	static std::map<cPlayer *, cMerchant *> m_PlayerToMerchant;

	/** The selected trade window, at start 0 */
	size_t m_SelectedTradeWindow;

private:
	/** True if a player is trading */
	bool m_IsTrading;

	/** The title of the trade window */
	AString m_WindowTitle;

};  // tolua_export
