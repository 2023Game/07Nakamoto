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
	//�R���C�_�^�C�v
	enum class EType {
		ESPHERE,	//���R���C�_
		ETRIANGLE,	//�O�p�R���C�_
		ELINE,		//�����R���C�_
	};

	//�f�t�H���g�R���X�g���N�^
	CCollider();
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
	//�R���C�_�^�C�v�̎擾
	CCollider::EType GetType();

	//�`��
	void Render();

	/// <summary>
	/// �ΖʂɃv���C���[�����킹��
	/// </summary>
	/// <param name="p">�v���C���[</param>
	/// <param name="t">�O�p�R���C�_</param>
	/// <returns></returns>
	static CVector Slope(CCollider* p, CCollider* t, CVector* adjust);

	/// <summary>
	/// �Փ˔���
	/// </summary>
	/// <param name="m">�R���C�_1</param>
	/// <param name="o">�R���C�_2</param>
	/// <returns>true(�Փ˂��Ă���) false(�Փ˂��Ă��Ȃ�)</returns>
	static bool Collision(CCollider* m, CCollider* o);

	/// <summary>
	/// �O�p�`�Ƌ��̏Փ˔���
	/// </summary>
	/// <param name="triangle">�O�p�R���C�_</param>
	/// <param name="sphere">���R���C�_</param>
	/// <param name="adjust">�����l:�Փ˂��Ȃ��ʒu�܂Ŗ߂��l</param>
	/// <returns>true(�Փ˂��Ă���) false(�Փ˂��Ă��Ȃ�)</returns>
	static bool CollisionTriangleSphere(CCollider* triangle, CCollider* sphere,
		CVector* adjust);

	/// <summary>
	/// �O�p�`�Ɛ����̏Փ˔͈�
	/// </summary>
	/// <param name="triangle">�O�p�R���C�_</param>
	/// <param name="line">�����R���C�_</param>
	/// <param name="adjust">�����l:�Փ˂��Ȃ��ʒu�܂Ŗ߂��l</param>
	/// <returns>true(�Փ˂��Ă���) false(�Փ˂��Ă��Ȃ�)</returns>
	static bool CollisionTriangleLine(CCollider* triangle,
		CCollider* line, CVector* adjust);

	/// <summary>
	/// �D��x�̕ύX
	/// </summary>
	/// <param name="priority">�D��x</param>
	void ChangePriority(int priority);

	//�D��x�̕ύX
	virtual void ChangePriority();

protected:
	EType mType;	//�R���C�_�^�C�v

	//���_
	CVector mV[3];

	CCharacter* mpParent;	//�e
	CMatrix* mpMatrix;		//�e�s��
	float mRadius;			//���a
};

#endif // !CCOLLIDER_H
