#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CPlayerBase.h"
#include "CRideableObject.h"
#include "CSound.h"

class CCollider;
class CDebugFieldOfView;
class CGaugeUI;
class CDemonPower;
class CHandGlow;

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayer2 : public CPlayerBase
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CPlayer2* Instance();

	// �R���X�g���N�^
	CPlayer2();
	// �f�X�g���N�^
	~CPlayer2();

	// �X�V
	void Update() override;

	// �X�e�[�^�X�𐮐��ɂ��Ď擾����
	int GetState();

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

	// �Ǐ]����ʒu��ݒ�
	void SetTrail();
	// �Ǐ]�p�̔z����擾
	const std::vector<CVector>& GetTrail() const;

	// �s���̏�Ԃ�ύX
	void ChangeAction();
	// �ꏏ�ɍs�����Ă��邩
	bool GatActingTogether();

private:
	// �I�u�W�F�N�g�폜��`����
	void DeleteObject(CObjectBase* obj) override;

	// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
	CVector CalcMoveVec();

	// �ҋ@���
	void UpdateIdle();
	// ������
	void UpdateHit();
	// ���S����
	void UpdateDeath();
	// �d�͂𒍂��ł���
	void UpdateChanneling();

	// �ړ��̍X�V����
	void UpdateMove();

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eIdle,		// �ҋ@
		eWalk,		// ���s
		eRun,		// ���s
		eHit,		// ������
		eDeath,		// ���S

		eChannelingStart,	// �d�͂𗬂����݊J�n
		eChanneling,		// �d�͂𗬂����ݒ�
		eChannelingEnd,	// �d�͂𗬂����ݏI��

		//eAttack,	// �U��

		Num
	};

	// �v���C���[�̃C���X�^���X
	static CPlayer2* spInstance;

	// �v���C���[�̏��
	enum class EState
	{
		eIdle,		// �ҋ@
		eHit,		// ������
		eDeath,		// ���S
		eChanneling	// �d�͂𒍂��ł���
	};
	// ��Ԃ�؂�ւ�
	void ChangeState(int state) override;

	CGaugeUI* mpHpGauge;	// HP�Q�[�W

	int mMaxSt;	// �X�^�~�i�̍ő�l
	int mSt;	// �X�^�~�i
	CGaugeUI* mpStGauge;	// �X�^�~�i�Q�[�W

	bool mTogether;	// �L�ƈꏏ�ɂ��邩

	// �d�͂𗬂�����ł���d�͂̌��̃I�u�W�F�N�g
	CDemonPower* mpChannelingDemonPower;
	float mChannelingTime;	// �d�͂𗬂�����ł��鎞��

	// ��̌��̃G�t�F�N�g
	//CHandGlow* mpHandGlow;

	// �Ǐ]�p�̈ړ�������ۊ�
	std::vector<CVector> mTrails;
	// �Ō�Ɉʒu��ۑ������Ƃ��̃v���C���[�̈ʒu
	CVector mLastPos;

#if _DEBUG
	CDebugFieldOfView* mpDebugFov;	// ����͈͂̃f�o�b�O�\��
#endif
};
