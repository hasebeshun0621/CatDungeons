#pragma once

#include	<directxmath.h>
#include	"CStaticMeshRenderer.h"
#include	"camera.h"
#include	"Player.h"

//　アビリティチェックUI初期処理
void InitAbilityCheckUI();
//　アビリティチェックUI更新処理
void UpdateAbilityCheckUI();
//　アビリティチェックUI描画処理
void DrawAbilityCheckUI();
//　アビリティチェックUI終了処理
void ExitAbilityCheckUI();

//	アビリティを使えるかを色で表現
void AbilityCheck();