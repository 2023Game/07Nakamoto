#pragma once
#include "CObjectBase.h"

class CImage3D;
class CInteractObject;

// 3D��Ԃɕ\������C���^���N�g�{�^����UI
class CInteractUI :public CObjectBase
{
public:
	// �R���X�g���N�^
	CInteractUI();
	// �f�X�g���N�^
	~CInteractUI();

	// ���ׂ�UI��\��
	void Show(CInteractObject* obj);
	// ���ׂ�UI���\��
	void Hide();

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CImage3D* mpButtonImage;	// ����{�^���̃C���[�W
	CImage3D* mpTextImage;		// ���ׂ���e�e�L�X�g�̃C���[�W

};