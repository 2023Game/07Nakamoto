#ifndef CMATERIAL_H
#define CMATERIAL_H
#define MATERIAL_NAME_LEN 64	//���O�̒���

#include "CTexture.h"

/*
�}�e���A���N���X
�}�e���A���̃f�[�^������
*/
class CMaterial
{
public:
	//�f�t�H���g�R���X�g���N�^
	CMaterial();
	//�}�e���A����L���ɂ���
	void Enabled();
	//�}�e���A���𖳌��ɂ���
	void Disabled();
	//�e�N�X�`���̎擾
	CTexture* GetTexture();
	//�}�e���A�����O�̎擾
	char* GetName();

	/// <summary>
	/// �}�e���A���̖��O�̎擾
	/// </summary>
	/// <param name="name">�}�e���A���̖��O</param>
	void SetName(char* name);

	/// �g�U�Z�̎擾
	float* GetDiffuse();

private:
	//�e�N�X�`��
	CTexture mTexture;
	//�}�e���A����
	char mName[MATERIAL_NAME_LEN + 1];
	//�g�U�Z�̐F RGBA
	float mDiffuse[4];
};

#endif // !CMATERIAL_H
