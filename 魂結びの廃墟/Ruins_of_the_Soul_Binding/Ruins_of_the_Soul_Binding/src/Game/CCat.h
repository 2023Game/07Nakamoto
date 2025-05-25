#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CPlayerBase.h"
#include "CRideableObject.h"
#include "CSound.h"

class CCollider;

/*
�L�N���X
�L�����N�^�N���X���p��
*/
class CCat : public CPlayerBase
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CCat* Instance();

	// �R���X�g���N�^
	CCat();
	// �f�X�g���N�^
	~CCat();

	// �X�V
	void Update() override;

	// �U������
	bool IsAttacking() const override;
	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;

	// �_���[�W���󂯂�
	void TakeDamage(int damage, CObjectBase* causer) override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	// �I�u�W�F�N�g�폜��`����
	void DeleteObject(CObjectBase* obj) override;

	// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
	CVector CalcMoveVec() const;

	// �ҋ@���
	void UpdateIdle();

	// �ړ��̍X�V����
	void UpdateMove();

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		//eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eWalk,		// ���s
		eAttack,	// �a��U��
		eKick,		// �R��U��
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
		eHit,		// ������

		Num
	};

	// �L�̃C���X�^���X
	static CCat* spInstance;

	// �L�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		eAttack1,	// �a��U��
		eAttack2,	// �R��U��
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
		eHit,		// ������
	};
	// ��Ԃ�؂�ւ�
	void ChangeState(int state) override;

};
