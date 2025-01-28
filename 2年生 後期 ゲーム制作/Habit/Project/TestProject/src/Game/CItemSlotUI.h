#pragma once
#include "CUIBase.h"
#include "CImage.h"
#include "ItemData.h"

class CItemSlotUI : public CUIBase
{
public:
	// �R���X�g���N�^
	CItemSlotUI();
	// �f�X�g���N�^
	~CItemSlotUI();

	/// <summary>
	/// �A�C�e���X���b�g�̐ݒ�
	/// </summary>
	/// <param name="data">�X���b�g�ɓ����Ă���A�C�e���̃f�[�^</param>
	/// <param name="count">�X���b�g�ɓ����Ă���A�C�e���̌�</param>
	void SetItemSloto(const ItemData* data, int count);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
private:
	CImage* mpIcon;		// �A�C�e���A�C�R���̃C���[�W
	CText* mpCountText;	// �A�C�e���̌��\���p�̃e�L�X�g
};