#ifndef GAME2_H
#define GAME2_H

#include <math.h>
#include <stdlib.h>

#include "wii.h"
#include "ledmat.h"
#include "arm.h"
#include "matrix_math.h"
#include "kinematics.h"
#include "helpers.h"

#define NUM_ASTEROIDS	10

typedef struct Asteroid
{
	// Position of the asteroid
	Point2 pos;
} Asteroid;

typedef struct Missle
{
	// true if ready to fire, false if already launched
	uint8_t ready;
	// Direction of travel
	Point2 dir;
	// Position of missle
	Point2 pos;
	// Distance traveled
	float dist;
} Missle;

typedef struct Game2Data
{
	// Position of hero in world coords
	Point2 hero;
	// Time since start
	float time;
	// Array of asteroids
	Asteroid asteroids[NUM_ASTEROIDS];
	// User input
	Point2 joy;
	// Last state of c button
	uint8_t last_cbut;
	// Fire
	uint8_t fire;
	// A missle
	Missle missle;
} Game2Data;

Point2 UnitVec(Point2 vec);
uint8_t PointPointCollision(Point2 p1, Point2 p2, float collision_dist);

Point2 Game2ConvertInch2Pix(Game2Data* game, Point2 pos);
void Game2UpdateArm(Game2Data* game);
void Game2LaunchMissle(Game2Data* game);
void Game2UpdateMissle(Game2Data* game);
void Game2UpdateAsteroids(Game2Data* game);
void Game2ResetAsteroid(Game2Data* game, uint8_t asteroid_index);
void Game2Draw(Game2Data* game);
void Game2DrawHero(Game2Data* game);
void Game2DrawAsteroids(Game2Data* game);
void Game2DrawMissle(Game2Data* game);
void Game2UserInput(Game2Data* game);
void Game2Update(Game2Data* game);
void Game2Reset(Game2Data* game);

void game2(void);

#endif // GAME2_H