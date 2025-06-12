#pragma once
#include "CInteractObject.h"

class CModel;
class CColliderMesh;
class CGaugeUI3D;

// �d�͂̌��̃N���X
class CDemonPower : public CInteractObject
{
public:
	// �R���X�g���N�^
	CDemonPower(const CVector& pos);
	// �f�X�g���N�^
	~CDemonPower();

	// ���ׂ�
	void Interact() override;

	// �j�󂳂ꂽ��
	bool BreakPower();

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	CModel* mpModel;	//�d�͂����̃��f���f�[�^
	CCollider* mpCollider;	// �d�͂̌��̃R���W�����f�[�^

	CGaugeUI3D* mpHpGauge;	// HP�Q�[�W
	CVector mGaugeOffsetPos;	// �Q�[�W�̃I�t�Z�b�g�ʒu

	bool mBroken;	//	��ꂽ��
};