#include "GAMEFLOW.H"

#include "3D_GEN.H"
#include "CONTROL.H"
#include "CD.H"
#include "DELTAPAK.H"
#include "DRAW.H"
#include "DRAWPHAS.H"
#include "FILE.H"
#include "HEALTH.H"
#include "ITEMS.H"
#include "MALLOC.H"
#include "NEWINV2.H"
#if PSXPC_VERSION
	#include "PSXPCINPUT.H"
#elif PSX_VERSION
	#include "PSXINPUT.H"
#endif
#include "ROOMLOAD.H"
#include "SAVEGAME.H"
#include "SOUND.H"

#include "SPECIFIC.H"
#include "SPOTCAM.H"
#include "TOMB4FX.H"

#include <assert.h>
#if PSXPC_VERSION
	#include <stdint.h>
#endif

#if !PC_VERSION
#include "GPU.H"
#include "MEMCARD.H"
#include "SPUSOUND.H"
#endif

#if PSX_VERSION
typedef unsigned int uintptr_t;
#endif

#include <string.h>
#include "LOT.H"

#define GF_SCRIPT_FILENAME "SCRIPT.DAT"

unsigned char gfGameMode = 1;
unsigned char gfNumMips;
int scroll_pos;
char DEL_playingamefmv;
char num_fmvs;
char fmv_to_play[2];
unsigned short dels_cutseq_selector_flag;
unsigned short dels_cutseq_player;
char Chris_Menu;
unsigned char gfLegendTime;
unsigned char bDoCredits;
static unsigned char gfCutNumber;
unsigned char gfInitialiseGame = 1;
long nframes = 1;
unsigned char gfNumPickups;
unsigned char gfNumTakeaways;
char CanLoad;
struct GAMEFLOW* Gameflow;
unsigned short* gfStringOffset;
char* gfStringWad;
unsigned short* gfFilenameOffset;
char* gfFilenameWad;
unsigned char gfCurrentLevel;
unsigned char gfLevelComplete;
unsigned char gfRequiredStartPos;
unsigned short gfLevelFlags;
unsigned char gfLevelNames[40];
unsigned char gfResidentCut[4];
unsigned char gfResetHubDest;
char gfUVRotate;
char gfLayer1Vel;
char gfLayer2Vel;
struct CVECTOR gfLayer1Col;
struct CVECTOR gfLayer2Col;
unsigned long GameTimer;
struct PHD_VECTOR gfLensFlare;
struct CVECTOR gfLensFlareColour;
unsigned char gfMirrorRoom;
unsigned char gfMips[8];
char title_controls_locked_out;
long gfScriptLen = 0;
unsigned char gfLegend;
unsigned char gfWadNames[40];
static unsigned short* gfScriptOffset;
static unsigned char* gfScriptWad;
static char* gfExtensions;
static int gfStatus;
static unsigned long OldSP;
unsigned char gfPickups[16];
unsigned char gfTakeaways[16];
#if PC_VERSION
char* gfScriptFile;
#endif

void DoGameflow()//10F5C(<), 10FD8(<)
{
	//unsigned char* gf;
	//unsigned char n;
	int op;
	unsigned short* scriptOffsetPtr;
	unsigned char* sequenceCommand;

	LoadGameflow();
#if 0//def PSX
	//0 = Eidos Logo FMV
	//1 = TRC Intro FMV
	S_PlayFMV(0);
	S_PlayFMV(1);
#endif

	num_fmvs = 0;
	fmv_to_play[0] = 0;
	fmv_to_play[1] = 0;

	//The game's title is disabled in Gameflow script. Automatically override the level id to 1 (skip it).
	gfCurrentLevel = Gameflow->TitleEnabled == 0 ? 1 : 0;

	//Current level's script offset
	scriptOffsetPtr = gfScriptOffset + (gfCurrentLevel & 0xFF);
	sequenceCommand = gfScriptWad + *scriptOffsetPtr;

	while (1)//?
	{
		op = *sequenceCommand;
		sequenceCommand++;
		switch (op)
		{
		case GF_TITLE_LEVEL://IB = 113D8, 11488
			gfLevelFlags = (sequenceCommand[0] | (sequenceCommand[1] << 8));
			DoTitle(sequenceCommand[2], sequenceCommand[3]);//a3[2]unconfirmed
			break;
		case GF_LEVEL_DATA_END://11048
			break;
		case GF_RESIDENTCUT1://112B8
			gfResidentCut[0] = *sequenceCommand++;
			break;
		case GF_RESIDENTCUT2://112CC
			gfResidentCut[1] = *sequenceCommand++;
			break;
		case GF_RESIDENTCUT3:
			gfResidentCut[2] = *sequenceCommand++;
			break;
		case GF_RESIDENTCUT4://112F4
			gfResidentCut[3] = *sequenceCommand++;
			break;
		case GF_LAYER1://1107C
		{
			int a2 = 10;//?
			int a1 = 10;//?

			LightningRGB[0] = *sequenceCommand;
			LightningRGBs[0] = *sequenceCommand;
			gfLayer2Col.r = *sequenceCommand++;

#if INTERNAL
			GameTimer = 46;
#else
			GameTimer = 44;
#endif
			LightningRGB[1] = *sequenceCommand;
			LightningRGBs[1] = *sequenceCommand;
			gfLayer2Col.g = *sequenceCommand++;

			GlobalRoomNumber = *sequenceCommand;
			//GLOBAL_gunflash_meshptr = *sequenceCommand;
			gfLayer1Col.cd = *sequenceCommand++;
			gfLayer1Vel = *sequenceCommand++;
			break;
		}
		default://11550
			assert(1);
			break;
		}
	}
}

void LoadGameflow()//102E0, 102B0
{
	char* s = NULL;
#if PC_VERSION
	LoadFile(GF_SCRIPT_FILENAME, &s);
#else
	int len = FILE_Length(GF_SCRIPT_FILENAME);
	s = game_malloc(len);
#endif
	int j = 0;
	int i = 0;
	struct STRINGHEADER sh;

	int num_strings, wad_len;

	unsigned char* sequenceCommand;
	int endOfSequence;
	unsigned char op;

#if PC_VERSION
	gfScriptFile = s;
#else
	FILE_Load(GF_SCRIPT_FILENAME, s);
#endif

	Gameflow = (struct GAMEFLOW*)s;
	s += sizeof(struct GAMEFLOW);

	gfExtensions = s;
	s += 0x28;

	gfFilenameOffset = (unsigned short*)s;
	s += Gameflow->nFileNames * sizeof(unsigned short);

	gfFilenameWad = s;
	s += Gameflow->FileNameLen;

	gfScriptOffset = (unsigned short*)s;
	s += Gameflow->nLevels * sizeof(unsigned short);

#if !PC_VERSION
	gfScriptLen = len;
#endif
	gfScriptWad = (unsigned char*)s;
	s += Gameflow->ScriptLen;

	//Align
	gfStringOffset = (unsigned short*)(char*)((uintptr_t)(s + 3) & (uintptr_t)-4);

#if CORE_UNSAFE
	//This is original code, unsafe (if no lang loc files exist on disk)
	while (FILE_Length(s) == -1)
	{
		//Null terminated
		s += strlen(s) + 1;
		i++;
	}
#else
	//Safer code (no inf loop).
	for (i = 0; i < NUM_GF_LANGUAGES; i++)
	{
#if PC_VERSION
		gfStringOffset = NULL;
		if (LoadFile((char*)s, &gfStringOffset))
#else
		if (FILE_Length((char*)s) != -1)
#endif
		{
			break;
		}

		//Null terminated
		s += strlen((char*)s) + 1;
	}
#endif

	Gameflow->Language = i;

#if !PC_VERSION
	FILE_Load((char*)s, gfStringOffset);
#endif

	memcpy(&sh, gfStringOffset, sizeof(struct STRINGHEADER));

#if PC_VERSION
	num_strings = sh.nStrings + sh.nPSXStrings + sh.nPCStrings;
	wad_len = sh.StringWadLen + sh.PSXStringWadLen + sh.PCStringWadLen;
#else
	num_strings = sh.nStrings + sh.nPSXStrings;
	wad_len = sh.StringWadLen + sh.PSXStringWadLen;
#endif

	memcpy(gfStringOffset, gfStringOffset + (sizeof(struct STRINGHEADER) / sizeof(unsigned short)), num_strings * sizeof(unsigned short));

	gfStringWad = (char*)(gfStringOffset + num_strings);

#if INTERNAL
	memcpy(gfStringOffset + num_strings, gfStringOffset + 317, wad_len);
#else
	memcpy(gfStringOffset + num_strings, gfStringOffset + 315, wad_len);
#endif

	gfScriptLen += (((num_strings * sizeof(unsigned short) + wad_len) + 3) & -4);

	if (num_strings != 0)
	{
		unsigned char* stringPtr = (unsigned char*)(gfStringOffset + num_strings);//s?

		for (i = 0; i < num_strings; i++)
		{
			int next = i + 1 == num_strings ? wad_len : gfStringOffset[i + 1];
			int stringLength = (next - gfStringOffset[i]) - 1;
			if (stringLength > 0)
			{
				for (j = 0; j < stringLength; j++)
				{
					stringPtr[j] ^= 0xA5;
				}
			}

			stringPtr += stringLength + 1;
		}
	}

	if (Gameflow->nLevels != 0)
	{
		sequenceCommand = gfScriptWad;
		for (i = 0; i < Gameflow->nLevels; i++)
		{
			endOfSequence = 0;

			while (!endOfSequence)
			{
				op = *sequenceCommand;
				sequenceCommand++;

				switch (op)
				{
				case GF_LEVEL:
					gfLevelNames[i] = *sequenceCommand;
					sequenceCommand += 5;
					break;

				case GF_TITLE_LEVEL:

				case GF_LAYER1:
				case GF_LAYER2:
					sequenceCommand += 4;
					break;

				case GF_LEVEL_DATA_END:
					endOfSequence = 1;
					break;

				case GF_FMV:
				case GF_CUT:

				case GF_RESIDENTCUT1:
				case GF_RESIDENTCUT2:
				case GF_RESIDENTCUT3:
				case GF_RESIDENTCUT4:

				case GF_UV_ROTATE:
				case GF_LEGEND:
				case GF_ANIMATING_MIP:
				case GF_RESET_HUB:
					sequenceCommand += 1;
					break;
				case GF_LENS_FLARE:
					sequenceCommand += 9;
					break;
				case GF_MIRROR:
					sequenceCommand += 5;
					break;
				case GF_FOG:
					sequenceCommand += 3;
					break;

				case GF_KEY_ITEM1:
				case GF_KEY_ITEM2:
				case GF_KEY_ITEM3:
				case GF_KEY_ITEM4:
				case GF_KEY_ITEM5:
				case GF_KEY_ITEM6:
				case GF_KEY_ITEM7:
				case GF_KEY_ITEM8:
				case GF_KEY_ITEM9:
				case GF_KEY_ITEM10:
				case GF_KEY_ITEM11:
				case GF_KEY_ITEM12:

				case GF_PUZZLE_ITEM1:
				case GF_PUZZLE_ITEM2:
				case GF_PUZZLE_ITEM3:
				case GF_PUZZLE_ITEM4:
				case GF_PUZZLE_ITEM5:
				case GF_PUZZLE_ITEM6:
				case GF_PUZZLE_ITEM7:
				case GF_PUZZLE_ITEM8:
				case GF_PUZZLE_ITEM9:
				case GF_PUZZLE_ITEM10:
				case GF_PUZZLE_ITEM11:
				case GF_PUZZLE_ITEM12:

				case GF_PICKUP_ITEM1:
				case GF_PICKUP_ITEM2:
				case GF_PICKUP_ITEM3:
				case GF_PICKUP_ITEM4:
				case GF_EXAMINE1:
				case GF_EXAMINE2:
				case GF_EXAMINE3:

				case GF_KEY_ITEM1_COMBO1:
				case GF_KEY_ITEM1_COMBO2:
				case GF_KEY_ITEM2_COMBO1:
				case GF_KEY_ITEM2_COMBO2:
				case GF_KEY_ITEM3_COMBO1:
				case GF_KEY_ITEM3_COMBO2:
				case GF_KEY_ITEM4_COMBO1:
				case GF_KEY_ITEM4_COMBO2:
				case GF_KEY_ITEM5_COMBO1:
				case GF_KEY_ITEM5_COMBO2:
				case GF_KEY_ITEM6_COMBO1:
				case GF_KEY_ITEM6_COMBO2:
				case GF_KEY_ITEM7_COMBO1:
				case GF_KEY_ITEM7_COMBO2:
				case GF_KEY_ITEM8_COMBO1:
				case GF_KEY_ITEM8_COMBO2:

				case GF_PUZZLE_ITEM1_COMBO1:
				case GF_PUZZLE_ITEM1_COMBO2:
				case GF_PUZZLE_ITEM2_COMBO1:
				case GF_PUZZLE_ITEM2_COMBO2:
				case GF_PUZZLE_ITEM3_COMBO1:
				case GF_PUZZLE_ITEM3_COMBO2:
				case GF_PUZZLE_ITEM4_COMBO1:
				case GF_PUZZLE_ITEM4_COMBO2:
				case GF_PUZZLE_ITEM5_COMBO1:
				case GF_PUZZLE_ITEM5_COMBO2:
				case GF_PUZZLE_ITEM6_COMBO1:
				case GF_PUZZLE_ITEM6_COMBO2:
				case GF_PUZZLE_ITEM7_COMBO1:
				case GF_PUZZLE_ITEM7_COMBO2:
				case GF_PUZZLE_ITEM8_COMBO1:
				case GF_PUZZLE_ITEM8_COMBO2:

				case GF_PICKUP_ITEM1_COMBO1:
				case GF_PICKUP_ITEM1_COMBO2:
				case GF_PICKUP_ITEM2_COMBO1:
				case GF_PICKUP_ITEM2_COMBO2:
				case GF_PICKUP_ITEM3_COMBO1:
				case GF_PICKUP_ITEM3_COMBO2:
				case GF_PICKUP_ITEM4_COMBO1:
				case GF_PICKUP_ITEM4_COMBO2:

				case GF_GIVE_ITEM_AT_STARTUP:
				case GF_LOSE_ITEM_AT_STARTUP:
					sequenceCommand += 2;
					break;
				default:
					sequenceCommand += 2;
					break;
				}
			}
		}
	}
}

void QuickControlPhase()//10274(<), 10264(<)
{
#if INTERNAL && PSX_VERSION
	ProfileRGB(255, 255, 255);
#endif

#if PSX_VERSION
	OldSP = SetSp(0x1F8003E0);
#endif

	gfStatus = ControlPhase(nframes, (gfGameMode ^ 2) < 1 ? 1 : 0);

#if PSX_VERSION
	SetSp(OldSP);
#endif

#if INTERNAL && PSX_VERSION
	ProfileRGB(0, 0, 0);
#endif
}

void DoTitle(unsigned char Name, unsigned char Audio)//10604(<), 105C4(<)
{
	int i, a0, a1, s0, s1, s2, v0;

	CreditsDone = 0;
	CanLoad = 0;

	a0 = Name;//most likely

#if !PC_VERSION
	if (Gameflow->LoadSaveEnabled)
	{
		s1 = a0 & 0xFF;
		mcOpen(1);
	}
#endif

	s1 = 0;//?

	num_fmvs = 0;
	fmv_to_play[1] = 0;
	fmv_to_play[0] = 0;

#if !PC_VERSION
	XAMasterVolume = savegame.VolumeCD;
#endif

	memset(&savegame.Level, 0, sizeof(struct STATS));
	memset(&savegame.Game, 0, sizeof(struct STATS));

	S_LoadLevelFile(Name);

	GLOBAL_lastinvitem = -1;
	dels_cutseq_player = 0;

	InitSpotCamSequences();

	title_controls_locked_out = 0;

#if PC_VERSION
	InitialiseFXArray(1);
	InitialiseLOTarray(1);
#endif

	InitialisePickUpDisplay();
	
#if !PC_VERSION
	phd_InitWindow(90);
#endif

	SOUND_Stop();

	IsAtmospherePlaying = 0;
#if !PC_VERSION
	S_SetReverbType(1);
#endif

	InitialiseCamera();

	if (!bDoCredits)
	{
		trigger_title_spotcam(1);
		ScreenFadedOut = 0;
		ScreenFade = 0;
		dScreenFade = 0;
		ScreenFadeBack = 0;
		ScreenFadeSpeed = 8;
		ScreenFading = 0;
	}
	else
	{
		//loc_106EC
		cutseq_num = 28;
		SetFadeClip(1, 1);
		ScreenFadedOut = 1;
		ScreenFade = -1;
		dScreenFade = -1;

#ifndef INTERNAL
		S_CDPlay(111, 1);
#endif
	}
	//j 10730
#ifndef INTERNAL
	a0 = 2;
#endif
	///struct ITEM_INFO* v1 = lara_item;//?
	bUseSpotCam = 1;
	gfGameMode = 1;

#if 1 //def INTERNAL
	show_game_malloc_totals();
	a0 = 2;//?
#endif

	gfLevelComplete = 0;
	nframes = 2;
	a1 = 0;//second arg?
	gfStatus = ControlPhase(nframes, 0);//@args todo @ret v0
	JustLoaded = 0;

#if !PC_VERSION
	v0 = 0x001F0000;
	if (gfStatus == 0)
	{
#if INTERNAL
		s2 = v0 - 0x2240;
		s0 = 1;
#else
		s0 = v0 - 0x630;
#endif

		while (gfStatus == 0)
		{
			GPU_BeginScene();

			if (bDoCredits)
			{
				//0x10790

			}//0x107CC

			if (GLOBAL_playing_cutseq == 0 && !bDoCredits && ScreenFading == 0 && cutseq_num == 0)
			{

#if 0//INTERNAL
				long v00 = RawPad & 0x201;
				if (RawPad & 0x201 == 0x201)//Debug Cheat?
				{
					dels_cutseq_selector_flag = 0;///@FIXME $s0, !0
				}//0x10868

				/*Merge vvvvvvvvvvvvvv*/
				int* v1 = &s2[0x34];//buff?
				CreditsDone = 1;
				int v0 = *v1;
				//a0 = s1;
				//jalr $v0 ///@critical unknown module

				gfStatus = v0;//v0 is ret of jalr v0

				bnez	$v0, loc_10A24

#else
				/*With Me ^^^^^^^^^^^^^*/
				//int* v1 = &s0[0x34];//buff?
				///byte_A3FF0 = 1; //Credits done?
				//int v0 = *v1;
				//a0 = s1;
				//	jalr	$v0 ///@critical unknown module //0x1081C!! Setup.mod?
				//	sw	$v0, 0x11B8($gp)//TODO gfStatus?

				///@FIXME temp
				v0 = 0;
				if (v0 == 0)
				{

					if (GLOBAL_playing_cutseq != 0)
					{
						///@RETAIL, loc_10844
						///@INTERNAL loc_108A4
						S_Warn("[DoTitle] - Reached unimplemented condition!\n");
					}

				}//0x109C8
#endif


			}//0x10844

			 ///@loc_1088C (IB - loc_108EC)
			nframes = DrawPhaseGame();

			if (PadConnected == 0)//0x108A0
			{
				//int a2 = 3; //?

#if INTERNAL
				PrintString(256, 128, gfStringWad + gfStringOffset[221]);
#else
				PrintString(256, 128, gfStringWad + gfStringOffset[219]);
#endif
				//int v0 = 4096;//?
			}//0x108CC

			handle_cutseq_triggering(Name);

			///@TODO figure const.
			if (gfGameMode == 2)//0x108DC
			{
				if ((dbinput & 0x100) == 0 && GLOBAL_enterinventory != -1)
				{
					//loc_10910
					if (cutseq_trig == 0 && lara_item->hit_points > 0)
					{
						S_CallInventory2();
					}
				}

			}
			//0x10948
			QuickControlPhase();

			if (gfGameMode == 2 && ScreenFadedOut != 0)
			{
				InitialiseItemArray(256);//TODO const

				if (number_rooms > 0)
				{
					for (i = 0; i < number_rooms; i++)
					{
						room[i].item_number = -1;
					}
				}//loc_109B0

#if INTERNAL
				gfGameMode = s0;//loc_10A10
#else
				gfGameMode = 1;
#endif
			}

			//loc_109B8
			if (XAFadeRate == 0)
			{
				//loc_10778
			}
		}

	}//0x109C8

	Motors[0] = 0;
	Motors[1] = 0;
	
	if (!Gameflow->LoadSaveEnabled)
	{
		mcClose();
	}

	//loc_109FC
	XAReqVolume = 0;

	if (XAVolume == 0)
	{
		NoInput = 0;
		S_SoundStopAllSamples();
		S_CDStop();

		bUseSpotCam = 0;
		bDisableLaraControl = 0;

#ifndef INTERNAL
		//if (gfLevelComplete == 1 && gfStatus != 2)
		//{
			//sub_5E7A0(1, 2);//a1, a0
		//}
#endif

	}//loc_10A58 @FIXME original game has infinite loop if XAVolume != 0
	//assert(0);//temporary
#endif
}

void DoLevel(unsigned char Name, unsigned char Audio)
{
}