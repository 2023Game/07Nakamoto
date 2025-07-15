#pragma once
#include <string>

// �����N���X
class CRoom
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="center">�����̒��S�ʒu</param>
	/// <param name="size">�����̃T�C�Y</param>
	/// <param name="name">�����̖��O</param>
	CRoom(const CVector& center, const CVector& size, std::string name);
	// �f�X�g���N�^
	~CRoom();

	// �����̒��S�ʒu���擾
	const CVector& GetCenter() const;
	// �����̃T�C�Y���擾
	const CVector& GetSize() const;
	// �����̖��O���擾
	std::string GetName() const;
	// �����̃o�E���f�B���O�{�b�N�X��Ԃ�
	const CBounds& GetBounds() const;

#if _DEBUG
	// �`��
	void Render();
#endif

private:
	CVector mCenter;	// �����̒��S�ʒu
	CVector mSize;		// �����̃T�C�Y
	std::string mName;	// �����̖��O
	CBounds mBounds;	// �����̃o�E���f�B���O�{�b�N�X
};