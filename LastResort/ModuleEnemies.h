#ifndef __ModuleEnemies_H__
#define __ModuleEnemies_H__

#include "Module.h"
#include "ModulePowerups.h"
#include "Enemy_Basic.h"
#include "Enemy_Oscilator.h"
#include "Enemy_PowerDropper.h"

#define MAX_ENEMIES 100

enum ENEMY_TYPES
{
	NO_TYPE = -1,
	BASIC,
	OSCILATOR,
	POWERDROPPER,
	METALCROW,

	MAX_ENEMY
};

class Enemy;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y;
	POWERUP_TYPE pu_Type = POWERUP_TYPE:: NOPOWERUP;
};

class ModuleEnemies : public Module
{
public:

	ModuleEnemies();
	~ModuleEnemies();

	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);

	bool AddEnemy(ENEMY_TYPES type, int x, int y, POWERUP_TYPE powerup_type = NOPOWERUP);

private:

	void SpawnEnemy(const EnemyInfo& info);

private:

	EnemyInfo queue[MAX_ENEMIES];
	Enemy* enemies[MAX_ENEMIES];
public:
	SDL_Texture* sprites;
};

#endif // __ModuleEnemies_H__
