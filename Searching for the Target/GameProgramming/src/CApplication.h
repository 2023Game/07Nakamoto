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

private:
	CPlayer mPlayer;	
	
	//���f���N���X�̃C���X�^���X�쐬
	CModel mModel;
	//�e�̃��f��
	CModel mModelBullet;
	//�I�̃��f��
	CModel mModelTarget;
	//�w�i���f��
	CModel mBackGround;

	//�O�p�R���C�_�̍쐬
	//�폜�@CColliderTriangle mColliderTriangle;

	//���f������R���C�_�𐶐�
	CColliderMesh mColliderMesh;

	CInput mInput;
	CVector mEye;
};