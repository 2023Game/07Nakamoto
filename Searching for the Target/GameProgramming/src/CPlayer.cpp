#include "CPlayer.h"
#include "CTaskManager.h"

#define ROTATION_YV CVector(0.0f,1.0f,0.0f)	//Y�̉�]���x
#define VELOCITY CVector(0.0f,0.0f,0.1f) //�ړ����x
#define VELOCITY_Y CVector(0.0f,2.0f,0.0f) //�W�����v���̈ړ���

//�W�����v�̉��̃t���O
//bool a = false;

//�R���X�g���N�^
CPlayer::CPlayer(const CVector& pos, const CVector& rot
	, const CVector& scale)
{
	CTransform::Update(pos, rot, scale);	//�s��̍X�V
}

//�X�V����
void CPlayer::Update()
{
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

	/* �Փ˔��肪�ł��Ă���W�����v�����\��
	//SPACE�L�[���͂ŃW�����v
	if (a == false) {
		if (mInput.Key(VK_SPACE))
		{
			//Y�������̒l���ړ�������
			mPosition = mPosition + VELOCITY_Y;
			a = true;
		}
	}

	if (a == true && !mInput.Key(VK_SPACE))
	{
		a = false;
	}
	*/

	//���N���b�N���͂Œe���ˁF�O�p�`�̒e
	/*if (mInput.Key(VK_SPACE) || mInput.Key(WM_LBUTTONDOWN))
	{
		CBullet* bullet = new CBullet();
		bullet->SetSize(0.2f, 1.5f);
		bullet->SetPosition(CVector(0.0f, 0.0f, 10.0f) * mMatrix);
		bullet->SetRotation(mRotation);
		bullet->Update();
	}*/

	//�ϊ��s��̍X�V
	CTransform::Update();
}