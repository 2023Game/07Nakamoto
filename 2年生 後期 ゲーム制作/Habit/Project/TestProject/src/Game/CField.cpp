#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"
#include "CLineEffect.h"
#include "CWall.h"
#include <assert.h>
#include "CNavManager.h"
#include "CNavNode.h"
#include "CSceneManager.h"

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

	// �V�[���^�C�v�̎擾
	mScene = CSceneManager::Instance()->GetCurrentScene();
	switch (mScene)
	{
		// �e�X�g�X�e�[�W
		case EScene::eGame:
			// ���̃��f���f�[�^���擾
			mpModel = CResourceManager::Get<CModel>("Field");
			break;
		// �X�e�[�W1
		case EScene::eGame2:
			// ���̃��f���f�[�^���擾
			mpModel = CResourceManager::Get<CModel>("Map_mini_floor");
			break;
		// �X�e�[�W2
		case EScene::eGame3:
			// ���̃��f���f�[�^���擾
			mpModel = CResourceManager::Get<CModel>("Field");
			break;
	}
	// ���̃R���C�_�[�𐶐�
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
	switch (mScene)
	{
		// �X�e�[�W�P
		case EScene::eGame:
		{
			// �Ǉ@����
			CWall* wall = new CWall
			(
				CVector(20.0f, 0.0f, 0.0f),
				CVector(0.0f, 90.0f, 0.0f),
				CVector(5.0f, 5.0f, 5.0f)
			);
			mWalls.push_back(wall);	// ���������ǂ�ǂ̃��X�g�ɒǉ�

			// �ǇA����
			wall = new CWall
			(
				CVector(-85.0f, 0.0f, -50.0f),
				CVector(0.0f, 0.0f, 0.0f),
				CVector(5.0f, 5.0f, 5.0f)
			);
			mWalls.push_back(wall);	// ���������ǂ�ǂ̃��X�g�ɒǉ�

			wall = new CWall
			(
				CVector(-15.0f, 0.0f, -50.0f),
				CVector(0.0f, 0.0f, 0.0f),
				CVector(5.0f, 5.0f, 5.0f)
			);
			mWalls.push_back(wall);	// ���������ǂ�ǂ̃��X�g�ɒǉ�

			wall = new CWall
			(
				CVector(-155.0f, 0.0f, -50.0f),
				CVector(0.0f, 0.0f, 0.0f),
				CVector(5.0f, 5.0f, 5.0f)
			);
			mWalls.push_back(wall);	// ���������ǂ�ǂ̃��X�g�ɒǉ�

			// �ǇB����
			wall = new CWall
			(
				CVector(250.0f, 0.0f, 200.0f),
				CVector(0.0f, 0.0f, 0.0f),
				CVector(5.0f, 5.0f, 5.0f)
			);
			mWalls.push_back(wall);	// ���������ǂ�ǂ̃��X�g�ɒǉ�

			break;
		}
		// �X�e�[�W�Q
		case EScene::eGame2:
		{
			// �ǂ̐���
			CWall* wall = new CWall
			(
				CVector(0.0f, 0.0f, 0.0f),
				CVector(0.0f, 0.0f, 0.0f),
				CVector(1.0f, 1.0f, 1.0f)
			);
			mWalls.push_back(wall);	// ���������ǂ�ǂ̃��X�g�ɒǉ�

			break;
		}
		// �X�e�[�W3
		case EScene::eGame3:
		{
			// �ǂ̐���
			CWall* wall = new CWall
			(
				CVector(0.0f, 0.0f, 0.0f),
				CVector(0.0f, 0.0f, 0.0f),
				CVector(1.0f, 1.0f, 1.0f)
			);
			mWalls.push_back(wall);	// ���������ǂ�ǂ̃��X�g�ɒǉ�

			break;
		}
	}	
}

void CField::CreateFieldObjects()
{
	switch (mScene)
	{
		// �X�e�[�W�P
		case EScene::eGame:
			mpCubeModel = CResourceManager::Get<CModel>("FieldCube");
			mpCylinderModel = CResourceManager::Get<CModel>("FieldCylinder");
			break;

		// �X�e�[�W�Q
		case EScene::eGame2:

			mpDoor = CResourceManager::Get<CModel>("Map_mini_door");

			new CMoveFloor
			(
				mpDoor,
				CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
				CVector(0.0f, 0.0f, 0.0f), 0.5f
			);

			break;
		case EScene::eGame3:
			break;
	}

	//mpMap = CResourceManager::Get<CModel>("Map_mini");

	//// �}�b�v�@
	//new CMoveFloor
	//(
	//	mpMap,
	//	CVector(0.0f, 1.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f),
	//	CVector(0.0f, 0.0f, 0.0f), 5.0f
	//);

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

	switch (mScene)
	{
		// �X�e�[�W�P
		case EScene::eGame:
			if (navMgr != nullptr)
			{
				// �Ǉ@�̎���̌o�H�T���m�[�h
				new CNavNode(CVector(30.0f, 0.0f, 35.0f));
				new CNavNode(CVector(10.0f, 0.0f, 35.0f));
				new CNavNode(CVector(10.0f, 0.0f, -35.0f));
				new CNavNode(CVector(30.0f, 0.0f, -35.0f));

				// �ǇA�̎���̌o�H�T���m�[�h
				new CNavNode(CVector(-15.0f, 0.0f, -40.0f));
				new CNavNode(CVector(-15.0f, 0.0f, -60.0f));
				new CNavNode(CVector(-85.0f, 0.0f, -60.0f));
				new CNavNode(CVector(-85.0f, 0.0f, -40.0f));

				// �ǇB�̎���̌o�H�T���m�[�h
				new CNavNode(CVector(285.0f, 0.0f, 190.0f));
				new CNavNode(CVector(285.0f, 0.0f, 210.0f));
				new CNavNode(CVector(215.0f, 0.0f, 210.0f));
				new CNavNode(CVector(215.0f, 0.0f, 190.0f));
			}
			break;

		// �X�e�[�W�Q
		case EScene::eGame2:
			if (navMgr != nullptr)
			{
				// �Ǉ@�̎���̌o�H�T���m�[�h
				new CNavNode(CVector(25.0f, 0.0f, 25.0f));
				new CNavNode(CVector(70.0f, 0.0f, 25.0f));
				new CNavNode(CVector(70.0f, 0.0f, 90.0f));
				new CNavNode(CVector(25.0f, 0.0f, 90.0f));

				// �ǇA�̎���̌o�H�T���m�[�h
				new CNavNode(CVector(85.0f, 0.0f, 15.0f));
				new CNavNode(CVector(175.0f, 0.0f, 15.0f));
				new CNavNode(CVector(175.0f, 0.0f, 80.0f));
				new CNavNode(CVector(85.0f, 0.0f, 80.0f));

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
			break;
		// �X�e�[�W�Q
		case EScene::eGame3:
			break;
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
