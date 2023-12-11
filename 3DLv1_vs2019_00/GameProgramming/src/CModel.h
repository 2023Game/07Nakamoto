#pragma once
#ifndef CMODEL_H
#define CMODEL_H
//vector�̃C���N���[�h
#include <vector>
#include "CTriangle.h"
#include "CMaterial.h"
#include "CVertex.h"

/*
���f���N���X
���f���f�[�^�̓��͂�\��
*/
class CModel
{
public:
	std::vector<CTriangle> Triangles() const;
	//�f�X�g���N�^�̐錾
	~CModel();
	//���f���t�@�C���̓���
	//Load(���f���t�@�C����,�}�e���A���t�@�C����)
	void Load(char* obj, char* mtl);
	//�`��
	void Render();
	//�`��
	//Render(�s��)
	void Render(const CMatrix& m);

private:
	//���_�̔z��
	CVertex* mpVertexes;
	void CreateVertexBuffer();
	//�}�e���A���|�C���^�̉ϒ��z��
	std::vector<CMaterial*>mpMaterials;
	//�O�p�`�̉ϒ��z��
	std::vector<CTriangle> mTriangles;
};

#endif
