#include "CCharacter.h"
#include "CApplication.h"

//�f�t�H���g�R���X�g���N�^
CCharacter::CCharacter()
	:mpModel(nullptr)
	,mTag(ETag::ENULL)
{
	//�^�X�N���X�g�ɒǉ�
	CTaskManager::GetInstance()->Add(this);
}
//�f�X�g���N�^
CCharacter::~CCharacter()
{
	//�^�X�N���X�g����폜
	CTaskManager::GetInstance()->Remove(this);
}

//�R���X�g���N�^
CCharacter::CCharacter(int priority)
	: mpModel(nullptr)
	, mTag(ETag::ENULL)
{
	mPriority = priority;
	CTaskManager::GetInstance()->Add(this);
}

//�L���t���O�̎擾
bool CCharacter::GetEnabled()
{
	return mEnabled;
}

//���f���̐ݒ�
void CCharacter::SetModel(CModel* m)
{
	mpModel = m;
}

//�^�O�̐ݒ�
CCharacter::ETag CCharacter::SetTag(ETag tag)
{
	return mTag = tag;
}

//�^�O�̎擾
CCharacter::ETag CCharacter::GetTag()
{
	return mTag;
}

//�`�揈��
void CCharacter::Render()
{
	mpModel->Render(mMatrix);
}