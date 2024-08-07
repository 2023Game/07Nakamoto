#ifndef CPLAYER_H
#define CPLAYER_H

#include "CCharacter.h"
#include "CInput.h"
#include "CBullet.h"
#include "CModel.h"

#include "CColliderHitManager.h"

#include <vector>

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/

class CPlayer :public CCharacter , public CColliderHitManager{
public:

	//�f�t�H���g�R���X�g���N�^
	CPlayer();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">�ʒu</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�k</param>
	CPlayer(const CVector& pos,
		const CVector& rot, const CVector& scale, CModel* model);

	//�X�V����
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="m">�����̃R���C�_</param>
	/// <param name="o">����̃R���C�_</param>
	void Collision(CCollider* m, CCollider* o);

	//�Փˏ���
	void Collision();

	//�R���C�_�̎擾
	CCollider *GetCollider();

	//�v���C���[�̃��f�����擾����
	static CModel* GetModelPlayer();
	static CPlayer* GetInstance();

private:
	static CPlayer* mpInstance;
	static CModel mModelPlayer;

	int mCursorX, mCursorY;	//�}�E�X�J�[�\���擾�p

	CInput mInput;

	CCollider mSphere;	//���R���C�_�[

	bool mBulletFlag;	//�e��������Ă邩
	bool mAir;	//�󒆂ɂ��邩

	//�ϒ��z��̃C���X�^���X
	CColliderHitManager mCollisionManager;

};

#endif // !CPLAYER_H
