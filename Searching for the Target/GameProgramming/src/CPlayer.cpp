#include "CPlayer.h"
#include "CTaskManager.h"
#include "CApplication.h"
#include "CColliderLine.h"
#include <math.h>

#include "CColliderHitManager.h"

#define ROTATION_YV CVector(0.0f,1.0f,0.0f)	//Y�̉�]���x
#define ROTATION_YX CVector(1.0f,0.0f,0.0f)	//X�̉�]���x�@�폜�\��
#define VELOCITY CVector(0.0f,0.0f,0.3f)	//�ړ����x

#define MOS_POS_X 400	//�}�E�X���W��X�␳
#define MOS_POS_Y 300	//�}�E�X���W��Y�␳

#define MOS_POS_FSX 960	//�}�E�X���W��X�␳
#define MOS_POS_FSY 540	//�}�E�X���W��Y�␳

#define AIM_POS 40		//�Ə��̕␳�l

#define GRAVITY (0.1f)	//�d�͉����x

CPlayer::CPlayer()
	: mBulletFlag(false)
	, mAir(false)
	, mCursorX(0)
	, mCursorY(0)
{

}

//�R���X�g���N�^
CPlayer::CPlayer(const CVector& pos, const CVector& rot
	, const CVector& scale, CModel* model)
	: mSphere(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), 2.0f)
	, mBulletFlag(false)
	, mAir(false)
	, mCursorX(0)
	, mCursorY(0)
	
{
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	mpModel = model;
}

//�X�V����
void CPlayer::Update()
{
	//�}�E�X�J�[�\�����W�̎擾
	CInput::GetMousePos(&mCursorX, &mCursorY);

	//�}�E�X�N���b�N���o
	if (mInput.Key(VK_LBUTTON))
	{
		//�}�E�X���W�R���\�[���ɏo��
		//printf("�}�E�X�̍��W:%d, %d\n", mCursorX, mCursorY);

		//�Q�[����ʒ��S����̍��W�֕ϊ�����
		mCursorX -= MOS_POS_FSX;
		mCursorY = MOS_POS_FSY - mCursorY;

		//printf("�Q�[����ʒ��S����̍��W�֕ϊ�����:%d, %d\n", mCursorX, mCursorY);

		//�v���C���[�ƃ}�E�X���W�̍������߂�
		mCursorX -= mPosition.GetX();
		mCursorY -= mPosition.GetY();

		//printf("�v���C���[�ƃ}�E�X���W�̍�:%d, %d\n", mCursorX, mCursorY);
	}

	//D�L�[���͂ŉE��]
	if (mInput.Key('D'))
	{
		//Y���̉�]�l�����Z
		mRotation = mRotation - ROTATION_YV;
	}
	//A�L�[���͂ō���]
	if (mInput.Key('A'))
	{
		//Y���̉�]�l�����Z
		mRotation = mRotation + ROTATION_YV;
	}
	//W�L�[���͂őO�i
	if (mInput.Key('W'))
	{
		//Z�������̒l����]�����ړ�������
		mPosition = mPosition + VELOCITY * mMatrixRotate;
	}
	//S�L�[���͂Ō��
	if (mInput.Key('S')) 
	{
		//Z�������̒l����]�����ړ�������
		mPosition = mPosition - VELOCITY * mMatrixRotate;
	}

	//�󒆂ɂ���Ƃ�
	//�d��
	if (mAir)
	{
		mPosition = mPosition - CVector(0.0f, GRAVITY, 0.0f);
		printf("��\n");
	}
	
	mAir = true;

	//�X�y�[�X���N���b�N�Œe����
	//���������͂��Ă��e��1�������o�Ȃ��悤�ɂ���
	if (mBulletFlag == false)
	{
		//�X�y�[�X or ���N���b�N�Œe�𔭎�
		if (mInput.Key(WM_LBUTTONDOWN))
		{
			CBullet* bullet = new CBullet();
			bullet->SetModel(CBullet::GetModelBullet());
			bullet->SetScale(CVector(5.0f, 5.0f, 5.0f));
			bullet->SetPosition(CVector(0.0f, 1.75f, 3.0f) * mMatrix);
			bullet->SetRotation(mRotation + CVector(-mCursorY / AIM_POS, -mCursorX / AIM_POS, 0.0f));
			bullet->Update();
			mBulletFlag = true;
		}
	}
	//���N���b�N��������ĂȂ��Ƃ��t���O��true�ɂ���
	if (!mInput.Key(WM_LBUTTONDOWN) && mBulletFlag == true)
	{
		mBulletFlag = false;
	}

	//�ϊ��s��̍X�V
	CTransform::Update();
}

//�Փˏ���
void CPlayer::Collision(CCollider* m, CCollider* o)
{
	//���g�̃R���C�_�^�C�v�̔���
	switch (m->GetType())
	{
	case CCollider::EType::ESPHERE:	//���R���C�_
		//����̃R���C�_���O�p�R���C�_�̎�
		if (o->GetType() == CCollider::EType::ETRIANGLE)
		{
			CVector adjust, n;	//�����l
			//�O�p�`�Ƌ��̏Փ˔���
			if (CCollider::CollisionTriangleSphere(o, m, &adjust, &n))
			{
				//���������O�p�R���C�_���ϒ��z��Ɋi�[
				mCollisionManager.AddColliders(o);

				//�߂荞�܂Ȃ��ʒu�ɖ߂�
				mPosition = mPosition + adjust;

				//�O�p�ʂɐ����ȏd��
				mPosition = mPosition - CVector(n.GetX() / 10, n.GetY() / 10, n.GetZ() / 10);

				mAir = false;
			}
			//�s��̍X�V
			CTransform::Update();
		}
		break;
	}
}

//�Փˏ���
void CPlayer::Collision()
{
	//�R���C�_�̗D��x�̕ύX
	mSphere.ChangePriority();

	//�Փˏ��������s
	CCollisionManager::GetInstance()->Collision(&mSphere, COLLISIONRANGE);
}

//�R���C�_�̎擾
CCollider *CPlayer::GetCollider()
{
	return &mSphere;
}

//static�ϐ��̒�`
CModel CPlayer::mModelPlayer;
//�v���C���[�̃��f�����擾����
CModel* CPlayer::GetModelPlayer()
{
	return &mModelPlayer;
}

//static�ϐ��̒�`
CPlayer* CPlayer::mpInstance = nullptr;
//�C���X�^���X�̎擾
CPlayer* CPlayer::GetInstance()
{
	if (mpInstance == nullptr)
	{
		mpInstance = new CPlayer();
	}
	return mpInstance;
}