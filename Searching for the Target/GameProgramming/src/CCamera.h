#ifndef CCAMERA_H
#define CCAMERA_H

class CCamera
{
public:
	//表示エリアの設定
	//Start(左座標、右座標、下座標、上座標)
	static void Start(double left, double right, double bottom, double top);
	static void End();
};

#endif // !CCAMERA_H

