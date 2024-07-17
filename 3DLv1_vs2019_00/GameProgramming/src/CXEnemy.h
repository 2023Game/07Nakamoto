#ifndef CXENEMY_H
#define XCENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy :public CXCharacter
{
public:
	//�f�t�H���g�R���X�g���N�^
	CXEnemy();

	//�����ݒ�
	void Init(CModelX* model);

	//�Փˏ���
	void Collision(CCollider* m, CCollider* o);
private:
	//�R���C�_�̐錾
	CCollider mColShereBody;	//��
	CCollider mColShereHead;	//��
	CCollider mColShereSword0;	//��
	CCollider mColShereSword1;	//��
	CCollider mColShereSword2;	//��
};

#endif // !CXENEMY_H

