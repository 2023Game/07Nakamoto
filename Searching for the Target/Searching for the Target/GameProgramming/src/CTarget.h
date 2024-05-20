#ifndef CTARGET_H
#define CTARGET_H

#include "CCharacter.h"
#include "CCollider.h"

/*
�^�[�Q�b�g�N���X
�L�����N�^�N���X���p��
*/
class CTarget :public CCharacter {
public:
	CTarget(CModel* model, const CVector& position,
		const CVector& rotation, const CVector& scale);

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="m">�R���C�_1</param>
	/// <param name="o">�R���C�_2</param>
	void Collision(CCollider* m, CCollider* o);

	//�X�V����
	void Update();

private:
	//�R���C�_
	CCollider mCollider;

	int count;
};

#endif // !CTARGET_H
