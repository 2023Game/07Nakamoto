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
class CCharacter :public CTransform ,public CTask {
public:
	//�f�t�H���g�R���X�g���N�^
	CCharacter();
	//�f�X�g���N�^
	~CCharacter();

	//�R���X�g���N�^
	CCharacter(int priority);

	/// <summary>
	/// ���f���̐ݒ�
	/// </summary>
	/// <param name="m">���f���N���X�̃|�C���^</param>
	void SetModel(CModel* m);

	//�`�揈��
	void Render();

	//�Փˏ���
	virtual void Collision(CCollider* m, CCollider* o) {}

protected:
	CModel* mpModel;	//���f���̃|�C���^

private:

};

#endif // !CCHARACTER_H

