#pragma once
#include "CObjectBase.h"
#include "CModel.h"

// �����ɂ��΋��̃N���X
class CFireball : public CObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^ 
	/// </summary>
	/// <param name="speed">�΋��̑��x</param>
	/// <param name="dist">�΋����ړ��ł��鋗��</param>
	CFireball(float speed, float dist);
	// �f�X�g���N�^
	~CFireball();

	// �Փˏ���
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

private:
	CModel* mpModel;		// �΋��̃��f���f�[�^
	CCollider* mpCollider;	// �΋��̃R���C�_�[
	float mMoveSpeed;		// �ړ����x
	float mMoveDist;		// �ړ��ł��鋗��
	float mCurrDist;		// ���݂̈ړ�����
};