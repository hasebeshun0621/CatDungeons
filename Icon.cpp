#include	"Icon.h"

//　アイコン初期処理
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

//　アイコン更新処理
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

//　アイコン描画処理
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

//　アイコン終了処理
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



