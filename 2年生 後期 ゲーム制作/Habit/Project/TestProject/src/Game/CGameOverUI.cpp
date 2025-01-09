#include "CGameOverUI.h"
#include "CFont.h"
#include "CText.h"
#include "CImage.h"
#include "Maths.h"
#include "CInput.h"
#include "CFade.h"
#include "CExpandButton.h"
#include "Easing.h"

// �uCLICK TO START�v�̓_�Ŏ���
#define START_TEXT_BLINK_TIME 1.0f
// �uCLICK TO START�v�̓_�ŊԊu
#define START_TEXT_INTERVAL_TIME 3.0f
// �^�C�g�����j���[�̃A�j���[�V��������
#define OPEN_ANIM_TIME 0.25f
// �^�C�g�����j���[�̃A�j���[�V������̑҂�����
#define OPENED_WAIT_TIME 0.5f

// �R���X�g���N�^
CGameOverUI::CGameOverUI()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault)
	, mState(EState::eIdle)
	, mStateStep(0)
	, mElapsedTime(0.0f)
	, mIsEnd(false)
{
	// �Q�[���I�[�o�[��ʂ̔w�i�C���[�W�𐶐�
	mpGameOverBg = new CImage
	(
		"UI\\gameover.png",
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);

	// [START]�{�^���𐶐�
	CExpandButton* btn1 = new CExpandButton
	(
		CVector2(WINDOW_WIDTH * 0.5f, 450.0f),
		CVector2(181.0f, 47.0f),
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	// �{�^���̉摜��ǂݍ���
	btn1->LoadButtonImage("UI\\title_start0.png", "UI\\title_start1.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn1->SetOnClickFunc(std::bind(&CGameOverUI::OnClickStart, this));
	// �{�^���͍ŏ��͖��������āA�X�P�[���l��0�ɂ��Ă���
	btn1->SetEnable(false);
	//btn1->SetScale(0.0f);
	// �{�^�����X�g�ɒǉ�
	mButtons.push_back(btn1);
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

// �R���e�B�j���[���邩
bool CGameOverUI::IsContinue() const
{
	// �I�����ڂ�1�ڂȂ�΁A���g���C
	return mSelectIndex == 0;
}

// �Q�[�����I�����邩
bool CGameOverUI::IsExitGame() const
{
	// �I�����ڂ�2�ڂȂ�΁A�Q�[���I��
	return mSelectIndex == 1;
}

// �X�V����
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

// �`�揈��
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

// [START]�N���b�N���̃R�[���o�b�N�֐�
void CGameOverUI::OnClickStart()
{
	if (mIsEnd) return;

	mSelectIndex = 0;
	mIsEnd = true;
}

// [QUIT]�N���b�N���̃R�[���o�b�N�֐�
void CGameOverUI::OnClickQuit()
{
	if (mIsEnd) return;

	mSelectIndex = 1;
	mIsEnd = true;
}