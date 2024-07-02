#ifndef CCHARACTER_H
#define CCHARACTER_H

//�ϊ��s��N���X�̃C���N���[�h
#include "CTransform.h"
//���f���N���X�̃C���N���[�h
#include "CModel.h"
#include "CTask.h"

class CCollider;

/*
�L�����N�^�[�N���X
�Q�[���L�����N�^�̊�{�I�ȋ@�\���`
*/
class CCharacter :public CTransform ,public CTask 
{
public:
	//�^�C�v
	enum class ETag
	{
		ENULL,		//�Ȃ�
		ESLOPE,		//�Ζ�
		EBULLET,	//�e
		ETARGET,	//�I
		ESWITCH,	//�X�C�b�`
	};

	//�f�t�H���g�R���X�g���N�^
	CCharacter();
	//�f�X�g���N�^
	~CCharacter();

	//�R���X�g���N�^
	CCharacter(int priority);

	//�L���t���O���擾
	bool GetEnabled();

	/// <summary>
	/// ���f���̐ݒ�
	/// </summary>
	/// <param name="m">���f���N���X�̃|�C���^</param>
	void SetModel(CModel* m);

	//�^�C�v�̐ݒ�
	CCharacter::ETag SetTag(ETag tag);
	//�^�C�v�̎擾
	CCharacter::ETag GetTag();

	//�`�揈��
	void Render();

	//�Փˏ���
	virtual void Collision(CCollider* m, CCollider* o) {}

protected:
	CModel* mpModel;	//���f���̃|�C���^
	CCharacter::ETag mTag;

private:
	
};

#endif // !CCHARACTER_H

