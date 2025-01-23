#include "CGameScene3.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer2.h"
#include "CEnemy1.h"
#include "CEnemy2.h"
#include "CGameCamera.h"
#include "CGameCamera2.h"
#include "CInput.h"
//#include "CGameMenu.h"
#include "CInventory.h"
#include "CBGMManager.h"
#include "CLineEffect.h"
#include "CNavManager.h"
#include "CPushSwitch.h"
#include "CPushSwitchManager.h"
#include "CSwitchDoor.h"
#include "CClearArea.h"
#include "ItemData.h"

// �R���X�g���N�^
CGameScene3::CGameScene3()
	: CSceneBase(EScene::eGame3)
{
}

// �f�X�g���N�^
CGameScene3::~CGameScene3()
{
	// ���ɓǂݍ��܂��V�[�����Q�[���V�[���łȂ���΁A
	// �A�C�e���̃��\�[�X��S�Ĕj������
	CSceneManager* sceneMgr = CSceneManager::Instance();
	if (sceneMgr != nullptr && !sceneMgr->IsNextGameScene())
	{
		Item::DeleteItemResources();
	}
}

// �V�[���ǂݍ���
void CGameScene3::Load()
{
	// �Q�[����ʂ̓J�[�\����\��
	CInput::ShowCursor(false);
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//�����ŃQ�[�����ɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��
	CResourceManager::Load<CModel>("Field", "Field\\stage1\\floor.obj");
	CResourceManager::Load<CModel>("Wall", "Field\\stage1\\wall.obj");
	CResourceManager::Load<CModel>("Wall_Col", "Field\\stage1\\wall_col.obj");
	CResourceManager::Load<CModel>("Gimmick_Wall", "Field\\stage1\\gimmick_wall.obj");
	CResourceManager::Load<CModel>("Switch", "Object\\Switch.obj");
	CResourceManager::Load<CModel>("Goal", "Object\\clear_area.obj");
	CResourceManager::Load<CModelX>("Player2", "Character\\Player2\\pico.x");
	CResourceManager::Load<CModelX>("Enemy2", "Character\\Enemy\\warrok\\warrok.x");

	// �A�C�e���̃��\�[�X��S�ǂݍ���
	Item::LoadItemResources();

	// �Q�[��BGM��ǂݍ���
	CBGMManager::Instance()->Play(EBGMType::eGame);

	// �o�H�T���Ǘ��N���X���쐬
	new CNavManager();
	// �t�B�[���h�쐬
	new CField();

	// �v���C���[����
	CPlayer2* pico = new CPlayer2();
	pico->Scale(1.0f, 1.0f, 1.0f);
	pico->Position(0.0f, 0.0f, 0.0f);

	// �G�@����
	CEnemy2* enemy1 = new CEnemy2
	(
		{
			CVector(110.0f, 0.0f, 100.0f),
			CVector(-80.0f, 0.0f, 100.0f),
			CVector(-90.0f, 0.0f, 0.0f),
			CVector(-180.0f, 0.0f, 0.0f),
			CVector(-180.0f, 0.0f, 100.0f),
			CVector(-180.0f, 0.0f, 0.0f),
			CVector(120.0f,0.0f,0.0f)
		}
	);
	enemy1->Scale(1.0f, 1.0f, 1.0f);
	enemy1->Position(CVector(0.0f, 0.0, 100.0f));

	// �G�A����
	CEnemy2* enemy2 = new CEnemy2
	(
		{
			CVector(-30.0f, 0.0f, 190.0f),
			CVector(-30.0f, 0.0f, 260.0f),
			CVector(-120.0f, 0.0f, 320.0f),
			CVector(-180.0f, 0.0f, 320.0f),
			CVector(-180.0f, 0.0f, 450.0f),
			CVector(0.0f, 0.0f, 450.0f),
			CVector(0.0f, 0.0f, 350.0f),
			CVector(-100.0f, 0.0f, 310.0f),
			CVector(-100.0f, 0.0f, 280.0f),
			CVector(-40.0f, 0.0f, 270.0f),
			CVector(90.0f, 0.0f, 280.0f),
			CVector(90.0f, 0.0f, 440.0f),
			CVector(100.0f, 0.0f, 190.0f),
		}
	);
	enemy2->Scale(1.0f, 1.0f, 1.0f);
	enemy2->Position(CVector(70.0f, 0.0, 190.0f));

	//// �G�B����
	//CEnemy2* enemy3 = new CEnemy2
	//(
	//	{
	//		CVector(110.0f, 0.0f, 100.0f),
	//		CVector(-80.0f, 0.0f, 100.0f),
	//		CVector(-90.0f, 0.0f, 0.0f),
	//		CVector(-180.0f, 0.0f, 0.0f),
	//		CVector(-180.0f, 0.0f, 100.0f),
	//		CVector(-180.0f, 0.0f, 0.0f),
	//		CVector(120.0f,0.0f,0.0f)
	//	}
	//);
	//enemy3->Scale(1.0f, 1.0f, 1.0f);
	//enemy3->Position(CVector(0.0f, 0.0, 100.0f));

	// �X�C�b�`�𐶐�
	CPushSwitch* push_switch = new CPushSwitch(CVector(-93.0f, 0.0f, 163.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
#if _DEBUG
	push_switch->SetDebugName("Switch1");
#endif

	// �X�C�b�`�𐶐�
	CPushSwitch* push_switch2 = new CPushSwitch(CVector(-175.0f, 0.0f, 243.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
#if _DEBUG
	push_switch2->SetDebugName("Switch2");
#endif

	// �X�C�b�`�𐶐�
	CPushSwitch* push_switch3 = new CPushSwitch(CVector(-153.0f, 0.0f, 127.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
#if _DEBUG
	push_switch3->SetDebugName("Switch3");
#endif

	// �X�C�b�`�𐶐�
	CPushSwitch* push_switch4 = new CPushSwitch(CVector(175.0f, 0.0f, 313.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
#if _DEBUG
	push_switch4->SetDebugName("Switch4");
#endif

	// �X�C�b�`�𐶐�
	CPushSwitch* push_switch5 = new CPushSwitch(CVector(-85.0f, 0.0f, 433.0f),
		CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f));
#if _DEBUG
	push_switch5->SetDebugName("Switch5");
#endif

	// �X�C�b�`�ŊJ�����@���쐬
	CSwitchDoor* door = new CSwitchDoor
	(
		CVector(-185.0f, 0.0f, 145.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door->SetAnimPos
	(
		CVector(-155.0f, 0.0f, 145.0f),	// �����J�������̍��W
		CVector(-185.0f, 0.0f, 145.0f)	// ����������̍��W
	);
	door->AddSwitch(push_switch2);		// ��������J���X�C�b�`��ǉ�
	door->AddSwitch(push_switch3);		// ��������J���X�C�b�`��ǉ�
	door->AddSwitch(push_switch5);		// ��������J���X�C�b�`��ǉ�

	// �X�C�b�`�ŊJ�����A���쐬
	CSwitchDoor* door2 = new CSwitchDoor
	(
		CVector(0.0f, 0.0f, 75.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door2->SetAnimPos
	(
		CVector(-75.0f, 0.0f, 225.0f),	// �����J�������̍��W
		CVector(-75.0f, 0.0f, 195.0f)	// ����������̍��W
	);
	door2->AddSwitch(push_switch2);		// ��������J���X�C�b�`��ǉ�
	door2->AddSwitch(push_switch3);		// ��������J���X�C�b�`��ǉ�
	door2->AddSwitch(push_switch5);		// ��������J���X�C�b�`��ǉ�

	// �X�C�b�`�ŊJ�����B���쐬
	CSwitchDoor* door3 = new CSwitchDoor
	(
		CVector(45.0f, 0.0f, 345.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door3->SetAnimPos
	(
		CVector(45.0f, 0.0f, 375.0f),	// �����J�������̍��W
		CVector(45.0f, 0.0f, 345.0f)	// ����������̍��W
	);
	door3->AddSwitch(push_switch4);		// ��������J���X�C�b�`��ǉ�

	// �X�C�b�`�ŊJ�����C���쐬
	CSwitchDoor* door4 = new CSwitchDoor
	(
		CVector(-75.0f, 0.0f, 385.0f),
		CVector(0.0f, 90.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);
	door4->SetAnimPos
	(
		CVector(-75.0f, 0.0f, 415.0f),	// �����J�������̍��W
		CVector(-75.0f, 0.0f, 385.0f)	// ����������̍��W
	);
	door4->AddSwitch(push_switch);		// ��������J���X�C�b�`��ǉ�
	door4->AddSwitch(push_switch2);		// ��������J���X�C�b�`��ǉ�
	door4->AddSwitch(push_switch3);		// ��������J���X�C�b�`��ǉ�
	door4->AddSwitch(push_switch4);		// ��������J���X�C�b�`��ǉ�
	door4->AddSwitch(push_switch5);		// ��������J���X�C�b�`��ǉ�

	CClearArea* clearArea = new CClearArea
	(
		CVector(-105.0f, 0.0f, 385.0f),
		CVector(0.0f, 0.0f, 0.0f),
		CVector(1.0f, 1.0f, 1.0f)
	);

	// CGameCamera2�̃e�X�g
	CVector atPos = pico->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera2* mainCamera = new CGameCamera2
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	mainCamera->SetFollowTargetTf(pico);

	// �Q�[�����j���[���쐬
	new CInventory();
}

// �V�[���X�V����
void CGameScene3::Update()
{
	if (CInput::PushKey('H'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}

	// �Q�[�����j���[���J���ĂȂ���΁A[�l]�L�[�Ń��j���[���J��
	CInventory* inv = CInventory::Instance();
	if (!inv->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			inv->Open();
		}
	}
}
