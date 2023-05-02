#include <windows.h>
#include <fmod.h>

#include "soundManager.h"

// 사운드 시스템
FMOD_SYSTEM* fsystem;
FMOD_SOUND* sound[32];
FMOD_RESULT result;
FMOD_CHANNEL* channelMusic;
FMOD_CHANNEL* channelEffect;

// 사운드 초기화
void SoundInit() {
	// FMOD 시스템 객체 초기화
	result = FMOD_System_Create(&fsystem, FMOD_VERSION);
	result = FMOD_System_Init(fsystem, 32, FMOD_INIT_NORMAL, NULL);

	// 사운드 매핑
	result = FMOD_System_CreateSound(fsystem, "./Music/Blop.mp3", FMOD_DEFAULT, 0, &sound[PONG_EF]);
	result = FMOD_System_CreateSound(fsystem, "./Music/ButtonSound.mp3", FMOD_DEFAULT, 0, &sound[SELECT_EF]);
	result = FMOD_System_CreateSound(fsystem, "./Music/CreaterSound.mp3", FMOD_DEFAULT, 0, &sound[GAME_CREATE_EF]);
	result = FMOD_System_CreateSound(fsystem, "./Music/Flip.mp3", FMOD_DEFAULT, 0, &sound[FLIPER_EF]);
	result = FMOD_System_CreateSound(fsystem, "./Music/Dodge.mp3", FMOD_DEFAULT, 0, &sound[DODGE_EF]);
	result = FMOD_System_CreateSound(fsystem, "./Music/Defence.mp3", FMOD_DEFAULT, 0, &sound[DEFENCE_EF]);
	result = FMOD_System_CreateSound(fsystem, "./Music/Gameover.mp3", FMOD_DEFAULT, 0, &sound[GAME_OVER_EF]);
	result = FMOD_System_CreateSound(fsystem, "./Music/Title.mp3", FMOD_LOOP_NORMAL, 0, &sound[TITLE_BG]);
	result = FMOD_System_CreateSound(fsystem, "./Music/GameOverBg.mp3", FMOD_LOOP_NORMAL, 0, &sound[END_BG]);
	result = FMOD_System_CreateSound(fsystem, "./Music/InGame.mp3", FMOD_LOOP_NORMAL, 0, &sound[INGAME_BG]);
}

// 이펙트 (단발성)
void EffectPlay(MUSIC index) {
	FMOD_System_PlaySound(fsystem, sound[index], NULL, 0, &channelEffect);
}

// BGM (반복)
void MusicPlay(MUSIC index) {
	FMOD_System_PlaySound(fsystem, sound[index], NULL, 0, &channelMusic);
}

// 해제
void ReleaseSound() {
	for (int i = 0; i < 32; i++) {
		FMOD_Sound_Release(sound[i]);
	}
	FMOD_System_Close(fsystem);
	FMOD_System_Release(fsystem);
}

// 이펙트 멈춤
void EffectStop() {
	FMOD_Channel_Stop(channelEffect);
}

// BGM 멈춤
void MusicStop() {
	FMOD_Channel_Stop(channelMusic);
}