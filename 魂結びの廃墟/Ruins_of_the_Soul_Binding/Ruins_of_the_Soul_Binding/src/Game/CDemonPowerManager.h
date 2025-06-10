#pragma once
#include <vector>
#include "CTask.h"

class CDemonPower;

// �d�͂̌��̊Ǘ��N���X
class CDemonPowerManager : public CTask
{
public:
	// �d�͂̌��̃N���X�̃C���X�^���X���擾
	static CDemonPowerManager* Instance();

	// �R���X�g���N�^
	CDemonPowerManager();
	// �f�X�g���N�^
	~CDemonPowerManager();

	// �d�͂̌��̒ǉ�
	void AddDemonPower(CDemonPower* power);
	// �d�͂̌�����菜��
	void RemovePower(CDemonPower* power);

	// �d�͂̌��̐����擾
	int GetDemonPower();

private:
	// �d�͂̌��̊Ǘ��N���X�̃C���X�^���X�ւ̃|�C���^
	static CDemonPowerManager* spInstance;
	// �d�͂̌��̃��X�g
	std::vector<CDemonPower*> mDemonPowers;

};