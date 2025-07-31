#pragma once

//シーンの種類
enum class EScene
{
	eNone,	//どのシーンにも所属していない

	eTitle,	//タイトルシーン
	eGame,	//ゲームシーン
	eTest,	//テストシーン
	eClear,	//クリアシーン
	eGameOver, // ゲームオーバーシーン

	eBootMenu,	//起動時メニュー
	eColTest,	//衝突判定テスト
};
