#ifndef CBULLET_H
#define CBULLET_H

#include "CCharacter.h"
#include "CTriangle.h"
#include "CCollider.h"
#include "CModel.h"

/*
�e�N���X
�O�p�`���΂�
*/
class CBullet : public CCharacter {
public:
	//�f�t�H���g�R���X�g���N�^
	CBullet();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="m">�R���C�_1</param>
	/// <param name="o">�R���C�_2</param>
	void Collision(CCollider* m, CCollider* o);
	
	//�X�V
	void Update();
	//�O�p�`�̒e��`��
	//void Render();

	//�Փˏ���
	void Collision();

	static CModel* GetModelBullet();
private:
	static CModel mModelBullet;

	//��������
	int mLife;
	//���R���C�_
	CCollider mCollider;
};

#endif // !CBULLET_H
