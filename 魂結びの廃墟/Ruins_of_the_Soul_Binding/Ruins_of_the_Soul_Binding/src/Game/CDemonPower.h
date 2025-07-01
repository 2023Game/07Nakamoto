#pragma once
#include "CInteractObject.h"

class CModel;
class CCollider;
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

	// ���ׂ���e�̃e�L�X�g�摜�̃p�X��Ԃ�
	std::string GetInteractTextPath() const override;
	// ���ׂ���e�̃e�L�X�g�摜�̃p�X��Ԃ�
	std::string GetInteractKeyPath() const override;

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