#ifndef CCUBE_H
#define CCUBE_H

#include "CCharacter.h"
#include "CColliderMesh.h"
#include "CMoveFloor.h"
#include "CCollider.h"

class CCube :public CCharacter
{
public:
	//�f�t�H���g�R���X�g���N�^
	CCube();

	//�X�V����
	void Update();

private:
	CCollider mMesh;
};


#endif // !CCUBE_H

