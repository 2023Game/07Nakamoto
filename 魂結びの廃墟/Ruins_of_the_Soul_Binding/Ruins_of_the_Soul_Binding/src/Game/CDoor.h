#pragma once
#include "CDoorBase.h"
class CRoom;

class CDoor : public CDoorBase
{
public:
	// �R���X�g���N�^
	CDoor(const CVector& pos, const CVector& angle, const CVector& openPos, std::string modelName, std::string colName);
	// �f�X�g���N�^
	virtual ~CDoor();

	// �X�V����
	void Update() override;

private:
	CVector mClosePos;	// �����܂��Ă��鎞�̍��W
	CVector mOpenPos;	// �����J���Ă��鎞�̍��W
	float mAnimTime;	// �J����
	float mElapsedTime;	// �o�ߎ��ԕۑ��p
};