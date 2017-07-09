
// VillagerTradeJobs.cpp

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "VillagerTradeJobs.h"





cVillagerTradeJobs::cVillagerTradeJobs()
{
}





cVillagerTradeJobs::~cVillagerTradeJobs()
{
}





void cVillagerTradeJobs::LoadTradeJobs()
{
	const AString content = cFile::ReadWholeFile("villager_trade_recipes.txt");
	if (content.empty())
	{
		return;
	}

	Json::Value root;
	Json::Reader reader;

	// Jsoncpp doesn't abort parsing if a key already exists and there is no setting to disallow duplicated keys

	if (!reader.parse(content, root, false))
	{
		LOGWARNING("Parsing Json failed: %s", reader.getFormattedErrorMessages().c_str());
		return;
	}

	for (const auto & villagerName : root.getMemberNames())
	{
		// Check if the villagerName is already in list
		cVillager::eCareerType careerType = cVillager::CareerNameToCareerType(villagerName);
		if (careerType == cVillager::eCareerType::ctUnknown)
		{
			LOGWARNING("The villager name %s is invalid.", villagerName.c_str());
			return;
		}

		MapTierRecipeInfosPtr mapTierRecipeInfos(cpp14::make_unique<std::map<size_t, ListRecipeInfosPtr>>());

		for (size_t iTier = 1; iTier <= root[villagerName].size(); iTier++)
		{
			const AString sTier("tier_" + std::to_string(iTier));

			if (!root[villagerName].isMember(sTier))
			{
				LOGWARNING("Didn't found %s under %s", sTier.c_str(), villagerName.c_str());
				return;
			}

			// Contains the recipe infos for the current tier
			ListRecipeInfosPtr listRecipeInfos(cpp14::make_unique<std::vector<RecipeInfoPtr>>());

			// Tier exists loop over it
			for (Json::Value::ArrayIndex i = 0; i != root[villagerName][sTier].size(); i++)
			{
				const Json::Value & items = root[villagerName][sTier][i];
				AString path = villagerName + "." + sTier.c_str() + "." + std::to_string(i);

				// Check if Input1 exists
				if (!items.isMember("Input1"))
				{
					LOGWARNING("%s: Missing required key Input1.", path.c_str());
					return;
				}

				// Check if Output exists
				if (!items.isMember("Output"))
				{
					LOGWARNING("%s: Missing required key Output.", path.c_str());
					return;
				}

				std::unique_ptr<cItemInfo> input1 = GetItemInfoFromJson(items["Input1"], AString(path).append(".Input1"));
				if (input1 == nullptr)
				{
					return;
				}

				std::unique_ptr<cItemInfo> output = GetItemInfoFromJson(items["Output"], AString(path).append(".Output"));
				if (output == nullptr)
				{
					return;
				}

				// The second input item is optional
				std::unique_ptr<cItemInfo> input2;
				if (items.isMember("Input2"))
				{
					input2 = GetItemInfoFromJson(items["Input2"], AString(path).append(".Input2"));
					if (input2 == nullptr)
					{
						// Input2 exists but is invalid, bail out
						return;
					}
				}

				RecipeInfoPtr recipeInfo(cpp14::make_unique<cRecipeInfo>());
				recipeInfo->m_Input1 = std::move(input1);
				recipeInfo->m_Output = std::move(output);
				if (input2 != nullptr)
				{
					recipeInfo->m_Input2 = std::move(input2);
				}

				// Add recipeInfo to list
				listRecipeInfos->emplace_back(std::move(recipeInfo));

			}  // List of items

			// Add tier to list of tiers
			mapTierRecipeInfos->insert(std::make_pair(iTier, std::move(listRecipeInfos)));

		}  // tier_X

		// Add villagerTradeJobs to list
		m_MapCareersTradeRecipes.insert(std::make_pair(careerType, std::move(mapTierRecipeInfos)));

	}  // villagerName
}





std::unique_ptr<cVillagerTradeJobs::cItemInfo> cVillagerTradeJobs::GetItemInfoFromJson(const Json::Value & a_Value, const AString & a_Path)
{
	const char * path = a_Path.c_str();

	// Check if item name exists
	if (!a_Value.isMember("ItemName"))
	{
		LOGWARNING("%s: The key ItemName is missing.", path);
		return nullptr;
	}

	if (!a_Value["ItemName"].isString())
	{
		LOGWARNING("%s.ItemName: The value is not a string.", path);
		return nullptr;
	}
	const AString & itemName = a_Value["ItemName"].asString();

	// Check if the item name is valid
	cItem item;
	if (!StringToItem(itemName, item))
	{
		LOGWARNING("%s.ItemName: The value %s is not a valid item name.", path, itemName.c_str());
		return nullptr;
	}

	std::unique_ptr<cItemInfo> itemInfo(cpp14::make_unique<cItemInfo>());
	itemInfo->m_ItemType = item.m_ItemType;

	// Check for ItemDamage
	if (a_Value.isMember("ItemDamage"))
	{
		if (!a_Value["ItemDamage"].isUInt())
		{
			LOGWARNING("%s.ItemDamage: The value is not a positive number.", path);
			return nullptr;
		}
		size_t itemDamage = a_Value["ItemDamage"].asUInt();
		if (itemDamage > 32767)
		{
			LOGWARNING("%s.ItemDamage: The max value can only be 32767.", path);
			return nullptr;
		}
		itemInfo->m_ItemDamage = static_cast<short>(itemDamage);
	}

	// Get the max stack size for the item
	size_t maxStackSize = static_cast<size_t>(item.GetMaxStackSize());

	// Check if Amount exists
	bool amountExists = false;
	if (a_Value.isMember("Amount"))
	{
		if (!a_Value["Amount"].isUInt())
		{
			LOGWARNING("%s.Amount: The value is not a positive number.", path);
			return nullptr;
		}
		amountExists = true;
		size_t amount = a_Value["Amount"].asUInt();
		if (amount > maxStackSize)
		{
			LOGWARNING("%s.Amount: The number is larger than the allowed stack size %zu.", path, maxStackSize);
			return nullptr;
		}
		itemInfo->m_Amount = static_cast<char>(amount);
	}

	// Check if AmountMin and AmountMax exists
	if (a_Value.isMember("AmountMin") && a_Value.isMember("AmountMax"))
	{
		if (amountExists)
		{
			LOGWARNING("%s: Found Amount, AmountMin and AmountMax. This is not allowed.", path);
			return nullptr;
		}

		// Check if the values are numbers
		if (!a_Value["AmountMin"].isUInt())
		{
			LOGWARNING("%s.AmountMin: The value is not a positive number.", path);
			return nullptr;
		}
		if (!a_Value["AmountMax"].isUInt())
		{
			LOGWARNING("%s.AmountMax: The value is not a positive number.", path);
			return nullptr;
		}

		// Check if the numbers are larger than the stacksize
		size_t amountMin = a_Value["AmountMin"].asUInt();
		if (amountMin > maxStackSize)
		{
			LOGWARNING("%s.AmountMin: The number is larger than the allowed stack size %zu", path, maxStackSize);
			return nullptr;
		}
		size_t amountMax = a_Value["AmountMax"].asUInt();
		if (amountMax > maxStackSize)
		{
			LOGWARNING("%s.AmountMax: The number is larger than the allowed stack size %zu", path, maxStackSize);
			return nullptr;
		}

		if (amountMin > amountMax)
		{
			LOGWARNING("%s: The numer from AmountMin is larger than AmountMax.", path);
			return nullptr;
		}

		itemInfo->m_AmountMin = static_cast<char>(amountMin);
		itemInfo->m_AmountMax = static_cast<char>(amountMax);
		amountExists = true;
	}

	if (!amountExists)
	{
		itemInfo->m_Amount = 1;
	}
	return itemInfo;
}





cVillager::MapTiersTradeRecipes cVillagerTradeJobs::GetMapTiersTradeRecipes(cVillager::eCareerType a_Type)
{
	auto pos = m_MapCareersTradeRecipes.find(a_Type);
	if (pos == m_MapCareersTradeRecipes.end())
	{
		return nullptr;
	}

	cVillager::MapTiersTradeRecipes mapTiersTradeRecipes(cpp14::make_unique<std::map<size_t, cVillager::ListTradeRecipesPtr>>());

	for (size_t iTier = 1; iTier <= pos->second->size(); iTier++)
	{
		auto & listRecipeInfos = pos->second;
		cVillager::ListTradeRecipesPtr listTradeRecipes(cpp14::make_unique<std::vector<cMerchant::TradeRecipePtr>>());

		for (const auto & recipeInfo : *listRecipeInfos->at(iTier))
		{
			// Set first input item
			cMerchant::TradeRecipePtr tradeRecipe(cpp14::make_unique<cMerchant::cTradeRecipe>());
			auto & input1 = recipeInfo->m_Input1;
			char amount = input1->m_Amount;
			if (input1->m_Amount == 0)
			{
				amount = static_cast<char>(GetRandomProvider().RandInt(input1->m_AmountMin, input1->m_AmountMax));
			}
			tradeRecipe->m_InputItem1 = cpp14::make_unique<cItem>(input1->m_ItemType, amount, input1->m_ItemDamage);

			// Set second input item
			if (recipeInfo->m_Input2 != nullptr)
			{
				auto & input2 = recipeInfo->m_Input2;
				amount = input2->m_Amount;
				if (input2->m_Amount == 0)
				{
					amount = static_cast<char>(GetRandomProvider().RandInt(input2->m_AmountMin, input2->m_AmountMax));
				}
				tradeRecipe->m_InputItem2 = cpp14::make_unique<cItem>(input2->m_ItemType, amount, input2->m_ItemDamage);
			}

			// Set output item
			auto & output = recipeInfo->m_Output;
			amount = output->m_Amount;
			if (output->m_Amount == 0)
			{
				amount = static_cast<char>(GetRandomProvider().RandInt(output->m_AmountMin, output->m_AmountMax));
			}
			tradeRecipe->m_OutputItem = cpp14::make_unique<cItem>(output->m_ItemType, amount, output->m_ItemDamage);

			// Add tradeRecipe to list
			listTradeRecipes->emplace_back(std::move(tradeRecipe));
		}

		// Add list to map
		mapTiersTradeRecipes->insert(std::make_pair(iTier, std::move(listTradeRecipes)));
	}

	return mapTiersTradeRecipes;
}
