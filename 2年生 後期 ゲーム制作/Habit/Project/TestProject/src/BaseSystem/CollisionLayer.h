#pragma once
#include <initializer_list>

// �Փ˔��背�C���[
enum class ELayer
{
	eNone = -1,
	eTest,
	eField,			// �t�B�[���h
	eWall,			// ��
	ePlayer,		// �v���C���[
	eEnemy,			// �G
	eItem,			// �A�C�e��
	eInteractObj,	// ���ׂ�I�u�W�F�N�g
	eInteractSearch,// ���ׂ�I�u�W�F�N�g�T�m�p

	eAttackCol,
};
// �Փ˔��背�C���[�̏��������X�g
using Layers = std::initializer_list<ELayer>;
