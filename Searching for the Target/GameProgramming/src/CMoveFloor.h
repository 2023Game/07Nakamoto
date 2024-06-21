#ifndef CMOVEFLOOR_H
#define CMOVEFLOOR_H

#include "CCharacter.h"
#include "CCollider.h"
#include "CColliderMesh.h"
#include "CModel.h"

class CMoveFloor :public CCharacter {

public:
	//�f�t�H���g�R���X�g���N�^
	CMoveFloor();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�k</param>
	CMoveFloor(CVector& pos,
		CVector& rot, CVector& scale);
	
	//�X�V
	void Update();

	static CModel* GetModelRedCube();
	static CModel* GetModelBlueCube();
private:
	static CModel mModelRedCube;
	static CModel mModelBuleCube;

	CColliderMesh mColliderMesh;
};

#endif // !CMOVEFLOOR_H

