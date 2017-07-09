
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Villager.h"
#include "../World.h"
#include "../BlockArea.h"
#include "../Blocks/BlockHandler.h"
#include "../BlockInServerPluginInterface.h"
#include "../Root.h"
#include "VillagerTradeJobs.h"




cVillager::cVillager(eVillagerType a_VillagerType) :
	super("Villager", mtVillager, "entity.villager.hurt", "entity.villager.death", 0.6, 1.8),
	m_ActionCountDown(-1),
	// m_VillagerType(a_VillagerType),
	m_VillagerType(vtFarmer),  // Harcoded for easier testing
	m_VillagerAction(false),
	// m_CareerType(GetRandomCareerType(m_VillagerType)),
	m_CareerType(ctFarmer),  // Harcoded for easier testing
	m_Merchant(cpp14::make_unique<cMerchant>(CareerTypeToString(m_CareerType)))
{
	LOG("m_CareerType = %d", m_CareerType);

	auto mapTiersTradeRecipes = cRoot::Get()->GetVillagerTradeJobs()->GetMapTiersTradeRecipes(m_CareerType);
	if (mapTiersTradeRecipes != nullptr)
	{
		m_MapTiersTradeRecipes = std::move(mapTiersTradeRecipes);

		// Add trade recipes from tier 1
		if (m_MapTiersTradeRecipes->find(1) == m_MapTiersTradeRecipes->end())
		{
			// WTF
			return;
		}

		for (size_t i = 0; i < m_MapTiersTradeRecipes->at(1)->size(); i++)
		{
			m_Merchant->AddTradeRecipe(std::move(m_MapTiersTradeRecipes->at(1)->at(i)));
		}

		// Content for tier 1 has been moved, remove the key
		m_MapTiersTradeRecipes->erase(1);
	}
}





bool cVillager::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!super::DoTakeDamage(a_TDI))
	{
		return false;
	}

	if ((a_TDI.Attacker != nullptr) && a_TDI.Attacker->IsPlayer())
	{
		if (GetRandomProvider().RandBool(1.0 / 6.0))
		{
			m_World->BroadcastEntityStatus(*this, esVillagerAngry);
		}
	}

	if (a_TDI.DamageType == dtLightning)
	{
		Destroy();
		m_World->SpawnMob(GetPosX(), GetPosY(), GetPosZ(), mtWitch, false);
		return true;
	}
	return true;
}





void cVillager::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (m_Merchant->IsTrading())
	{
		StopMovingToPosition();  // Needs to be replaced with a better idea.
								 // Villager should not move, but without calling the super tick the villager could be already dead.
		return;
	}

	if (m_ActionCountDown > -1)
	{
		m_ActionCountDown--;
		if (m_ActionCountDown == 0)
		{
			if (m_VillagerType == vtFarmer)
			{
				HandleFarmerPlaceCrops();
			}
		}
		return;
	}

	if (m_VillagerAction)
	{
		if (m_VillagerType == vtFarmer)
		{
			HandleFarmerPlaceCrops();
		}
		m_VillagerAction = false;
		return;
	}

	// Don't always try to do a special action. Each tick has 1% to do a special action.
	if (GetRandomProvider().RandBool(0.99))
	{
		return;
	}

	if (m_VillagerType == vtFarmer)
	{
		HandleFarmerPrepareFarmCrops();
	}
}




////////////////////////////////////////////////////////////////////////////////
// Farmer functions:

void cVillager::HandleFarmerPrepareFarmCrops()
{
	if (!m_World->VillagersShouldHarvestCrops())
	{
		return;
	}

	cBlockArea Surrounding;

	// Read a 11x7x11 area:
	Surrounding.Read(
		*m_World,
		FloorC(GetPosX()) - 5,
		FloorC(GetPosX()) + 6,
		FloorC(GetPosY()) - 3,
		FloorC(GetPosY()) + 4,
		FloorC(GetPosZ()) - 5,
		FloorC(GetPosZ()) + 6
	);

	for (int I = 0; I < 5; I++)
	{
		for (int Y = 0; Y < 6; Y++)
		{
			// Pick random coordinates and check for crops.
			int X = m_World->GetTickRandomNumber(11);
			int Z = m_World->GetTickRandomNumber(11);

			// A villager can't farm this.
			if (!IsBlockFarmable(Surrounding.GetRelBlockType(X, Y, Z)))
			{
				continue;
			}
			if (Surrounding.GetRelBlockMeta(X, Y, Z) != 0x7)
			{
				continue;
			}

			m_VillagerAction = true;
			m_CropsPos = Vector3i(static_cast<int>(GetPosX()) + X - 5, static_cast<int>(GetPosY()) + Y - 3, static_cast<int>(GetPosZ()) + Z - 5);
			MoveToPosition(Vector3d(m_CropsPos.x + 0.5, m_CropsPos.y + 0.0, m_CropsPos.z + 0.5));
			return;
		}  // for Y loop.
	}  // Repeat the procces 5 times.
}





void cVillager::HandleFarmerTryHarvestCrops()
{
	// Harvest the crops if the villager isn't moving and if the crops are closer then 2 blocks.
	if (!m_PathfinderActivated && (GetPosition() - m_CropsPos).Length() < 2)
	{
		// Check if the blocks didn't change while the villager was walking to the coordinates.
		BLOCKTYPE CropBlock = m_World->GetBlock(m_CropsPos.x, m_CropsPos.y, m_CropsPos.z);
		if (IsBlockFarmable(CropBlock) && m_World->GetBlockMeta(m_CropsPos.x, m_CropsPos.y, m_CropsPos.z) == 0x7)
		{
			cBlockHandler * Handler = cBlockInfo::GetHandler(CropBlock);
			cChunkInterface ChunkInterface(m_World->GetChunkMap());
			cBlockInServerPluginInterface PluginInterface(*m_World);
			Handler->DropBlock(ChunkInterface, *m_World, PluginInterface, this, m_CropsPos.x, m_CropsPos.y, m_CropsPos.z);
			m_World->SetBlock(m_CropsPos.x, m_CropsPos.y, m_CropsPos.z, E_BLOCK_AIR, 0);
			m_ActionCountDown = 20;
		}
	}
}





void cVillager::HandleFarmerPlaceCrops()
{
	// Check if there is still farmland at the spot where the crops were.
	if (m_World->GetBlock(m_CropsPos.x, m_CropsPos.y - 1, m_CropsPos.z) == E_BLOCK_FARMLAND)
	{
		m_World->SetBlock(m_CropsPos.x, m_CropsPos.y, m_CropsPos.z, E_BLOCK_CROPS, 0);
	}
}





bool cVillager::IsBlockFarmable(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_BEETROOTS:
		case E_BLOCK_CROPS:
		case E_BLOCK_POTATOES:
		case E_BLOCK_CARROTS:
		{
			return true;
		}
	}
	return false;
}






cVillager::eCareerType cVillager::GetRandomCareerType(eVillagerType a_VillagerType)
{
	cFastRandom Rand;

	switch (a_VillagerType)
	{
		case vtFarmer:
		{
			return static_cast<eCareerType>(Rand.RandInt(0, 3));
		}
		case vtLibrarian:
		{
			return static_cast<eCareerType>(Rand.RandInt(4, 5));
		}
		case vtPriest:
		{
			return ctCleric;
		}
		case vtBlacksmith:
		{
			return static_cast<eCareerType>(Rand.RandInt(7, 9));
		}
		case vtButcher:
		{
			return static_cast<eCareerType>(Rand.RandInt(10, 11));
		}
		case vtGeneric:
		{
			return ctGeneric;
		}
		default:
		{
			return ctGeneric;
		}
	}
}





bool cVillager::IsValidCareerTypeForVillager(eVillagerType a_VillagerType, eCareerType a_Career)
{
	switch (a_VillagerType)
	{
		case vtFarmer:
		{
			return ((a_Career >= 0) && (a_Career <= 3));
		}
		case vtLibrarian:
		{
			return ((a_Career == 4) || (a_Career == 5));
		}
		case vtPriest:
		{
			return a_Career == ctCleric;
		}
		case vtBlacksmith:
		{
			return ((a_Career >= 7) && (a_Career <= 9));
		}
		case vtButcher:
		{
			return ((a_Career == 10) || (a_Career == 11));
		}
		case vtGeneric:
		{
			return a_Career == ctGeneric;
		}
		default:
		{
			return false;
		}
	}
}





void cVillager::SetCareerType(eCareerType a_Career)
{
	if (IsValidCareerTypeForVillager(m_VillagerType, a_Career))
	{
		m_CareerType = a_Career;
	}
}





int cVillager::CareerTypeToNBTInt(eCareerType a_CareerType)
{
	switch (a_CareerType)
	{
		// Farmer
		case ctFarmer:
		{
			return 1;
		}
		case ctFisherman:
		{
			return 2;
		}
		case ctShepherd:
		{
			return 3;
		}
		case ctFletcher:
		{
			return 4;
		}

		// Librarian
		case ctLibrarian:
		{
			return 1;
		}
		case ctCartographer:
		{
			return 2;
		}

		// Priest
		case ctCleric:
		{
			return 1;
		}

		// Blacksmith
		case ctArmorer:
		{
			return 1;
		}
		case ctWeaponSmith:
		{
			return 2;
		}
		case ctToolSmith:
		{
			return 3;
		}

		// Butcher
		case ctButcher:
		{
			return 1;
		}
		case ctLeatherworker:
		{
			return 2;
		}

		// Generic
		case ctGeneric:
		case ctUnknown:
		{
			return 1;
		}
	}

	LOGWARNING("Invalid career type for villager %d. Reset to ctGeneric (1).", a_CareerType);
	return 1;
}





AString cVillager::CareerTypeToString(eCareerType a_CareerType)
{
	switch (a_CareerType)
	{
		// Farmer
		case ctFarmer:
		{
			return "Farmer";
		}
		case ctFisherman:
		{
			return "Fisherman";
		}
		case ctShepherd:
		{
			return "Shepherd";
		}
		case ctFletcher:
		{
			return "Fletcher";
		}

		// Librarian
		case ctLibrarian:
		{
			return "Librarian";
		}
		case ctCartographer:
		{
			return "Cartographer";
		}

		// Priest
		case ctCleric:
		{
			return "Cleric";
		}

		// Blacksmith
		case ctArmorer:
		{
			return "Armorer";
		}
		case ctWeaponSmith:
		{
			return "Weapon Smith";
		}
		case ctToolSmith:
		{
			return "Tool Smith";
		}

		// Butcher
		case ctButcher:
		{
			return "Butcher";
		}
		case ctLeatherworker:
		{
			return "Leatherworker";
		}

		// Generic
		case ctGeneric:
		case ctUnknown:
		{
			return "Nitwit";
		}
	}

	return "Nitwit";
}





cVillager::eCareerType cVillager::CareerNameToCareerType(const AString & a_CareerName)
{
	static const std::map<const AString, eCareerType> CarrerTypeToCareerName
	{
		// Brown Robed Villager
		{ "Farmer", ctFarmer },
		{ "Fisherman", ctFisherman },
		{ "Shepherd", ctShepherd },
		{ "Fletcher", ctFletcher },

		// White Robed Villager
		{ "Librarian", ctLibrarian },
		{ "Cartographer", ctCartographer },

		// Purple Robed Villager
		{ "Cleric", ctCleric },

		// Black Apron Villager
		{ "Armorer", ctArmorer },
		{ "Weapon Smith", ctWeaponSmith },
		{ "Tool Smith", ctToolSmith },

		// White Apron Villager
		{ "Butcher", ctButcher },
		{ "Leatherworker", ctLeatherworker },
	};

	auto pos = CarrerTypeToCareerName.find(a_CareerName);
	if (pos != CarrerTypeToCareerName.end())
	{
		return pos->second;
	}
	return ctUnknown;
}





void cVillager::SetTradeRecipes(MapTiersTradeRecipes a_MapTiersTradeRecipes)
{
	if (a_MapTiersTradeRecipes == nullptr)
	{
		// Not trade recipes for this villager's career
		return;
	}

	m_MapTiersTradeRecipes = std::move(a_MapTiersTradeRecipes);
}





void cVillager::OnRightClicked(cPlayer & a_Player)
{
	if (m_VillagerType == vtGeneric)
	{
		// The only villager that cannot trade
		return;
	}

	// TODO: Set look vector, should look at player

	m_Merchant->StartTrade(a_Player);
}
