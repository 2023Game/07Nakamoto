#ifndef CMODEL_H 
#define CMODEL_H

#include "CTriangle.h"
#include "CMaterial.h"

//vector�̃C���N���[�h
#include <vector>

/*
���f���N���X
���f���f�[�^�̓��͂�\��
*/
class CModel
{
public:
	//�f�X�g���N�^
	~CModel();
	//���f���t�@�C���̓���
	void Load(char* obj, char* mtl);
	//�`��
	void Render();

private:
	//�}�e���A���|�C���^�̉ϒ��z��
	std::vector<CMaterial*> mpMaterials;
	//�O�p�`�̉ϒ��z��
	std::vector<CTriangle> mTriangles;
};

#endif // !CMODEL_H 
