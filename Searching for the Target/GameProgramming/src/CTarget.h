#ifndef CTARGET_H
#define CTARGET_H

#include "CCharacter.h"
#include "CCollider.h"
#include "CModel.h"

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

	//�f�X�g���N�^
	~CTarget();

	//�X�V����
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="m">�R���C�_1</param>
	/// <param name="o">�R���C�_2</param>
	void Collision(CCollider* m, CCollider* o);

	//�Փˏ���
	void Collision();

	//�I�̃��f�����擾����
	static CModel* GetModelTarget();
	//�I�̐����擾����
	static int GetNum();
	//�����ݒ�
	static void InitNum();

private:
	static CModel mModelTarget;
	//�I�̐�
	static int sNum;

	//���
	EState mState;
	//�R���C�_
	CCollider mCollider;
	//�ړ���������
	int count;
};

#endif // !CTARGET_H
