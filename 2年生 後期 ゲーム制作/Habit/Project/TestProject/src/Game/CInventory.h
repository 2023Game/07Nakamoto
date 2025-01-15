#pragma once
#include "CTask.h"
#include "CImage.h"
#include <vector>

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

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CImage* mpBackground;
	std::vector<CImage*> mMenuItems;
	CImage* mpSelectFrame;
	int mSelectIndex;
	bool mIsOpened;
};