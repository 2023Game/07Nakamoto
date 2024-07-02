#ifndef CTASKMANAGER_H
#define CTASKMANAGER_H

#include "CTask.h"
#include "CCharacter.h"
#include <vector>

/*
�^�X�N�}�l�[�W��
�^�X�N���X�g�̊Ǘ�
*/
class CTaskManager {
public:
	//�f�X�g���N�^
	virtual ~CTaskManager();

	//�^�X�N�̍폜
	void Delete();

	//�S�C���X�^���X�폜
	void AllDelete();

	/// <summary>
	/// ���X�g����폜
	/// </summary>
	/// <param name="task">�^�X�N�̃|�C���^</param>
	void Remove(CTask* task);

	//�X�V
	void Update();
	//�`��
	void Render();

	/// <summary>
	/// ���X�g�ɒǉ�
	/// </summary>
	/// <param name="addTask">�^�X�N�̃|�C���^</param>
	void Add(CTask* addTask);

	//�Փˏ���
	void Collision();

	//�C���X�^���X�̎擾
	static CTaskManager* GetInstance();

protected:
	//�f�t�H���g�R���X�g���N�^
	CTaskManager();

	CTask mHead;	//�擪�^�X�N
	CTask mTail;	//�ŏI�^�X�N

private:
	//�^�X�N�}�l�[�W���̃C���X�^���X
	static CTaskManager* mpInstance;

	//CCharacter�̃|�C���^�̉ϒ��z��
	std::vector<CCharacter*>mpCharacters;
};

#endif // !CTASKMANAGER_H
