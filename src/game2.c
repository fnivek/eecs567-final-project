#include "game2.h"

// Global const
#define kGame2TimeStep 			0.02f 					// 20ms
#define kGame2HeroSpeed		 	(1.5f * kGame2TimeStep)	// 1.5inch/sec * time_step
#define kAsteroidSpeed			(1.5f * kGame2TimeStep)	// 1.5inch/sec * time_step
#define kAsteroidMaxStartDist	6.0f 					// Max distance that a asteroid can start at
#define kAsteroidMinStartDist	3.0f 					// Min distance that a asteroid can start at
#define kMissleSpeed			(3.0f * kGame2TimeStep)	// 3.0inch/sec * time_step
#define kMissleRange			3.0f 					// Max range of a missle

// Z offset from the frame of reference of shoulder servo to endeffector xyz
#define kZoffset				(Arm[kShoulder].DHd + Arm[kElbow].DHd + Arm[kWrist].DHd)

#define kRotationFreq			(2.0f * M_PI * 0.1f)	// 0.1 hz period of 10s
#define kRotationAmp			(M_PI / 3.0f)			// 60 degrees to match 120 degrees of servo

#define kCollisionDist			0.125f					// Objects colide if within 1/8 inch of eachother

#define kPixCenter				3.5f					// Center of the LED MAT
#define kInch2Pix				8.0f 					// 8 pixels to one inch 

// Global vars
Game2Data game2_data;
uint8_t isInitilized = 0;

Point2 Game2ConvertInch2Pix(Game2Data* game, Point2 pos) {
	// Shift to hero
	pos.x -= game->hero.x;
	pos.y -= game->hero.y;

	// Scale
	pos.x *= kInch2Pix;
	pos.y *= kInch2Pix;

	// Shift to led zero
	pos.x += kPixCenter;
	pos.y += kPixCenter;

	return pos;
}

Point2 UnitVec(Point2 vec) {
	// Normalize vector
	float mag = sqrt(pow(vec.x, 2) + pow(vec.y, 2));
	Point2 unit_vec = {vec.x / mag, vec.y / mag};
	return unit_vec;
}


uint8_t PointPointCollision(Point2 p1, Point2 p2, float collision_dist) {
	float dx = p1.x - p2.x;
	float dy = p1.y - p2.y;
	float mag = sqrt(pow(dx, 2) + pow(dy, 2));
	if(mag < collision_dist) {
		return 1;
	}
	return 0;
}

void Game2UpdateArm(Game2Data* game) {
	// Update the the rotation of the arm
	game->time += kGame2TimeStep;
	float th1 = kRotationAmp * sin(kRotationFreq * game->time);

	// Use homagraphy to transform points in kSholder rotated by pi about z frame to base frame
    Point3 target = {
    	-game->hero.x * cos(th1) - kZoffset * sin(th1),
    	kZoffset * cos(th1) - game->hero.x * sin(th1),
    	Arm[kBase].DHd + game->hero.y
    };

    // Move the arm
    KinematicsSetTargetPosition(target);
}

void Game2LaunchMissle(Game2Data* game) {
	// Shoot
	if(!game->fire) return; // Not trying to shoot

	// Reset fire
	game->fire = 0;
	if(!game->missle.ready) return; // Missle already launched

	// Missle ready for fire
	// Set missle ready to false
	game->missle.ready = 0;

	// Set dir to direction of joystick or to the right
	if(game->joy.x == 0 && game->joy.y == 0) {
		game->missle.dir.x = 1;
		game->missle.dir.y = 0;
	} else {
		game->missle.dir = UnitVec(game->joy);
	}

	// Set initial position
	game->missle.pos.x = 0;
	game->missle.pos.y = 0;
}

void Game2UpdateMissle(Game2Data* game) {
	// Update missle
	if(game->missle.ready) return; // Missle hasn't been fired

	// Update position
	game->missle.pos.x = kMissleSpeed * game->missle.dir.x;
	game->missle.pos.y = kMissleSpeed * game->missle.dir.y;
	game->missle.dist += kMissleSpeed;

	// Check if max range reached
	if(game->missle.dist > kMissleRange) {
		game->missle.ready = 1;
		return;
	}

	// Check for collision with asteroids
	for(uint8_t i = 0; i < NUM_ASTEROIDS; ++i) {
		if(PointPointCollision(game->missle.pos, game->asteroids[i].pos, kCollisionDist)) {
			// Reset asteroid and missile
			game->missle.ready = 1;
			Game2ResetAsteroid(game, i);
		}
	}
}

void Game2UpdateAsteroids(Game2Data* game) {
	for(uint8_t i = 0; i < NUM_ASTEROIDS; ++i) {
		// Move asteroids towards hero
		// Get vector towards hero
		Point2 to_hero = {game->hero.x - game->asteroids[i].pos.x,
						  game->hero.y - game->asteroids[i].pos.y};

		// Normalize vector
		to_hero = UnitVec(to_hero);

		// Move the asteroid in towards hero
		game->asteroids[i].pos.x += to_hero.x * kAsteroidSpeed;
		game->asteroids[i].pos.y += to_hero.y * kAsteroidSpeed;

		// Check for colisions with hero
		if(PointPointCollision(game->hero, game->asteroids[i].pos, kCollisionDist)) {
			Game2Reset(game);
		}
	}
}

void Game2ResetAsteroid(Game2Data* game, uint8_t asteroid_index) {
	// Generate random starting conditions
	float ran = (float)rand() / (float)RAND_MAX;
	Point2 ran_dir = {0.5f - (float)rand() / (float)RAND_MAX,
					  0.5f - (float)rand() / (float)RAND_MAX};
	ran_dir = UnitVec(ran_dir);

	// Set position
	float range = kAsteroidMaxStartDist - kAsteroidMinStartDist;
	float start_dist = kAsteroidMinStartDist + ran * range;
	game->asteroids[asteroid_index].pos.x = start_dist * ran_dir.x + game->hero.x;
	game->asteroids[asteroid_index].pos.y = start_dist * ran_dir.y + game->hero.y;
}

void Game2DrawHero(Game2Data* game) {
	// Hero is always at the center of the screen
	Point2 pt = {3, 3};
	LedmatDrawPixel(pt, LEDMAT_LED_GREEN);
	pt.x = 4;
	LedmatDrawPixel(pt, LEDMAT_LED_GREEN);
	pt.y = 4;
	LedmatDrawPixel(pt, LEDMAT_LED_GREEN);
	pt.x = 3;
	LedmatDrawPixel(pt, LEDMAT_LED_GREEN);
}

void Game2DrawAsteroids(Game2Data* game) {
	for(uint8_t i = 0; i < NUM_ASTEROIDS; ++i) {
		Point2 pix_center = Game2ConvertInch2Pix(game, game->asteroids[i].pos);
		Point2 points[4] = {{pix_center.x + 0.5f, pix_center.y + 0.5f},
							{pix_center.x - 0.5f, pix_center.y + 0.5f},
							{pix_center.x + 0.5f, pix_center.y - 0.5f},
							{pix_center.x - 0.5f, pix_center.y - 0.5f}};
		LedmatDrawPolygon(points, 4, LEDMAT_LED_YELLOW);
	}
}

void Game2DrawMissle(Game2Data* game) {
	// Draw it on the ship if its ready
	if(game->missle.ready) {
		Point2 dir = UnitVec(game->joy);
		// TODO: Do this right i.e. breshenhams alg.
		// Move 1.5 away from {kPixCenter, kPixCenter} center of LED
		dir.x *= 1.5;
		dir.y *= 1.5;
		dir.x += kPixCenter;
		dir.y += kPixCenter;
		LedmatDrawPixel(dir, LEDMAT_LED_RED);
	// Draw it in flight if its not ready
	} else {
		Point2 pt = Game2ConvertInch2Pix(game, game->missle.pos);
		LedmatDrawPixel(pt, LEDMAT_LED_RED);
	}
}


void Game2Draw(Game2Data* game) {
	//Write: to Screen
	// Star with clear buffer
    LedmatClearBuffer();

    // Draw hero
	Game2DrawHero(game);

	// Draw asteroids
	Game2DrawAsteroids(game);

	// Draw missle
	Game2DrawMissle(game);

	// Draw score

	// Refresh display
	LedmatRefreshDisplay();
}

void Game2UserInput(Game2Data* game) {
	//Read: Joystick
	NunchuckData data;
	ReadWii(&data);

	// Convert joy x and y to game x and y
	game->joy.x = kGame2HeroSpeed * data.joy_x;
	game->joy.y = kGame2HeroSpeed * data.joy_y;

	// Get c button release
	if(data.button_c == 0 && game->last_cbut) {
		game->fire = 1;
	}

	// Update last_cbut
	game->last_cbut = data.button_c;
}

void Game2Update(Game2Data* game) {
	// Add new joy cmd to position
	game->hero.x += game->joy.x;
	game->hero.y += game->joy.y;

	Game2LaunchMissle(game);
	Game2UpdateMissle(game);
	Game2UpdateAsteroids(game);
	Game2UpdateArm(game);
}

void Game2Reset(Game2Data* game) {
	// Reseed srand
	srand(Now());

	// Reset heros position
	game->hero.x = 0;
	game->hero.y = 10;

	// Reset time
	game->time = 0;

	// Reset all asteroids
	for(uint8_t i = 0; i < NUM_ASTEROIDS; ++i) {
		Game2ResetAsteroid(game, i);
	}

	// Reset last_cbut
	game->last_cbut = 0;

	// Reset fire
	game->fire = 0;

	// Reset missle
	game->missle.ready = 1;
}
		
void game2(void) {
	if(isInitilized == 0) {
		Game2Reset(&game2_data);
		isInitilized = 1;
	}

	Game2UserInput(&game2_data);
	Game2Update(&game2_data);
	Game2Draw(&game2_data);
    
}