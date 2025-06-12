#pragma once
#include "CXCharacter.h"

class CCollider;
class CInteractObject;
class CNavNode;

class CPlayerBase : public CXCharacter
{
public:
	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
		float speed;		// �A�j���[�V�����̍Đ����x�i1.0�œ��{�j
	};

	// �R���X�g���N�^
	CPlayerBase();
	// �f�X�g���N�^
	~CPlayerBase();

	/// <summary>
	/// ���삷�邩�ǂ����ݒ�
	/// </summary>
	/// <param name="operate">true�ł���΁A����J�n</param>
	virtual void SetOperate(bool operate);

	/// <summary>
	/// ���쒆�̃v���C���[���ǂ���
	/// </summary>
	/// <returns>true�ł���΁A���쒆</returns>
	bool IsOperate() const;


	/// <summary>
	/// ���쒆�̃J�����̃|�C���^��ݒ�
	/// </summary>
	/// <param name="camera">�J�����̃|�C���^</param>
	void SetCamera(CCamera* camera);

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="self">�Փ˂������g�̃R���C�_�[</param>
	/// <param name="other">�Փ˂�������̃R���C�_�[</param>
	/// <param name="hit">�Փ˂������̏��</param>
	void Collision(CCollider* self, CCollider* other, const CHitInfo& hit) override;

	// �X�V
	void Update() override;
	// �`��
	void Render() override;

protected:
	// �v���C���[�̏�����
	void InitPlayer(std::string path, const std::vector<AnimData>* pAnimData);

	// ��Ԑ؂�ւ�
	virtual void ChangeState(int state);

	// �A�j���[�V�����؂�ւ�
	void ChangeAnimation(int type, bool restart = false);

	int mState;				// ���
	int mStateStep;			// ��ԓ��̃X�e�b�v�Ǘ��p
	float mElapsedTime;		// �o�ߎ��Ԍv���p

	// �A�j���[�V�����f�[�^�̃e�[�u��
	const std::vector<AnimData>* mpAnimData;

	CVector mMoveSpeed;		// �O�㍶�E�̈ړ����x
	float mMoveSpeedY;		// �d�͂�W�����v�ɂ��㉺�̈ړ����x

	bool mIsGrounded;		// �ڒn���Ă��邩�ǂ���
	CVector mGroundNormal;	// �ڒn���Ă���n�ʂ̖@��

	CCollider* mpBodyCol;	// �{�̂̃R���C�_�[
	CCollider* mpSearchCol;	// ���ׂ�I�u�W�F�N�g��T�m����R���C�_�\

	// ��ԋ߂��ɂ��钲�ׂ���I�u�W�F�N�g���擾
	CInteractObject* GetNearInteractObj();
	// �߂��ɂ��钲�ׂ���I�u�W�F�N�g�̃��X�g
	std::vector<CInteractObject*> mNearInteractObjs;
	float mSearchDist;	// �߂��ɂ���I�u�W�F�N�g�𒲂ׂ�Ƃ��̋���

	float mFovAngle;	// ����͈͂̊p�x

	CTransform* mpRideObject;	// ��邱�Ƃ̏o����I�u�W�F�N�g��

	bool mIsOperate;	// ���̃v���C���[�𑀍쒆���ǂ���
	CCamera* mpCamera;	// ���̃v���C���[�𑀍삵�Ă��鎞�̃J����
};	