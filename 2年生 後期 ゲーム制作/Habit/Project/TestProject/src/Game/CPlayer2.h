#ifndef CPLAYER_H
#define CPLAYER_H
#include "CXCharacter.h"
#include "CColliderLine.h"

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

	//�X�V����
	void Update() override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	//�`�揈��
	void Render() override;

private:
	// �L�[�̓��͏�񂩂�ړ��x�N�g�������߂�
	CVector CalcMoveVec() const;

	// �ҋ@���
	void UpdateIdle();
	// �W�����v�J�n
	void UpdateJump();

	// �ړ��̍X�V����
	void UpdateMove();

	//�A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		//T�|�[�Y
		eIdle,		//�ҋ@
		eWalk,		//���s
		eRun,		//���s
		eJump,		//�W�����v
		eCrawl,		//����
		eSneak,		//���Ⴊ�݈ړ�
		eCrouch_up, //���Ⴊ��ŏE��

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
		eTPose,		//T�|�[�Y
		eIdle,		//�ҋ@
		eWalk,		//���s
		eRun,		//���s
		eJumpStart,	// �W�����v�J�n
		eJump,		//�W�����v
		eJumpEnd,	// �W�����v�I��
		eCrawl,		//����
		eSneak,		//���Ⴊ�݈ړ�
		eCrouch_up, //���Ⴊ��ŏE��
	};

	std::string ToString(EState state);
	

	EState mState;	// �v���C���[�̏��
	int mStateStep;	// ��ԓ��̃X�e�b�v�Ǘ��p

	CVector mMoveSpeed;	// �O�㍶�E�̈ړ����x
	float mMoveSpeedY;	// �d�͂�W�����v�ɂ��㉺�̈ړ����x

	CVector mJunpStartPos;// �W�����v�J�n���̈ʒu
	CVector mJunpEndPos;	// �W�����v�I�����̈ʒu

	bool mIsGrounded;	// �ڒn���Ă��邩�ǂ���
	CVector mGroundNormal;	// �ڒn���Ă���n�ʂ̖@��

	CColliderLine* mpColliderLine;	// �c�����̐����R���C�_�[
	CColliderLine* mpColliderLineX;	// �������iX���j�̐����R���C�_�[
	CColliderLine* mpColliderLineZ;	// �������iZ���j�̐����R���C�_�[
	CTransform* mpRideObject;
};

#endif // !CPLAYER_H

