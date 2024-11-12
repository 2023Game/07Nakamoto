#pragma once
#include <list>
#include <vector>
#include "CTask.h"

class CNavNode;

// �o�H�T�������̊Ǘ��N���X
class CNavManager : public CTask
{
public:

private:
	// �o�H�Ǘ��N���X�̃C���X�^���X�ւ̃|�C���^
	static CNavManager* spInstance;

	// �o�H�T���p�̃m�[�h�̃��X�g
	std::list<CNavNode*> mNodes;

	// �o�H�T���̃f�o�b�O�\�����s�����ǂ���
	bool mIsRender;
};