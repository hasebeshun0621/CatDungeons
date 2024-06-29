#include	"TitleMenu.h"

using namespace DirectX::SimpleMath;

static	int Select = 0;

//　タイトルメニュー初期処理
void InitTitleMenu() {

	InitTitleUI();
	InitTitleMenuUI_game();
	InitTitleMenuUI_tutorial();
	//InitTMUI_a();

}

//　タイトルメニュー更新処理
void UpdateTitleMenu() {
	
	UpdateTitleUI();
	UpdateTitleMenuUI_game();
	UpdateTitleMenuUI_tutorial();
	//UpdateTMUI_a();

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_RETURN)) {
		if (Select == 0) {
			StopSound(TitleBGM);
			PlaySound(TitleMEnterSE);
			SetScene(ROAD);
		}
		if (Select == 1) {
			PlaySound(TitleMEnterSE);
			SetScene(TUTORIAL);
		}
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_UP)) {
		Select--;
		if (Select < 0) {
			Select = 2;
		}
		PlaySound(TitleMSE);
	}

	if (CDirectInput::GetInstance().CheckKeyBufferTrigger(DIK_DOWN)) {
		Select++;
		if (Select > 2) {
			Select = 0;
		}
		PlaySound(TitleMSE);
	}
	
}

//　タイトルメニュー描画処理
void DrawTitleMenu() {

	DrawTitleUI();
	DrawTitleMenuUI_game();
	DrawTitleMenuUI_tutorial();
	//DrawTMUI_a();

}


//　タイトルメニュー終了処理
void ExitTitleMenu() {

	ExitTitleUI();
	ExitTitleMenuUI_game();
	ExitTitleMenuUI_tutorial();
	//ExitTMUI_a();

}

//　選んでいるタイトルメニュー
int GetSelect() {
	return Select;
}
