#pragma once
#ifndef CMODEL_H
#define CMODEL_H
//vector�̃C���N���[�h
#include <vector>
#include "CTriangle.h"
#include "CMaterial.h"

/*
���f���N���X
���f���f�[�^�̓��͂�\��
*/
class CModel
{
public:
	//�f�X�g���N�^�̐錾
	~CModel();
	//���f���t�@�C���̓���
	//Load(���f���t�@�C����,�}�e���A���t�@�C����)
	void Load(char* obj, char* mtl);
	//�`��
	void Render();

private:
	//�}�e���A���|�C���^�̉ϒ��z��
	std::vector<CMaterial*>mpMaterials;
	//�O�p�`�̉ϒ��z��
	std::vector<CTriangle> mTriangles;
	
};

#endif
