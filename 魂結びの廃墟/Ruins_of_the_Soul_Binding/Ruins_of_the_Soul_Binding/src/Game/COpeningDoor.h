#pragma once
#include "CInteractObject.h"
#include "CColliderMesh.h"
#include <functional>

class COpeningDoor : public CInteractObject
{
public:
	// �R���X�g���N�^
	COpeningDoor(const CVector& pos, const CVector& angle, const CVector& openAngle);
	// �f�X�g���N�^
	~COpeningDoor();

	// �������ݒ肷��
	void SetOwner(CObjectBase* owner);
	// �h�A�̊J��Ԃ��؂�ւ�������ɌĂяo���֐��̃|�C���^��ݒ�
	void SetOnChangeFunc(std::function<void()> func);

	// �h�A���J���Ă��邩�ǂ���
	bool IsOpened() const;

	// ���ׂ�
	void Interact() override;
	// ��ꂽ���̏���
	void Death();

	/// <summary>
	/// ���C�ƃt�B�[���h�I�u�W�F�N�g�̏Փ˔���
	/// </summary>
	/// <param name="start">���C�̊J�n�ʒu</param>
	/// <param name="end">���C�̏I���ʒu</param>
	/// <param name="hit">�Փˏ��ԋp�p</param>
	/// <returns>�Փ˂�����Atrue��Ԃ�</returns>
	bool CollisionRay(const CVector& start, const CVector& end,
		CHitInfo* hit) override;

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	CModel* mpDoor;	// ���f���f�[�^
	CModel* mpDoorCol;// �R���C�_�[�f�[�^

	CColliderMesh* mpDoorColliderMesh;	// �R���C�_�[

	bool mIsOpened;	// �����J���Ă��邩�ǂ���

	CVector mCloseAngle;	// �����܂��Ă��鎞�̍��W
	CVector mOpenAngle;	// �����J���Ă��鎞�̍��W
	float mAnimTime;	// �J����
	float mElapsedTime;	// �o�ߎ��ԕۑ��p
	bool mIsPlaying;	// �J�����ǂ���

	CObjectBase* mpOwner;	//	���̃h�A�̎�����
	// �h�A�̊J��Ԃ��؂�ւ�������ɌĂяo���֐��̃|�C���^
	std::function<void()> mOnChangeFunc;

};