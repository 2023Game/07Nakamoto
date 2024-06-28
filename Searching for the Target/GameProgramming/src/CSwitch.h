#ifndef CSWITCH_H
#define CSWITCH_H

#include "CCharacter.h"
#include "CCollider.h"

class CSwitch :public CCharacter
{
public:
	//�f�t�H���g�R���X�g���N�^
	CSwitch();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="pos">�ʒu</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�k</param>
	void SetSwitch(CModel* model, const CVector& pos,
		const CVector& rot, const CVector& scale);

	void SetSwitch(CSwitch* parent, CModel* model,
		const CVector& spos, const CVector& srot,const CVector& sscale,
		const CVector& dpos, const CVector& drot, const CVector& dscale);

	//�t���O���擾
	bool GetFlag();

	//�X�V����
	void Update();

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="m">�R���C�_1</param>
	/// <param name="o">�R���C�_2</param>
	void Collision(CCollider* m, CCollider* o);

	//�Փˏ���
	void Collision();

private:
	//�X�C�b�`�̃I���I�t
	bool mFlag;

	//���R���C�_
	CCollider mCollider;
};


#endif // !CSWITCH_H

