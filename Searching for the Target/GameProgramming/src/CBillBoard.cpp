#include "CBillBoard.h"
#include "CApplication.h"

//�f�t�H���g�R���X�g���N�^
CBillBoard::CBillBoard()
{

}

//�R���X�g���N�^
CBillBoard::CBillBoard(CVector pos, float w, float h)
{
	SetBillBoard(pos, w, h);
}

//�R���X�g���N�^
CBillBoard::CBillBoard(CVector pos, float w, float h, int priority)
	: CCharacter(priority)
{
	SetBillBoard(pos, w, h);
}

//�r���{�[�h�̐ݒ�
void CBillBoard::SetBillBoard(CVector pos, float w, float h)
{
	//�ʒu
	mPosition = pos;
	//�傫���̐ݒ�
	mScale = CVector(1.0f, 1.0f, 1.0f);
	//�O�p�`�̒��_���W�ݒ�
	mT[0].SetVertex(CVector(w, h, 0.0f), CVector(-w, -h, 0.0f), CVector(w, -h, 0.0f));
	mT[1].SetVertex(CVector(-w, h, 0.0f), CVector(-w, -h, 0.0f), CVector(w, h, 0.0f));
	//�@����Z������
	mT[0].SetNormal(CVector(0.0f, 0.0f, 1.0f));
	mT[1].SetNormal(CVector(0.0f, 0.0f, 1.0f));
	//�F�𔒐F��ݒ�
	mMaterial.GetDiffuse()[0] = 1.0f;
	mMaterial.GetDiffuse()[1] = 1.0f;
	mMaterial.GetDiffuse()[2] = 1.0f;
	mMaterial.GetDiffuse()[3] = 1.0f;
}

//�X�V
void CBillBoard::Update()
{
	//�s��̍X�V
	CTransform::Update();
}

//�`��
void CBillBoard::Render()
{
	Render(&mMaterial);
}

//�`��(�}�e���A���̃|�C���^)
void CBillBoard::Render(CMaterial* mpMaterial)
{
	//�s��̕ۑ�
	glPushMatrix();
	//�g�k�A��]�A�ړ�������
	glMultMatrixf((CApplication::ModelViewInverse() * mMatrix).GetM());
	//���C�g�I�t
	glDisable(GL_LIGHTING);
	//�`��F�̐ݒ�
	glColor4fv(mpMaterial->GetDiffuse());
	//�}�e���A���K�p
	mpMaterial->Enabled();
	//�O�p�`�̕`��
	mT[0].Render();
	mT[1].Render();
	//�}�e���A������
	mpMaterial->Disabled();
	//���C�g�I��
	glEnable(GL_LIGHTING);
	//�s���߂�
	glPopMatrix();
	
}