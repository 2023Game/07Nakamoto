#include "CGameClearUI.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// ���j���[�̃A�j���[�V��������
#define OPEN_ANIM_TIME 0.25f
// ���j���[�̃A�j���[�V������̑҂�����
#define OPENED_WAIT_TIME 0.5f

// �R���X�g���N�^
CGameClearUI::CGameClearUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// �Q�[���N���A��ʂ̔w�i�C���[�W�𐶐�
	mpGameClearBg = new CImage
	(
		"UI\\clear.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	// [�^�C�g����]�{�^���𐶐�
	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 450.0f),
		CVector2(224.0f, 50.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �^�C�g���ւ̉摜��ǂݍ���
	btn1->LoadButtonImage("UI\\title_he0.png", "UI\\title_he1.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn1->SetOnClickFunc(std::bind(&CGameClearUI::OnClickStart, this));
	// �{�^���͍ŏ��͖��������āA�X�P�[���l��0�ɂ��Ă���
	btn1->SetEnable(false);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn1);

	// [�����]�{�^���𐶐�
	CExpandButton* btn2 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 550.0f),
		CVector2(180.0f, 52.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �^�C�g���ւ̉摜��ǂݍ���
	btn2->LoadButtonImage("UI\\Title\\title_quit0.png", "UI\\Title\\title_quit1.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn2->SetOnClickFunc(std::bind(&CGameClearUI::OnClickQuit, this));
	// �{�^���͍ŏ��͖��������āA�X�P�[���l��0�ɂ��Ă���
	btn2->SetEnable(false);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn2);

}

// �f�X�g���N�^
CGameClearUI::~CGameClearUI()
{
	SAFE_DELETE(mpGameClearBg);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

// �Q�[���N���A��ʏI����
bool CGameClearUI::IsEnd() const
{
	return mIsEnd;
}

// �^�C�g����ʂɂ��邩
bool CGameClearUI::IsTitle() const
{
	// �I�����ڂ�1�ڂȂ�΁A�^�C�g�����
	return mSelectIndex == 0;
}

// �Q�[�����I�����邩
bool CGameClearUI::IsExitGame() const
{
	// �I�����ڂ�2�ڂȂ�΁A�Q�[���I��
	return mSelectIndex == 1;
}

// �X�V����
void CGameClearUI::Update()
{
	switch (mState)
	{
		// �ҋ@���
	case EState::eIdle:
		UpdateIdle();
		break;
		// �t�F�[�h�A�E�g
	case EState::eFadeOut:
		UpdateFadeOut();
		break;
	}

	mpGameClearBg->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

// �`�揈��
void CGameClearUI::Render()
{
	// �w�i�`��
	mpGameClearBg->Render();
	// ���j���[�{�^����\��
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

// �ҋ@���
void CGameClearUI::UpdateIdle()
{
	switch (mStateStep)
	{
		// �X�e�b�v0�F���j���[�̓���A�j���[�V����
	case 0:
		if (mElapsedTime < OPEN_ANIM_TIME)
		{
			// �X�P�[���l����U1.0���傫�����āA1.0�֖߂�C�[�W���O�A�j���[�V����
			float scale = Easing::BackOut(mElapsedTime, OPEN_ANIM_TIME, 0.0f, 1.0f, 2.0f);
			for (CExpandButton* btn : mButtons)
			{
				btn->SetScale(scale);
			}
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			for (CExpandButton* btn : mButtons)
			{
				btn->SetScale(1.0f);
			}
			mStateStep++;
			mElapsedTime = 0.0f;
		}
		break;
		// �X�e�b�v1�F���j���[�����̑҂�
	case 1:
		if (mElapsedTime < OPENED_WAIT_TIME)
		{
			mElapsedTime += Times::DeltaTime();
		}
		else
		{
			// ��莞�ԑ҂�����A�{�^�����I���ɂ��ă^�b�`�ł���悤�ɂ���
			// �i��^�b�`��h�����߂̑҂����ԁj
			for (CExpandButton* btn : mButtons)
			{
				btn->SetEnable(true);
			}
			ChangeState(EState::eIdle);
		}
		break;
	}
}

// �t�F�[�h�A�E�g
void CGameClearUI::UpdateFadeOut()
{
}

// ��Ԑ؂�ւ�
void CGameClearUI::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// [�^�C�g����]�N���b�N���̃R�[���o�b�N�֐�
void CGameClearUI::OnClickStart()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

// [�����]�N���b�N���̃R�[���o�b�N�֐�
void CGameClearUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}
