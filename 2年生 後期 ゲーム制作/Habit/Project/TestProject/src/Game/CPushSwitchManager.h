#pragma once
#include <list>
#include <vector>
#include "CTask.h"

class CPushSwitch;

// �X�C�b�`�̃^�X�N�Ǘ��N���X
class CPushSwitchManager : public CTask
{
public:
	// �C���X�^���X���擾
	static CPushSwitchManager* Instance();

	// �R���X�g���N�^
	CPushSwitchManager();
	// �f�X�g���N�^
	~CPushSwitchManager();

	/// <summary>
	/// �w�肵���X�C�b�`�����X�g�ɒǉ�
	/// </summary>
	/// <param name="pushswitch">�ǉ�����X�C�b�`</param>
	void AddSwitch(CPushSwitch* pushswitch);
	/// <summary>
	/// �w�肵���X�C�b�`�����X�g�����菜��
	/// </summary>
	/// <param name="pushswitch">��菜���X�C�b�`</param>
	void RemoveSwitch(CPushSwitch* pushswitch);


private:
	// �X�C�b�`�Ǘ��N���X�̃C���X�^���X�ւ̃|�C���^
	static CPushSwitchManager* spInstance;

	//�X�C�b�`�̃��X�g
	std::vector<CPushSwitch*> mPushSwitchList;

};
