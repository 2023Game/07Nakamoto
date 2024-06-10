#include "CEffect.h"

CMaterial CEffect::sMaterial;	//�}�e���A���A�e�N�X�`��

//�R���X�g���N�^
CEffect::CEffect(const CVector& pos, float w, float h, char* texture
	, int row, int col, int fps)
	: CBillBoard(pos, w, h), mRows(row), mCols(col), mFps(fps), mFrame(0)
{
	//�e�N�X�`����ǂ�ł��Ȃ��ꍇ�͓ǂ�
	if (sMaterial.GetTexture()->Id() == 0)
	{
		sMaterial.GetTexture()->Load(texture);
		sMaterial.GetDiffuse()[0] = 1.0f;
		sMaterial.GetDiffuse()[1] = 1.0f;
		sMaterial.GetDiffuse()[2] = 1.0f;
		sMaterial.GetDiffuse()[3] = 1.0f;
	}
}

//�X�V
void CEffect::Update()
{
	//�R�}���̌v�Z
	int frame = mFrame++ / mFps;
	frame %= mRows * mCols;
	//UV��
	float left = 1.0f / mCols * (frame % mCols);
	//UV�E
	float right = left + 1.0f / mCols;
	//UV��
	float top = 1.0f - 1.0f / mRows * (frame / mCols);
	//UV��
	float bot = top - 1.0f / mRows;
	//�e�N�X�`���}�b�s���O
	mT[0].SetUV(CVector(right, top, 0.0f), CVector(left, bot, 0.0f),
		CVector(right, bot, 0.0f));
	mT[1].SetUV(CVector(left, top, 0.0f), CVector(left, bot, 0.0f),
		CVector(right, top, 0.0f));
	//�r���{�[�h�X�V
	CBillBoard::Update();
}

//�`��
void CEffect::Render()
{
	glDisable(GL_DEPTH_TEST);	//�[�x�e�X�g����
	CBillBoard::Render(&sMaterial);
	glEnable(GL_DEPTH_TEST);	//�[�x�e�X�g�L��
}