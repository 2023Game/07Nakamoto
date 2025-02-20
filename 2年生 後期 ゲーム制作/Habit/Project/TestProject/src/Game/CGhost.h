#pragma once
#include "CEnemy.h"

// ����͈͂̃f�o�b�O�\���N���X�̑O�錾
class CDebugFieldOfView;

class CNavNode;

// �H��̓G�N���X
class CGhost : public CEnemy
{
public:
	// �R���X�g���N�^
	CGhost(std::vector<CVector> patrolPoints);
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

	// �I�u�W�F�N�g�폜��`����֐�
	void DeleteObject(CObjectBase* obj) override;

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
		ePatrol,	// ����
		eChase,		// �ǂ�������
	};

	// �퓬����̕��֌���
	void LookAtBattleTarget(bool immediate = false);

	// ��Ԑ؂�ւ�
	void ChangeState(int state) override;

	// �v���C���[������͈͓��ɓ��������ǂ���
	bool IsFoundPlayer() const;
	// ���݈ʒu����v���C���[�������Ă��邩�ǂ���
	bool IsLookPlayer() const;

	// �w�肵���ʒu�܂ňړ�����
	bool MoveTo(const CVector& targetPos, float speed);
	// ���ɏ��񂷂�|�C���g��ύX����
	void ChangePatrolPoint();

	// �ҋ@��Ԃ̍X�V����
	void UpdateIdle();
	// �p�j��Ԃ̍X�V����
	void UpdateePatrol();
	// �ǂ������鎞�̍X�V����
	void UpdateChase();

	float mFovAngle;	// ����͈͂̊p�x
	float mFovLength;	// ����͈͂̋���
	//CDebugFieldOfView* mpDebugFov;	// ����͈͂̃f�o�b�O�\��

	// ����|�C���g�̃��X�g
	std::vector<CNavNode*> mPatrolPoints;
	int mNextPatrolIndex;	// ���ɏ��񂷂�ԍ�

	std::vector<CNavNode*> mMoveRoute;	// ���߂��ŒZ�o�H�L���p
	int mNextMoveIndex;					// ���Ɉړ�����m�[�h�̃C���f�b�N�X�l

};