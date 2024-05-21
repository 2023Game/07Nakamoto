#ifndef CPLAYER_H
#define CPLAYER_H

#include "CCharacter.h"
#include "CInput.h"
#include "CBullet.h"

/*
�v���C���[�N���X
�L�����N�^�N���X���p��
*/

class CPlayer :public CCharacter {
public:
	//�f�t�H���g�R���X�g���N�^
	CPlayer() {}

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

private:
	CInput mInput;
};

#endif // !CPLAYER_H
