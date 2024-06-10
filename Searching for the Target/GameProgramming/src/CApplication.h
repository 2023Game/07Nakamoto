#pragma once
#include "CModel.h"
#include "CInput.h"
#include "CCharacter.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CColliderTriangle.h"
#include "CColliderMesh.h"

class CApplication
{
public:
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();

	//���f���r���[�s��̎擾
	static const CMatrix& ModelViewInverse();

private:
	CPlayer mPlayer;	
	
	//���f���N���X�̃C���X�^���X�쐬
	CModel mModel;
	//�e�̃��f��
	CModel mModelBullet;
	//�I�̃��f��
	CModel mModelTarget;
	//�}�b�v�̃��f��
	CModel mModelMap;
	//�w�i���f��
	CModel mBackGround;

	//���f������R���C�_�𐶐�
	CColliderMesh mColliderMesh;
	CColliderMesh mColliderMesh2;

	CInput mInput;
	CVector mEye;

	//���f���r���[�t�s��
	static CMatrix mModelViewInverse;
};