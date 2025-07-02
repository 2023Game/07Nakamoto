#pragma once
#include <vector>
#include "CTask.h"

class CPlayerBase;

// �v���C���[�̊Ǘ��N���X
class CPlayerManager : public CTask
{
public:
	// �v���C���[�Ǘ��N���X�̃C���X�^���X���擾
	static CPlayerManager* Instance();

	// �R���X�g���N�^
	CPlayerManager();
	// �f�X�g���N�^
	~CPlayerManager();

	// �v���C���[�̒ǉ�
	void AddPlayer(CPlayerBase* player);
	// �v���C���[����菜��
	void RemovePlayer(CPlayerBase* player);

	// ����L������؂�ւ���
	void ChangePlayer();

	// �X�V
	void Update() override;


private:
	//EOwnerState mOwnerState;

	// �v���C���[�Ǘ��N���X�̃C���X�^���X�ւ̃|�C���^
	static CPlayerManager* spInstance;

	// �v���C���[�̃��X�g
	std::vector<CPlayerBase*> mPlayers;

	int mCurrentIndex;	// ���݂̃C���f�b�N�X
};