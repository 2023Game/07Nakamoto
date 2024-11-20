#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CWall.h"
#include <assert.h>
#include "CNavManager.h"
#include "CNavNode.h"


CField* CField::spInstance = nullptr;

CField* CField::Instance()
{
	return spInstance;
}

CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
	, mEffectAnimData(1, 11, true, 11, 0.03f)
{
	assert(spInstance == nullptr);
	spInstance = this;

	mpModel = CResourceManager::Get<CModel>("Field");

	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpModel, true);

	// �ǂ𐶐�
	CreateWalls();
	CreateFieldObjects();
	// �o�H�T���p�̃m�[�h���쐬
	CreateNavNodes();
}

CField::~CField()
{
	spInstance = nullptr;

	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

// �ǂ𐶐�
void CField::CreateWalls()
{
	// �Ǉ@����
	CWall* wall = new CWall
	(
		CVector(20.0f,  1.0f, 0.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(5.0f,  5.0f, 5.0f)
	);
	mWalls.push_back(wall);	// ���������ǂ�ǂ̃��X�g�ɒǉ�

	// �ǇA����
	wall = new CWall
	(
		CVector(-50.0f, 1.0f, -50.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(5.0f, 5.0f, 5.0f)
	);
	mWalls.push_back(wall);	// ���������ǂ�ǂ̃��X�g�ɒǉ�

	// �ǇB����
	wall = new CWall
	(
		CVector(250.0f, 1.0f, 200.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(5.0f, 5.0f, 5.0f)
	);
	mWalls.push_back(wall);	// ���������ǂ�ǂ̃��X�g�ɒǉ�
}

void CField::CreateFieldObjects()
{
	mpCubeModel = CResourceManager::Get<CModel>("FieldCube");
	mpCylinderModel = CResourceManager::Get<CModel>("FieldCylinder");

	mpMap1 = CResourceManager::Get<CModel>("Map1");

	// �}�b�v�@
	/*new CMoveFloor
	(
		mpMap1,
		CVector(0.0f, 1.0f, 0.0f), CVector(10.0f, 10.0f, 10.0f),
		CVector(0.0f, 0.0f, 0.0f), 5.0f
	);*/

	//new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(0.0f, 10.0f, -50.0f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(50.0f, 0.0f, 0.0f), 10.0f
	//);
	//new CRotateFloor
	//(
	//	mpCylinderModel,
	//	CVector(-40.0f, 15.0f, 20.0f), CVector(1.0f, 1.0f, 1.0f),
	//	1.0f
	//);

	//// �����Ȃ����@
	//new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(20.0f, 10.0f, 0.0f), CVector(0.5f, 1.0f, 0.25f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// �������@
	//new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(60.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
	//	CVector(20.0f, 0.0f, 0.0f), 5.0f
	//);
	//// �����Ȃ����A
	//new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(100.0f, 20.0f, 0.0f), CVector(0.25f, 1.0f, 0.25f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// ��]���鏰�@
	//new CRotateFloor
	//(
	//	mpCubeModel,
	//	CVector(135.0f, 20.0f, 0.0f), CVector(1.0f, 1.0f, 0.25f),
	//	0.5f
	//);
	//// �����Ȃ����A
	//new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(135.0f, 20.0f, -35.0f), CVector(0.25f, 1.0f, 0.25f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//// �����Ȃ����A
	//new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(135.0f, 70.0f, -52.5f), CVector(0.25f, 1.0f, 0.25f),
	//	CVector(0.0f, 50.0f, 0.0f), 5.0f
	//);
	//// �����Ȃ����B�i�⓹�j
	//CMoveFloor* mf = new CMoveFloor
	//(
	//	mpCubeModel,
	//	CVector(0.0f, 20.0f, 200.5f), CVector(4.0f, 1.0f, 2.0f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);
	//mf->Rotate(0.0f, 0.0f, 30.0f);

	//// �d���G�t�F�N�g
	//CLineEffect* le = new CLineEffect(ETag::eField);
	//le->SetTexture("LightningBolt");
	//le->SetBaseUV(CRect(0.0f, 0.0f, 128.0f, 1024.0f));
	//le->SetAnimData(&mEffectAnimData);

	//CVector startPos = CVector(50.0f, 10.0f, 0.0f);
	//CVector endPos = CVector(50.0f, 10.0f, 150.0f);
	//int div = 3;
	//float width = 5.0f;
	//le->AddPoint(startPos, width, width);
	//for (int i = 0; i < div; i++)
	//{
	//	float alpha = (float)(i + 1) / div;
	//	CVector pos = CVector::Lerp(startPos, endPos, alpha);
	//	le->AddPoint(pos, width, width);
	//}
}

void CField::CreateNavNodes()
{
	CNavManager* navMgr = CNavManager::Instance();
	if (navMgr != nullptr)
	{
		// �Ǉ@�̎���̌o�H�T���m�[�h
		new CNavNode(CVector( 30.0f, 0.0f,  35.0f));
		new CNavNode(CVector( 10.0f, 0.0f,  35.0f));
		new CNavNode(CVector( 10.0f, 0.0f, -35.0f));
		new CNavNode(CVector( 30.0f, 0.0f, -35.0f));

		// �ǇA�̎���̌o�H�T���m�[�h
		new CNavNode(CVector(-15.0f, 0.0f, -40.0f));
		new CNavNode(CVector(-15.0f, 0.0f, -60.0f));
		new CNavNode(CVector(-85.0f, 0.0f, -60.0f));
		new CNavNode(CVector(-85.0f, 0.0f, -40.0f));

		// �ǇB�̎���̌o�H�T���m�[�h
		new CNavNode(CVector(285.0f, 1.0f, 190.0f));
		new CNavNode(CVector(285.0f, 0.0f, 210.0f));
		new CNavNode(CVector(215.0f, 0.0f, 210.0f));
		new CNavNode(CVector(215.0f, 0.0f, 190.0f));
		
	}
}

// ���C�ƃt�B�[���h�I�u�W�F�N�g�̏Փ˔���
bool CField::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
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
	for (CWall* wall : mWalls)
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


void CField::Update()
{
}

void CField::Render()
{
	mpModel->Render(Matrix());
}
