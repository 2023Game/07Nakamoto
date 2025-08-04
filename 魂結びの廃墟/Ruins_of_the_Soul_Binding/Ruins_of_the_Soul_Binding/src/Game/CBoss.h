#pragma once
#include "CEnemy.h"

// ����͈͂̃f�o�b�O�\���N���X�̑O�錾
class CDebugFieldOfView;

class CInteractObject;
class CNavNode;

// �E�H�[���b�N�̓G�N���X
class CBoss : public CEnemy
{
public:
	// �C���X�^���X�̃|�C���^���擾
	static CBoss* Instance();

	// �R���X�g���N�^
	CBoss(std::vector<std::vector<CVector>> patrolPoints);
	// �f�X�g���N�^
	~CBoss();

	// �U������
	bool IsAttacking() const override;
	// �U���J�n
	void AttackStart() override;
	// �U���I��
	void AttackEnd() override;

	// �d�͂̌��̌���
	void PowerDown();

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
	// �E�H�[���b�N�̃C���X�^���X
	static CBoss* spInstance;

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eIdle,		// �ҋ@
		eWalk,		// ���s
		eRun,		// ���s
		eAttack,	// �U��
		eAlert,		// �x��

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
		eAlert,
		eDeath,		// ���S
	};

	// ��Ԑ؂�ւ�
	void ChangeState(int state) override;

	// ���݂̐퓬������擾
	CObjectBase* GetBattleTarget() const;

	// �퓬����̕��֌���
	void LookAtBattleTarget(bool immediate = false);

	// ���̐��ʕ����x�N�g�����擾
	CVector GetHeadForwardVec() const;

	// �w�肵���^�[�Q�b�g������͈͓��ɓ��������ǂ���
	bool IsFoundTarget(CObjectBase* target) const;
	// ���݈ʒu����^�[�Q�b�g�������Ă��邩�ǂ���
	bool IsLookTarget(CObjectBase* target) const;
	// �퓬������U���ł��邩�ǂ���
	bool CanAttackBattleTarget() const;
	// �󂹂�I�u�W�F�N�g���U�����邩�m�F
	bool CheckAttackBreakObj();
	// �L�����N�^�[���U�����邩�m�F
	bool CheckAttackChara();
	// �퓬����̎��S�m�F�����āA�퓬���肪�܂����݂��邩�m�F
	CObjectBase* ChackDeathBattleTargets();

	// �w�肵���ʒu�܂ňړ�����
	bool MoveTo(const CVector& targetPos, float speed);
	// ���ɏ��񂷂�|�C���g��ύX����
	bool ChangePatrolPoint();
	// ���񃋁[�g���X�V����
	bool UpdatePatrolRoute();

	// �ҋ@��Ԃ̍X�V����
	void UpdateIdle();
	// ���񒆂̍X�V����
	void UpdatePatrol();
	// �ǂ������鎞�̍X�V����
	void UpdateChase();
	// �v���C���[�������������̍X�V����
	void UpdateLost();
	// �x�����Ă��鎞�̏���
	void UpdateAlert();
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

#if _DEBUG
	CDebugFieldOfView* mpDebugFov;	// ����͈͂̃f�o�b�O�\��
	CMatrix mHeadForwardMtx;
#endif

	CNavNode* mpLostPlayerNode;	// �v���C���[�����������ʒu�̃m�[�h
	float mLostElapsedTime;		// �������Ă���̌o�ߎ���

	// ����|�C���g�̃��X�g
	std::vector<CVector>* mpCurrentPatrolRoute;
	std::vector<std::vector<CVector>> mPatrolRoutes;
	CNavNode* mpPatrolStartPoint;

	int mNextPatrolIndex;	// ���ɏ��񂷂�ԍ�

	std::vector<CNavNode*> mMoveRoute;	// ���߂��ŒZ�o�H�L���p
	int mNextMoveIndex;					// ���Ɉړ�����m�[�h�̃C���f�b�N�X�l

	std::vector<CCharaBase*> mTargetCharas;	//�W�I�̃��X�g

	bool mIsBattle;					// �퓬��Ԃ�
	float mBattleIdletime;			// �퓬���̑ҋ@����
	CCharaBase* mpBattleTargetChara;	// �퓬����i�L�����N�^�[�p�j
	CObjectBase* mpBattleTargetObj;		// �퓬����i�I�u�W�F�N�g�p�j
	CCollider* mpAttack1Col;		// �p���`�U���p�̃R���C�_�[

	// ��ԋ߂��ɂ���󂹂�I�u�W�F�N�g���擾
	CObjectBase* GetNearBreakObj() const;
	// �߂��ɂ���󂹂�I�u�W�F�N�g�̃��X�g
	std::vector<CObjectBase*> mNearBreakObjs;
	CCollider* mpSearchCol;	// ���ׂ�I�u�W�F�N�g��T�m����R���C�_�[

	// �d�͂̌��̍ő�l
	int mMaxDemonPower;
	// �d�͂̌��̐�
	int mDemonPower;
	// �U����
	int mPower;

	// ���̍s��
	const CMatrix* mpHeadMtx;

	float mStopElapsedTime;
	CVector mLastPos;
};