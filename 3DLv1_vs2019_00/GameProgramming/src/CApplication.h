#pragma once
#include "CModelX.h"

#include "CTexture.h"
#include "CBullet.h"
#include "CEnemy.h"
#include "CPlayer.h"
#include "CInput.h"
#include "CFont.h"
#include <vector>
#include "CColliderTriangle.h"

#include "CSound.h"
#include "CModel.h"
#include "CCharacter3.h"
#include "CTaskManager.h"
#include "CColliderMesh.h"
#include "CXCharacter.h"
#include "CXPlayer.h"
#include "CXEnemy.h"

class CApplication
{
private:
	//�L�����N�^�̃C���X�^���X
	CXCharacter mCharacter;
	//�L�����N�^�̃C���X�^���X
	CXPlayer mXPlayer;

	CMatrix mMatrix;

	//���f������R���C�_�𐶐�
	CColliderMesh mColliderMesh;

	//�O�p�R���C�_�̍쐬
	//�폜 CColliderTriangle mColliderTriangle;
	//�폜 CColliderTriangle mColliderTriangle2;
	
	//C5���f��
	CModel mModelC5;
	//CPlayer mPlayer;
	//CCharacter3 mCharacter;
	CModel mBackGround;	//�w�i���f��
	//���f���N���X�̃C���X�^���X�쐬
	CModel mModel;

	CSound mSoundBgm;	//BGM
	CSound mSoundOver;	//�Q�[���I�[�o�[

	//���f���r���[�̋t�s��
	static CMatrix mModelViewInverse;

	enum class EState
	{
		ESTART,	//�Q�[���J�n
		EPLAY,	//�Q�[����
		ECLEAR,	//�Q�[���N���A
		EOVER,	//�Q�[���I�[�o�[
	};
	EState mState;
//	CCharacter mRectangle;
	CPlayer* mpPlayer;
	static CTexture mTexture;
	CEnemy* mpEnemy;
//	CBullet* mpBullet;
	CInput mInput;
	CFont mFont;
	//CCharacter�̃|�C���^�̉ϒ��z��
//	std::vector<CCharacter*> mCharacters;

public:
	CModelX mModelX;

	CXEnemy mXEnemy;

	//���f���r���[�s��̎擾
	static const CMatrix& ModelViewInverse();

	static CTexture* Texture();
	//�ŏ��Ɉ�x�������s����v���O����
	void Start();
	//�J��Ԃ����s����v���O����
	void Update();
};