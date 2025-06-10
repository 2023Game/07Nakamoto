#pragma once
#include <vector>

class CInteractObject;

// ���ׂ�I�u�W�F�N�g�̊Ǘ��p�N���X
class CInteractObjectManager
{
public:
	// �C���X�^���X���擾
	static CInteractObjectManager* Instance();
	// �C���X�^���X��j��
	static void CIearInstance();

	// �R���X�g���N�^
	CInteractObjectManager();
	// �f�X�g���N�^
	~CInteractObjectManager();

	// ���ׂ�I�u�W�F�N�g��ǉ�
	void Add(CInteractObject* obj);
	// ���ׂ�I�u�W�F�N�g����菜��
	void Remove(CInteractObject* obj);

	/// <summary>
	/// �w�肵�����W�ɋ߂����ׂ�I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <param name="pos">���ׂ���W</param>
	/// <param name="distance">���ׂ鋗��</param>
	/// <param name="list">���������I�u�W�F�N�g���i�[���郊�X�g</param>
	/// <returns>true�ł���΁A�߂��̃I�u�W�F�N�g�����݂���</returns>
	bool GetNearInteractObjects(const CVector& pos, float distance,
		std::vector<CInteractObject*>& list);

private:
	// �C���X�^���X
	static CInteractObjectManager* spInstance;
	// �����ς݂̒��ׂ�I�u�W�F�N�g�̃��X�g
	std::vector<CInteractObject*> mInteractObjects;
};