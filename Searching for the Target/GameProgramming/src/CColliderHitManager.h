#ifndef CCOLLIDERHITMANAGER_H
#define CCOLLIDERHITMANAGER_H

#include "CCollider.h"

class CColliderHitManager
{
public:
	//��������R���C�_
	enum class ECollider{
		EFLOOR,		//��
		ESLOPE,		//��
	};

	//�f�t�H���g�R���X�g���N�^
	CColliderHitManager();

	//�C���X�^���X�̎擾
	static CColliderHitManager* GetInstance();

	/// <summary>
	/// �ϒ��z��ɒǉ�����
	/// </summary>
	/// <param name="o">�v���C���[�Ɠ��������R���C�_</param>
	void AddColliders(CCollider* o);

	//�^�O�̊m�F
	CCollider *HitColliderSerch();

	//�Փ˔���
	void HitCollision(CCollider* m, CCollider* o);

	//�C���l�̎擾
	CVector GetAjustRote();

	//��������R���C�_�̎�ނ̎擾
	ECollider GetCol();

	//�ϒ��z����폜
	void Delete();

private:
	//�ϒ��z��Ƀv���C���[�Ɠ��������R���C�_���i�[����
	std::vector<CCollider*> static mpHitColliders;

	static CColliderHitManager* mpInstance;

	//��]�l�̏C���l���i�[
	CVector ajustRote;

	ECollider mCol;
};

#endif // !CCOLLIDERHITMANAGER_H

