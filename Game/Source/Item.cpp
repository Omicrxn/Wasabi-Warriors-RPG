#include "Item.h"
#include "Log.h"
Item::Item() : Interactive()
{
    texture = NULL;
    animRec = { 0,0,32,32 };
}

Item::~Item()
{
}

Stats Item::Interact(Stats stats) {
     
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
        LOG("Loading entity info");
        this->classType = name;

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
    }

    LOG("Saving enemy info from %s", newName.GetString());

    return ret;
}