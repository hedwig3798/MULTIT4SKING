#pragma once

enum MUSIC {
	TITLE_BG = 0,
	INGAME_BG,
	END_BG,
	PONG_EF,
	DODGE_EF,
	DEFENCE_EF,
	FLIPER_EF,
	SELECT_EF,
	GAME_CREATE_EF,
	GAME_OVER_EF
};

void SoundInit();
void EffectPlay(MUSIC index);
void MusicPlay(MUSIC index);
void ReleaseSound();
void EffectStop();
void MusicStop();