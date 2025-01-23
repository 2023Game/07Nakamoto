#pragma once
#include "CInteractObject.h"
#include "ItemData.h"
#include "CModel.h"

class CCollider;

// 3D��Ԃɔz�u����A�C�e���̃x�[�X�N���X
class CItemObj :public CInteractObject
{
public:
	// �R���X�g���N�^
	CItemObj(ItemType type);
	// �f�X�g���N�^
	virtual ~CItemObj() = 0;

	// ���ׂ����Ԃ��ǂ���
	bool CanInteract() const override;
	// ���ׂ�
	void Interact() override;

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

protected:
	// �R���C�_�[�쐬(�p����ŏ㏑����)
	virtual void CreateCollider();

	ItemType mItemTyope;	// �A�C�e���̎��
	const ItemData* mpItemData;	// �A�C�e���f�[�^�̃|�C���^
	CModel* mpModel;		// �A�C�e���̃��f���f�[�^
	CCollider* mpCollider;	// �A�C�e���̃R���C�_�[

};