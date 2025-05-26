#include "CLDoor.h"
#include "Maths.h"
#include "CInput.h"

#define MOVE_POS 9.9f	// �ړ�����
#define MOVE_TIME 5.0f	// �ړ�����

// �R���X�g���N�^
CLDoor::CLDoor(const CVector& pos, const CVector& rot)
	: mDefaultPos(pos)
	, mOpen(false)
	, mElapsedTime(0.0f)
{
	// ���̃��f���f�[�^�̎擾
	mpL_Door = CResourceManager::Get<CModel>("LeftDoor");
	// ���̃R���C�_�[�f�[�^�̎擾
	mpL_DoorCol = CResourceManager::Get<CModel>("LeftDoorCol");

	// ���̃R���C�_�[����
	mpL_DoorColliderMesh = new CColliderMesh(this, ELayer::eInteractObj, mpL_DoorCol, true);

	// �ʒu�ƌ�����ݒ�
	Position(mDefaultPos);
	Rotate(rot);

	mInteractStr = "�܂��Ă���";
}

// �f�X�g���N�^
CLDoor::~CLDoor()
{
	// �R���C�_�[�̍폜
	if (mpL_DoorColliderMesh != nullptr)
	{
		delete mpL_DoorColliderMesh;
		mpL_DoorColliderMesh = nullptr;
	}
}

// ���ׂ�
void CLDoor::Interact()
{
	mOpen = !mOpen;
	mInteractStr = mOpen ? "�܂��Ă���" : "�J���Ă���";
}

// �X�V����
void CLDoor::Update()
{
	// �܂��Ă���Ƃ�
	if (!mOpen)
	{
		if (CInput::PushKey('E'))
		{

			float per = mElapsedTime / MOVE_TIME;
			Position(mDefaultPos + CVector(MOVE_POS, 0.0f, 0.0f) * sinf(M_PI * 2.0f * per));

			mElapsedTime += 1.0f / 60.0f;
			if (mElapsedTime >= MOVE_TIME)
			{
				mElapsedTime -= MOVE_TIME;
			}

			//Position(mDefaultPos + CVector(MOVE_POS, 0.0f, 0.0f));
			//mOpen = true;
		}
		
	}
	// �J���Ă���Ƃ�
	else
	{
		if (CInput::PushKey('E'))
		{
			Position(mDefaultPos);
			//mOpen = false;
		}

		//float per = mElapsedTime / MOVE_TIME;
		//Position(mDefaultPos + mMoveVec * sinf(M_PI * 2.0f * per));

		//mElapsedTime += 1.0f / 60.0f;
		//if (mElapsedTime >= MOVE_TIME)
		//{
		//	mElapsedTime -= MOVE_TIME;
		//}
	}

}

// �`�揈��
void CLDoor::Render()
{
	mpL_Door->Render(Matrix());
}
