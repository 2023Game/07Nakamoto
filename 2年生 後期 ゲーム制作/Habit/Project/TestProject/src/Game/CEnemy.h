#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CCharaBase.h"
#include "CCollider.h"
#include "CModel.h"

// ����͈͂̃f�o�b�O�\���N���X�̑O�錾
class CDebugFieldOfView;
class CNavNode;

/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemy : public CXCharacter
{
public:
	// �R���X�g���N�^
	CEnemy(std::vector<CVector> patrolPoints);
	// �f�X�g���N�^
	~CEnemy();

	void DeleteObject(CObjectBase* obj) override;
	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

private:
	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eIdle,		// �ҋ@
		eWalk,		// ���s
		eRun,		// ���s
		eJump,		// �W�����v
		eJumpAttack,// �W�����v�U��
		eAttack,	// �U��

		Num			// �A�j���[�V�����̎�ނ̐�
	};
	// �A�j���[�V�����̐؂�ւ�
	void ChangeAnimation(EAnimType type);

	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float framelength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];
	static const AnimData ANIM_DATA2[];

	enum class EState
	{
		eIdle,		// �ҋ@
		ePatrol,	// ����
		eChase,		// �v���C���[��ǂ�������
		eLost,		// �v���C���[��������
		eAttack,	// �v���C���[�U��
	};

	// ��Ԃ�؂�ւ�
	void ChangeState(EState state);

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

	// �ҋ@��Ԏ��̍X�V����
	void UpdateIdle();
	// ���񒆂̍X�V����
	void  UpdatePatrol();
	// �ǐՎ��̍X�V����
	void UpdateChase();
	// �v���C���[�������������̍X�V����
	void UpdateLost();
	// �U�����̍X�V����
	void UpdateAttack();

	// ��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;
	// ��Ԃ̐F���擾
	CColor GetStateColor(EState state) const;

	EState mState;	// �G�̏��
	int mStateStep;	// ��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime;	// �o�ߎ��Ԍv���p

	float mFovAngle;	// ����͈͂̊p�x
	float mFovLength;	// ����͈͂̋���
	CDebugFieldOfView* mpDebugFov;	// ����͈͂̃f�o�b�O�\��

	CNavNode* mpLostPlayerNode;	// �v���C���[�����������ʒu�̃m�[�h

	CVector mAttackStartPos;// �U���J�n���̈ʒu
	CVector mAttackEndPos;	// �U���I�����̈ʒu

	// ����|�C���g�̃��X�g
	std::vector<CNavNode*> mPatrolPoints;
	int mNextPatrolIndex;	// ���ɏ��񂷂�ԍ�

	std::vector<CNavNode*> mMoveRoute;	//���߂��ŒZ�o�H�L���p
	int mNextMoveIndex;					// ���Ɉړ�����m�[�h�̃C���f�b�N�X�l
};
#endif
