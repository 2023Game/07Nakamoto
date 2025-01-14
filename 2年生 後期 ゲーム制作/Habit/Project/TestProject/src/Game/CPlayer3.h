#pragma once
//�L�����N�^�N���X�̃C���N���[�h
#include "CXCharacter.h"
#include "CColliderCapsule.h"

class CInteractObject;
class CGaugeUI;
class CDebugFieldOfView;

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayer3 : public CXCharacter
{
public:
	// �C���X�^���X�̃|�C���^�̎擾
	static CPlayer3* Instance();

	// �R���X�g���N�^
	CPlayer3();
	// �f�X�g���N�^
	~CPlayer3();

	// �X�V����
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
	void TakeDamege(int damage);

	//�`�揈��
	void Render() override;

private:
	//�A�j���[�V�����̎��
	enum class EAnimType
	{
		None = -1,

		eTPose,		// T�|�[�Y
		eIdle,		// �ҋ@

		Num
	};

	//�A�j���[�V�����؂�ւ�
	void ChangeAnimation(EAnimType type);

	//�v���C���[�̃C���X�^���X
	static CPlayer3* spInstatnce;

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
	};

	EState mState;	// �v���C���[�̏��

};
