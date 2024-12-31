#include "CClearArea.h"
#include "CSceneManager.h"

// �R���X�g���N�^
CClearArea::CClearArea(const CVector& pos, const CVector& angle, const CVector& size)
	: CObjectBase(ETag::eClear, ETaskPriority::eDefault, 0, ETaskPauseType::eGame)
{	// �N���A�G���A�̃��f���f�[�^���擾
	mpModel = CResourceManager::Get<CModel>("Goal");

	// �N���A�G���A�̃R���C�_�[���쐬
	mpColliderMesh = new CColliderMesh(this, ELayer::eWall, mpModel, true);	
	// �v���C���[�ƃt�B�[���h�ƏՓ˂���悤�ɐݒ�
	mpColliderMesh->SetCollisionTags({ ETag::ePlayer });
	mpColliderMesh->SetCollisionLayers({ ELayer::ePlayer });

	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);
}

// �f�X�g���N�^
CClearArea::~CClearArea()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpColliderMesh);
}

void CClearArea::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �v���C���[�ɏՓ˂���
	if (other->Layer() == ELayer::ePlayer)
	{
		// �Q�[���I�[�o�[�V�[����ǂݍ���
		CSceneManager::Instance()->LoadScene(EScene::eClear);
	}
}

//// �X�V����
//void CClearArea::Update()
//{
//}

// �`�揈��
void CClearArea::Render()
{
	mpModel->SetColor(mColor);
	mpModel->Render(Matrix());
}
