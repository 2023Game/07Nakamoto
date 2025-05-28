#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CRDoor;
class CLDoor;

class CField : public CObjectBase
{
public:
	// �t�B�[���h�̃C���X�^���X��Ԃ�
	static CField* Instance();

	CField();
	~CField();

	// ���̃R���C�_�[���擾
	CColliderMesh* GetFloorCol() const;
	// �ǂ̃R���C�_�[�̎擾
	CColliderMesh* GetWallCol() const;

	/// <summary>
	/// ���C�ƃt�B�[���h�I�u�W�F�N�g�̏Փ˔���
	/// </summary>
	/// <param name="start">���C�̊J�n�ʒu</param>
	/// <param name="end">���C�̏I���ʒu</param>
	/// <param name="hit">�Փˏ��ԋp�p</param>
	/// <returns>�Փ˂�����Atrue��Ԃ�</returns>
	bool CollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit) override;

	void Update();
	void Render();

private:
	// �o�H�T���p�̃m�[�h���쐬
	void CreateNavNodes();

	void CreateFieldObjects();

	// �t�B�[���h�̃C���X�^���X
	static CField* spInstance;

	CModel* mpFloor;
	CModel* mpWall;
	CModel* mpWallCol;

	CRDoor* mpRDoor;
	CLDoor* mpLDoor;

	CColliderMesh* mpFloorColliderMesh;
	CColliderMesh* mpWallColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;
};