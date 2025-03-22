#pragma once
#include "CObjectBase.h"

class CImage3D;

// 3D��Ԃɕ\������C���^���N�g�{�^����UI
class CInteractUI :public CObjectBase
{
public:
	// �R���X�g���N�^
	CInteractUI(CObjectBase* owner);
	// �f�X�g���N�^
	~CInteractUI();

	// �������ݒ�
	void SetOwner(CObjectBase* owner);

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CObjectBase* mpOwner;	// �C���^���N�g�{�^���̎�����
	CImage3D* mpInteractUI;	// �C���^���N�g�{�^���̃C���[�W
	CVector2 mGaugeSize;	// �Q�[�W�̃C���[�W�̃T�C�Y

};