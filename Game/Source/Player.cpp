#include "Player.h"

#include "Audio.h"

#include "Map.h"
#include "Log.h"

Player::Player(SString name, Render* render, Textures* tex, AudioManager* audio, Collisions* collisions, EntityManager* entityManager, EntityType type, EntitySubtype subtype, iPoint position) : Being()
{
    this->render = render;
    this->tex = tex;
    this->audio = audio;
    this->entityManager = entityManager;
    this->position = position;
    this->type = type;
    this->name = name;
    this->subtype = subtype;

    active = false;
    stopPlayer = false;
    transitioning = false;

    // Setting Being parameters
    velocity = { 150.0f, 150.0f };
    direction = { 0,0 };
    idleAnim = {};
    walkRightAnim = {};
    walkLeftAnim = {};
    walkUpAnim = {};
    walkDownAnim = {};
    animRec = {};

    currentAnim = Animations::IDLE;
    width = 5;
    height = 5;
    this->collisions = collisions;
    collider = collisions->AddCollider({ position.x,position.y , width, height }, Collider::Type::PLAYER, (Module*)entityManager);
    isGod = false;

    switch (subtype)
    {
    case EntitySubtype::PLAYER_KENZO:
        SetUpClass("kenzo");
        SetTexture(8);
        SetPivot(8, 30);
        break;
    case EntitySubtype::PLAYER_EIKEN:
        SetUpClass("eiken");
        SetTexture(12);
        SetPivot(8, 30);
        break;
    case EntitySubtype::PLAYER_REI:
        SetUpClass("rei");
        SetTexture(0);
        SetPivot(8, 30);
        break;
    case EntitySubtype::PLAYER_DOG:
        SetUpClass("dog");
        SetTexture(15);
        SetPivot(8, 30);
        break;
    default:
        break;
    }

    // Start timer
    footstepTimer.Start();
}

Player::~Player()
{
    LOG("Player destructor...");
    if (collider != nullptr)
        collider->pendingToDelete = true;
    if (texture != nullptr) 
        tex->UnLoad(texture);
}

bool Player::Update(Input* input, float dt)
{
    tempPosition = position;
    if (!Notifier::GetInstance()->GetBattle() && !stopPlayer && !transitioning)
    {
        Walk(direction, dt);
        direction.x = input->GetAxisRaw(AxisName::HORIZONTAL);
        direction.y = input->GetAxisRaw(AxisName::VERTICAL);
        // Update collider position
        if (collider != nullptr)
        {
            collider->SetPos(position.x + 14, position.y + 28);
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
    
    if (!isGod)
    {
        collisions->DetectTilemapCollision(collider, (Map*)entityManager->SearchEntity("map"), tempPosition, position);
    }
    transitioning = false;

    if (direction.x != 0 || direction.y != 0)
    {
        if (footstepTimer.ReadSec() > 0.5f)
        {
            footstepTimer.Start();
            audio->PlayFx(entityManager->footstepPlayerFx);
        }
    }

    render->scale = 3;
    render->CameraFollow(position.x + width, position.y);
    render->scale = 1;

    return true;
}

bool Player::Draw(Render* render)
{
    if (active)
    {
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
        if (active) render->DrawTexture(entityManager->entitiesTexture, position.x, position.y, &animRec);
        render->scale = 1;
    }
    return true;
}

void Player::SetUpTexture()
{
    LOG("Setting player texture");

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
                SDL_Rect rect = { x,y,32,32 };
                idleAnim.PushBack(rect);
                idleAnim.speed = 0.2f;
            }
            else if (y == textureStartYPos + 32)
            {
                walkRightAnim.PushBack({ x,y,32,32 });
                walkRightAnim.speed = 0.2f;
            }
            else if (y == textureStartYPos + 32 * 2)
            {
                walkUpAnim.PushBack({ x,y,32,32 });
                walkUpAnim.speed = 0.2f;
            }
            else if (y == textureStartYPos + 32 * 3)
            {
                walkDownAnim.PushBack({ x,y,32,32 });
                walkDownAnim.speed = 0.2f;
            }
            else if (y == textureStartYPos + 32 * 4)
            {
                walkLeftAnim.PushBack({ x,y,32,32 });
                walkLeftAnim.speed = 0.2f;
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
        LOG("Loading entity player info");

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

        LOG("Entity player info loaded");
    }

    LOG("Saving player info from %s", newName.GetString());

    return ret;
}

void Player::OnCollision(Collider* collider)
{
    if (collider->type = Collider::Type::SECRET_WALL)
    {
        position = tempPosition;
    }
}