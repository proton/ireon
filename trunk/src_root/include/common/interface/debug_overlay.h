// get on OGRE3D.ORG.RU
// AUTHOR: MY.NAME

//=========================================================================================
#ifndef __dubugOverlay_H__
#define __dubugOverlay_H__
//=========================================================================================
// ���� ������� [my.name (my.work(a)inbox.ru)]
//=========================================================================================
#ifndef _DEBUG
// ����� ������ ��� �������
#define _OUT
#else
//=========================================================================================
#include "Ogre.h"
using namespace Ogre;

void _OUT(uint numLine, char* fmt, ... );

//=========================================================================================
#endif
//=========================================================================================
#endif
//=========================================================================================