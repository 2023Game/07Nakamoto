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

		eTPose,		//T�|�[�Y
		eIdle,		//�ҋ@
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

};
#endif
