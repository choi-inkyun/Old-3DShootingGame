#ifndef __Music_ENGINE_H__
#define __Music_ENGINE_H__

#define WIN32_LEAN_AND_MEAN 
#define NODEFAULTLIB 


#pragma comment(lib, "dsound.lib")

#define INITGUID

#define WIN32_LEAN_AND_MEAN  // just say no to MFC


#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream.h> // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <direct.h>
#include <wchar.h>
#include <fcntl.h>

#include <ddraw.h>
#include <dsound.h> // include dsound, dmusic
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>



#define DM_NUM_SEGMENTS 64 // number of midi segments that can be cached in memory

// midi object state defines
#define MIDI_NULL     0   // this midi object is not loaded
#define MIDI_LOADED   1   // this midi object is loaded
#define MIDI_PLAYING  2   // this midi object is loaded and playing
#define MIDI_STOPPED  3   // this midi object is loaded, but stopped


#define MAX_SOUNDS     256 // max number of sounds in system at once 

// digital sound object state defines
#define SOUND_NULL     0 // " "
#define SOUND_LOADED   1
#define SOUND_PLAYING  2
#define SOUND_STOPPED  3

// MACROS /////////////////////////////////////////////////

#define DSVOLUME_TO_DB(volume) ((DWORD)(-30*(100 - volume)))

// Convert from multibyte format to Unicode using the following macro:
#define MULTI_TO_WIDE( x,y )  MultiByteToWideChar( CP_ACP,MB_PRECOMPOSED, y,-1,x,_MAX_PATH);

// initializes a direct draw struct
#define DD_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

// TYPES //////////////////////////////////////////////////

// this holds a single sound
typedef struct pcm_sound_typ
	{
	LPDIRECTSOUNDBUFFER dsbuffer;   // the ds buffer containing the sound
	int state;                      // state of the sound
	int rate;                       // playback rate
	int size;                       // size of sound
	int id;                         // id number of the sound
	} pcm_sound, *pcm_sound_ptr;

// directmusic MIDI segment
typedef struct DMUSIC_MIDI_TYP
{
IDirectMusicSegment        *dm_segment;  // the directmusic segment
IDirectMusicSegmentState   *dm_segstate; // the state of the segment
int                        id;           // the id of this segment               
int                        state;        // state of midi song

} DMUSIC_MIDI, *DMUSIC_MIDI_PTR;

extern IDirectMusicPerformance    *dm_perf;
extern IDirectMusicLoader         *dm_loader;
extern IDirectMusicSegment        *dm_segment;
extern IDirectMusicSegmentState   *dm_segstate;
extern LPDIRECTSOUND		lpds;           // directsound interface pointer
extern DSBUFFERDESC			dsbd;           // directsound description
extern DSCAPS				dscaps;         // directsound caps
extern HRESULT				dsresult;       // general directsound result
extern DSBCAPS				dsbcaps;        // directsound buffer caps
extern LPDIRECTSOUNDBUFFER	lpdsbprimary;   // the primary mixing buffer
extern pcm_sound			sound_fx[MAX_SOUNDS];    // the array of secondary sound buffers

extern WAVEFORMATEX			pcmwf;          // generic waveformat structure

// direct music globals
// this hold all the directmusic midi objects
extern DMUSIC_MIDI                dm_midi[DM_NUM_SEGMENTS];
extern int dm_active_id;                               // currently active midi segment


////////////////////////////////////////////////////////////////// 
// 함수 원형(Prototype) 선언 ///////////////////////////////////// 
//////////////////////////////////////////////////////////////////
class SoundEngine
{
public:
int DSound_Load_WAV(char *filename);
int DSound_Replicate_Sound(int source_id);
int DSound_Play(int id, int flags, int volume, int rate, int pan);
int DSound_Stop_Sound(int id);
int DSound_Stop_All_Sounds(void);
int DSound_Init(void);
int DSound_Shutdown(void);
int DSound_Delete_Sound(int id);
int DSound_Delete_All_Sounds(void);
int DSound_Status_Sound(int id);
int DSound_Set_Volume(int id,int vol);
int DSound_Set_Freq(int id,int freq);
int DSound_Set_Pan(int id,int pan);	
	
	SoundEngine() {};
	~SoundEngine() {};
};

class MusicEngine
{
public:
int DMusic_Load_MIDI(char *filename);
int DMusic_Play(int id);
int DMusic_Stop(int id);
int DMusic_Shutdown(void);
int DMusic_Delete_MIDI(int id);
int DMusic_Delete_All_MIDI(void);
int DMusic_Status_MIDI(int id);
int DMusic_Init(void);
				
	MusicEngine() {};
	~MusicEngine() {};
};

#endif
