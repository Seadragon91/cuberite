
// VillagerTradeJobs.h

#pragma once

#include "Villager.h"


class cVillagerTradeJobs
{
public:
	cVillagerTradeJobs();

	~cVillagerTradeJobs();

	/** Loads the json file villager_trade_recipes.txt */
	void LoadTradeJobs();

	/** Contains the villager name and the jobs with the default recipes grouped by their tiers */
	cVillager::MapTiersTradeRecipes GetMapTiersTradeRecipes(cVillager::eCareerType a_Type);

private:
	struct cItemInfo
	{
		/** The item type of the item */
		short m_ItemType;

		/** The damage value of the item */
		short m_ItemDamage;

		/** The fixed amount of the item */
		char m_Amount;

		/** The min amount */
		char m_AmountMin;

		/** The max amount */
		char m_AmountMax;
	};

	struct cRecipeInfo
	{
		/** The first input item, is required */
		std::unique_ptr<cItemInfo> m_Input1;

		/** The second input item, is optional */
		std::unique_ptr<cItemInfo> m_Input2;

		/** The output item, is required */
		std::unique_ptr<cItemInfo> m_Output;
	};

	typedef std::unique_ptr<cRecipeInfo> RecipeInfoPtr;
	typedef std::unique_ptr<std::vector<RecipeInfoPtr>> ListRecipeInfosPtr;
	typedef std::unique_ptr<std::map<size_t, ListRecipeInfosPtr>> MapTierRecipeInfosPtr;

	/** Reads and returns a cItemInfo of the json section, nullptr if a problem has been detected */
	std::unique_ptr<cItemInfo> GetItemInfoFromJson(const Json::Value & a_Value, const AString & a_Path);

	/** Contains the jobs / default trade recipes. Is created from file trade_recipes.json */
	std::map<cVillager::eCareerType, MapTierRecipeInfosPtr> m_MapCareersTradeRecipes;
};
