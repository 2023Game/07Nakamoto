#pragma once
#include "CTexture.h"
#include "CUIBase.h"

class CImage;

class CIcon : public CUIBase
{
public:
	// �C���X�^���X�̎擾
	static CIcon* Instance();

	// �R���X�g���N�^
	CIcon();
	// �f�X�g���N�^
	~CIcon();

	enum class EIcon
	{
		eCat,
		ePlayer,
		eTogether,
	};

	// �A�C�R���̐ݒ�
	void SetIcon(int icon);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	static CIcon* spInstance;	// �A�C�R���̃C���X�^���X

	CImage* mpIcon;			// �A�C�R���̃C���[�W
	CImage* mpIconFrame;	// �A�C�R���̃t���[��


};