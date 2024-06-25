#include "CPlayer.h"
#include "CTaskManager.h"
#include "CApplication.h"
#include "CColliderLine.h"
#include <math.h>

#define ROTATION_YV CVector(0.0f,1.0f,0.0f)	//Y�̉�]���x
#define ROTATION_YX CVector(1.0f,0.0f,0.0f)	//X�̉�]���x�@�폜�\��
#define VELOCITY CVector(0.0f,0.0f,0.3f)	//�ړ����x

#define MOS_POS_X 400	//�}�E�X���W��X�␳
#define MOS_POS_Y 300	//�}�E�X���W��Y�␳

#define AIM_POS 10		//�Ə��̕␳�l

#define GRAVITY (0.1f)	//�d�͉����x

//�f�t�H���g�R���X�g���N�^
CPlayer::CPlayer()
	: mSphere(this, &mMatrix, CVector(0.0f, 1.0f, 0.0f), 2.0f)
	//: mLine(this, &mMatrix, CVector(0.0f, 0.0f, 0.5f), CVector(0.0f, 0.0f, -0.5f))
	//, mLine2(this, &mMatrix, CVector(0.0f, 2.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f))
	//, mLine3(this, &mMatrix, CVector(1.1f, 0.0f, 0.0f), CVector(-1.1f, 0.0f, 0.0f))
	, mLine4(this, &mMatrix, CVector(0.0f, 2.0f, 3.0f), CVector(0.0f, -2.1f, 0.0f))
	, mBulletFlag(nullptr)
	, mCursorX(0)
	, mCursorY(0)
	, mFx(0)
	, mFy(0)
{

}

//�R���X�g���N�^
CPlayer::CPlayer(const CVector& pos, const CVector& rot
	, const CVector& scale)
	: mBulletFlag(nullptr)
	, mCursorX(0)
	, mCursorY(0)
	, mFx(0)
	, mFy(0)
	
{
	CTransform::Update(pos, rot, scale);	//�s��̍X�V
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
	mCursorX -= MOS_POS_X;
	mCursorY = MOS_POS_Y - mCursorY;

	//�v���C���[�ƃ}�E�X���W�̍������߂�
	mCursorX -= mPosition.GetX();
	mCursorY -= mPosition.GetY();

	//printf("�v���C���[�ƃ}�E�X���W�̍�:%d, %d\n", mCursorX, mCursorY);

	}

	//X���ňړ�
	if (mCursorX < -100 && mFx > -4.0f)
	{
		//�J���������_�����ֈړ�
		mFx -= 0.1f;
	}
	else if (mCursorX > 100 && mFx < 4.0f)
	{
		//�J���������_���E�ֈړ�
		mFx += 0.1f;
	}

	//Y���ňړ�
	if (mCursorY < -100 && mFy > -4.0f)
	{
		//�J���������_�����ֈړ�
		mFy -= 0.1f;
	}
	else if (mCursorY > 100 && mFy < 4.0f)
	{
		//�J���������_����ֈړ�
		mFy += 0.1f;
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

	//�m�F�p X����](��)
	//if (mInput.Key('K'))
	//{
	//	mRotation = mRotation + ROTATION_YX;
	//}
	//�m�F�p X����](��)
	//if (mInput.Key('I'))
	//{
	//	mRotation = mRotation - ROTATION_YX;
	//}

	//�d��
	mPosition = mPosition - CVector(0.0f, GRAVITY, 0.0f);

	//�X�y�[�X���N���b�N�Œe����
	//���������͂��Ă��e��1�������o�Ȃ��悤�ɂ���
	if (mBulletFlag == false)
	{
		//�X�y�[�X or ���N���b�N�Œe�𔭎�
		if (mInput.Key(VK_SPACE) || mInput.Key(WM_LBUTTONDOWN))
		{
			CBullet* bullet = new CBullet();
			bullet->SetTag(ETag::EBULLET);
			bullet->SetModel(CBullet::GetModelBullet());
			bullet->SetScale(CVector(10.0f, 10.0f, 10.0f));
			bullet->SetPosition(CVector(0.0f, 1.75f, 3.0f) * mMatrix);
			bullet->SetRotation(mRotation + CVector(-mCursorY / AIM_POS, -mCursorX / AIM_POS, 0.0f));
			bullet->Update();
			mBulletFlag = true;
		}
	}
	//�X�y�[�X or ���N���b�N��������ĂȂ��Ƃ��t���O��true�ɂ���
	if (!mInput.Key(VK_SPACE) && !mInput.Key(WM_LBUTTONDOWN) && mBulletFlag == true)
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
			CVector adjust;	//�����l
			//�R���C�_��m��o���Փ˂��Ă��邩����
			if (CCollider::CollisionTriangleSphere(o, m, &adjust))
			{
				//�ʒu�̍X�V
				mPosition = mPosition + adjust;

				//�^�O��nullptr�̃R���C�_�[�͔��肵�Ȃ�
				if (o->GetParent() != nullptr) {
					//��̃^�O�����Ă��邩����
					if (o->GetParent()->GetTag() == CCharacter::ETag::ESLOPE)
					{
						CVector ajustRote;
						//�Ζʂ̊p�x�����߂�
						CCollider::Slope(m, o, &ajustRote);

						//�m�F�p
						//printf("�C���O�F%10f %10f %10f\n",
						//	mRotation.GetX(), mRotation.GetY(), mRotation.GetZ());

						//��ɓ���������Ζʂɍ��킹�ĉ�]
						SetRotation(ajustRote);

						//�m�F�p
						//printf("�C���l�F%10f %10f %10f\n",
						//	ajustRote.GetX(), ajustRote.GetY(), ajustRote.GetZ());
						//printf("�C����F%10f %10f %10f\n",
						//	mRotation.GetX(), mRotation.GetY(), mRotation.GetZ());
					}
				}
			}
			//�s��̍X�V
			CTransform::Update();
		}
		break;

	case CCollider::EType::ELINE:	//���R���C�_
		//����̃R���C�_���O�p�R���C�_�̎�
		if (o->GetType() == CCollider::EType::ETRIANGLE)
		{
			CVector adjust;	//�����p�x�N�g��
			//�O�p�`�Ɛ����̏Փ˔���
			if (!CCollider::CollisionTriangleLine(o, m, &adjust))
			{

				//printf("HIT");

				//if (m == &mLine || m == &mLine2 || m == &mLine3)
					//�ʒu�̍X�V
				//	mPosition = mPosition + adjust;

				//if (m == &mLine4)
				//{
					
					//mRotation = mRotation - CVector(-0.1f, 0.0f, 0.0f);
				//}

				/*
				CVector ajustRote;
				//�Ζʂ̊p�x�����߂�
				CCollider::Slope(m, o, &ajustRote);

				printf("�C���O�F%10f %10f %10f\n",
					mRotation.GetX(), mRotation.GetY(), mRotation.GetZ());

				//��ɓ����������]
				//mRotation = mRotation + ajustRote;

				//��ɓ���������Ζʂɍ��킹�ĉ�]
				SetRotation(ajustRote);

				//printf("�C���l�F%10f %10f %10f\n",
				//	ajustRote.GetX(), ajustRote.GetY(), ajustRote.GetZ());

				printf("�C����F%10f %10f %10f\n",
					mRotation.GetX(), mRotation.GetY(), mRotation.GetZ());
				*/
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
	//mLine.ChangePriority();
	//mLine2.ChangePriority();
	//mLine3.ChangePriority();
	mLine4.ChangePriority();

	mSphere.ChangePriority();
	//�Փˏ��������s
	//CCollisionManager::GetInstance()->Collision(&mLine, COLLISIONRANGE);
	//CCollisionManager::GetInstance()->Collision(&mLine2, COLLISIONRANGE);
	//CCollisionManager::GetInstance()->Collision(&mLine3, COLLISIONRANGE);
	CCollisionManager::GetInstance()->Collision(&mLine4, COLLISIONRANGE);
	CCollisionManager::GetInstance()->Collision(&mSphere, COLLISIONRANGE);
	
}

//�J�[�\����X���W���擾
float CPlayer::GetFx()
{
	return mFx;
}
//�J�[�\����Y���W���擾
float CPlayer::GetFy()
{
	return mFy;
}