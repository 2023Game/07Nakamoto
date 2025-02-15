#pragma once
#include "CUIBase.h"
#include "CTask.h"

class CText;
class CImage;
class CExpandButton;

// �A�C�e���I�𒆂̃��j���[
class CItemMenu : public CUIBase
{
public:
	// �C���X�^���X���擾
	static CItemMenu* Instance();

	// �R���X�g���N�g
	CItemMenu();
	// �f�X�g���N�^
	~CItemMenu();

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	static CItemMenu* spInstance;

	// �ҋ@���
	void UpdateIdle();
	// ���j���[���J��
	void UpdateOpen();
	// ���j���[�I��
	void UpdateSelect();

	// ���e�̎��
	enum class EState
	{
		eIdle,		// �ҋ@���
		eOpen,		// ���j���[���J��
		eSelect,	// ���j���[�I��
		eUse,		// �A�C�e�����g��
		eBack,		// �߂�
	};

	// ��Ԑ؂�ւ�
	void ChangeState(EState state);

	// [Use]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickUse();
	// [Back]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickBack();

	EState mState;	// ���݂̏��
	int mStateStep;	// ��ԓ��ł̃X�e�b�v�Ǘ��p

	CImage* mpBack;	// �����̕`��̔w�i
	CText* mpText;	// ������`��p

	bool mIsOpened; // �A�C�e�����j���[���J���Ă��邩

	std::vector<CExpandButton*> mButtons;	// �{�^���i�[�p

};