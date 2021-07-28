#pragma once

//Managers
#define PROFILE_MANAGER
#define GAME_MANAGER

//Offsets
#define HEALTH 0x170
#define SPEED  0x1128
#define RECOIL 0x260   //starts at 260 and goes up by 4 bytes every time. Meaning the recoil x, y and deviation is stored 4 bytes away from each other.