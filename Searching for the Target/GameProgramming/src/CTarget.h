#ifndef CTARGET_H
#define CTARGET_H

#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"

/*
�^�[�Q�b�g�N���X
�L�����N�^�N���X���p��
*/
class CTarget :public CCharacter {
public:
	//Target�̃^�C�v
	enum class EState
	{
		ESTAY,		//�؍�
		EMOVE1,		//���ړ�
	};

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�ʒu</param>
	/// <param name="rotation">��]</param>
	/// <param name="scale">�g�k</param>
	/// <param name="state">���</param>
	CTarget(CModel* model, const CVector& position,
		const CVector& rotation, const CVector& scale, EState state);

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="m">�R���C�_1</param>
	/// <param name="o">�R���C�_2</param>
	void Collision(CCollider* m, CCollider* o);

	//�Փˏ���
	void Collision();

	//�X�V����
	void Update();

private:
	//���
	EState mState;
	//�R���C�_
	CCollider mCollider;

	int count;
};

#endif // !CTARGET_H