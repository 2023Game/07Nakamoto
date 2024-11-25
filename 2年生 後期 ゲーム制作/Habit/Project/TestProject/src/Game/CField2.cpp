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

// �R���X�g���N�^
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

// �f�X�g���N�^
CField2::~CField2()
{
	spInstance = nullptr;

	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

// �X�V
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

	// �ǂƂ̏Փ˔���
	for (CWall2* wall : mWalls)
	{
		if (wall->CollisionRay(start, end, &tHit))
		{
			// �܂��ق��ɏՓ˂��Ă��Ȃ��ꍇ���A
			// ���ɏՓ˂��Ă���R���C�_�[���߂��ꍇ�́A
			if (!isHit || tHit.dist < hit->dist)
			{
				// �Փˏ����X�V
				*hit = tHit;
				isHit = true;
			}
		}
	}

	return isHit;

}

// �ǂ𐶐�
void CField2::CreateWalls()
{
	// �}�b�v�@
	CWall2* wall = new CWall2
	(
		CVector(0.0f, 0.0f, 0.0f), 
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	mWalls.push_back(wall);
}

// �I�u�W�F�N�g�̐���
void CField2::CreateFieldObjects()
{
}

void CField2::CreateNavNodes()
{	
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// �Ǉ@�̎���̌o�H�T���m�[�h
		new CNavNode(CVector(25.0f, 0.0f, 25.0f));
		new CNavNode(CVector(70.0f, 0.0f, 25.0f));
		new CNavNode(CVector(70.0f, 0.0f, 90.0f));
		new CNavNode(CVector(25.0f, 0.0f, 90.0f));

		// �ǇA�̎���̌o�H�T���m�[�h
		new CNavNode(CVector( 85.0f, 0.0f, 15.0f));
		new CNavNode(CVector(175.0f, 0.0f, 15.0f));
		new CNavNode(CVector(175.0f, 0.0f, 80.0f));
		new CNavNode(CVector( 85.0f, 0.0f, 80.0f));

		// �ǇB�̎���̌o�H�T���m�[�h
		new CNavNode(CVector(185.0f, 0.0f, 45.0f));
		new CNavNode(CVector(250.0f, 0.0f, 45.0f));
		new CNavNode(CVector(250.0f, 0.0f, 100.0f));
		new CNavNode(CVector(185.0f, 0.0f, 100.0f));

		// �ǇC�̎���̌o�H�T���m�[�h
		new CNavNode(CVector(15.0f, 0.0f, 105.0f));
		new CNavNode(CVector(95.0f, 0.0f, 105.0f));
		new CNavNode(CVector(95.0f, 0.0f, 165.0f));
		new CNavNode(CVector(50.0f, 0.0f, 165.0f));
		new CNavNode(CVector(15.0f, 0.0f, 165.0f));

		// �ǇD�̎���̌o�H�T���m�[�h
		new CNavNode(CVector(115.0f, 0.0f, 105.0f));
		new CNavNode(CVector(185.0f, 0.0f, 105.0f));
		new CNavNode(CVector(185.0f, 0.0f, 160.0f));
		new CNavNode(CVector(115.0f, 0.0f, 160.0f));

		// �ǇE�̎���̌o�H�T���m�[�h
		new CNavNode(CVector(195.0f, 0.0f, 115.0f));
		new CNavNode(CVector(265.0f, 0.0f, 115.0f));
		new CNavNode(CVector(265.0f, 0.0f, 180.0f));
		new CNavNode(CVector(195.0f, 0.0f, 180.0f));
	}
}

void CField2::Render()
{
	mpModel->Render(Matrix());
}
