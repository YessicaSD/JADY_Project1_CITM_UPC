#include "Application.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModulePowerups.h"
#include "Powerup.h"
#include "Powerup_Speed.h"
#include "Powerup_Laser.h"
#include "ModuleCollision.h"

ModulePowerups::ModulePowerups()
{
	for (uint i = 0; i < MAX_POWERUPS; ++i)
		powerups[i] = nullptr;
}

ModulePowerups::~ModulePowerups()
{

}

bool ModulePowerups::Start()
{
	powerupTx = App->textures->Load("Assets/PowerUps.png");
	return true;
}

update_status ModulePowerups::Update()
{
	//Move the powerups
	for (uint i = 0; i < MAX_POWERUPS; ++i)
	{
		if (powerups[i] != nullptr)
		{
			//- Collider
			if (powerups[i]->collider != nullptr)
			{
				powerups[i]->position.x -= (int)moveSpeed;
			}
		}
	}

	//Render the powerups
	for (uint i = 0; i < MAX_POWERUPS; ++i)
	{
		if (powerups[i] != nullptr)
		{
			//- Collider
			if (powerups[i]->collider != nullptr)
			{
				powerups[i]->collider->SetPos(powerups[i]->position.x, powerups[i]->position.y);
			}
			//- Sprite
			if (powerups[i]->animation != nullptr)
			{
				App->render->Blit(powerupTx, powerups[i]->position.x, powerups[i]->position.y, &(powerups[i]->animation->GetCurrentFrame()));
			}
		}
	}
	return UPDATE_CONTINUE;
}

update_status ModulePowerups::PostUpdate()
{
	//Check camera position to decide what to despawn
	for (uint i = 0; i < MAX_POWERUPS; ++i)
	{
		if (powerups[i] != nullptr)
		{
			if (powerups[i]->position.x  < 0 - DESPAWN_MARGIN)
			{
				LOG("DeSpawning powerup at %d", powerups[i]->position.x);
				delete powerups[i];
				powerups[i] = nullptr;
			}
		}
	}

	return UPDATE_CONTINUE;
}

bool ModulePowerups::CleanUp()
{
	LOG("Freeing all powerups");

	App->textures->Unload(powerupTx);

	for (uint i = 0; i < MAX_POWERUPS; ++i)
	{
		if (powerups[i] != nullptr)
		{
			delete powerups[i];
			powerups[i] = nullptr;
		}
	}

	return true;
}

bool ModulePowerups::AddPowerup(int x, int y, POWERUP_TYPE type)//x and y should be the position in which the enemy that drops the powerup has died
{
	bool ret = false;

	//Here we will have the code based on SpawnEnemy, because we'll be rendering the collider directly
	//Find room for the new powerup
	uint i = 0;
	for (; powerups[i] != nullptr && i < MAX_POWERUPS; ++i);

	if (i != MAX_POWERUPS)
	{
		switch (type)
		{
		case POWERUP_TYPE::SPEED:
			powerups[i] = new Powerup_Speed(x, y);
			break;
		case POWERUP_TYPE::DESPEED:
			powerups[i] = new Powerup_Speed(x, y);
			break;
		case POWERUP_TYPE::LASER:
			powerups[i] = new Powerup_Laser(x, y);
			break;
		case POWERUP_TYPE::HOMING:
			powerups[i] = new Powerup_Laser(x, y);
			break;
		case POWERUP_TYPE::GROUND:
			powerups[i] = new Powerup_Laser(x, y);
			break;
		}
		//For now, we'll have speed for despeed and laser for homing and ground
	}
	return ret;
}

void ModulePowerups::OnCollision(Collider* c1, Collider* c2)
{
	for (uint i = 0; i < MAX_POWERUPS; ++i)
	{
		if (powerups[i] != nullptr && powerups[i]->GetCollider() == c1)
		{
			powerups[i]->OnCollision(c2);
			delete powerups[i];
			powerups[i] = nullptr;
		}
	}
}