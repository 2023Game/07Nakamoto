#pragma once

class CItemPositions
{
public:
	// �R���X�g���N�^
	CItemPositions(std::vector<CVector> positions);
	// �f�X�g���N�^
	~CItemPositions();

private:
	std::vector<CVector> mPositions;	// �A�C�e���̔z�u�̃��X�g
};