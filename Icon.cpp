#include	"Icon.h"

//�@�A�C�R����������
void InitIcon() {

	InitAbilityIcon();
	InitBulletIcon();
	InitHPPortionIcon();

	InitAbilityCheckUI();
	InitBulletCnt();
	InitHPPortionCnt();
	InitBagMaxCnt();
	InitBagCnt();

	InitBagUI();
	InitSelectUI();

}

//�@�A�C�R���X�V����
void UpdateIcon(){

	UpdateAbilityIcon();
	UpdateBulletIcon();
	UpdateHPPortionIcon();

	UpdateAbilityCheckUI();
	UpdateBulletCnt();
	UpdateHPPortionCnt();
	UpdateBagMaxCnt();
	UpdateBagCnt();

	UpdateBagUI();
	UpdateSelectUI();

}

//�@�A�C�R���`�揈��
void DrawIcon() {

	if (GetTutorialDrawSwitch() == false) {
		DrawSelectUI();
		DrawBagUI();

		DrawAbilityIcon();
		DrawBulletIcon();
		DrawHPPortionIcon();

		DrawAbilityCheckUI();
		DrawBulletCnt();
		DrawHPPortionCnt();
		DrawBagMaxCnt();
		DrawBagCnt();

	}

}

//�@�A�C�R���I������
void ExitIcon() {

	ExitAbilityIcon();
	ExitBulletIcon();
	ExitHPPortionIcon();

	ExitAbilityCheckUI();
	ExitBulletCnt();
	ExitHPPortionCnt();
	ExitBagMaxCnt();
	ExitBagCnt();

	ExitBagUI();
	ExitSelectUI();

}



