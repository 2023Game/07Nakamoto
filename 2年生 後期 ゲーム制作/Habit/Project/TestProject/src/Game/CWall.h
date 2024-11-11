#pragma once
#include "CObjectBase.h"
#include "CModel.h"
#include "CColliderMesh.h"

// �Ǖ\���N���X
class CWall : public CObjectBase
{
public:
	// �R���X�g���N�^
	CWall(const CVector& pos, const CVector& angle, const CVector& size);
	// �f�X�g���N�^
	~CWall();

	/// <summary>
	/// ���C�ƕǂƂ̏Փ˔���
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <param name="hit"></param>
	/// <returns></returns>
	//bool CollisionRay(const CVector& start, const CVector& end,
	//	CHitInfo* hit) override;

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	CModel* mpModel;	// �ǂ̃��f���f�[�^
	CColliderMesh* mpColliderMesh;	// �ǂ̃R���W�����f�[�^
};