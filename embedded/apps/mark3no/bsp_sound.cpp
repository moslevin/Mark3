#include "mark3cfg.h"
#include "driver.h"
#include "bsp_sound.h"

SoundDriver clSound;

void bsp_sound_init()
{
    clSound.Init();
    clSound.SetName("/dev/snd");
    clSound.Open();
}
