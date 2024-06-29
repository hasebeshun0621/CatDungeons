#include	<functional>
#include	"scene.h"

// カレントシーンインデックス
int g_CurrentSceneIdx = 0;
int BeforeScene = 0;
// 現在シーン
SceneFunctioTable	g_SceneTable[8] =
{
	// 0 タイトルシーン
	{
		InitTitleScene,
		UpdateTitleScene,
		DrawTitleScene,
		ExitTitleScene,
	},
	//1 タイトルメニュー
	{
		InitTitleMenuScene,
		UpdateTitleMenuScene,
		DrawTitleMenuScene,
		ExitTitleMenuScene,
	},
	// 2 操作説明シーン(チュートリアル)
	{
		InitTutorialScene,
		UpdateTutorialScene,
		DrawTutorialScene,
		ExitTutorialScene,
	},
	// 3 ゲームシーン
	{
		InitWave,
		UpdateWave,
		DrawWave,
		ExitWave,
	},
	// 4 ボスゲームシーン
	{
		InitBossScene,
		UpdateBossScene,
		DrawBossScene,
		ExitBossScene,
	},
	// 5 ゲームオーバーシーン
	{
		InitGameOverScene , 
		UpdateGameOverScene,
		DrawGameOverScene,
		ExitGameOverScene,
	},
	// 6 クリアシーン
	{
		InitGClear,
		UpdateGClear,
		DrawGClear,
		ExitGClear,
	},
	// 7 ロードシーン
	{
		InitRoadScene,
		UpdateRoadScene,
		DrawRoadScene,
		ExitRoadScene,
	},
};

// シーン初期処理
void InitScene() {
	//ランダムシード変更
	srand(time(NULL));

	g_SceneTable[g_CurrentSceneIdx].Init();

}

// シーン更新処理
void UpdateScene() {

	g_SceneTable[g_CurrentSceneIdx].Update();

}

// シーン描画処理
void DrawScene() {

	g_SceneTable[g_CurrentSceneIdx].Draw();

}

// シーン終了処理
void ExitScene() {

	g_SceneTable[g_CurrentSceneIdx].Exit();

}

// シーン番号セット
void SetScene(int idx) {

	if (g_CurrentSceneIdx != idx) {
		//ひとつ前のシーンを保管
		BeforeScene = GetScene();
	
		//g_SceneTable[g_CurrentSceneIdx].Exit();
		g_CurrentSceneIdx = idx;
		g_SceneTable[g_CurrentSceneIdx].Init();
	}

}

//ひとつ前のシーンを取得	
int GetBeforeScene() {

	return BeforeScene;

}

//今のシーンを取得
int GetScene() {

	int idx = g_CurrentSceneIdx;
	return idx;

}
