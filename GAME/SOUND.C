#include "SOUND.H"

#include "GAMEFLOW.H"
#include "SPECIFIC.H"
#include "EFFECTS.H"
#include <stddef.h>

int sound_active;
short* sample_lut;
struct SAMPLE_INFO* sample_infos;
struct SoundSlot LaSlot[MAX_SOUND_SLOTS];

void SayNo()//55BE0(<), 56044(<) (F)
{
	int fx = 2;

	if (Gameflow->Language == LNG_FRENCH)
	{
		fx = 348;
	}
	else if (Gameflow->Language == LNG_JAPAN)
	{
		fx = 349;
	}

	SoundEffect(fx, NULL, 2);

	return;
}

void SOUND_Init()//920DC(<), 94128(<) (F)
{
	int i;

	for (i = 0; i < MAX_SOUND_SLOTS; i++)
	{
		LaSlot[i].nSampleInfo = -1;
	}

	sound_active = 1;
}

void SOUND_Stop()//920A4(<), 940F0(<) (F)
{
	int i;

	if (sound_active != 0)
	{
		for (i = 0; i < MAX_SOUND_SLOTS; i++)
		{
			LaSlot[i].nSampleInfo = -1;
		}

		S_SoundStopAllSamples();
	}
}