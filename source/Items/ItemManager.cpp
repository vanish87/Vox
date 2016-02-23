// ******************************************************************************
// Filename:    Item.cpp
// Project:     Game
// Author:      Steven Ball
//
// Purpose:
//
// Revision History:
//   Initial Revision - 12/04/14
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "ItemManager.h"
#include "../utils/Random.h"
#include "../Lighting/LightingManager.h"

#include <algorithm>

float ItemManager::ITEM_INTERACTION_DISTANCE = 2.5f;
float ItemManager::ITEM_INERACCTION_RADIUS_CHECK = 0.75f;


ItemManager::ItemManager(Renderer* pRenderer, ChunkManager* pChunkManager, Player* pPlayer)
{
	m_pRenderer = pRenderer;
	m_pChunkManager = pChunkManager;
	m_pPlayer = pPlayer;
	m_pQubicleBinaryManager = NULL;
	m_pInventoryManager = NULL;
	m_pLightingManager = NULL;
	m_pBlockParticleManager = NULL;

	m_numRenderItems = 0;

	// Chest
	AddItemSubSpawnData(eItem_Chest, BlockType_Default, eItem_Coin, "media/gamedata/items/Coin/Coin.item", "Coin", false, true, 0.0225f,
		"media/gamedata/items/Coin/Coin.item", "media/textures/items/coin.tga", InventoryType_Item, eItem_Coin, ItemStatus_None, EquipSlot_NoSlot, ItemQuality_Common, false, false, "Coin", "Coin", 1.0f, 1.0f, 1.0f, 1);

	// Ores
	AddItemSubSpawnData(eItem_CopperVein, BlockType_Default, eItem_CopperOre, "media/gamedata/items/CopperOre/CopperOre.item", "Copper Ore", false, true, 0.08f,
		"media/gamedata/items/CopperOre/CopperOre.item", "media/textures/items/copper_ore.tga", InventoryType_Item, eItem_CopperOre, ItemStatus_None, EquipSlot_NoSlot, ItemQuality_Common, false, false, "Copper Ore", "Copper ore", 1.0f, 1.0f, 1.0f, 1);
	AddItemSubSpawnData(eItem_IronVein, BlockType_Default, eItem_IronOre, "media/gamedata/items/IronOre/IronOre.item", "Iron Ore", false, true, 0.08f,
		"media/gamedata/items/IronOre/IronOre.item", "media/textures/items/iron_ore.tga", InventoryType_Item, eItem_IronOre, ItemStatus_None, EquipSlot_NoSlot, ItemQuality_Common, false, false, "Iron Ore", "Iron ore", 1.0f, 1.0f, 1.0f, 1);
	AddItemSubSpawnData(eItem_SilverVein, BlockType_Default, eItem_SilverOre, "media/gamedata/items/SilverOre/SilverOre.item", "Silver Ore", false, true, 0.08f,
		"media/gamedata/items/SilverOre/SilverOre.item", "media/textures/items/silver_ore.tga", InventoryType_Item, eItem_SilverOre, ItemStatus_None, EquipSlot_NoSlot, ItemQuality_Common, false, false, "Silver Ore", "Silver ore", 1.0f, 1.0f, 1.0f, 1);
	AddItemSubSpawnData(eItem_GoldVein, BlockType_Default, eItem_GoldOre, "media/gamedata/items/GoldOre/GoldOre.item", "Gold Ore", false, true, 0.08f,
		"media/gamedata/items/GoldOre/GoldOre.item", "media/textures/items/gold_ore.tga", InventoryType_Item, eItem_GoldOre, ItemStatus_None, EquipSlot_NoSlot, ItemQuality_Common, false, false, "Gold Ore", "Gold ore", 1.0f, 1.0f, 1.0f, 1);

	// Blocks
	AddItemSubSpawnData(eItem_None, BlockType_Grass, eItem_Block_Grass, "media/gamedata/items/Block_Grass/Block_Grass.item", "Grass Block", false, true, 0.06f,
		"media/gamedata/items/Block_Grass/Block_Grass.item", "media/textures/items/block_grass.tga", InventoryType_Block, eItem_Block_Grass, ItemStatus_None, EquipSlot_RightHand, ItemQuality_Common, false, false, "Grass Block", "Grass block for world building", 0.23f, 0.13f, 0.02f, 1);
	AddItemSubSpawnData(eItem_None, BlockType_Dirt, eItem_Block_Dirt, "media/gamedata/items/Block_Dirt/Block_Dirt.item", "Dirt Block", false, true, 0.06f,
		"media/gamedata/items/Block_Dirt/Block_Dirt.item", "media/textures/items/block_dirt.tga", InventoryType_Block, eItem_Block_Dirt, ItemStatus_None, EquipSlot_RightHand, ItemQuality_Common, false, false, "Dirt Block", "Dirt block for world building", 0.23f, 0.13f, 0.02f, 1);
	AddItemSubSpawnData(eItem_None, BlockType_Stone, eItem_Block_Stone, "media/gamedata/items/Block_Stone/Block_Stone.item", "Stone Block", false, true, 0.06f,
		"media/gamedata/items/Block_Stone/Block_Stone.item", "media/textures/items/block_stone.tga", InventoryType_Block, eItem_Block_Stone, ItemStatus_None, EquipSlot_RightHand, ItemQuality_Common, false, false, "Stone Block", "Stone block for world building", 0.23f, 0.13f, 0.02f, 1);
	AddItemSubSpawnData(eItem_None, BlockType_Wood, eItem_Block_Wood, "media/gamedata/items/Block_Wood/Block_Wood.item", "Wood Block", false, true, 0.06f,
		"media/gamedata/items/Block_Wood/Block_Wood.item", "media/textures/items/block_wood.tga", InventoryType_Block, eItem_Block_Wood, ItemStatus_None, EquipSlot_RightHand, ItemQuality_Common, false, false, "Wood Block", "Wood block for world building", 0.23f, 0.13f, 0.02f, 1);
	AddItemSubSpawnData(eItem_None, BlockType_Leaf, eItem_Block_Leaf, "media/gamedata/items/Block_Leaf/Block_Leaf.item", "Leaf Block", false, true, 0.06f,
		"media/gamedata/items/Block_Leaf/Block_Leaf.item", "media/textures/items/block_leaf.tga", InventoryType_Block, eItem_Block_Leaf, ItemStatus_None, EquipSlot_RightHand, ItemQuality_Common, false, false, "Leaf Block", "Leaf block for world building", 0.23f, 0.13f, 0.02f, 1);
	AddItemSubSpawnData(eItem_None, BlockType_Sand, eItem_Block_Sand, "media/gamedata/items/Block_Sand/Block_Sand.item", "Sand Block", false, true, 0.06f,
		"media/gamedata/items/Block_Sand/Block_Sand.item", "media/textures/items/block_sand.tga", InventoryType_Block, eItem_Block_Sand, ItemStatus_None, EquipSlot_RightHand, ItemQuality_Common, false, false, "Sand Block", "Sand block for world building", 0.23f, 0.13f, 0.02f, 1);
	AddItemSubSpawnData(eItem_None, BlockType_Cactus, eItem_Block_Cactus, "media/gamedata/items/Block_Cactus/Block_Cactus.item", "Cactus Block", false, true, 0.06f,
		"media/gamedata/items/Block_Cactus/Block_Cactus.item", "media/textures/items/block_cactus.tga", InventoryType_Block, eItem_Block_Cactus, ItemStatus_None, EquipSlot_RightHand, ItemQuality_Common, false, false, "Cactus Block", "Cactus block for world building", 0.23f, 0.13f, 0.02f, 1);
	AddItemSubSpawnData(eItem_None, BlockType_Rock, eItem_Block_Rock, "media/gamedata/items/Block_Rock/Block_Rock.item", "Rock Block", false, true, 0.06f,
		"media/gamedata/items/Block_Rock/Block_Rock.item", "media/textures/items/block_rock.tga", InventoryType_Block, eItem_Block_Rock, ItemStatus_None, EquipSlot_RightHand, ItemQuality_Common, false, false, "Rock Block", "Rock block for world building", 0.23f, 0.13f, 0.02f, 1);
	AddItemSubSpawnData(eItem_None, BlockType_Snow, eItem_Block_Snow, "media/gamedata/items/Block_Snow/Block_Snow.item", "Snow Block", false, true, 0.06f,
		"media/gamedata/items/Block_Snow/Block_Snow.item", "media/textures/items/block_snow.tga", InventoryType_Block, eItem_Block_Snow, ItemStatus_None, EquipSlot_RightHand, ItemQuality_Common, false, false, "Snow Block", "Snow block for world building", 1.0f, 1.0f, 1.0f, 1);
}

ItemManager::~ItemManager()
{
	ClearItems();
	ClearSubSpawnData();
}

void ItemManager::SetLightingManager(LightingManager* pLightingManager)
{
	m_pLightingManager = pLightingManager;
}

void ItemManager::SetBlockParticleManager(BlockParticleManager* pBlockParticleManager)
{
	m_pBlockParticleManager = pBlockParticleManager;
}

void ItemManager::SetQubicleBinaryManager(QubicleBinaryManager* pQubicleBinaryManager)
{
	m_pQubicleBinaryManager = pQubicleBinaryManager;
}

void ItemManager::SetInventoryManager(InventoryManager* pInventoryManager)
{
	m_pInventoryManager = pInventoryManager;
}

// Deletion
void ItemManager::ClearItems()
{
	for(unsigned int i = 0; i < m_vpItemList.size(); i++)
	{
		delete m_vpItemList[i];
		m_vpItemList[i] = 0;
	}
	m_vpItemList.clear();
}

void ItemManager::ClearSubSpawnData()
{
	for(unsigned int i = 0; i < m_vpItemSubSpawnDataList.size(); i++)
	{
		delete m_vpItemSubSpawnDataList[i];
		m_vpItemSubSpawnDataList[i] = 0;
	}
	m_vpItemSubSpawnDataList.clear();
}

// Item sub spawn data
void ItemManager::AddItemSubSpawnData(eItem sourceItem, BlockType sourceBlockType, eItem spawnedItem, string spawnedItemFilename, string spawnedItemTitle, bool interactable, bool collectible,float scale,
	string droppedItemFilename, string droppedItemTextureFilename, InventoryType droppedItemInventoryType, eItem droppedItemItem, ItemStatus droppedItemStatus, EquipSlot droppedItemEquipSlot, ItemQuality droppedItemQuality,
	bool droppedItemLeft, bool droppedItemRight, string droppedItemTitle, string droppedItemDescription, float droppedItemPlacementR, float droppedItemPlacementG, float droppedItemPlacementB, int droppedItemQuantity)
{
	ItemSubSpawnData* pItemSubSpawnData = new ItemSubSpawnData();

	pItemSubSpawnData->m_sourceItem = sourceItem;
	pItemSubSpawnData->m_sourceBlockType = sourceBlockType;

	pItemSubSpawnData->m_spawnedItem = spawnedItem;
	pItemSubSpawnData->m_spawnedItemFilename = spawnedItemFilename;
	pItemSubSpawnData->m_spawnedItemTitle = spawnedItemTitle;
	pItemSubSpawnData->m_collectible = collectible;
	pItemSubSpawnData->m_interactable = interactable;
	pItemSubSpawnData->m_scale = scale;

	pItemSubSpawnData->m_droppedItemFilename = droppedItemFilename;
	pItemSubSpawnData->m_droppedItemTextureFilename = droppedItemTextureFilename;
	pItemSubSpawnData->m_droppedItemInventoryType = droppedItemInventoryType;
	pItemSubSpawnData->m_droppedItemItem = droppedItemItem;
	pItemSubSpawnData->m_droppedItemStatus = droppedItemStatus;
	pItemSubSpawnData->m_droppedItemEquipSlot = droppedItemEquipSlot;
	pItemSubSpawnData->m_droppedItemQuality = droppedItemQuality;
	pItemSubSpawnData->m_droppedItemLeft = droppedItemLeft;
	pItemSubSpawnData->m_droppedItemRight = droppedItemRight;
	pItemSubSpawnData->m_droppedItemTitle = droppedItemTitle;
	pItemSubSpawnData->m_droppedItemDescription = droppedItemDescription;
	pItemSubSpawnData->m_droppedItemPlacementR = droppedItemPlacementR;
	pItemSubSpawnData->m_droppedItemPlacementG = droppedItemPlacementG;
	pItemSubSpawnData->m_droppedItemPlacementB = droppedItemPlacementB;
	pItemSubSpawnData->m_droppedItemQuantity = droppedItemQuantity;

	m_vpItemSubSpawnDataList.push_back(pItemSubSpawnData);
}

ItemSubSpawnData* ItemManager::GetItemSubSpawnData(eItem sourceItem)
{
	for(unsigned int i = 0; i < m_vpItemSubSpawnDataList.size(); i++)
	{
		if(m_vpItemSubSpawnDataList[i]->m_sourceItem == sourceItem)
		{
			return m_vpItemSubSpawnDataList[i];
		}
	}

	return NULL;
}

ItemSubSpawnData* ItemManager::GetItemSubSpawnData(BlockType sourceBlockType)
{
	for(unsigned int i = 0; i < m_vpItemSubSpawnDataList.size(); i++)
	{
		if(m_vpItemSubSpawnDataList[i]->m_sourceBlockType == sourceBlockType)
		{
			return m_vpItemSubSpawnDataList[i];
		}
	}

	return NULL;
}

// Creation
Item* ItemManager::CreateItem(vec3 position, vec3 velocity, vec3 rotation, const char* objectFilename, eItem itemType, const char* itemTitle, bool interactable, bool collectible, float scale)
{
	Item* pNewItem = new Item(m_pRenderer, m_pChunkManager, m_pQubicleBinaryManager, itemTitle, itemType, scale);
	pNewItem->LoadItem(objectFilename);

	pNewItem->SetLightingManager(m_pLightingManager);
	pNewItem->SetBlockParticleManager(m_pBlockParticleManager);
	pNewItem->SetPlayer(m_pPlayer);
	pNewItem->SetItemManager(this);
	pNewItem->SetInventoryManager(m_pInventoryManager);

	vec3 gravityDir = vec3(0.0f, -1.0f, 0.0f);
	pNewItem->SetPosition(position);
	pNewItem->SetRotation(rotation);
	pNewItem->SetGravityDirection(gravityDir);
	pNewItem->SetVelocity(velocity);

	pNewItem->SetIsCollectible(collectible);
	pNewItem->SetCollectionDelay(GetRandomNumber(1, 2, 2));

	pNewItem->SetInteractable(interactable);

	m_vpItemList.push_back(pNewItem);

	return pNewItem;
}

void ItemManager::RemoveItem(const char* itemTitle)
{
	for(unsigned int i = 0; i < m_vpItemList.size(); i++)
	{
		if(m_vpItemList[i]->NeedsErasing())
		{
			continue;
		}

		if(strcmp(m_vpItemList[i]->GetItemTitle(), itemTitle) == 0)
		{
			m_vpItemList[i]->SetErase(true);
		}
	}
}

// Get number of items
int ItemManager::GetNumItems()
{
	int numItems =  (int)m_vpItemList.size();

	return numItems;
}

int ItemManager::GetNumRenderItems()
{
	return m_numRenderItems;
}

// Collision detection
bool ItemManager::CheckCollisions(vec3 center, vec3 previousCenter, float radius, vec3 *pNormal, vec3 *pMovement)
{
	bool colliding = false;
	for(unsigned int i = 0; i < m_vpItemList.size() && colliding == false; i++)
	{
		//if(m_pChunkManager->IsInsideLoader(m_vpItemList[i]->GetCenter()) == false)
		//{
		//	continue;
		//}

		if(m_vpItemList[i]->IsCollisionEnabled())
		{
			vec3 toPlayer = m_vpItemList[i]->GetCenter() - m_pPlayer->GetCenter();

			if(length(toPlayer) < radius + m_vpItemList[i]->GetCollisionRadius())
			{
				m_vpItemList[i]->CalculateWorldTransformMatrix();

				if(m_vpItemList[i]->IsColliding(center, previousCenter, radius, pNormal, pMovement))
				{
					colliding = true;
				}
			}
		}
	}

	return colliding;
}

// Player item interaction
Item* ItemManager::CheckItemPlayerInteraction()
{
	Item* pInteractingItem = NULL;
	float maxDistance = 99.9f;
	float maxDotProduct = 0.0f;

	// Check if any item are within interaction range
	ItemList::iterator item_iterator;
	for(item_iterator = m_vpItemList.begin(); item_iterator != m_vpItemList.end(); ++item_iterator)
	{
		Item* pItem = (*item_iterator);

		if(pItem->NeedsErasing())
		{
			continue;
		}

		if(pItem->IsInteractable() == false)
		{
			// Don't test non-interactable items
			continue;
		}

		vec3 distance = pItem->GetCenter() - m_pPlayer->GetCenter();
		vec3 direction = normalize(distance);
		vec3 facing = m_pPlayer->GetForwardVector();

		distance.y = 0.0f;
		float lengthSize = length(distance);

		//if(lengthSize < maxDistance)
		{
			float dotProduct = dot(direction, facing);

			if(lengthSize > pItem->GetRadius() && lengthSize < ITEM_INTERACTION_DISTANCE + pItem->GetRadius())
			{
				// Make sure we are facing
				if(dotProduct > ITEM_INERACCTION_RADIUS_CHECK)
				{
					if(dotProduct > maxDotProduct)
					{
						maxDotProduct = dotProduct;
						//maxDistance = lengthSize;
						pInteractingItem = pItem;
					}
				}
			}
		}
	}

	return pInteractingItem;
}

bool needs_erasing(Item* aI)
{
	bool needsErase = aI->NeedsErasing();// SB REDO && aI->IsSaved();

	if(needsErase == true)
	{
		delete aI;
	}

	return needsErase;
}

// Rendering Helpers
void ItemManager::SetWireFrameRender(bool wireframe)
{
	for(unsigned int i = 0; i < m_vpItemList.size(); i++)
	{
		Item* pItem = m_vpItemList[i];

		pItem->SetWireFrameRender(wireframe);
	}
}

// Update
void ItemManager::Update(float dt)
{
	// Remove any items that need to be erased
	m_vpItemList.erase( remove_if(m_vpItemList.begin(), m_vpItemList.end(), needs_erasing), m_vpItemList.end() );

	UpdateHoverItems();

	for(unsigned int i = 0; i < m_vpItemList.size(); i++)
	{
		Item* pItem = m_vpItemList[i];

		if(pItem->NeedsErasing())
		{
			continue;
		}

		pItem->Update(dt);
	}
}

void ItemManager::UpdateItemLights(float dt)
{
	for(unsigned int i = 0; i < m_vpItemList.size(); i++)
	{
		Item* pItem = m_vpItemList[i];

		if(pItem->NeedsErasing())
		{
			continue;
		}

		pItem->UpdateItemLights(dt);
	}
}

void ItemManager::UpdateItemParticleEffects(float dt)
{
	for(unsigned int i = 0; i < m_vpItemList.size(); i++)
	{
		Item* pItem = m_vpItemList[i];

		if(pItem->NeedsErasing())
		{
			continue;
		}

		pItem->UpdateItemParticleEffects(dt);
	}
}

void ItemManager::UpdateHoverItems()
{
	Item* pHoverItem = NULL;
	float maxDistance = 99.9f;
	float maxDotProduct = 0.0f;

	// Check if any item are within interaction range
	ItemList::iterator item_iterator;
	for(item_iterator = m_vpItemList.begin(); item_iterator != m_vpItemList.end(); ++item_iterator)
	{
		Item* pItem = (*item_iterator);

		if(pItem->NeedsErasing())
		{
			continue;
		}

		pItem->SetOutlineRender(false);

		if(pItem->IsInteractable() == false)
		{
			// Don't hover highlight non-interactable items
			continue;
		}
		if(pItem->IsCollectible() == true)
		{
			// Don't hover highlight collectible items
			continue;
		}
		
		vec3 distance = pItem->GetCenter() - m_pPlayer->GetCenter();
		vec3 direction = normalize(distance);
		vec3 facing = m_pPlayer->GetForwardVector();

		distance.y = 0.0f;
		float lengthSize = length(distance);

		//if(length < maxDistance)
		{
			float dotProduct = dot(direction, facing);

			if(lengthSize > pItem->GetRadius() && lengthSize < ITEM_INTERACTION_DISTANCE + pItem->GetRadius())
			{
				// Make sure we are facing
				if(dotProduct > ITEM_INERACCTION_RADIUS_CHECK)
				{
					if(dotProduct > maxDotProduct)
					{
						maxDotProduct = dotProduct;
						//maxDistance = lengthSize;
						pHoverItem = pItem;
					}
				}
			}
		}
	}

	if(pHoverItem != NULL)
	{
		pHoverItem->SetOutlineRender(true);
	}
}

// Rendering
void ItemManager::Render(bool outline, bool reflection, bool silhouette, bool shadow)
{
	m_numRenderItems = 0;

	for(unsigned int i = 0; i < m_vpItemList.size(); i++)
	{
		Item* pItem = m_vpItemList[i];

		if(pItem->NeedsErasing())
		{
			continue;
		}

		if((outline || silhouette) && pItem->IsOutlineRender() == false)
		{
			continue;
		}

		// TODO : Add me back in
		//float toCamera = (m_pGameWindow->GetGameCamera()->GetPosition() - pItem->GetCenter()).GetLength();
		//if(toCamera > m_pGameWindow->GetGUIHelper()->GetFogRadius() + (Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE*5.0f))
		//{
		//	continue;
		//}
		//if(toCamera > m_pGameWindow->GetGUIHelper()->GetFogRadius() - Chunk::CHUNK_SIZE*Chunk::BLOCK_RENDER_SIZE*1.0f)
		//{
		//	m_pRenderer->EnableTransparency(BF_SRC_ALPHA, BF_ONE_MINUS_SRC_ALPHA);
		//}

		// TODO : Add me back in
		//if(shadow || m_pRenderer->SphereInFrustum(m_pGameWindow->GetMainViewport(), pItem->GetCenter(), pItem->GetRadius()))
		{
			pItem->Render(outline, reflection, silhouette);

			m_numRenderItems++;
		}
	}
}

void ItemManager::RenderDebug()
{
	for(unsigned int i = 0; i < m_vpItemList.size(); i++)
	{
		Item* pItem = m_vpItemList[i];

		if(pItem->NeedsErasing())
		{
			continue;
		}

		// TODO : Add me back in
		//if(m_pRenderer->SphereInFrustum(m_pGameWindow->GetMainViewport(), pItem->GetCenter(), pItem->GetRadius()))
		{
			pItem->RenderDebug();
		}
	}
}