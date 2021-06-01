#include "Item.h"
#include "Log.h"

#include "EntityManager.h"
#include "Render.h"
#include "SceneGameplay.h"
#include "Notifier.h"
#include "Collisions.h"
#include "Audio.h"

Item::Item(SString name, Textures* tex, AudioManager* audio, Collisions* collisions, EntityManager* entityManager, EntityType type, EntitySubtype subtype, iPoint position) : Interactive()
{
    // Needed modules
    this->tex = tex;
    this->entityManager = entityManager;
    this->name = name;
    this->type = type;
    this->position = position;
    this->audio = audio;
    // Saving item type
    this->subtype = subtype;

    pickUpFx = -1;
    consumeFx = -1;

    // Setting the texture section depending on the item type
    switch (subtype)
    {
    case EntitySubtype::ITEM_POTION:
        section = { 16, 538, width = 16, height = 30 };
        SetUpClass("potion");
        break;
    case EntitySubtype::ITEM_BPOTION:
        section = { 60, 538, width = 24, height = 30 };
        SetUpClass("bigPotion");
        break;
    case EntitySubtype::ITEM_MPOTION:
        section = { 636, 534, width = 24, height = 35 };
        SetUpClass("megaPotion");
        break;
    case EntitySubtype::ITEM_WASABI:
        section = { 10, 62, width = 28, height = 24 };
        SetUpClass("wasabi");
        break;
    case EntitySubtype::ITEM_PAN:
        section = { 63, 16, width = 22, height = 18 };
        SetUpClass("pan");
        break;
    case EntitySubtype::ITEM_FORK:
        section = { 148, 290, width = 42, height = 42 };
        SetUpClass("pan");
        break;
    default:
        break;
    }

    // Adding collider
    collider = collisions->AddCollider({ position.x, position.y, width, height }, Collider::Type::ITEM, (Module*)entityManager);

    // Starts visible on map, ready to be picked
    onMap = true;
}

Item::~Item()
{
    LOG("Destroying item");
}

bool Item::Draw(Render* render)
{
    /*if (collider != nullptr)
    {
        collider->SetPos(position.x, position.y);
    }*/

    if(onMap) render->DrawTexture(entityManager->itemsTexture, position.x, position.y, &section);

    return true;
}

void Item::OnCollision(Collider* collider)
{
    if (!hasInteracted)
    {
        audio->PlayFx(pickUpFx);
        ConvertToInvItem();
    }
}

void Item::ConvertToInvItem()
{
    hasInteracted = true;
    onMap = false;
    Notifier::GetInstance()->SetItemAddition(true);
    Notifier::GetInstance()->SetItem(this);
    if (collider != nullptr)
    {
        this->collider->pendingToDelete = true;
        this->collider = nullptr;
    }
}

Stats Item::Interact(Stats stats)
{
    return this->stats + stats;
}

bool Item::SetUpClass(SString name)
{
    bool ret = true;

    SString newName("entity_info");
    newName += ".xml";
    pugi::xml_document docData;
    pugi::xml_node docNode;

    pugi::xml_parse_result result = docData.load_file(newName.GetString());

    // Check result for loading errors
    if (result == NULL)
    {
        LOG("Could not load entity info xml file entity_info.xml. pugi error: %s", result.description());
        ret = false;
    }
    else
    {
        LOG("Loading item entity info");
        docNode = docData.child("entity").child("items");
        docNode = docNode.child(name.GetString());
        this->description = docNode.attribute("description").as_string();
        this->stats.level = docNode.attribute("level").as_int(0);
        this->stats.damage = docNode.attribute("damage").as_int(0);
        this->stats.maxHP = docNode.attribute("max_hp").as_int(0);
        this->stats.currentHP = docNode.attribute("current_hp").as_int(0);
        this->stats.strength = docNode.attribute("strength").as_int(0);
        this->stats.defense = docNode.attribute("defense").as_int(0);
        this->stats.attackSpeed = docNode.attribute("attack_speed").as_int(0);
        this->stats.criticalRate = docNode.attribute("critical_rate").as_int(0);
        LOG("Item entity info loaded");
    }

    LOG("Saving enemy info from %s", newName.GetString());

    return ret;
}

SDL_Rect Item::GetSection()
{
    return section;
}
