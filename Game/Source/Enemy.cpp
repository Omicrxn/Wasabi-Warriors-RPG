#include "Enemy.h"

#include "EntityManager.h"
#include "Pathfinding.h"
#include "Transitions.h"
#include "Input.h"

#include "Log.h"

#define DEFAULT_PATH_LENGTH 50

Enemy::Enemy(SString name, Input* input, Textures* tex, Collisions* collisions, EntityManager* entityManager, Transitions* transitions, EntityType type, EntitySubtype subtype, iPoint position) : Being()
{
    this->input = input;
    this->entityManager = entityManager;
    this->transitions = transitions;
    this->position = position;
    this->name = name;
    this->type = type;
    this->subtype = subtype;

    currentAnim = Animations::IDLE;
    width = 32;
    height = 32;
    direction = { 0,0 };

    collider = collisions->AddCollider({ position.x,position.y,width,height }, Collider::Type::ENEMY, (Module*)entityManager);
    active = true;

    readyForCombat = true;

    if (subtype == EntitySubtype::ENEMY_HENCHMAN)
    {
        SetUpClass("henchman");
        SetTexture(3);
        SetPivot(8, 30);
    }
    else if (subtype == EntitySubtype::ENEMY_BRUISER)
    {
        SetUpClass("bruiser");
        SetTexture(6);
        SetPivot(8, 30);
    }
    else if (subtype == EntitySubtype::ENEMY_BOSS)
    {
        SetUpClass("boss");
        SetTexture(7);
        SetPivot(8, 30);
    }
}

Enemy::~Enemy()
{
    LOG("Enemy destructor...");
    if (collider != nullptr)
        collider->pendingToDelete = true;
    if (texture != nullptr)
        tex->UnLoad(texture);
}

bool Enemy::Update(Input* input, float dt)
{
    // Update collider position
    if (collider != nullptr)
    {
        collider->SetPos(position.x, position.y);
    }
	return true;
}

bool Enemy::Draw(Render* render)
{
	animRec = idleAnim.GetFrame(0);
	render->scale = 3;
	if (active) render->DrawTexture(entityManager->entitiesTexture, position.x, position.y, &animRec);
	render->scale = 1;
	return true;
}

void Enemy::SetUpTexture()
{
    LOG("Setting enemy texture");

    // Define player textures / animations
    int textureStartYPos = spritePos * 32 * 5;
    for (int y = textureStartYPos; y < textureStartYPos + 160; y += 32)
    {
        for (int x = 0; x < 8 * 32; x += 32)
        {
            if (y == textureStartYPos && x == 128)
                break;
            if (y == textureStartYPos)
            {
                idleAnim.PushBack({ x,y,32,32 });
                idleAnim.speed = 0.2;
            }
            else if (y == textureStartYPos + 32)
            {
                walkRightAnim.PushBack({ x,y,32,32 });
                walkRightAnim.speed = 0.2;
            }
            else if (y == textureStartYPos + 32 * 2)
            {
                walkUpAnim.PushBack({ x,y,32,32 });
                walkUpAnim.speed = 0.2;
            }
            else if (y == textureStartYPos + 32 * 3)
            {
                walkDownAnim.PushBack({ x,y,32,32 });
                walkDownAnim.speed = 0.2;
            }
            else if (y == textureStartYPos + 32 * 4)
            {
                walkLeftAnim.PushBack({ x,y,32,32 });
                walkLeftAnim.speed = 0.2;
            }
        }
    }
}

void Enemy::Walk(iPoint direction, float dt)
{
   
}

void Enemy::SetName(SString name)
{
    this->name = name;
}

void Enemy::OnCollision(Collider* collider)
{
    if (input->GetKey(SDL_SCANCODE_F) == KEY_DOWN || input->GetControllerButton(CONTROLLER_BUTTON_A) == KeyState::KEY_DOWN)
    {
        if (readyForCombat == true)
        {
            readyForCombat = false;

            transitions->TransitionCombat(WhichAnimation::WIPE, this);
        }
    }

    if (readyForCombat && !Notifier::GetInstance()->GetInteractionNotifier())
    {
        Notifier::GetInstance()->NotifyInteraction();
        Notifier::GetInstance()->SetInteractingEntity((Entity*)this);
    }
}

void Enemy::StartCombat()
{
    Notifier::GetInstance()->NotifyBattle();
    Notifier::GetInstance()->SetEnemy(this->name);
    Notifier::GetInstance()->SetBattleState(true);
}

bool Enemy::SetUpClass(SString name)
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
        LOG("Loading entity enemy info");

        docNode = docData.child("entity").child("enemy");
        docNode = docNode.child(name.GetString());

        this->stats.level = docNode.attribute("level").as_int(0);
        this->stats.damage = docNode.attribute("damage").as_int(0);
        this->stats.maxHP = docNode.attribute("max_hp").as_int(0);
        this->stats.currentHP = docNode.attribute("current_hp").as_int(0);
        this->stats.strength = docNode.attribute("strength").as_int(0);
        this->stats.defense = docNode.attribute("defense").as_int(0);
        this->stats.attackSpeed = docNode.attribute("attack_speed").as_int(0);
        this->stats.criticalRate = docNode.attribute("critical_rate").as_int(0);

        LOG("Entity enemy info loaded");
    }

    LOG("Saving enemy info from %s", newName.GetString());

    return ret;
}
