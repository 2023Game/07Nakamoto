#pragma once
#include "CEnemy.h"

// ����͈͂̃f�o�b�O�\���N���X�̑O�錾
class CDebugFieldOfView;

class CNavNode;

// �E�H�[���b�N�̓G�N���X
class CWarrok : public CEnemy
{
public:
	// �R���X�g���N�^
	CWarrok(std::vector<CVector> patrolPoints);
	// �f�X�g���N�^
	~CWarrok();

	// �U������
	bool IsAttacking() const override;
	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;

	// �_���[�W���󂯂�
	void TakeDamage(int damage, CObjectBase* causer) override;
	// ���S
	void Death() override;
	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �I�u�W�F�N�g�폜��`����֐�
	void DeleteObject(CObjectBase* obj) override;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eIdle,		// �ҋ@
		eWalk,		// ���s
		eRun,		// ���s
		eAttack,	// �U��

		Num			// �A�j���[�V�����̎�ނ̐�
	};

	// �v���C���[�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		ePatrol,	// ����
		eChase,		// �v���C���[��ǂ�������
		eLost,		// �v���C���[��������
		eAttack,	// �v���C���[�U��
	};

	// ��Ԑ؂�ւ�
	void ChangeState(int state) override;

	// �퓬����̕��֌���
	void LookAtBattleTarget(bool immediate = false);

	// �v���C���[������͈͓��ɓ��������ǂ���
	bool IsFoundPlayer() const;
	// ���݈ʒu����v���C���[�������Ă��邩�ǂ���
	bool IsLookPlayer() const;
	// �v���C���[���U���ł��邩�ǂ���
	bool CanAttackPlayer() const;

	// �w�肵���ʒu�܂ňړ�����
	bool MoveTo(const CVector& targetPos, float speed);
	// ���ɏ��񂷂�|�C���g��ύX����
	void ChangePatrolPoint();

	// �ҋ@��Ԃ̍X�V����
	void UpdateIdle();
	// ���񒆂̍X�V����
	void UpdatePatrol();
	// �ǂ������鎞�̍X�V����
	void UpdateChase();
	// �v���C���[�������������̍X�V����
	void UpdateLost();
	// �p���`�U�����̍X�V����
	void UpdateAttack1();
	// �j�U�����̍X�V����
	void UpdateAttack2();
	// �������Ԃ̍X�V����
	void UpdateHit();
	// ���S��Ԃ̍X�V����
	void UpdateDeath();

	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;
	// ��Ԃ̐F���擾
	CColor GetStateColor(EState state) const;

	float mFovAngle;	// ����͈͂̊p�x
	float mFovLength;	// ����͈͂̋���
	CDebugFieldOfView* mpDebugFov;	// ����͈͂̃f�o�b�O�\��

	CNavNode* mpLostPlayerNode;	// �v���C���[�����������ʒu�̃m�[�h
	// ����|�C���g�̃��X�g
	std::vector<CNavNode*> mPatrolPoints;
	int mNextPatrolIndex;	// ���ɏ��񂷂�ԍ�

	std::vector<CNavNode*> mMoveRoute;	// ���߂��ŒZ�o�H�L���p
	int mNextMoveIndex;					// ���Ɉړ�����m�[�h�̃C���f�b�N�X�l

	bool mIsBattle;					// �퓬��Ԃ�
	float mBattleIdletime;			// �퓬���̑ҋ@����
	CObjectBase* mpBattleTarget;	// �퓬����
	CCollider* mpAttack1Col;		// �p���`�U���p�̃R���C�_�[


};