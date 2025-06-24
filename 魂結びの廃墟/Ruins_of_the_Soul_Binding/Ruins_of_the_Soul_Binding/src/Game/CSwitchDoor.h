#pragma once
#include "CObjectBase.h"
#include <functional>

class CModel;
class CColliderMesh;
class CSwitch;
class CNavNode;

class CSwitchDoor : public CObjectBase
{
public:
	// �R���X�g���N�^
	CSwitchDoor(const CVector& pos, const CVector& angle, const CVector& openPos,
		std::string modelName, std::string colName);
	// �f�X�g���N�^
	~CSwitchDoor();

	// �ڑ�����X�C�b�`��ǉ�
	void AddSwitch(CSwitch* sw);
	// �h�A���J���Ă��邩�ǂ���
	bool IsOpened() const;

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
	// �X�C�b�`���S�ĉ�����Ă��邩�ǂ���
	bool IsSwitchOn() const;

	CModel* mpModel;	// ���̃��f���f�[�^
	CColliderMesh* mpColliderMesh;	// ���̃R���W�����f�[�^

	// ���̃h�A���J�����߂ɉ����K�v������X�C�b�`
	std::list<CSwitch*> mpSwitches;

	bool mIsOpened;	// �����J���Ă��邩�ǂ���

	CVector mClosePos;	// �����܂��Ă��鎞�̍��W
	CVector mOpenPos;	// �����J���Ă��鎞�̍��W
	float mAnimTime;	// �J����
	float mElapsedTime;	// �o�ߎ��ԕۑ��p
	bool mIsPlaying;	// �J�����ǂ���

	CObjectBase* mpOwner;	// ���̔��̎�����

	CNavNode* mpNavNode1;	//���̌o�H�T���m�[�h�P��
	CNavNode* mpNavNode2;	//���̌o�H�T���m�[�h�Q��
};