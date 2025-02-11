#pragma once
#include "CUIBase.h"

class CText;

// �A�C�e���I�𒆂̃��j���[
class CItemMenu
{
public:
	// �C���X�^���X���擾
	static CItemMenu* Instance();

	// �R���X�g���N�g
	CItemMenu();
	// �f�X�g���N�^
	~CItemMenu();

	// �J��
	void Open();
	// ����
	void Close();
	// �C���x���g�����J���Ă��邩�ǂ���
	bool IsOpened() const;

	// �X�V
	void Update();
	// �`��
	void Render();

private:
	static CItemMenu* spInstance;

	CText* mpText;	// ������`��p

	// ���e�̎��
	enum EContent
	{
		eUse,	// �g��
		eBack,	// �߂�
	};

	// �A�C�e�����j���[
	struct MenuInfo
	{
		std::string text;	// �e�L�X�g
		EContent content;	// ���e
	};
	// �A�C�e�����j���[�̃��X�g
	std::vector<MenuInfo> mMenuList;

	bool mIsOpened;
};