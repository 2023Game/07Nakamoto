#pragma once
#ifndef CCOLLIDERTRIANGLE_H
#define CCOLLIDERTRIANGLE_H
#include "CCollider.h"
/*
�O�p�`�R���C�_�̒�`
*/
class CColliderTriangle :public CCollider
{
public:
	CColliderTriangle(){}
	//�R���X�g���N�^(�O�p�`�R���C�_)
	//CColliderTraiangle(�e,�e�s��,���_1,���_2,���_3)
	CColliderTriangle(CCharacter3* parent, CMatrix* matrix
		, const CVector& v0, const CVector& v1, const CVector& v2);
	//Set(�e,�e�s��,���_1,���_2,���_3)
	void Set(CCharacter3* parent, CMatrix* matrix
		, const CVector& v0, const CVector& v1, const CVector& v2);
	//�`��
	void Render();
};
#endif