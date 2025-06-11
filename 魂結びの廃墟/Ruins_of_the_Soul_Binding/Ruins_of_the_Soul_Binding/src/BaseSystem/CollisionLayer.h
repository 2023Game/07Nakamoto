#pragma once
#include <initializer_list>

// �Փ˔��背�C���[
enum class ELayer
{
	eNone = -1,
	eTest,
	eField,
	eWall,
	eDoor,
	eGround,
	ePlayer,
	eEnemy,
	eInteractObj,	// ���ׂ�I�u�W�F�N�g
	eInteractSearch,// ���ׂ�I�u�W�F�N�g�T�m�p
	eDemon,			// �d�͂̍U�����s����I�u�W�F�N�g
	eAttackCol,
};
// �Փ˔��背�C���[�̏��������X�g
using Layers = std::initializer_list<ELayer>;
