#pragma once
#include "CInteractObject.h"
#include "CColliderMesh.h"
#include <functional>

class CDoorBase : public CInteractObject
{
public:
	// �R���X�g���N�^
	CDoorBase(const CVector& pos, const CVector& angle, std::string modelName, std::string colName);
	// �f�X�g���N�^
	virtual ~CDoorBase();

	// �������ݒ肷��
	void SetOwner(CObjectBase* owner);
	// �h�A�̊J��Ԃ��؂�ւ�������ɌĂяo���֐��̃|�C���^��ݒ�
	void SetOnChangeFunc(std::function<void()> func);

	// ���ׂ���e�̃e�L�X�g�摜�̃p�X��Ԃ�
	std::string GetInteractTextPath() const override;

	// �h�A���J���Ă��邩�ǂ���
	bool IsOpened() const;

	// ���ׂ����Ԃ��ǂ���
	bool CanInteract() const override;
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

	// �`�揈��
	void Render() override;

protected:
	bool mIsOpened;	// �����J���Ă��邩�ǂ���
	bool mIsPlaying;	// �J�����ǂ���

private:
	CModel* mpDoor;	// ���f���f�[�^
	CModel* mpDoorCol;// �R���C�_�[�f�[�^

	CColliderMesh* mpDoorColliderMesh;	// �R���C�_�[


	CObjectBase* mpOwner;	//	���̃h�A�̎�����
	// �h�A�̊J��Ԃ��؂�ւ�������ɌĂяo���֐��̃|�C���^
	std::function<void()> mOnChangeFunc;

};