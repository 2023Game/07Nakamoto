#include "CCharacter.h"
#include "CApplication.h"

//�R���X�g���N�^
CCharacter::CCharacter()
	:mpModel(nullptr)
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

//���f���̐ݒ�
void CCharacter::SetModel(CModel* m)
{
	mpModel = m;
}

//�`�揈��
void CCharacter::Render()
{
	mpModel->Render(mMatrix);
}