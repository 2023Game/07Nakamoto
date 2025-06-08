#include "CInteractObjectManager.h"
#include "CInteractObject.h"
#include <assert.h>

// �C���X�^���X
CInteractObjectManager* CInteractObjectManager::spInstance = nullptr;

// �C���X�^���X���擾
CInteractObjectManager* CInteractObjectManager::Instance()
{
	return spInstance;
}

// �C���X�^���X��j��
void CInteractObjectManager::CIearInstance()
{
	SAFE_DELETE(spInstance);
}

// �R���X�g���N�^
CInteractObjectManager::CInteractObjectManager()
{
	assert(spInstance == nullptr);
	spInstance = this;
}

// �f�X�g���N�^
CInteractObjectManager::~CInteractObjectManager()
{
	spInstance = nullptr;
}

// ���ׂ�I�u�W�F�N�g��ǉ�
void CInteractObjectManager::Add(CInteractObject* obj)
{
	mInteractObjects.push_back(obj);
}

// ���ׂ�I�u�W�F�N�g����菜��
void CInteractObjectManager::Remove(CInteractObject* obj)
{
	auto result = std::remove(mInteractObjects.begin(), mInteractObjects.end(), obj);
	mInteractObjects.erase(result, mInteractObjects.end());
}

// �w�肵�����W�ɋ߂����ׂ�I�u�W�F�N�g��Ԃ�
bool CInteractObjectManager::GetNearInteractObjects(const CVector& pos, float distance,
	std::vector<CInteractObject*>& list)
{
	list.clear();
	for (CInteractObject* obj : mInteractObjects)
	{
		// ���ׂ�I�u�W�F�N�g���L���łȂ��ꍇ�́A�X���[
		if (!obj->IsEnable()) continue;
		// ���ׂ�I�u�W�F�N�g�̏Փ˔͈͂��I�t�̏ꍇ�́A�X���[
		if (!obj->IsEnableCol()) continue;
		// ���ׂ�I�u�W�F�N�g�̍폜�t���O�������Ă���ꍇ�́A�X���[
		if (obj->IsKill()) continue;

		// ���ׂ�I�u�W�F�N�g�Ǝw�肳�ꂽ���W�̋������擾
		float dist = (obj->Position() - pos).LengthSqr();
		// ���߂��������w�肳�ꂽ������艓����΁A���̒��ׂ�I�u�W�F�N�g�֐i��
		if (dist > distance * distance) continue;

		// ���X�g�ɒ��ׂ�I�u�W�F�N�g��ǉ�
		list.push_back(obj);
	}
	// ���X�g�̒��g��1�ȏ゠��΁A�߂��̃I�u�W�F�N�g����������
	return list.size() > 0;
}
