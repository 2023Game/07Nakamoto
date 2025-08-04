#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CPlayerBase.h"
#include "CRideableObject.h"
#include "CSound.h"

class CCollider;
class CNavNode;
class CNavManager;
class CGaugeUI;

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
	// �`��
	void Render() override;

	// �W�����v�J�n
	void UpdateJumpStart();
	// �W�����v��
	void UpdateJump();
	// �W�����v�I��
	void UpdateJumpEnd();
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

	/// <summary>
	/// ���삷�邩�ǂ����ݒ�
	/// </summary>
	/// <param name="operate">true�ł���΁A����J�n</param>
	void SetOperate(bool operate) override;

	// �v���C���[�̃o�E���f�B���O�{�b�N�X��Ԃ�
	const CBounds& GetBounds() const override;

private:
	CVector mFollowPos;	// �L���v���C���[�ɂ��Ă������̍��W
	CVector mLookAtPos;	// ����������W

	// �I�u�W�F�N�g�폜��`����
	void DeleteObject(CObjectBase* obj) override;

	// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
	CVector CalcMoveVec() const;

	// �ҋ@���
	void UpdateIdle();
	// �Ǐ]���
	void UpdateTracking();
	// �U�����
	void UpdateAttack();
	// ���S����
	void UpdateDeath();

	// �Ǐ]���̈ړ��o�H���v�Z���邩�ǂ���
	bool IsCalcTrackingRoute() const;
	
	// �ړ��̍X�V����
	void UpdateMove();

	// �w�肵���ʒu�܂ňړ�����
	bool MoveTo(const CVector& targetPos, float speed);

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
		eAttack1,	// �Ђ������U��
		eAttack2,	// �U��
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v��
		eJumpEnd,	// �W�����v�I��
		eHit,		// ������
		eTracking,	// �Ǐ]
		eLost,		// �v���C���[��������
		eDeath,		// ���S
	};
	// ��Ԃ�؂�ւ�
	void ChangeState(int state) override;

	CCollider* mpAttackCol;

	CGaugeUI* mpHpGauge;	// HP�Q�[�W

	float mMaxSt;	// �X�^�~�i�̍ő�l
	float mSt;	// �X�^�~�i
	bool mIsDash;	// �_�b�V���t���O
	CGaugeUI* mpStGauge;	// �X�^�~�i�Q�[�W

	// ���݈ʒu����v���C���[�������Ă��邩�ǂ���
	bool IsLookTarget(CObjectBase* target) const;

	// �Ǐ]���邽�߂̃m�[�h
	CNavNode* mpTrackingNode;
	// �Ǐ]�|�C���g�̃��X�g
	std::vector<CNavNode*> mTrackingRouteNodes;
	// ���ɏ��񂷂�ԍ�
	int mNextTrackingIndex;
};
