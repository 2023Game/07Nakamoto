#ifndef CMATRIX_H
#define CMATRIX_H

/*
マトリクスクラス
4行4列の行列データを扱う
*/
class CMatrix {
public:
	//表示確認用
	//4×4の行列を描画出力
	void Print();

	//デフォルトコンストラクタ
	CMatrix();
	//単位行列の作成
	CMatrix Identity();

	/// <summary>
	/// 拡大縮小行列の作成
	/// </summary>
	/// <param name="sx">倍率X</param>
	/// <param name="sy">倍率Y</param>
	/// <param name="sz">倍率Z</param>
	/// <returns></returns>
	CMatrix SetScale(float sx, float sy, float sz);

	//行列取得
	float* GetM() const;

	//行列値の取得
	//M(行,列)
	//mM[行][列]を取得
	float GetM(int r, int c)const;

	/// <summary>
	/// 行列値の代入
	/// </summary>
	/// <param name="row">行数</param>
	/// <param name="col">列数</param>
	/// <param name="value">値</param>
	void SetM(int row, int col, float value);

	/// <summary>
	/// 回転行列(X軸)
	/// </summary>
	/// <param name="degree">角度</param>
	/// <returns></returns>
	CMatrix SetRotateX(float degree);

	/// <summary>
	/// 回転行列(Y軸)
	/// </summary>
	/// <param name="degree">角度</param>
	/// <returns></returns>
	CMatrix SetRotateY(float degree);

	/// <summary>
	/// 回転行列(Z軸)
	/// </summary>
	/// <param name="degree">角度</param>
	/// <returns></returns>
	CMatrix SetRotateZ(float degree);

	/// <summary>
	/// 移動行列
	/// </summary>
	/// <param name="mx">移動量X</param>
	/// <param name="my">移動量Y</param>
	/// <param name="mz">移動量Z</param>
	/// <returns></returns>
	CMatrix SetTranslate(float mx, float my, float mz);

	// *演算子のオーバーロード
	//CMatrix * CMatrix の演算結果を返す
	const CMatrix operator* (const CMatrix& m)const;

private:
	//4×4の行列データを設定
	float mM[4][4];
};

#endif // !CMATRIX_H

