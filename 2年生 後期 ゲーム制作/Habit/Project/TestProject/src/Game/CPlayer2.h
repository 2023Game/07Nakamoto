#ifndef CPLAYER_H
#define CPLAYER_H
#include "CXCharacter.h"

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/
class CPlayer2 : public CXCharacter
{
public:
	//�R���X�g���N�^
	CPlayer2();
	//�f�X�g���N�^
	~CPlayer2();

	//�X�V����
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

#endif // !CPLAYER_H

