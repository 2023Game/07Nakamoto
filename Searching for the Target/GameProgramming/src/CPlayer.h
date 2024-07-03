#ifndef CPLAYER_H
#define CPLAYER_H

#include "CCharacter.h"
#include "CInput.h"
#include "CBullet.h"
#include "CColliderLine.h"
#include "CModel.h"

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/

class CPlayer :public CCharacter {
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
		const CVector& rot, const CVector& scale);

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

	//�J�[�\����X���W���擾
	float GetFx();
	//�J�[�\����Y���W���擾
	float GetFy();
private:
	int mCursorX, mCursorY;	//�}�E�X�J�[�\���擾�p
	float mFx, mFy;			//�J�����̈ړ���

	CInput mInput;

	bool mSlopeFlag;

	//CColliderLine mLine;	//�O��̐����R���C�_
	//CColliderLine mLine2;	//�㉺�̐����R���C�_
	//CColliderLine mLine3;	//���E�̐����R���C�_

	//CColliderLine mLine4;	//���̐����R���C�_

	CCollider mSphere;		//���R���C�_�[

	bool mBulletFlag;
};

#endif // !CPLAYER_H
