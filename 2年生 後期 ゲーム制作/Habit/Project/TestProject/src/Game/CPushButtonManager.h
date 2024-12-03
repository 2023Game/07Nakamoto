#pragma once
#include "CTask.h"
#include <list>

// �X�C�b�`�̃^�X�N�Ǘ��N���X
class CPushButtonManager
{
public:
	// �C���X�^���X���擾
	static CPushButtonManager* Instance();
	// �C���X�^���X��j��
	static void ClearInstance();

	/// <summary>
	/// �w�肵���{�^�������X�g�ɒǉ�
	/// </summary>
	/// <param name="pushbottun">�ǉ�����{�^��</param>
	void Add(CTask* pushbottun);
	/// <summary>
	/// �w�肵���^�X�N�����X�g�����菜��
	/// </summary>
	/// <param name="pushbottun">��菜���{�^��</param>
	void Remove(CTask* pushbottun);

	// �{�^���̃��X�g���擾
	//const std::list<CTask*>& GetTasks() const;

private:
	// �R���X�g���N�^
	CPushButtonManager();
	// �f�X�g���N�^
	~CPushButtonManager();

	// �^�X�N�}�l�[�W���̃C���X�^���X
	static CPushButtonManager* mpInstance;

	//�{�^���̃��X�g
	std::list<CTask*> mPushButtonList;

};
