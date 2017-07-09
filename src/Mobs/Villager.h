
#pragma once

#include "PassiveMonster.h"
#include "../Blocks/ChunkInterface.h"
#include "../Merchant.h"
#include "../Entities/Player.h"
#include "json/json.h"





class cVillager :
	public cPassiveMonster
{
	typedef cPassiveMonster super;

public:
	typedef std::unique_ptr<std::vector<cMerchant::TradeRecipePtr>> ListTradeRecipesPtr;
	typedef std::unique_ptr<std::map<size_t, ListTradeRecipesPtr>> MapTiersTradeRecipes;

	enum eVillagerType
	{
		vtFarmer     = 0,
		vtLibrarian  = 1,
		vtPriest     = 2,
		vtBlacksmith = 3,
		vtButcher    = 4,
		vtGeneric    = 5,
		vtMax
	};

	enum eCareerType
	{
		// Farmer
		ctFarmer,
		ctFisherman,
		ctShepherd,
		ctFletcher,

		// Librarian
		ctLibrarian,
		ctCartographer,

		// Priest
		ctCleric,

		// Blacksmith
		ctArmorer,
		ctWeaponSmith,
		ctToolSmith,

		// Butcher
		ctButcher,
		ctLeatherworker,

		// Generic
		ctGeneric,

		ctUnknown,
	};

	cVillager(eVillagerType VillagerType);

	CLASS_PROTODEF(cVillager)

	// cEntity overrides
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void Tick        (std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;

	// cVillager functions
	/** return true if the given blocktype are: crops, potatoes or carrots. */
	bool IsBlockFarmable(BLOCKTYPE a_BlockType);

	// Farmer functions
	/** Searches in a 11x7x11 area for crops. If it found some it will navigate to them. */
	void HandleFarmerPrepareFarmCrops();

	/** Looks if the farmer has reached it's destination, and if it's still crops and the destination is closer then 2 blocks it will harvest them. */
	void HandleFarmerTryHarvestCrops();

	/** Replaces the crops he harvested. */
	void HandleFarmerPlaceCrops();

	// Get and set functions.
	int      GetVilType(void)              const { return m_VillagerType; }
	Vector3i GetCropsPos(void)             const { return m_CropsPos; }
	bool     DoesHaveActionActivated(void) const { return m_VillagerAction; }

	/** Set the career of the villager, depends on the villager type */
	void SetCareerType(eCareerType a_Type);

	/** Set a randóm career of the villager, depends on the villager type */
	static eCareerType GetRandomCareerType(eVillagerType a_VillagerType);

	/** Checks if the career is valid for the current villager type */
	static bool IsValidCareerTypeForVillager(eVillagerType a_VillagerType, eCareerType a_CareerType);

	/** Returns the career type as number. Used in saving to NBT */
	static int CareerTypeToNBTInt(eCareerType a_CareerType);

	/** Returns the carrer type as string. Used e.g. as title in the trade window */
	static AString CareerTypeToString(eCareerType a_CareerType);

	/** Returns the carrer type from the career name */
	static eCareerType CareerNameToCareerType(const AString & a_CareerName);

	void SetTradeRecipes(MapTiersTradeRecipes a_MapTiersTradeRecipes);

private:
	int m_ActionCountDown;

	/** The type of the villager */
	eVillagerType m_VillagerType;

	bool m_VillagerAction;

	Vector3i m_CropsPos;

	/** The carerr of the villager, depends on the type of the villager */
	eCareerType m_CareerType;

	/** Trade interface */
	std::unique_ptr<cMerchant> m_Merchant;

	/** Contains the trade recipes grouped by the tier */
	MapTiersTradeRecipes m_MapTiersTradeRecipes;
};
