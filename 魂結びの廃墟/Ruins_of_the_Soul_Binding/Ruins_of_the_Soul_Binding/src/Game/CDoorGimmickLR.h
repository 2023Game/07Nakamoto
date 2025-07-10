#pragma once
#include "CObjectBase.h"

class CDoor;
class CNavNode;
class CRoom;

// ���E�h�A�̃M�~�b�N
class CDoorGimmickLR : public CObjectBase
{
public:
	// �R���X�g���N�^
	CDoorGimmickLR(const CVector& pos, const CVector& angle, CRoom* room);
	// �f�X�g���N�^
	~CDoorGimmickLR();

	// �I�u�W�F�N�g�̍폜��`����֐�
	void DeleteObject(CObjectBase* obj) override;

	// ���E�̃h�A�̊J��Ԃ��؂�ւ�������ɌĂяo������
	void OnChangeDoor();

private:
	CDoor* mpDoorL;	// �����̃h�A
	CDoor* mpDoorR;	// �E���̃h�A

	CNavNode* mpNavNodeL1;	// �����̃h�A�̌o�H�T���m�[�h 1��
	CNavNode* mpNavNodeL2;	// �����̃h�A�̌o�H�T���m�[�h 2��
	CNavNode* mpNavNodeR1;	// �E���̃h�A�̌o�H�T���m�[�h 1��
	CNavNode* mpNavNodeR2;	// �E���̃h�A�̌o�H�T���m�[�h 2��

};