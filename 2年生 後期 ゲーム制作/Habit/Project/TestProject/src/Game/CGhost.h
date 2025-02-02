#pragma once
#include "CEnemy.h"

// �H��̓G�N���X
class CGhost : public CEnemy
{
public:
	// �R���X�g���N�^
	CGhost();
	// �f�X�g���N�^
	~CGhost();

	// �U������
	bool IsAttacking() const override;
	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update() override;

private:
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eIdle,		// �ҋ@

		Num
	};

	// �v���C���[�̏��
	enum class EState
	{
		eIdle,		// �ҋ@���
		eChase,		// �ǂ�������
	};

	// �퓬����̕��֌���
	void LookAtBattleTarget(bool immediate = false);

	// ��Ԑ؂�ւ�
	void ChangeState(int state) override;

	// �ҋ@��Ԃ̍X�V����
	void UpdateIdle();
	// �ǂ������鎞�̍X�V����
	void UpdateChase();

	CObjectBase* mpBattleTarget;	// �퓬����

};