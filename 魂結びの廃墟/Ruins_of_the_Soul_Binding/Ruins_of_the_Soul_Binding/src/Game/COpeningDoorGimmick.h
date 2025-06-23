#pragma once
#include "CObjectBase.h"

class COpeningDoor;
class CNavNode;

// �J���˂̃M�~�b�N
class COpeningDoorGimmick : public CObjectBase
{
public:
	// �R���X�g���N�^
	COpeningDoorGimmick(const CVector& pos, const CVector& angle);
	// �f�X�g���N�^
	~COpeningDoorGimmick();

	// �I�u�W�F�N�g�̍폜��`����֐�
	void DeleteObject(CObjectBase* obj) override;

	//�J���˂̌o�H�T���m�[�h�P�ڂ��J��Ԃ��؂�ւ�������ɌĂяo������
	void OnChangeDoor();

private:
	COpeningDoor* mpOpeningDoor;	// �J����

	CNavNode* mpNavNode1;	//�J���˂̌o�H�T���m�[�h�P��
	CNavNode* mpNavNode2;	//�J���˂̌o�H�T���m�[�h�Q��
};