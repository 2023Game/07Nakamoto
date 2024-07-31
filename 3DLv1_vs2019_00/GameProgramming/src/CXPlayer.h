#ifndef CXCPLAYER_H
#define CXCPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CInput.h"
#include "CColliderCapsule.h"

class CXPlayer :public CXCharacter{

public:
	//�f�t�H���g�R���X�g���N�^
	CXPlayer();

	//�X�V����
	void Update();

	//�����ݒ�
	void Init(CModelX* model);

	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);

private:
	CColliderCapsule mColBody; //��
	//�R���C�_�̐錾
	CCollider mColShereBody;	//��
	CCollider mColShereHead;	//��
	CCollider mColShereSword;	//��

	CInput mInput;
};

#endif // !CXCPLAYER_H
