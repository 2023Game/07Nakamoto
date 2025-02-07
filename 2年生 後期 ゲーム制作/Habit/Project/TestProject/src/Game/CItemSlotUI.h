#pragma once
#include "CUIBase.h"
#include "CImage.h"
#include "ItemData.h"

class CItemSlotUI : public CUIBase
{
public:
	// �R���X�g���N�^
	CItemSlotUI(int slotIdx);
	// �f�X�g���N�^
	~CItemSlotUI();

	/// <summary>
	/// �A�C�e���X���b�g�̐ݒ�
	/// </summary>
	/// <param name="data">�X���b�g�ɓ����Ă���A�C�e���̃f�[�^</param>
	/// <param name="count">�X���b�g�ɓ����Ă���A�C�e���̌�</param>
	void SetItemSloto(const ItemData* data, int count);

	void OnPointerEnter(const CVector2& pos) override;
	void OnPointerExit(const CVector2& pos) override;
	void OnPointerDown(const CVector2& pos) override;
	void OnPointerUp(const CVector2& pos) override;
	void OnMove(const CVector2& move) override;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;
private:
	int mSlotIndex;		// ���Ԗڂ̃A�C�e���X���b�g��
	const ItemData* mpItemData;
	CImage* mpIcon;		// �A�C�e���A�C�R���̃C���[�W
	CText* mpCountText;	// �A�C�e���̌��\���p�̃e�L�X�g
	CVector2 mCursor_pos;	// �J�[�\���̈ʒu

	CImage* mpSlotHighlight;	// �J�[�\�����d�Ȃ��Ă���A�C�e���X���b�g�������\������
};