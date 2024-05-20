#ifndef CCOLLIDER_H
#define CCOLLIDER_H

#include "CCharacter.h"

class CCollisionManager;

/*
�R���C�_�N���X
�Փ˔���f�[�^
*/

class CCollider : public CTransform , public CTask {
	friend CCollisionManager;
public:
	//�f�X�g���N�^
	~CCollider();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="parent">�e</param>
	/// <param name="matrix">�e�s��</param>
	/// <param name="position">�ʒu</param>
	/// <param name="radius">���a</param>
	CCollider(CCharacter* parent, CMatrix* matrix,
		const CVector& position, float radius);

	//�e�|�C���^�̎擾
	CCharacter* GetParent();
	//�`��
	void Render();

	/// <summary>
	/// �Փ˔���
	/// </summary>
	/// <param name="m">�R���C�_1</param>
	/// <param name="o">�R���C�_2</param>
	/// <returns>true(�Փ˂��Ă���) false(�Փ˂��Ă��Ȃ�)</returns>
	static bool Collision(CCollider* m, CCollider* o);

protected:
	CCharacter* mpParent;	//�e
	CMatrix* mpMatrix;		//�e�s��
	float mRadius;			//���a
};

#endif // !CCOLLIDER_H
