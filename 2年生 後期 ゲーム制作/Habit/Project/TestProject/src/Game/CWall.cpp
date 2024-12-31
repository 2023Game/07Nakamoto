#include "CWall.h"
#include "CSceneManager.h"

// �R���X�g���N�^
CWall::CWall(const CVector& pos, const CVector& angle, const CVector& size)
{
	// ���݂̃V�[�����擾
	mScene = CSceneManager::Instance()->GetCurrentScene();
	switch (mScene)
	{
	// �Q�[���V�[���P
	case EScene::eGame:
		{
			// �ǂ̃��f���f�[�^�擾
			mpModel = CResourceManager::Get<CModel>("Wall");

			// �ǂ̃R���C�_�[���쐬
			CModel* colModel = CResourceManager::Get<CModel>("WallCol");
			mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);

			break;
		}
	// �Q�[���V�[��2
	case EScene::eGame2:
		{
			// �ǂ̃��f���f�[�^�擾
			mpModel = CResourceManager::Get<CModel>("Map_mini");

			// �ǂ̃R���C�_�[���쐬
			CModel* colModel = CResourceManager::Get<CModel>("Map_mini_Col");
			mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);
			break;
		}
	// �Q�[���V�[��2
	case EScene::eGame3:
		{
			// �ǂ̃��f���f�[�^�擾
			mpModel = CResourceManager::Get<CModel>("Wall");

			// �ǂ̃R���C�_�[���쐬
			CModel* colModel = CResourceManager::Get<CModel>("Wall_Col");
			mpColliderMesh = new CColliderMesh(this, ELayer::eWall, colModel, true);
			break;
		}
	}

	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);

}

// �f�X�g���N�^
CWall::~CWall()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpColliderMesh);
}

// ���C�ƕǂƂ̏Փ˔���
bool CWall::CollisionRay(const CVector& start, const CVector& end, CHitInfo* hit)
{
	//�ǂ̃R���C�_�[�����݂��Ȃ���΁A�Փ˂��Ă��Ȃ�
	if (mpColliderMesh == nullptr) return false;

	return CCollider::CollisionRay(mpColliderMesh, start, end, hit);
}

// �X�V����
void CWall::Update()
{
}

// �`�揈��
void CWall::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}
