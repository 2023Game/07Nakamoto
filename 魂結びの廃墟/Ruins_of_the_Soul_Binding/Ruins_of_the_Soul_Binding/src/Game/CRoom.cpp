#include "CRoom.h"
#include "CRoomManager.h"
#include "Primitive.h"

// �o�E���f�B���O�{�b�N�X�̃T�C�Y�𕔉��̃T�C�Y���珬�������钷��
#define BOUNDS_REDUCE_DIST 3.0f

// �R���X�g���N�^
CRoom::CRoom(const CVector& center, const CVector& size, std::string name)
	: mCenter(center)
	, mSize(size)
	, mName(name)
{
	// �����Ǘ��N���X�̃��X�g�Ɏ��M��o�^
	CRoomManager::Instance()->Add(this);

	// �����̃o�E���f�B���O�{�b�N�X�̐ݒ�
	CVector boundsSize = mSize;
	boundsSize.X(boundsSize.X() - BOUNDS_REDUCE_DIST * 2.0f);
	boundsSize.Z(boundsSize.Z() - BOUNDS_REDUCE_DIST * 2.0f);
	mBounds.SetPos(mCenter, boundsSize);
}

// �f�X�g���N�^
CRoom::~CRoom()
{
	// �����Ǘ��N���X�̃��X�g���玩�g����菜��
	CRoomManager::Instance()->Remove(this);
}

// �����̒��S�ʒu���擾
const CVector& CRoom::GetCenter() const
{
	return mCenter;
}

// �����̃T�C�Y���擾
const CVector& CRoom::GetSize() const
{
	return mSize;
}

// �����̖��O���擾
std::string CRoom::GetName() const
{
	return mName;
}

// �����̃o�E���f�B���O�{�b�N�X��Ԃ�
const CBounds& CRoom::GetBounds() const
{
	return mBounds;
}

// �`��
void CRoom::Render()
{
	CColor color = CColor::cyan;
	// �����̃T�C�Y�Ń��C���[�{�b�N�X��`��
	Primitive::DrawWireBox(mCenter, mSize, color);
	// �����̃o�E���f�B���O�{�b�N�X�̃T�C�Y��`��
	color.A(0.5f);
	Primitive::DrawBox(mBounds.Center(), mBounds.Size(), color);
}
