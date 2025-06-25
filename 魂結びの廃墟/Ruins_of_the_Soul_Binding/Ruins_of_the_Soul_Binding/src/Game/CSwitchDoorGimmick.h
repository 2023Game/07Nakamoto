#pragma once
#include "CObjectBase.h"

class CSwitchDoor;
class CSwitch;
class CNavNode;

class CSwitchDoorGimmick : public CObjectBase
{
public:
	// �R���X�g���N�^
	CSwitchDoorGimmick(const CVector& pos, const CVector& angle, const CVector& openPos,
		std::string modelName, std::string colName);
	// �f�X�g���N�^
	~CSwitchDoorGimmick();

	// �ڑ�����X�C�b�`��ǉ�
	void AddSwitch(CSwitch* sw);

	//�o�H�T���m�[�h�P�ڂ��J��Ԃ��؂�ւ�������ɌĂяo������
	void OnChangeDoor();

private:
	CSwitchDoor* mpSwitchDoor;	// �X�C�b�`�������ꂽ��J����

	CNavNode* mpNavNode1;	//���̌o�H�T���m�[�h�P��
	CNavNode* mpNavNode2;	//���̌o�H�T���m�[�h�Q��
};