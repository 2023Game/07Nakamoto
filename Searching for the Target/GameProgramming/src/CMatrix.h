#ifndef CMATRIX_H
#define CMATRIX_H

/*
}gNXNX
4s4ńĚsńf[^đľ¤
*/
class CMatrix {
public:
	//\ŚmFp
	//4~4Ěsńđ`ćoÍ
	void Print();

	//ftHgRXgN^
	CMatrix();
	//PĘsńĚěŹ
	CMatrix Identity();

	/// <summary>
	/// gĺkŹsńĚěŹ
	/// </summary>
	/// <param name="sx">{ŚX</param>
	/// <param name="sy">{ŚY</param>
	/// <param name="sz">{ŚZ</param>
	/// <returns></returns>
	CMatrix SetScale(float sx, float sy, float sz);

	//sńćž
	float* GetM() const;

	//sńlĚćž
	//M(s,ń)
	//mM[s][ń]đćž
	float GetM(int r, int c)const;

	/// <summary>
	/// sńlĚăü
	/// </summary>
	/// <param name="row">s</param>
	/// <param name="col">ń</param>
	/// <param name="value">l</param>
	void SetM(int row, int col, float value);

	/// <summary>
	/// ń]sń(X˛)
	/// </summary>
	/// <param name="degree">px</param>
	/// <returns></returns>
	CMatrix SetRotateX(float degree);

	/// <summary>
	/// ń]sń(Y˛)
	/// </summary>
	/// <param name="degree">px</param>
	/// <returns></returns>
	CMatrix SetRotateY(float degree);

	/// <summary>
	/// ń]sń(Z˛)
	/// </summary>
	/// <param name="degree">px</param>
	/// <returns></returns>
	CMatrix SetRotateZ(float degree);

	/// <summary>
	/// ÚŽsń
	/// </summary>
	/// <param name="mx">ÚŽĘX</param>
	/// <param name="my">ÚŽĘY</param>
	/// <param name="mz">ÚŽĘZ</param>
	/// <returns></returns>
	CMatrix SetTranslate(float mx, float my, float mz);

	//tsńćž
	CMatrix GetTranspose() const;

	// *ZqĚI[o[[h
	//CMatrix * CMatrix ĚZĘđÔˇ
	const CMatrix operator* (const CMatrix& m)const;

private:
	//4~4Ěsńf[^đÝč
	float mM[4][4];
};

#endif // !CMATRIX_H

