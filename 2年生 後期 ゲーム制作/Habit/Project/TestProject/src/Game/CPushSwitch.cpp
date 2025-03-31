#include "CPushSwitch.h"
#include "Primitive.h"
#include "CPlayer2.h"
#include "Maths.h"
#include "CColliderSphere.h"
#include "CBillBoard.h"

#define INTERACT_TEXT_PATH_ON "UI\\Interact\\on.png"
#define INTERACT_TEXT_PATH_OFF "UI\\Interact\\off.png"
#define POSITION CVector(0.0f,15.0f,0.0f)

// �R���X�g���N�^
CPushSwitch::CPushSwitch(const CVector& pos, const CVector& angle, const CVector& size)
	: mSwitch(false)
{
	// ���ׂ�e�L�X�g�̉摜��ǂݍ���
	CResourceManager::Load<CTexture>(INTERACT_TEXT_PATH_ON, INTERACT_TEXT_PATH_ON);
	CResourceManager::Load<CTexture>(INTERACT_TEXT_PATH_OFF, INTERACT_TEXT_PATH_OFF);

	// �X�C�b�`�̃��f���f�[�^�擾
	mpModel = CResourceManager::Get<CModel>("Switch");

	// �X�C�b�`�̃R���C�_�[����
	mpCollider = new CColliderSphere
	(
		this, ELayer::eInteractObj,
		4.0f, true
	);
	mpCollider->Position(0.0f, 2.0f, 0.0f);
	mpCollider->SetCollisionTags({ ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::ePlayer, ELayer::eInteractSearch });

	// �ʒu�ƌ����ƃT�C�Y��ݒ�
	Position(pos);
	Rotation(angle);
	Scale(size);

	mInteractStr = "�I���ɂ���";
}

// �f�X�g���N�^
CPushSwitch::~CPushSwitch()
{
	// �R���C�_�[���폜
	SAFE_DELETE(mpCollider);
}

// �X�C�b�`�̏�Ԃ��I�����I�t��
bool CPushSwitch::IsOnSwtch()
{
	return mSwitch;
}

// ���ׂ�
void CPushSwitch::Interact()
{
	mSwitch = !mSwitch;
	mInteractStr = mSwitch ? "�I�t�ɂ���" : "�I���ɂ���";
}

// ���ׂ���e�̃e�L�X�g�摜�̃p�X��Ԃ�
std::string CPushSwitch::GetInteractTextPath() const
{
	return mSwitch ? INTERACT_TEXT_PATH_OFF : INTERACT_TEXT_PATH_ON;
}

// ���ׂ�UI��\��������W��Ԃ�
CVector CPushSwitch::GetInteractUIPos() const
{
	return Position() + CVector(0.0f, 10.0f, 0.0f);
}

// �`�揈��
void CPushSwitch::Render()
{
	if (mSwitch)
	{
		// �X�C�b�`�̃��f���f�[�^�擾
		mpModel = CResourceManager::Get<CModel>("Switch_ON");
	}
	else
	{
		// �X�C�b�`�̃��f���f�[�^�擾
		mpModel = CResourceManager::Get<CModel>("Switch_OFF");
	}

	mpModel->Render(Matrix());
}
