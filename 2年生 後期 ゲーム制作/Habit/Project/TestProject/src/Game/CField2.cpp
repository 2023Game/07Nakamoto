#include "CField2.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CWall2.h"
#include <assert.h>
#include "CNavManager.h"
#include "CNavNode.h"

CField2* CField2::spInstance = nullptr;

CField2* CField2::Instance()
{
	return spInstance;
}

CField2::CField2()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
{
	assert(spInstance == nullptr);
	spInstance = this;
	//���̐���
	mpModel = CResourceManager::Get<CModel>("Map_mini_floor");

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

	// �ǂ𐶐�
	CreateWalls();
	CreateFieldObjects();
	// �o�H�T���p�̃m�[�h���쐬
	CreateNavNodes();
}

CField2::~CField2()
{
	spInstance = nullptr;

	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

void CField2::Update()
{
}

// ���C�ƃt�B�[���h�I�u�W�F�N�g�̏Փ˔���
bool CField2::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	// �Փˏ��ۑ��p
	CHitInfo tHit;
	// �Փ˂������ǂ����̃t���O
	bool isHit = false;

	// �t�B�[���h�̃I�u�W�F�N�g�Ƃ̏Փ˔���
	if (CCollider::CollisionRay(mpColliderMesh, start, end, &tHit))
	{
		*hit = tHit;
		isHit = this;
	}

	//// �ǂƂ̏Փ˔���
	//for (CWall2* wall : mWalls)
	//{
	//	if (wall->CollisionRay(start, end, &tHit))
	//	{
	//		// �܂��ق��ɏՓ˂��Ă��Ȃ��ꍇ���A
	//		// ���ɏՓ˂��Ă���R���C�_�[���߂��ꍇ�́A
	//		if (!isHit || tHit.dist < hit->dist)
	//		{
	//			// �Փˏ����X�V
	//			*hit = tHit;
	//			isHit = true;
	//		}
	//	}
	//}

	return isHit;

}

// �ǂ𐶐�
void CField2::CreateWalls()
{
	mpMap = CResourceManager::Get<CModel>("Map_mini");

	// �}�b�v�@
	new CMoveFloor
	(
		mpMap,
		CVector(0.0f, 1.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);
}

// �I�u�W�F�N�g�̐���
void CField2::CreateFieldObjects()
{
}

void CField2::CreateNavNodes()
{
	
}

void CField2::Render()
{
	mpModel->Render(Matrix());
}
