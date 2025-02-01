#pragma once

//シーンの種類
enum class EScene
{
	eNone,	//どのシーンにも所属していない

	eTitle,	//タイトルシーン
	eTestGame,	//ゲームシーン
	eGame1,	// ゲームシーン1

	eGameOver,	// ゲームオーバーシーン
	eClear,		// クリアシーン

	eBootMenu,	//起動時メニュー
	eColTest,	//衝突判定テスト
};
