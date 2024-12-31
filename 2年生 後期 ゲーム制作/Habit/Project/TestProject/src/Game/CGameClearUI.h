#pragma once
#include "CTask.h"

class CFont;
class CImage;
class CExpandButton;

class CGameClearUI : public CTask
{
public:
	// �R���X�g���N�^
	CGameClearUI();
	// �f�X�g���N�^
	~CGameClearUI();

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
	bool mIsEnd;		// �Q�[���N���A��ʏI���t���O
	int mSelectIndex;	// ���ݑI�����Ă��鍀��

	CImage* mpGameClearBg;	// �Q�[���N���A�w�i�C���[�W
	std::vector<CExpandButton*> mButtons;
};