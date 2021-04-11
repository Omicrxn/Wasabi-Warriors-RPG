#include "Enemy.h"

#include "Pathfinding.h"

#include "Notifier.h"

#define DEFAULT_PATH_LENGTH 50

Enemy::Enemy(Collisions* collisions, EntityManager* entityManager) : Being()
{
    texture = NULL;
    position = iPoint(10 * 16, 27 * 16);
    currentAnim = Animations::IDLE;
    width = 32;
    height = 32;
    direction = { 0,0 };
	// Define enemy parameters
	this->stats.name = "Enemy";
	this->stats.level = 1;
	this->stats.damage = 10;
	this->stats.maxHP = 120;
	this->stats.currentHP = 100;
	this->stats.strength = 10;
	this->stats.defense = 10;
	this->stats.attackSpeed = 5;
	this->stats.criticalRate = 5;

    collider = collisions->AddCollider({ position.x + 86,position.y + 43,width,height }, Collider::Type::ENEMY, (Module*)entityManager);
    active = true;

    readyForCombat = true;
}

Enemy::~Enemy()
{

}

bool Enemy::Update(Input* input, float dt)
{
    // Update collider position
    if (collider != nullptr)
    {
        collider->SetPos(position.x + 86, position.y + 43);
    }
	return true;
}

bool Enemy::Draw(Render* render)
{
	animRec = idleAnim.GetFrame(0);
	render->scale = 3;
	if (active) render->DrawTexture(texture, position.x, position.y, &animRec);
	render->scale = 1;
	return true;
}

void Enemy::SetUpTexture()
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
    this->stats.name = name;
}
void Enemy::OnCollision(Collider* collider)
{
    if (readyForCombat == true)
    {
        readyForCombat = false;
        Notifier::GetInstance()->NotifyBattle();
    }

}
