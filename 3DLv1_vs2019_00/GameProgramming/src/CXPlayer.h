#ifndef CXCPLAYER_H
#define CXCPLAYER_H

#include "CXCharacter.h"
#include "CCollider.h"
#include "CInput.h"

class CXPlayer :public CXCharacter{

public:
	//�f�t�H���g�R���X�g���N�^
	CXPlayer();

	//�X�V����
	void Update();

	//�����ݒ�
	void Init(CModelX* model);

private:
	//�R���C�_�̐錾
	CCollider mColShereBody;	//��
	CCollider mColShereHead;	//��
	CCollider mColShereSword;	//��

	CInput mInput;
};

#endif // !CXCPLAYER_H
