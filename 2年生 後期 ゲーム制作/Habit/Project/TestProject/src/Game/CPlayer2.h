#ifndef CPLAYER_H
#define CPLAYER_H
#include "CXCharacter.h"
#include "CColliderCapsule.h"

class CInteractObject;
class CGaugeUI;
class CDebugFieldOfView;
struct ItemData;

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayer2 : public CXCharacter
{
public:
	// �C���X�^���X�̃|�C���^�̎擾
	static CPlayer2* Instance();

	//�R���X�g���N�^
	CPlayer2();
	//�f�X�g���N�^
	~CPlayer2();

	// �I�u�W�F�N�g�폜��`����
	void DeleteObject(CObjectBase* obj) override;

	//�X�V����
	void Update() override;

	// �X�e�[�^�X�𐮐��ɂ��Ď擾����
	int GetState();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �_���[�W����
	void TakeDamage(int damage, CObjectBase* causer) override;

	// �w�肵���A�C�e�����g�p�ł��邩�ǂ���
	bool CanUseItem(const ItemData* item);
	// �A�C�e���̌��ʂ��g��
	void UseItem(const ItemData* item);

	// �����x���Ȃ�l
	void TakeSlow(float slow, int time);

	//�`�揈��
	void Render() override;

private:
	// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
	CVector CalcMoveVec() const;

	// �ҋ@���
	void UpdateIdle();
	// �W�����v�J�n
	void UpdateJumpStart();
	// �W�����v��
	void UpdateJump();
	// �W�����v�I��
	void UpdateJumpEnd();
	// �]�|����
	void UpdateFall();
	// ���S����
	void UpdateDeath();

	// �ړ��̍X�V����
	void UpdateMove();

	//�A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eWalk,		// ���s
		eRun,		// ���s
		eJumpStart,	// �W�����v�J�n
		eJumping,	// �W�����v��
		eJumpEnd,	// �W�����v�I��
		eFall,		// �]�|
		eDeath,		// ���S

		eCrawl,		// ����
		eSneak,		// ���Ⴊ�݈ړ�
		eCrouch_up, // ���Ⴊ��ŏE��

		Num
	};
	//�A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	//�v���C���[�̃C���X�^���X
	static CPlayer2* spInstatnce;

	//�A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	//�A�j���[�V�����f�[�^�̃p�X
		bool loop;			//���[�v���邩�ǂ���
		float framelength;	//�A�j���[�V�����̃t���[����
	};
	//�A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �v���C���[�̏��
	enum class EState
	{
		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@
		eWalk,		// ���s
		eRun,		// ���s
		eJumpStart,	// �W�����v�J�n
		eJump,		// �W�����v
		eJumpEnd,	// �W�����v�I��
		eFall,		// �]�|
		eDeath,		// ���S

		eCrawl,		// ����
		eSneak,		// ���Ⴊ�݈ړ�
		eCrouch_up, // ���Ⴊ��ŏE��
	};

	int mMaxSt;	// �X�^�~�i�̍ő�l
	int mSt;	// �X�^�~�i
	//int mSan;	// ���C�x

	float mSlowSpeed;	// �����x���Ȃ�l
	int mSlowTime;	// �����x���Ȃ鎞��

	std::string ToString(EState state);

	EState mState;	// �v���C���[�̏��
	int mStateStep;	// ��ԓ��̃X�e�b�v�Ǘ��p

	CVector mMoveSpeed;	// �O�㍶�E�̈ړ����x
	float mMoveSpeedY;	// �d�͂�W�����v�ɂ��㉺�̈ړ����x

	CVector mJunpStartPos;// �W�����v�J�n���̈ʒu
	CVector mJunpEndPos;	// �W�����v�I�����̈ʒu

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	CVector mGroundNormal;	// �ڒn���Ă���n�ʂ̖@��

	CColliderCapsule* mpColliderCapsule;	//�J�v�Z���R���C�_�[

	CTransform* mpRideObject;	// ��邱�Ƃ̏o����I�u�W�F�N�g��

	// ��ԋ߂��ɂ��钲�ׂ���I�u�W�F�N�g���擾
	CInteractObject* GetNearInteractObj() const;
	// �߂��ɂ��钲�ׂ���I�u�W�F�N�g�̃��X�g
	std::list<CInteractObject*> mNearInteractObjs;
	CCollider* mpSearchCol;	// ���ׂ�I�u�W�F�N�g��T�m����R���C�_�\

	float mFovAngle;	// ����͈͂̊p�x

	CGaugeUI* mpHpGauge;	// HP�Q�[�W
	CGaugeUI* mpStGauge;	// �X�^�~�i�Q�[�W

	CDebugFieldOfView* mpDebugFov;	// ����͈͂̃f�o�b�O�\��

};

#endif // !CPLAYER_H

