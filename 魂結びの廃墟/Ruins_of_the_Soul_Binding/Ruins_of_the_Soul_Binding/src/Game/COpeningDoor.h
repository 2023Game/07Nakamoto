#pragma once
#include "CDoorBase.h"
class CRoom;

class COpeningDoor : public CDoorBase
{
public:
	// �R���X�g���N�^
	COpeningDoor(const CVector& pos, const CVector& angle, const CVector& openAngle, CRoom* room);
	// �f�X�g���N�^
	~COpeningDoor();

	// �X�V����
	void Update() override;

private:
	CVector mCloseAngle;	// �����܂��Ă��鎞�̍��W
	CVector mOpenAngle;	// �����J���Ă��鎞�̍��W
	float mAnimTime;	// �J����
	float mElapsedTime;	// �o�ߎ��ԕۑ��p
};