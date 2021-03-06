#include "CONTROL.H"

#include "3D_GEN.H"
#include "BOX.H"
#include "DELTAPAK.H"
#include "DRAWPHAS.H"
#include "EFFECTS.H"
#include "EFFECT2.H"
#include "GAMEFLOW.H"
#include "ITEMS.H"
#include "NEWINV2.H"
#include "LARA.H"
#include "PICKUP.H"
#if PSXPC_VERSION
#include "PSXPCINPUT.H"
#elif PSX_VERSION
#include "PSXINPUT.H"
#endif
#ifdef PC_VERSION
#include "GAME.H"
#else
#include "SETUP.H"
#include "GPU.H"
#include "LOAD_LEV.H"
#endif
#include "SOUND.H"
#include "SPECIFIC.H"
#include "SPHERE.H"
#include "SPOTCAM.H"
#include "TOMB4FX.H"

#include <assert.h>
#include "DRAW.H"
#include "ROOMLOAD.H"
#include "DEBRIS.H"
#include "OBJECTS.H"

int flipeffect = -1;
int fliptimer;
unsigned char ShatterSounds[18][10] =
{
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x13, 0x13, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0xA8, 0xA8, 0xA8, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0xA8, 0xA8, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0xA8, 0x0C, 0x0C, 0x0C, 0x0C, 0xA8, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C },
	{ 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C }
};
unsigned char WeaponDelay;
unsigned char KeyTriggerActive;
unsigned short GlobalCounter;
char TriggerTimer;
int reset_flag;
short SlowMotion;
short SlowMoFrameCount = 16;
unsigned char InItemControlLoop;
short ItemNewRoomNo;
short SmashedMeshCount;
char richcutfrigflag;
int nRope;
char GetLaraOnLOS;
int NoInput;
int number_los_rooms;
int framecount;
struct ITEM_INFO* items;
int flip_status;
#if PC_VERSION
int flipmap[255];
int flip_stats[255];
#else
int flipmap[10];
int flip_stats[10];
#endif
int height_type;
int tiltxoff;
int tiltyoff;
unsigned long _CutSceneTriggered1;
unsigned long _CutSceneTriggered2;
unsigned long FmvSceneTriggered;
unsigned char CurrentAtmosphere;
unsigned char IsAtmospherePlaying;
char* OutsideRoomTable;
short* OutsideRoomOffsets;
short IsRoomOutsideNo;
short FXType;
int OnObject;
short* trigger_index;
char cd_flags[136];
unsigned char InGameCnt;
struct RAT_STRUCT* Rats;
struct BAT_STRUCT* Bats;
struct SPIDER_STRUCT* Spiders;
struct TWOGUN_INFO twogun[4];
int SetDebounce;
short WB_room;
struct ITEM_INFO* WB_item;
unsigned char HeavyTriggered;
struct MESH_INFO* SmashedMesh[16];
short SmashedMeshRoom[16];
struct PENDULUM CurrentPendulum;
char LaraDrawType;
char WeatherType;
char RoomDrawType;
struct PHD_VECTOR ClosestCoord;
int ClosestItem;
int ClosestDist;
short XSoff1;
short YSoff1;
short ZSoff1;
short XSoff2;
short YSoff2;
short ZSoff2;
short los_rooms[20];
char globoncuttrig;
short ItemNewRooms[256][2];
struct CHARDEF CharDef[106] =
{
	{ -0x52, 0x34, 4, 0xD, -0xB, 0, 0xB },
	{ 0x62, 0x3A, 7, 5, -0xA, 1, 5 },
	{ 0x52, 0x1A, 0xE, 0xC, -0xA, 1, 0xB },
	{ 0x4E, 0x26, 0xA, 0xE, -0xA, 1, 0xD },
	{ -0x2A, 0xD, 0xF, 0xC, -9, 2, 0xC },
	{ 0x28, 0x1A, 0xE, 0xC, -0xA, 1, 0xB },
	{ -0x62, 0x39, 5, 5, -0xB, 0, 4 },
	{ -0x34, 0x27, 6, 0x10, -0xC, 0, 0xD },
	{ 0x22, 0x28, 6, 0x10, -0xB, 0, 0xE },
	{ -0x48, 0x3B, 5, 5, -0xB, 0, 4 },
	{ 0x16, 0x28, 0xB, 0xB, -9, 2, 0xB },
	{ -0x4E, 0x3B, 5, 5, -2, 8, 0xC },
	{ 0x6A, 0x3C, 8, 3, -4, 6, 9 },
	{ 0x72, 0x3C, 5, 4, -2, 8, 0xB },
	{ -0x2C, 0x26, 9, 0xF, -0xC, 0, 0xC },
	{ 0x58, 0x31, 0xA, 0xA, -8, 3, 0xB },
	{ -0x38, 0x37, 6, 0xA, -8, 3, 0xB },
	{ 0x2E, 0x34, 9, 0xA, -8, 3, 0xB },
	{ 0x58, 0x26, 8, 0xB, -8, 3, 0xC },
	{ 0x3E, 0x28, 0xB, 0xB, -8, 3, 0xC },
	{ -0x72, 0x30, 9, 0xC, -9, 2, 0xC },
	{ -0x18, 0x32, 9, 0xB, -9, 2, 0xB },
	{ 0x78, 0x2F, 9, 0xC, -9, 2, 0xC },
	{ 0x16, 0x33, 9, 0xB, -9, 2, 0xB },
	{ 0x6E, 0x31, 9, 0xB, -8, 3, 0xC },
	{ -0x68, 0x39, 5, 9, -7, 4, 0xB },
	{ -0x78, 0x39, 5, 0xA, -7, 4, 0xC },
	{ -0x4E, 0x28, 0xC, 0xA, -8, 3, 0xB },
	{ -0x2E, 0x35, 0xB, 7, -7, 4, 9 },
	{ -0x10, 0x28, 0xC, 0xA, -8, 3, 0xB },
	{ 0xC, 0x27, 0xA, 0xD, -0xB, 0, 0xB },
	{ 0x42, 0xD, 0x10, 0xE, -0xA, 1, 0xD },
	{ -0x7E, 0xD, 0xE, 0xD, -0xB, 0, 0xB },
	{ -0x2A, 0x19, 0xD, 0xD, -0xB, 0, 0xB },
	{ -0x7C, 0x23, 0xB, 0xD, -0xB, 0, 0xB },
	{ 0, 0x1A, 0xD, 0xD, -0xB, 0, 0xB },
	{ 0xE, 0x1A, 0xD, 0xD, -0xB, 0, 0xB },
	{ 0x42, 0x1B, 0xC, 0xD, -0xB, 0, 0xB },
	{ -0x4A, 0x1B, 0xC, 0xD, -0xB, 0, 0xB },
	{ -0x38, 0xD, 0xE, 0xD, -0xB, 0, 0xB },
	{ -0x22, 0x36, 5, 0xD, -0xB, 0, 0xB },
	{ 0x38, 0x34, 5, 0x10, -0xB, 0, 0xE },
	{ -0x1A, 0xF, 0xD, 0xD, -0xB, 0, 0xB },
	{ -0x70, 0x23, 0xB, 0xD, -0xB, 0, 0xB },
	{ 0x30, 0xD, 0x12, 0xD, -0xB, 0, 0xB },
	{ -0x70, 0xD, 0xE, 0xD, -0xB, 0, 0xB },
	{ 0x36, 0x1A, 0xC, 0xD, -0xB, 0, 0xB },
	{ -0x38, 0x1A, 0xC, 0xD, -0xB, 0, 0xB },
	{ -0x10, 0, 0xE, 0xF, -0xB, 0, 0xD },
	{ -0x62, 0xD, 0xE, 0xD, -0xB, 0, 0xB },
	{ -0x64, 0x23, 0xB, 0xD, -0xB, 0, 0xB },
	{ -0x54, 0xD, 0xE, 0xD, -0xB, 0, 0xB },
	{ 0x62, 0xD, 0xF, 0xD, -0xB, 0, 0xB },
	{ 0x52, 0xD, 0xF, 0xD, -0xB, 0, 0xB },
	{ 0x18, 0xD, 0x17, 0xD, -0xB, 0, 0xB },
	{ -0x46, 0xD, 0xD, 0xE, -0xB, 0, 0xC },
	{ 0x72, 0xD, 0xF, 0xD, -0xB, 0, 0xB },
	{ -0x1C, 0x1C, 0xC, 0xD, -0xB, 0, 0xB },
	{ 0x3E, 0x3C, 6, 4, -4, 6, 9 },
	{ -8, 0x3B, 6, 4, -4, 6, 9 },
	{ 0x58, 0x3B, 8, 4, -4, 6, 9 },
	{ -0x72, 0x3C, 7, 3, -3, 7, 9 },
	{ 0x78, 0x3B, 8, 4, -4, 6, 9 },
	{ -0xE, 0x3B, 5, 5, -0xB, 0, 4 },
	{ 0x62, 0x31, 0xB, 9, -7, 4, 0xB },
	{ 0x60, 0x23, 0xB, 0xE, -0xC, 0, 0xB },
	{ 0x48, 0x34, 9, 9, -7, 4, 0xB },
	{ 0, 0x27, 0xB, 0xC, -0xA, 1, 0xB },
	{ -0x5C, 0x34, 9, 9, -7, 4, 0xB },
	{ -0x58, 0x26, 0xA, 0xE, -0xC, 0, 0xB },
	{ 0x78, 0x23, 0xC, 0xC, -7, 4, 0xE },
	{ 0x6C, 0x23, 0xB, 0xE, -0xC, 0, 0xB },
	{ -0x3E, 0x1B, 6, 0xC, -0xA, 1, 0xB },
	{ 0x28, 0x33, 6, 0x10, -0xA, 1, 0xE },
	{ 0x1C, 0x1A, 0xC, 0xE, -0xC, 0, 0xB },
	{ 0x52, 0x34, 6, 0xD, -0xB, 0, 0xB },
	{ 0x60, 0x1A, 0x12, 9, -7, 4, 0xB },
	{ -0x68, 0x30, 0xC, 9, -7, 4, 0xB },
	{ 0x3E, 0x33, 0xA, 9, -7, 4, 0xB },
	{ -0xC, 0xF, 0xB, 0xD, -7, 4, 0xE },
	{ 0x34, 0x27, 0xA, 0xD, -7, 4, 0xE },
	{ 0xA, 0x34, 0xA, 9, -7, 4, 0xB },
	{ -0x42, 0x34, 9, 9, -7, 4, 0xB },
	{ 0, 0x33, 9, 0xB, -9, 2, 0xB },
	{ -0x4E, 0x32, 0xB, 9, -7, 4, 0xB },
	{ -0x7E, 0x30, 0xC, 9, -7, 4, 0xB },
	{ -0x7C, 0x1A, 0x12, 9, -7, 4, 0xB },
	{ -0xE, 0x32, 0xB, 9, -7, 4, 0xB },
	{ 0x28, 0x26, 0xB, 0xD, -7, 4, 0xE },
	{ -0x18, 0x29, 8, 9, -7, 4, 0xB },
	{ -0x22, 0x29, 9, 0xD, -7, 4, 0xE },
	{ -0x7E, 0x39, 6, 9, -7, 4, 0xB },
	{ -0x3E, 0x27, 0xA, 0xD, -0xA, 1, 0xC },
	{ 0x20, 0x38, 5, 0xC, -0xA, 1, 0xB },
	{ 0, 0xD, 0x18, 0xD, -0xA, 6, 0xB },
	{ -0x40, 0, 0x18, 0xD, -0xA, 6, 0xB },
	{ -0x58, 0, 0x18, 0xD, -0xA, 6, 0xB },
	{ -0x28, 0, 0x18, 0xD, -0xA, 6, 0xB },
	{ -0x6A, 0x1A, 0x12, 9, -8, 6, 0xB },
	{ -0x58, 0x1A, 0xD, 0xC, -9, 6, 0xB },
	{ 0x72, 0x1A, 0x12, 9, -8, 6, 0xB },
	{ -0x10, 0x1C, 0xD, 0xC, -9, 6, 0xB },
	{ 0, 0, 0x29, 0xD, -0xA, 6, 0xB },
	{ 0x54, 0, 0x29, 0xD, -0xA, 6, 0xB },
	{ 0x2A, 0, 0x29, 0xD, -0xA, 6, 0xB },
	{ 0x7E, 0, 0x29, 0xD, -0xA, 6, 0xB }
};

char byte_A3660;

long ControlPhase(long nframes, int demo_mode)//1D538(<), 1D6CC
{
#if PC_VERSION
	S_Warn("[ControlPhase] - Unimplemented!\n");
#else
	short item_num;
	int s0 = nframes;
	int v0 = SlowMotion;
	int a0 = SlowMotion;
	int s6 = demo_mode;
	int t1;
	int v000000;
	int v11111;
	int v000;
	char* s1;
	int a11111;
	int v1111;
	int s0000;
	int a1;
	int v1;

	if (SlowMotion == 0)
	{
		//******************** VERIFIED v

		if (SlowMoFrameCount > 16)
		{
			SlowMoFrameCount--;
		}

		RegeneratePickups();

		if (nframes > 11)
		{
			nframes = 10;
		}

		if (bTrackCamInit != 0)
		{
			bUseSpotCam = 0;
		}
		//******************** VERIFIED ^
	}
	else
	{
		//******************** NOT VERIFIED v
		SlowMotion--;

		if (SlowMoFrameCount > 39)
		{
			SlowMoFrameCount = 1;
		}
		
		//loc_1D5CC
		if (!((SlowMoFrameCount / 8) < nframes))
		{
			while ((SlowMoFrameCount / 8) < nframes)
			{
#if PSX_VERSION
				VSync(0);
#endif
				nframes++;
			}
		}

		//loc_1D60C
		nframes = 2;
		GnLastFrameCount = 0;

		//What?
		if (nframes > 10)
		{
			nframes = 10;
		}//loc_1D630


		//loc_1D618
		RegeneratePickups();

		if (bTrackCamInit != 0)
		{
			bUseSpotCam = 0;
		}

		//******************** NOT VERIFIED^
	}

	//loc_1D64C
	//******************** VERIFIED v
	framecount += nframes;
	SetDebounce = 1;

	if (framecount <= 0)
	{
		//loc_1E3B8
		return -1;
	}

	if (GLOBAL_enterinventory != -1)
	{
		return 0;
	}

	//******************** VERIFIED ^

	//loc_1D684
	GlobalCounter++;
	UpdateSky();
	S_UpdateInput();

	//FIXME: bDisableLaraControl should be 1, it's altered in trigger_title_spotcam
	if (bDisableLaraControl != 0)
	{
		//Not title
		if (gfCurrentLevel != 0)
		{
			dbinput = 0;
		}

		//loc_1D6D4
		input &= IN_LOOK;
	}

	//loc_1D6EC
	//Cutseq playing? lock controls to 0?
	if (cutseq_trig != 0)
	{
		input = IN_NONE;
	}

	//loc_1D708
	SetDebounce = 0;
	if (gfLevelComplete)
	{
		return 3;
	}

	if (reset_flag != 0)
	{
		reset_flag = 0;
		return 1;
	}

	if (lara.death_count > 91)
	{
		//loc_1D5A0
		reset_flag = 0;
		//S_Death();
		assert(0);//find ret val.
		return -1;//FIXME, s2?
	}

	if (demo_mode != 0)
	{
		if (PadConnected != 0 && ScreenFading == 0)
		{
			///@TODO retail ver string offset index
			PrintString(256, 230, &gfStringWad[gfStringOffset[176]]); //TODO IDA didn't dump me :-)
		}

		//loc_1D7A0
		if (input == -1)
		{
			input = IN_NONE;
			Motors[0] = 0;
			Motors[1] = 0;
		}

	}//loc_1D7D4
	else
	{
		if (gfGameMode != 1 && Gameflow->CheatEnabled)
		{
			if (input == IN_NONE)
			{
				if (Gameflow->InputTimeout > NoInput)//bad check
				{
					NoInput++;
					return 1;
				}

				NoInput++;

			}//1D844
			else
			{
				input = IN_NONE;
			}
		}//loc_1D848
	}

	//1D848
#if 1//def INTERNAL
	if (InGameCnt < 4)
	{
		InGameCnt++;
	}
#endif

#if 1//def INTERNAL
	//loc_1D860
	if ((input & IN_LOOK) == 0 && SniperCamActive != 0 && bUseSpotCam != 0 && bTrackCamInit != 0 && lara_item->current_anim_state != STATE_LARA_STOP || lara.hit_direction == 0x67 && lara.LitTorch == 0 && (input & IN_PAUSE) != 0 && lara_item->anim_number != ANIMATION_LARA_CROUCH_IDLE && lara_item->goal_anim_state != STATE_LARA_CROUCH_IDLE)
	{
		//loc_1D9D0
		input |= IN_LOOK;

		if (BinocularRange == 0)
		{
			//loc_1DA80
			if (SniperCamActive == 0 || bUseSpotCam == 0)
			{
				input &= ~IN_LOOK;
			}//loc_1DABC
		}
		else if (LaserSight != 0)
		{
			BinocularRange = 0;
			LaserSight = 0;

			AlterFOV(16380);

			lara_item->mesh_bits = -1;
			camera.type = (enum camera_type)BinocularOldCamera;

			lara.head_x_rot = 0;
			lara.head_y_rot = 0;
			lara.torso_x_rot = 0;
			lara.torso_y_rot = 0;

			BinocularOn = -8;
			camera.bounce = 0;
			input &= ~IN_LOOK;
			lara.look = 0;//check
		}

		v0 = BinocularRange;
	}
	else
	{
		//loc_1D920
		if (BinocularRange == 0)
		{
			if (lara.gun_type != 5 && (lara.gun_type > 6 && lara.gun_type != 6) || (lara.gun_type == 2 && (lara.sixshooter_type_carried & 4)) == 0 || lara.gun_status != 4)
			{
				v0 = BinocularRange;

			}//0x1D990
			else //loc_1D990
			{
				lara.has_fired = 1;
				BinocularRange = 128;
				LaserSight = 1;
				BinocularOldCamera = camera.old_type;
			}
		}
	}
#else
	///@HACK ********************************************************************************
	PadConnected = 1;

	//loc_1D9EC
	if ((input & IN_PAUSE) == 0)
	{
		if (PadConnected == 0)
		{
			assert(0);

			//andi	$v0, $a0, 0xFF
				if (gfGameMode == 0)
				{
					if (gfGameMode > 3)
					{
						assert(0);
						//addiu	$v0, $a0, 1
						//addiu	$v0, $s2, 0x71E8
						//lh	$v1, 0x1A($v0)
						//nop
						//bnez	$v1, loc_1DA5C
						//andi	$v0, $a0, 0xFF

						//loc_1DA3C:
						Motors[0] = 0;
						Motors[1] = 0;
						//jal	sub_62190
						return 0;
					}
					else
					{
						//loc_1DA68
					}


				}//loc_1DA5C
			
				

		}//loc_1DA5C

		a0 = byte_A3660;
		if (InGameCnt > 3)
		{
			assert(0);
		}

		//loc_1DA6C

	}
#endif
	
	//loc_1DAD0 ****************
	v1 = 0;
	if (BinocularRange != 0)
	{
		if (LaserSight != 0)
		{
			//loc_1DB28
			if ((gfLevelFlags & GF_LVOP_TRAIN) == 0)
			{
				v1 = 1;
			}
			//loc_1DB40
		}
		
		if ((gfLevelFlags & GF_LVOP_TRAIN) != 0)
		{
			if ((inputBusy & 0x40) != 0)
			{
				//loc_1DB40
				v1 = 1;
			}
		}//1DB44

	}//loc_1DB44, 1DD50

	InfraRed = v1;

	//lui	$s5, 0x1F
	//ClearDynamics();	
	ClearFires();

	item_num = next_item_active;//FIXME illegal value, should be 1F, check &objects looks like pointer not setup, continue setup.c
	GotLaraSpheres = 0;
	InItemControlLoop = 1;

	if (item_num != -1)
	{
		//loc_1DB80
		while (items[item_num].next_active != -1)
		{
			if (items[item_num].after_death > 127)
			{
				KillItem(item_num);
			}
			else
			{
				//loc_1DBB4
				if (objects[items[item_num].object_number].control != 0)
				{
					//SayNo();//Delete me testing call here
				}
			}

			item_num++;
		} 

	}//loc_1DBE8, 1DDF4

	InItemControlLoop = 0;

	KillMoveItems();

	a11111 = next_fx_active;

	InItemControlLoop = 1;

	if (next_fx_active != -1)//-1 on first call
	{
		S_Warn("[ControlPhase] -Unimplemented condition!\n");
	}

	//loc_1DC60
	InItemControlLoop = 0;

	KillMoveEffects();

	if (KillEverythingFlag != 0)
	{
		KillEverything();
	}

	//loc_1DC88
	if (SmokeCountL != 0)
	{
		SmokeCountL--;
	}

	//loc_1DCA4
	if (SmokeCountR != 0)
	{
		SmokeCountR--;
	}

	//loc_1DCC0
	if (SplashCount != 0)
	{
		SplashCount--;
	}

	//loc_1DCDC
	if (WeaponDelay != 0)
	{
		WeaponDelay--;
	}

	//loc_1DCF0

	///struct lara_info* s000000000000000 = &lara;

	if (lara.burn && !(wibble & 0x7F))
	{
		AlertNearbyGuards(lara_item);
		//lw	$v0, 0x44($s0)
		//li	$v1, 0xFFFFEFFF
		//and	$v0, $v1
		//sw	$v0, 0x44($s0)

	}//loc_1DD40

	XSoff1 += 150;
	YSoff1 += 230;
	ZSoff1 += 660;

	XSoff2 += 270;
	YSoff2 += 440;
	ZSoff2 += 160;

	////a000 = lara;

	if (lara.poisoned)
	{
		if (GLOBAL_playing_cutseq == 0)
		{
			assert(0);
		}//loc_1DE90
	}
	
	//loc_1DE84
	v000 = GLOBAL_playing_cutseq;
	//a1 = lara

	//loc_1DE90
	lara.skelebob = 0;
	InItemControlLoop = 1;
	
	if (GLOBAL_playing_cutseq == 0 && gfGameMode == 0)
	{
		assert(0);
#if 0
			lui	$v1, 0xFFFE
			li	$v1, 0xFFFEFFFF
			lw	$v0, 0x44($a1)
			move	$a0, $zero
			and	$v0, $v1
			jal	sub_4A838
			sw	$v0, 0x44($a1)
			lbu	$v1, 0x1C58($gp)
			li	$v0, 5
			bne	$v1, $v0, loc_1DEF4
			addiu	$v0, $s5, -0x2240
			lw	$v1, 0x18($v0)
			nop
			lw	$a0, 0($v1)
			nop
			jalr	$a0
			nop
#endif
	}
	//loc_1DEF4
	InItemControlLoop = 0;

	KillMoveItems();

	if ((gfLevelFlags & GF_LVOP_TRAIN) && bUseSpotCam == 0)
	{
		//v0 = lara_item;
		assert(0);
	}
	
	//loc_1DF7C
	if (GLOBAL_inventoryitemchosen != -1)
	{
		SayNo();
		GLOBAL_inventoryitemchosen = 1;
	}
	
	//loc_1DFA0
	if (GLOBAL_playing_cutseq == 0)//TODO
	{
		if (LaraDrawType != 5)
		{
			//HairControl(0, 0, 0);

			if ((gfLevelFlags) & GF_LVOP_YOUNG_LARA)
			{
				//HairControl(0, 1, 2);
			}
		}
		//loc_1DFF4
		if (GLOBAL_playing_cutseq == 0)//redudant
		{
			if (bUseSpotCam != 0)
			{
				CalculateSpotCams();
				//j loc_1E054
			}//loc_1E02C
			CalculateCamera();
		}//loc_1E044

	}//loc_1E044
	
	camera.type = CINEMATIC_CAMERA;
	CalculateCamera();

	//loc_1E054
#if 0 //TODO debug output once tabs are inited
	camera.pos.z = 0x6A00;
	camera.pos.x = 0xCE00;
	camera.target.x = 0xD158;
	camera.target.z = 0x6BEE;
#endif
	v000000 = mGetAngle(camera.pos.x, camera.pos.z, camera.target.x, camera.target.z);
	v000000 >>= 4;
	CamRot.vy = v000000 & 0xFFF;
	v11111 = wibble;

	//TriggerLaraDrips();

	t1 = v000000;
	if (SmashedMeshCount > 0)
	{
		//TODO
	}
	
	//loc_1E15C
#if 0
	UpdateSparks();
	//addiu	$s0, $s5, -0x2240
	UpdateFireSparks();
	UpdateSmokeSparks();
	UpdateBubbles();
	UpdateSplashes();
	UpdateDebris();
	UpdateBlood();
	UpdateDrips();
	UpdateGunShells();

	//lw	$v0, 0x24($s0)
	//beqz	$v0, loc_1E1C4
	//lw	$v0, 0x14($v0)
	//jalr	$v0

#endif

#if 0
	loc_1E1C4:
	lw	$v0, 0x54($s0)
	nop
	beqz	$v0, loc_1E1E4
	nop
	lw	$v0, 4($v0)
	nop
	jalr	$v0
	nop
#endif
	
	//loc_1E3B8:
	//ret 0

	//loc_1E3BC:
#endif
	return 0;
}


void KillMoveItems()//1D420(<), 1D5B4(<) (F)
{
	short nex;

	if (ItemNewRoomNo > 0)
	{
		for(nex = 0; nex < ItemNewRoomNo; nex++)
		{
			if (ItemNewRooms[nex][0] & 0x8000)
			{
				KillItem(ItemNewRooms[nex][0] & 0x7FFF);
			}
			else
			{
				ItemNewRoom(ItemNewRooms[nex][0], ItemNewRooms[nex][1]);
			}

		}
	}

	ItemNewRoomNo = 0;
}

void KillMoveEffects()//1D4AC(<), 1D640(<) (F) 
{
	short nex;

	if (ItemNewRoomNo > 0)
	{
		for (nex = 0; nex < ItemNewRoomNo; nex++)
		{
			if (ItemNewRooms[nex][0] & 0x8000)
			{
				KillEffect(ItemNewRooms[nex][0] & 0x7FFF);
			}
			else
			{
				EffectNewRoom(ItemNewRooms[nex][0], ItemNewRooms[nex][1]);
			}

		}
	}

	ItemNewRoomNo = 0;
}

void TestTriggers(short* data, int heavy, int HeavyFlags)
{
	S_Warn("[TestTriggers] - Unimplemented!\n!");
}

long rand_1 = 0xD371F947;

long GetRandomControl()//5E9F0, 926F8 (F)
{
	rand_1 = (rand_1 * 0x41C64E6D) + 0x3039;
	return (rand_1 >> 16) & 0x7FFF;
}

void SeedRandomControl(long seed)
{
	rand_1 = seed;
}

long rand_2 = 0xD371F947;

long GetRandomDraw()//5EA18, 5F6F8 (F)
{
	rand_2 = (rand_2 * 0x41C64E6D) + 0x3039;
	return (rand_2 >> 16) * 0x7FFF;
}

void SeedRandomDraw(long seed)
{
	rand_2 = seed;
}

void ClearFires()//8B1C8(<), 8D20C(<) (F)
{
	int i;

	for (i = 0; i < 32; i++)
	{
		fires[i].on = 0;
	}
}

int is_object_in_room(int roomnumber, int objnumber)
{
	S_Warn("[is_object_in_room] - Unimplemented!\n");
	return 0;
}

void NeatAndTidyTriggerCutscene(int value, int timer)
{
	S_Warn("[NeatAndTidyTriggerCutscene] - Unimplemented!\n");
}

int CheckCutPlayed(int num)//20E34(<), 21040(<) (F)
{
	if (num < 32)
		return _CutSceneTriggered1 & (1 << num);
	else
		return _CutSceneTriggered2 & (1 << (num - 32));
}

void SetCutNotPlayed(int num)//20DEC(<), 20FF8(<) (F)
{
	if (num < 32)
		_CutSceneTriggered1 &= ~(1 << num);
	else
		_CutSceneTriggered2 &= ~(1 << (num - 32));
}

void SetCutPlayed(int num)//20DA0(<), 20FAC(<) (F)
{
	if (num < 32)
		_CutSceneTriggered1 |= 1 << num;
	else
		_CutSceneTriggered2 |= 1 << (num - 32);
}

void InitCutPlayed()//20D90, 20F9C
{
	_CutSceneTriggered1 = 0;
	_CutSceneTriggered2 = 0;
}

void ResetGuards()
{
	S_Warn("[ResetGuards] - Unimplemented!\n");
}

void InterpolateAngle(short dest, short* src, short* diff, short speed)
{
	S_Warn("[InterpolateAngle] - Unimplemented!\n");
}

int CheckGuardOnTrigger()
{
	S_Warn("[CheckGuardOnTrigger] - Unimplemented!\n");
	return 0;
}

int ExplodeItemNode(struct ITEM_INFO* item, int Node, int NoXZVel, long bits)//207DC(<), 209F0(<) (F)
{
	struct object_info* object;
	short* meshp;
	short num;

	if (item->mesh_bits & (1 << Node))
	{
		if (item->object_number != SWITCH_TYPE7 || gfCurrentLevel != 7 && gfCurrentLevel != 4)
		{
			num = bits;
			if (bits == 256)
				num = -64;
		}
		else
		{
			SoundEffect(168, &item->pos, 0);
			num = bits;
		}
		GetSpheres(item, Slist, 3);
		object = &objects[item->object_number];
		ShatterItem.YRot = item->pos.y_rot;
		meshp = meshes[object->mesh_index + 2 * Node];
		ShatterItem.Bit = 1 << Node;
		ShatterItem.meshp = meshp;
		ShatterItem.Sphere.x = Slist[Node].x;
		ShatterItem.Sphere.y = Slist[Node].y;
		ShatterItem.Sphere.z = Slist[Node].z;
		ShatterItem.il = &item->il;
		ShatterItem.Flags = item->object_number != CROSSBOW_BOLT ? 0 : 0x400;
		ShatterObject(&ShatterItem, 0, num, item->room_number, NoXZVel);
		item->mesh_bits &= ~ShatterItem.Bit;
		return 1;
	}
	else
	{
		return 0;
	}
}

int GetTargetOnLOS(struct GAME_VECTOR* src, struct GAME_VECTOR* dest, int DrawTarget, int firing)
{
	S_Warn("[GetTargetOnLOS] - Unimplemented!\n");
	return 0;
}

void FireCrossBowFromLaserSight(struct GAME_VECTOR* src, struct GAME_VECTOR* target)
{
	S_Warn("[FireCrossBowFromLaserSight] - Unimplemented!\n");
}

void TriggerNormalCDTrack(short value, short flags, short type)
{
	S_Warn("[TriggerNormalCDTrack] - Unimplemented!\n");
}

void TriggerCDTrack(short value, short flags, short type)
{
	S_Warn("[TriggerCDTrack] - Unimplemented!\n");
}

void RemoveRoomFlipItems(struct room_info* r)//1F938(<), 1FB4C(<) (F)
{
	short item_num;

	for (item_num = r->item_number; item_num != -1; item_num = items[item_num].next_item)
	{
		if (items[item_num].flags & 0x100)
		{
			if (objects[items[item_num].object_number].intelligent)
			{
				if (items[item_num].hit_points <= 0 && items[item_num].hit_points != -16384)
				{
					KillItem(item_num);
				}
			}
		}
	}
}

void FlipMap(int FlipNumber)
{
	S_Warn("[FlipMap] - Unimplemented!\n");
}

void _TestTriggers(short* data, int heavy, int HeavyFlags)
{
	S_Warn("[_TestTriggers] - Unimplemented!\n");
}

void RefreshCamera(short type, short* data)
{
	S_Warn("[RefreshCamera] - Unimplemented!\n");
}

long GetWaterHeight(long x, long y, long z, short room_number)
{
	S_Warn("[GetWaterHeight] - Unimplemented!\n");
	return 0;
}

void AlterFloorHeight(struct ITEM_INFO* item, int height)
{
	S_Warn("[AlterFloorHeight] - Unimplemented!\n");
}

short GetHeight(struct FLOOR_INFO* floor, int x, int y, int z)
{
	S_Warn("[GetHeight] - Unimplemented!\n");
	return 0;
}

struct FLOOR_INFO* GetFloor(int x, int y, int z, short* room_number)//78954(<), 7A998(<) (F)
{
	struct room_info* r;
	struct FLOOR_INFO* floor;
	int y_floor, x_floor, next_room;
	int tmp;

	for (r = &room[*room_number]; ; r = &room[next_room])
	{
		x_floor = (z - r->z) >> 10;
		y_floor = (x - r->x) >> 10;

		if (x_floor <= 0)
		{
			x_floor = 0;
			if (y_floor < 1)
			{
				y_floor = 1;
			}
			else if (y_floor > r->y_size - 2)
			{
				y_floor = r->y_size - 2;
			}
		}
		else if (x_floor >= r->x_size - 1)
		{
			x_floor = r->x_size - 1;
			if (y_floor < 1)
			{
				y_floor = 1;
			}
			else if (y_floor > r->y_size - 2)
			{
				y_floor = r->y_size - 2;
			}
		}
		else if (y_floor < 0)
		{
			y_floor = 0;
		}
		else if (y_floor >= r->y_size)
		{
			y_floor = r->y_size - 1;
		}

		floor = &r->floor[x_floor + y_floor * r->x_size];
		next_room = GetDoor(floor);
		if (next_room == 255)
			break;
		*room_number = next_room;
	}

	if (y < floor->floor << 8)
	{
		if (y < floor->ceiling << 8 && floor->sky_room != -1)
		{
			do
			{
				tmp = CheckNoColCeilingTriangle(floor, x, z);
				if (tmp == 1 || tmp == -1 && y >= r->maxceiling)
					break;
				*room_number = floor->sky_room;
				r = &room[floor->sky_room];
				floor = &r->floor[((z - r->z) >> 10) + r->x_size * ((x - r->x) >> 10)];
				if (y >= floor->ceiling << 8)
					break;
			} while (floor->sky_room != 0xFF);
		}
	}
	else if (floor->pit_room != 0xFF)
	{
		while (1)
		{
			tmp = CheckNoColFloorTriangle(floor, x, z);
			if (tmp == 1 || tmp == -1 && y < r->minfloor)
				break;
			*room_number = floor->pit_room;
			r = &room[floor->pit_room];
			tmp = ((z - r->z) >> 10) + r->x_size * ((x - r->x) >> 10);
			/*if (ABS(tmp) > 1048576)
			{
				S_Warn("[GetFloor] - sector num too big -> probably room array not initialized\n");
				S_Warn("[GetFloor] - returning or the vc runtime will shit brixes\n");
				return floor;
			}*/
			floor = &r->floor[tmp];
			if (y < r->floor[tmp].floor << 8)
				break;
			if (floor->pit_room == 0xFF)
				return &r->floor[tmp];
		}
	}

	return floor;
}

short GetCeiling(struct FLOOR_INFO* floor, int x, int y, int z)
{
	S_Warn("[GetCeiling] - Unimplemented!\n");
	return 0;
}

int TriggerActive(struct ITEM_INFO* item)
{
	S_Warn("[TriggerActive] - Unimplemented!\n");
	return 0;
}

void AddRoomFlipItems(struct room_info* r)//1FA0C(<), 1FC20(<) (F)
{
	short item_num;

	for (item_num = r->item_number; item_num != -1; item_num = items[item_num].next_item)
	{
		if (items[item_num].item_flags[1])
		{
			switch (items[item_num].object_number)
			{
			case RAISING_BLOCK1:
				AlterFloorHeight(&items[item_num], -SECTOR);
				break;

			case RAISING_BLOCK2:
				AlterFloorHeight(&items[item_num], -2 * SECTOR);
				break;
			}
		}
	}
}

void IsRoomOutside(long x, long y, long z)
{
	S_Warn("[IsRoomOutside] - Unimplemented!\n");
}

short GetDoor(struct FLOOR_INFO* floor)//787CC(<), 7A810(<) (F)
{
	int type, fixtype;
	short* data;

	if (!floor->index)
		return 255;

	type = floor_data[floor->index];
	data = &floor_data[floor->index + 1];

	fixtype = type & 0x1F;
	if (fixtype == 2 || fixtype == 7 || fixtype == 8 || fixtype == 12 || fixtype == 11 || fixtype == 14 || fixtype == 13)
	{
		if (type & 0x8000)
			return 255;
		type = data[1];
		data += 2;
	}

	fixtype = type & 0x1F;
	if (fixtype == 3 || fixtype == 9 || fixtype == 10 || fixtype == 16 || fixtype == 15 || fixtype == 18 || fixtype == 17)
	{
		if (type & 0x8000)
			return 255;
		type = data[1];
		data += 2;
	}

	if ((type & 0x1F) == 1)
		return *data;
	return 255;
}

int LOS(struct GAME_VECTOR* start, struct GAME_VECTOR* target)//79460(<), 7B4A4(<) (F)
{
	struct FLOOR_INFO* floor;
	int los1, los2;

	if ((ABS(target->z - start->z)) > (ABS(target->x - start->x)))
	{
		los1 = xLOS(start, target);
		los2 = zLOS(start, target);
	}
	else
	{
		los1 = zLOS(start, target);
		los2 = xLOS(start, target);
	}

	if (los2)
	{
		floor = GetFloor(target->x, target->y, target->z, &target->room_number);

		if (ClipTarget(start, target, floor) || los1 == 1 || los2 == 1)
			return 1;
	}

	return 0;
}

int xLOS(struct GAME_VECTOR* start, struct GAME_VECTOR* target)
{
	S_Warn("[xLOS] - Unimplemented!\n");
	return 0;
}

int zLOS(struct GAME_VECTOR* start, struct GAME_VECTOR* target)
{
	S_Warn("[zLOS] - Unimplemented!\n");
	return 0;
}

int CheckNoColCeilingTriangle(struct FLOOR_INFO* floor, int x, int z)
{
	S_Warn("[CheckNoColCeilingTriangle] - Unimplemented!\n");
	return 0;
}

int CheckNoColFloorTriangle(struct FLOOR_INFO* floor, int x, int z)
{
	S_Warn("[CheckNoColFloorTriangle] - Unimplemented!\n");
	return 0;
}

int ClipTarget(struct GAME_VECTOR* start, struct GAME_VECTOR* target, struct FLOOR_INFO* floor)
{
	S_Warn("[ClipTarget] - Unimplemented!\n");
	return 0;
}


