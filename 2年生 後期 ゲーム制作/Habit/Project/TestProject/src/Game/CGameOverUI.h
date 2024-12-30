#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;

class CGameOverUI : public CTask
{
public:
	// �R���X�g���N�^
	CGameOverUI();
	// �f�X�g���N�^
	~CGameOverUI();

	// �Q�[���I�[�o�[��ʏI����
	bool IsEnd() const;
	// �Q�[�����R���e�B�j���[���邩
	bool IsContinue() const;
	// �Q�[�����I�����邩
	bool IsExitGame() const;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// �ҋ@���
	void UpdateIdle();
	// ���j���[�I��
	void UpdateSelect();
	// �t�F�[�h�A�E�g
	void UpdateFadeOut();

	// �V�[���̏��
	enum class EState
	{
		eIdle,		// �ҋ@���
		eFadeOut,	// �t�F�[�h�A�E�g
	};

	// ��Ԑ؂�ւ�
	void ChangeState(EState state);

	// [START]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickStart();
	// [QUIT]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickQuit();

	EState mState;		// ���݂̏��
	int mStateStep;		// ��ԓ��ł̃X�e�b�v�Ǘ��p
	float mElapsedTime;	// �o�ߎ��Ԍv���p
	bool mIsEnd;		// �Q�[���I�[�o�[��ʏI���t���O
	int mSelectIndex;	// ���ݑI�����Ă��鍀��

	CImage* mpGameOverBg;	// �Q�[���I�[�o�[�w�i�C���[�W
	std::vector<CExpandButton*> mButtons;
};