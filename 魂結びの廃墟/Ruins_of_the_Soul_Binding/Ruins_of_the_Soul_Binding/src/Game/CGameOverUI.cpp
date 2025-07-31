#include "CGameOverUI.h"
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
CGameOverUI::CGameOverUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// �Q�[���N���A��ʂ̔w�i�C���[�W�𐶐�
	mpGameOverBg = new CImage
	(
		"UI\\game_over.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	// [�R���e�B�j���[]�{�^���𐶐�
	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 350.0f),
		CVector2(224.0f, 50.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �R���e�B�j���[�̉摜��ǂݍ���
	btn1->LoadButtonImage("UI\\Continue0.png", "UI\\Continue1.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn1->SetOnClickFunc(std::bind(&CGameOverUI::OnClickStart, this));
	// �{�^���͍ŏ��͖��������āA�X�P�[���l��0�ɂ��Ă���
	btn1->SetEnable(false);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn1);


	// [�^�C�g����]�{�^���𐶐�
	CExpandButton* btn2 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 450.0f),
		CVector2(224.0f, 50.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �^�C�g���ւ̉摜��ǂݍ���
	btn2->LoadButtonImage("UI\\title_he0.png", "UI\\title_he1.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn2->SetOnClickFunc(std::bind(&CGameOverUI::OnClickTitle, this));
	// �{�^���͍ŏ��͖��������āA�X�P�[���l��0�ɂ��Ă���
	btn2->SetEnable(false);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn2);

	// [�����]�{�^���𐶐�
	CExpandButton* btn3 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 550.0f),
		CVector2(180.0f, 52.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �^�C�g���ւ̉摜��ǂݍ���
	btn3->LoadButtonImage("UI\\Title\\title_quit0.png", "UI\\Title\\title_quit1.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn3->SetOnClickFunc(std::bind(&CGameOverUI::OnClickQuit, this));
	// �{�^���͍ŏ��͖��������āA�X�P�[���l��0�ɂ��Ă���
	btn3->SetEnable(false);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn3);
}

// �f�X�g���N�^
CGameOverUI::~CGameOverUI()
{
	SAFE_DELETE(mpGameOverBg);

	int size = mButtons.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButtons[i];
		mButtons[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButtons.clear();
}

// �Q�[���I�[�o�[��ʏI����
bool CGameOverUI::IsEnd() const
{
	return mIsEnd;
}

// �Q�[�����R���e�B�j���[���邩
bool CGameOverUI::IsContinue() const
{
	// �I�����ڂ�1�ڂȂ�΁A���g���C
	return mSelectIndex == 0;
}

// �^�C�g���֖߂邩
bool CGameOverUI::IsTitle()
{
	// �I�����ڂ�2�ڂȂ�΁A�^�C�g����
	return mSelectIndex == 1;
}

// �Q�[�����I�����邩
bool CGameOverUI::IsExitGame() const
{
	// �I�����ڂ�3�ڂȂ�΁A�Q�[���I��
	return mSelectIndex == 2;
}

// �X�V
void CGameOverUI::Update()
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

	mpGameOverBg->Update();
	for (CButton* btn : mButtons)
	{
		btn->Update();
	}
}

// �`��
void CGameOverUI::Render()
{
	// �w�i�`��
	mpGameOverBg->Render();
	// ���j���[�{�^����\��
	for (CButton* btn : mButtons)
	{
		btn->Render();
	}
}

// �ҋ@���
void CGameOverUI::UpdateIdle()
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
void CGameOverUI::UpdateFadeOut()
{
}

// ��Ԑ؂�ւ�
void CGameOverUI::ChangeState(EState state)
{
	if (state == mState) return;
	mState = state;
	mStateStep = 0;
	mElapsedTime = 0.0f;
}

// [�R���e�B�j���[]�N���b�N���̃R�[���o�b�N�֐�
void CGameOverUI::OnClickStart()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

// [�^�C�g����]�N���b�N���̃R�[���o�b�N�֐�
void CGameOverUI::OnClickTitle()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}

// [�����]�N���b�N���̃R�[���o�b�N�֐�
void CGameOverUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 2;
	mIsEnd = true;
}
