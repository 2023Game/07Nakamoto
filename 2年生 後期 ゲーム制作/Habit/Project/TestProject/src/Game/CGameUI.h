#pragma once

class CInteractUI;
class CInteractObject;

class CGameUI
{
public:
	// �C���X�^���X���擾����
	static CGameUI* Instance();
	// �C���X�^���X��j������
	static void ClearInstance();

	// �R���X�g���N�^
	CGameUI();
	// �f�X�g���N�^
	~CGameUI();

	/// <summary>
	/// �w�肵���I�u�W�F�N�g�̒��ׂ�UI��\��
	/// </summary>
	/// <param name="obj">UI��\�����钲�ׂ�I�u�W�F�N�g</param>
	void ShowInteractUI(CInteractObject* obj);
	// ���ׂ�UI���\���ɂ���
	void HideInteractUI();

private:
	static CGameUI* spInstance;	// CGameUI�̃C���X�^���X
	CInteractUI* mpInteractUI;	// ���ׂ�UI
};