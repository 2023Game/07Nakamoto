#include "CGame.h"
#include "CTarget.h"
#include "CApplication.h"
#include "CCollider.h"

//�f�t�H���g�R���X�g���N�^
CGame::CGame()
	: mpUi(nullptr)
{
	//�I�̐��̏�����
	CTarget::InitNum();

	//Ui�̐���
	mpUi = new CUi();
	//�v���C���[�̐���
	mpPlayer = new CPlayer(CVector(0.0f, 0.0f, -5.0f), CVector(0.0f, 180.0f, 0.0f), 
		CVector(1.5f, 1.5f, 1.5f), CPlayer::GetModelPlayer());

	//�I�̃R���C�_�𐶐�
	new CTarget(CTarget::GetModelTarget(), CVector(-21.0f, 4.5f, -1.0f),
		CVector(0.0f, 90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(CTarget::GetModelTarget(), CVector(85.0f, 24.0f, 35.0f),
		CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(CTarget::GetModelTarget(), CVector(1.5f, 10.0f, 33.0f),
		CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(CTarget::GetModelTarget(), CVector(30.0f, 40.0f, -100.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::ESTAY);
	new CTarget(CTarget::GetModelTarget(), CVector(60.0f, 9.0f, 5.0f),
		CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CTarget::EState::EMOVE1);

	//�X�C�b�`�̐���
	new CSwitch(CSwitch::GetModelSwitch(),
		CVector(-17.0f, 10.0f, -11.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	//������
	new CMoveFloor(CVector(31.0f, 12.0f, -40.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));

	//��̐���
	new CSlope(CVector(-16.5f, 5.325f, 33.0f), CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.065f, 0.8f), CSlope::GetModelSlope());
	new CSlope(CVector(31.0f, 4.25f, -21.25f), CVector(0.0f, 90.0f, 0.0f),
		CVector(1.0f, 0.875f, 0.8f), CSlope::GetModelSlope());
	
	//��
	new CFloor(CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CFloor::GetModelFloor());
	//�I�u�W�F�N�g
	new CObject(CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), CObject::GetModelObject());

	//���_
	CVector e, c, u;
	//���_�����߂�
	e = CVector(60.0f, 2.0f, 10.0f);
	//�����_�����߂�
	c = CVector(0.0f, 0.0f, 0.0f);
	//����������߂�
	u = CVector(0, 1, 0);
	//�J�����ݒ�
	gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());
	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
	//�t�s��̎擾
	mModelViewInverse = mModelViewInverse.GetTranspose();
	mModelViewInverse.SetM(0, 3, 0);
	mModelViewInverse.SetM(1, 3, 0);
	mModelViewInverse.SetM(2, 3, 0);

	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::GetInstance()->Update();
	
}

//�f�X�g���N�^
CGame::~CGame()
{
	//�S�ẴC���X�^���X�폜
	CTaskManager::GetInstance()->AllDelete();
	CCollisionManager::GetInstance()->AllDelete();
	//Ui�𐶐����Ă���Ƃ�
	if (mpUi != nullptr)
	{
		//UI���폜���A������
		delete mpUi;
		mpUi = nullptr;
	}
}

//�X�^�[�g����
void CGame::Start()
{
	//���_
	CVector e, c, u;
	//���_�����߂�
	e = CVector(60.0f, 2.0f, 10.0f);
	//�����_�����߂�
	c = CVector(0.0f, 0.0f, 0.0f);
	//����������߂�
	u = CVector(0, 1, 0);
	//�J�����ݒ�
	gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());
	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
	//�t�s��̎擾
	mModelViewInverse = mModelViewInverse.GetTranspose();
	mModelViewInverse.SetM(0, 3, 0);
	mModelViewInverse.SetM(1, 3, 0);
	mModelViewInverse.SetM(2, 3, 0);

	//�}�b�v�̕`��
	CColliderMesh::GetModelFloor()->Render();
	CColliderMesh::GetModelObject()->Render();

	//�^�X�N�}�l�[�W���̕`��
	CTaskManager::GetInstance()->Render();

	mpUi->State();
}

//�X�V����
void CGame::Update() 
{
	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::GetInstance()->Update();
	//�R���W�����}�l�[�W���̏Փ˔���
	CTaskManager::GetInstance()->Collision();

	//�v���C���[�̏Փˏ���
	mpPlayer->HitCollision(mpPlayer->GetCollider(), CColliderHitManager::GetInstance()->HitColliderSerch());

	//HitManeger�̃��X�g�̍폜
	CColliderHitManager::GetInstance()->Delete();

	//���_
	CVector e, c, u;
	//���_�����߂�
	e = mpPlayer->GetPosition() + CVector(0, 5, -8) * mpPlayer->GetMatrixRotate();
	//�����_�����߂�
	c = mpPlayer->GetPosition() + CVector(0.0f, 3.0f, 0.0f);
	//����������߂�
	u = CVector(0, 1, 0) * mpPlayer->GetMatrixRotate();
	//�J�����ݒ�
	gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());
	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
	//�t�s��̎擾
	mModelViewInverse = mModelViewInverse.GetTranspose();
	mModelViewInverse.SetM(0, 3, 0);
	mModelViewInverse.SetM(1, 3, 0);
	mModelViewInverse.SetM(2, 3, 0);

	//�}�b�v�̕`��
	CColliderMesh::GetModelFloor()->Render();
	CColliderMesh::GetModelObject()->Render();

	//�^�X�N���X�g�̍폜
	CTaskManager::GetInstance()->Delete();
	//�^�X�N�}�l�[�W���̕`��
	CTaskManager::GetInstance()->Render();
	//�R���W�����}�l�[�W���̕`��(�R���C�_�m�F�p)
	//CCollisionManager::GetInstance()->Render();
	
	mpUi->Time();
	mpUi->Render();
	
}

//�Q�[���N���A����
void CGame::Clear()
{
	mpUi->Clear();
}

//�Q�[���N���A����
bool CGame::IsClear()
{
	return CTarget::GetNum() == 0 ;
}

//�Q�[���I�[�o�[����
void CGame::Over()
{
	
}

bool CGame::IsOver()
{
	return CUi::GetTime() == 0;
}

CMatrix CGame::mModelViewInverse;
//���f���r���[�s��̎擾
const CMatrix& CGame::ModelViewInverse()
{
	return mModelViewInverse;
}