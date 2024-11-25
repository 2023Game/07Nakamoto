#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CWall;
class CWall2;

class CField : public CObjectBase
{
public:
	// �t�B�[���h�̃C���X�^���X��Ԃ�
	static CField* Instance();

	CField();
	~CField();

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
	// �ǂ𐶐�
	void CreateWalls();
	void CreateFieldObjects();
	// �o�H�T���p�̃m�[�h���쐬
	void CreateNavNodes();

	// �t�B�[���h�̃C���X�^���X
	static CField* spInstance;

	CModel* mpModel;
	CColliderMesh* mpColliderMesh;

	CModel* mpCubeModel;
	CModel* mpCylinderModel;

	TexAnimData mEffectAnimData;

	// �ǂ̃I�u�W�F�N�g�̃��X�g
	std::list<CWall*> mWalls;

	// ��
	//CModel* mpMap;
	// ��
	//CWall2* mpFloor;
};