#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CModel.h"

/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemy : public CXCharacter
{
public:
	// �R���X�g���N�^
	CEnemy();
	//�f�X�g���N�^
	~CEnemy();

	// �X�V����
	void Update() override;
	
	//�`�揈��
	void Render() override;

private:
	//�A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eIdle,		//�ҋ@
		eWalk,		//���s
		eRun,		//���s
		eJump,		//�W�����v
		eJumpAttack,//�W�����v�U��
		eAttack,	//�U��

		Num			//�A�j���[�V�����̎�ނ̐�
	};
	//�A�j���[�V�����̐؂�ւ�
	void ChangeAnimation(EAnimType type);

	//�A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	//�A�j���[�V�����f�[�^�̃p�X
		bool loop;			//���[�v���邩�ǂ���
		float framelength;	//�A�j���[�V�����̃t���[����
	};
	//�A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	enum class EState
	{
		eIdle,		// �ҋ@
		ePatrol,	// ����
		eChase,		// �v���C���[��ǂ�������
		eLost,		// �v���C���[��������
		eAttack,	// �v���C���[�U��
	};

	//��Ԃ�؂�ւ�
	void ChangeState(EState state);

	//�ҋ@��Ԏ��̍X�V����
	void UpdateIdle();
	//���񒆂̍X�V����
	void  UpdatePatrol();
	//�ǐՎ��̍X�V����
	void UpdateChase();
	//�v���C���[�������������̍X�V����
	void UpdateLost();
	//�U�����̍X�V����
	void UpdateAttack();

	//��Ԃ̕�������擾
	std::string GetStateStr(EState state) const;

	EState mState;	//�G�̏��
	int mStateStep;	//��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime;	//�o�ߎ��Ԍv���p

};
#endif
