#ifndef CGAME_H
#define CGAME_H

#include "CPlayer.h"
#include "CSlope.h"
#include "CSwitch.h"
#include "CMoveWall.h"
#include "CMoveFloor.h"
#include "CFloor.h"
#include "CObject.h"
#include "CUi.h"

class CGame
{
public:
	//�f�t�H���g�R���X�g���N�^
	CGame();
	//�f�X�g���N�^
	~CGame();
	//�X�^�[�g����
	void Start();
	//�X�V����
	void Update();
	//�Q�[���N���A����
	void Clear();
	//�Q�[���N���A����
	bool IsClear();
	//�Q�[���I�[�o�[����
	void Over();
	//�Q�[���I�[�o�[����
	bool IsOver();

	//���f���r���[�s��̎擾
	static const CMatrix& ModelViewInverse();
	
private:
	//UI�N���X�̃|�C���^
	CUi* mpUi;

	//�v���C���[
	CPlayer* mpPlayer;
	//��
	//CSlope mSlope;
	//CSlope mSlope2;
	//�X�C�b�`
	//CSwitch mSwhith;
	//������
	//CMoveWall mMoveWall;
	//������
	//CMoveFloor mMoveFloor;

	//���f������R���C�_�𐶐�
	//��̃R���C�_
	//CColliderMesh mCSlope;
	//CColliderMesh mCSlope2;
	//���̃R���C�_
	//CColliderMesh* mCFloor;
	//�I�u�W�F�N�g�̃R���C�_
	//CColliderMesh* mCObject;

	//���f���r���[�t�s��
	static CMatrix mModelViewInverse;
};

#endif // !CGAME_H
