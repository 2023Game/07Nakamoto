#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>

class CPlayer2;

class CInventory : public CTask
{
public:
	// �R���X�g���N�^
	CInventory();
	// �f�X�g���N�^
	~CInventory();

	// �J��
	void Open();
	// ����
	void Close();
	// �C���x���g�����J���Ă��邩�ǂ���
	bool IsOpened() const;

	// ���߂�
	void Decide(int select);
	// �A�C�e����ǉ�����
	void AddItem();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CImage* mpBackground;
	CImage* mpInventoryFrame;
	CImage* mpBackMenu;
	CImage* mpSelectFrame;
	CImage* mpChoco;

	std::vector<CImage*> mItemList;

	int mSelectIndex;
	bool mIsOpened;
};