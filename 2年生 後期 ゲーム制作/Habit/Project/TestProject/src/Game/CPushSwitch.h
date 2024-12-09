#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// スイッチのクラス
class CPushSwitch : public CObjectBase
{
public:
	// コンストラクタ
	CPushSwitch(const CVector& pos, const CVector& angle, const CVector& size);
	// デストラクタ
	~CPushSwitch();


	
	// スイッチが押されたかどうか
	bool IsOnSwtch();

	// 番号の取得
	int GetNumber();

	// 描画処理
	void Render() override;

private:
	CModel* mpModel;	// スイッチのモデルデータ
	CColliderMesh* mpColliderMesh;	// スイッチのコリジョンデータ

	bool mSwitch;	// スイッチがオンかオフか
	int mNumber;	// 番号
	int mNum;		// 何個作られたか

};