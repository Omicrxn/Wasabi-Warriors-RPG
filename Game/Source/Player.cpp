#include "Player.h"

#include "Log.h"

Player::Player(Textures* tex, Collisions* collisions, EntityManager* entityManager) : Being()
{
    texture = NULL;
    position = iPoint(12 * 16, 27 * 16);
    active = false;
    stopPlayer = false;
    transitioning = false;
    // Setting Being parameters
    velocity = { 150.0f, 150.0f };
    direction = { 0,0 };
    /*this->stats.name = "Player";
    this->stats.level = 1;
    this->stats.damage = 20;
    this->stats.maxHP = 120;
    this->stats.currentHP = 100;
    this->stats.strength = 10;
    this->stats.defense = 15;
    this->stats.attackSpeed = 5;
    this->stats.criticalRate = 10;*/
    idleAnim = {};
    walkRightAnim = {};
    walkLeftAnim = {};
    walkUpAnim = {};
    walkDownAnim = {};
    animRec = {};

    currentAnim = Animations::IDLE;
    width = 32;
    height = 32;

    collider = collisions->AddCollider({ position.x,position.y ,width,height }, Collider::Type::PLAYER, (Module*)entityManager);
    isGod = false;
}

Player::~Player()
{
    collider->pendingToDelete = true;
}

bool Player::Update(Input* input, float dt)
{
    if (!Notifier::GetInstance()->GetBattle() && !stopPlayer && !transitioning)
    {
        Walk(direction, dt);
        direction.x = input->GetAxisRaw(AxisName::HORIZONTAL);
        direction.y = input->GetAxisRaw(AxisName::VERTICAL);
        // Update collider position
        if (collider != nullptr)
        {
            collider->SetPos(position.x, position.y);
        }
    }
    else
    {
        direction = { 0,0 };
    }

    if (active)
    {
        if (direction.x > 0)
        {
            currentAnim = Animations::RIGHT;
        }
        else if (direction.x < 0)
        {
            currentAnim = Animations::LEFT;
        }
        if (direction.y > 0)
        {
            currentAnim = Animations::DOWN;
        }
        else if (direction.y < 0)
        {
            currentAnim = Animations::UP;
        }
        if ((direction.x == 0 && direction.y == 0) || (direction.x != 0 && direction.y != 0))
        {
            currentAnim = Animations::IDLE;
        }
    }

    transitioning = false;
    return true;
}

bool Player::Draw(Render* render)
{
    if (active) {
        animRec;
        switch (currentAnim)
        {
        case Animations::IDLE:
            animRec = idleAnim.GetFrame(0);
            break;
        case Animations::UP:
            animRec = walkUpAnim.GetCurrentFrame();
            break;
        case Animations::DOWN:
            animRec = walkDownAnim.GetCurrentFrame();
            break;
        case Animations::RIGHT:
            animRec = walkRightAnim.GetCurrentFrame();
            break;
        case Animations::LEFT:
            animRec = walkLeftAnim.GetCurrentFrame();
            break;
        default:
            break;
        }
        render->scale = 3;
        render->CameraFollow(position.x, position.y);

        if (active) render->DrawTexture(texture, position.x, position.y, &animRec);

        render->scale = 1;
    }
    return true;
}

void Player::SetUpTexture()
{
    // Define player textures / animations
    int textureStartYPos = spritePos * 32 * 5;
    for (int y = textureStartYPos; y < y + 160; y += 32)
    {
        for (int x = 0; x < 8 * 32; x += 32)
        {
            if (y == textureStartYPos && x == 128)
                break;
            if (y == textureStartYPos)
            {
                SDL_Rect rect = { x,y,32,32 };
                idleAnim.PushBack(rect);
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

void Player::Walk(iPoint direction, float dt)
{
    if (direction.x != 0 && direction.y != 0)
    {
        // limit movement speed diagonally, so you move at 70% speed
        //velocity.x *= 0.7;
        //velocity.y *= 0.7;
     
        velocity = { 0,0 };
    }
    else
    {
        velocity = { 150.0f, 150.0f };
    }

    position.x = position.x + (direction.x * (velocity.x * dt));
    position.y = position.y + (direction.y * (velocity.y * dt));
}

SDL_Rect Player::GetBounds()
{
    return { (int)position.x, (int)position.y, width, height };
}

void Player::SetName(SString name)
{
    this->name = name;
}

bool Player::SetUpClass(SString name)
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

        docNode = docData.child("entity").child("player");
        docNode = docNode.child(name.GetString());

        this->stats.level = docNode.attribute("level").as_int(0);
        this->stats.damage = docNode.attribute("damage").as_int(0);
        this->stats.maxHP = docNode.attribute("max_hp").as_int(0);
        this->stats.currentHP = docNode.attribute("current_hp").as_int(0);
        this->stats.strength = docNode.attribute("strength").as_int(0);
        this->stats.defense = docNode.attribute("defense").as_int(0);
        this->stats.attackSpeed = docNode.attribute("attack_speed").as_int(0);
        this->stats.criticalRate = docNode.attribute("critical_rate").as_int(0);
    }

    LOG("Saving player info from %s", newName.GetString());

    return ret;
}
