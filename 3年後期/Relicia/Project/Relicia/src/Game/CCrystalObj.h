#pragma once
#include "CInteractObject.h"
#include "CrystalData.h"

class CCollider;
class CModel;

// クリスタルの
class CCrystalObj : public CInteractObject
{
public:
	// コンストラクタ
	CCrystalObj(ElementType type, CVector pos);
	// デストラクタ
	~CCrystalObj();

private:
	ElementType mElementType;	// 属性の種類
	const CrystalData* mpCrystalData;	// クリスタルのデータのポインタ
	CModel* mpModel;			// クリスタルのモデルデータ

	CCollider* mpCollider;		// クリスタルのコライダー
};