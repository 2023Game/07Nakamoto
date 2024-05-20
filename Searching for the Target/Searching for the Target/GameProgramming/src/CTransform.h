#ifndef CTRANSFORM_H
#define CTRANSFORM_H

#include "CVector.h"
/*
トランスフォームクラス
位置、回転、拡縮、変換行列の基本クラス
*/
class CTransform {
public:
	//位置の取得
	const CVector& GetPosition() const;
	
	/// <summary>
	/// 位置の設定
	/// </summary>
	/// <param name="v">位置</param>
	void SetPosition(const CVector& v);

	//回転行列の取得
	const CMatrix& GetMatrixRotate() const;

	//回転値の取得
	const CVector& GetRotation() const;

	/// <summary>
	/// 回転値の設定
	/// </summary>
	/// <param name="v">回転値</param>
	void SetRotation(const CVector& v);

	/// <summary>
	/// 拡大縮小の設定
	/// </summary>
	/// <param name="v">拡大縮小値</param>
	void SetScale(const CVector& v);

	//合成行列の取得
	const CMatrix& GetMatrix() const;

	//行列更新処理
	void Update();

	/// <summary>
	/// 位置、回転、拡縮の更新
	/// </summary>
	/// <param name="pos">位置</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡縮</param>
	void Update(const CVector& pos, const CVector& rot, const CVector& scale);

protected:	//子クラスはアクセス可能
	CVector mPosition;			//位置
	CVector mRotation;			//回転
	CVector mScale;				//拡大縮小
	CMatrix mMatrixTranslate;	//平行移動行列
	CMatrix mMatrixRotate;		//回転行列
	CMatrix mMatrixScale;		//拡大縮小行列
	CMatrix mMatrix;			//合成行列
	
};

#endif // !CTRANSFORM_H

