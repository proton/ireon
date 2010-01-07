#include "ireon_client/stdafx.h"
#include "common/interface/debug_overlay.h"

void _OUT(uint numLine, char* fmt, ... )
{
	static char g_stringDebugBuffer[512]; // буфер для преобразования строки
	static BYTE g_isOverlayDubugCreate = FALSE; // создан ли оверлей
	static OverlayContainer* g_overlayDebugInfo1 = NULL;
	static OverlayContainer* g_overlayDebugInfo2 = NULL;
	static OverlayContainer* g_overlayDebugInfo3 = NULL;
	static OverlayContainer* g_overlayDebugInfo4 = NULL;
	static OverlayContainer* g_overlayDebugInfo5 = NULL;

	if (!g_isOverlayDubugCreate) { // при первом обращении создаем оверлей
		// создаем оверлей
		Overlay* pDebugOverlay = OverlayManager::getSingleton().create("__debugOverlay");
		// создаем контейнер
		OverlayContainer* pDebugOverlayContainer =
			static_cast< OverlayContainer*>(OverlayManager::getSingleton().Ogre::OverlayManager::createOverlayElement
			("Panel", "__debugOverlayContainer", false));
		pDebugOverlayContainer->setMetricsMode(GMM_RELATIVE);
		pDebugOverlayContainer->setLeft(0.0);
		pDebugOverlayContainer->setTop((Real)0.01);
		pDebugOverlayContainer->setHeight((Real)0.4);
		pDebugOverlayContainer->setWidth(1);
		// строка 1
		g_overlayDebugInfo1 =
			static_cast< OverlayContainer*>(OverlayManager::getSingleton().Ogre::OverlayManager::createOverlayElement
			("TextArea", "__debugOverlayElement1", false));
		g_overlayDebugInfo1->setMetricsMode(GMM_PIXELS);
		g_overlayDebugInfo1->setLeft(0);
		g_overlayDebugInfo1->setTop(0);
		g_overlayDebugInfo1->setHeight(16);
		g_overlayDebugInfo1->setWidth(1000);
		g_overlayDebugInfo1->setParameter("font_name", "BlueHighway");
		g_overlayDebugInfo1->setParameter("char_height", "16" );
		g_overlayDebugInfo1->setParameter("colour_top", "1.0 1.0 1.0");
		g_overlayDebugInfo1->setParameter("colour_bottom", "1.0 1.0 1.0");
		pDebugOverlayContainer->addChild(g_overlayDebugInfo1);
		// строка 2
		g_overlayDebugInfo2 =
			static_cast< OverlayContainer*>(OverlayManager::getSingleton().Ogre::OverlayManager::createOverlayElement
			("TextArea", "__debugOverlayElement2", false));
		g_overlayDebugInfo2->setMetricsMode(GMM_PIXELS);
		g_overlayDebugInfo2->setLeft(0);
		g_overlayDebugInfo2->setTop(16);
		g_overlayDebugInfo2->setHeight(16);
		g_overlayDebugInfo2->setWidth(1000);
		g_overlayDebugInfo2->setParameter("font_name", "BlueHighway");
		g_overlayDebugInfo2->setParameter("char_height", "16" );
		g_overlayDebugInfo2->setParameter("colour_top", "1.0 1.0 1.0");
		g_overlayDebugInfo2->setParameter("colour_bottom", "1.0 1.0 1.0");
		pDebugOverlayContainer->addChild(g_overlayDebugInfo2);
		// строка 3
		g_overlayDebugInfo3 =
			static_cast< OverlayContainer*>(OverlayManager::getSingleton().Ogre::OverlayManager::createOverlayElement
			("TextArea", "__debugOverlayElement3", false));
		g_overlayDebugInfo3->setMetricsMode(GMM_PIXELS);
		g_overlayDebugInfo3->setLeft(0);
		g_overlayDebugInfo3->setTop(32);
		g_overlayDebugInfo3->setHeight(16);
		g_overlayDebugInfo3->setWidth(1000);
		g_overlayDebugInfo3->setParameter("font_name", "BlueHighway");
		g_overlayDebugInfo3->setParameter("char_height", "16" );
		g_overlayDebugInfo3->setParameter("colour_top", "1.0 1.0 1.0");
		g_overlayDebugInfo3->setParameter("colour_bottom", "1.0 1.0 1.0");
		pDebugOverlayContainer->addChild(g_overlayDebugInfo3);
		// строка 4
		g_overlayDebugInfo4 =
			static_cast< OverlayContainer*>(OverlayManager::getSingleton().Ogre::OverlayManager::createOverlayElement
			("TextArea", "__debugOverlayElement4", false));
		g_overlayDebugInfo4->setMetricsMode(GMM_PIXELS);
		g_overlayDebugInfo4->setLeft(0);
		g_overlayDebugInfo4->setTop(48);
		g_overlayDebugInfo4->setHeight(16);
		g_overlayDebugInfo4->setWidth(1000);
		g_overlayDebugInfo4->setParameter("font_name", "BlueHighway");
		g_overlayDebugInfo4->setParameter("char_height", "16" );
		g_overlayDebugInfo4->setParameter("colour_top", "1.0 1.0 1.0");
		g_overlayDebugInfo4->setParameter("colour_bottom", "1.0 1.0 1.0");
		pDebugOverlayContainer->addChild(g_overlayDebugInfo4);
		// строка 5
		g_overlayDebugInfo5 =
			static_cast< OverlayContainer*>(OverlayManager::getSingleton().Ogre::OverlayManager::createOverlayElement
			("TextArea", "__debugOverlayElement5", false));
		g_overlayDebugInfo5->setMetricsMode(GMM_PIXELS);
		g_overlayDebugInfo5->setLeft(0);
		g_overlayDebugInfo5->setTop(64);
		g_overlayDebugInfo5->setHeight(16);
		g_overlayDebugInfo5->setWidth(1000);
		g_overlayDebugInfo5->setParameter("font_name", "BlueHighway");
		g_overlayDebugInfo5->setParameter("char_height", "16" );
		g_overlayDebugInfo5->setParameter("colour_top", "1.0 1.0 1.0");
		g_overlayDebugInfo5->setParameter("colour_bottom", "1.0 1.0 1.0");
		pDebugOverlayContainer->addChild(g_overlayDebugInfo5);

		pDebugOverlay->add2D(pDebugOverlayContainer); // добавляем контейнер на оверлей
		pDebugOverlay->show();

		g_isOverlayDubugCreate = TRUE;
	}

	try {
		if (!fmt) return;
		if (!g_overlayDebugInfo1) return;
		if (!g_overlayDebugInfo2) return;
		if (!g_overlayDebugInfo3) return;
		if (!g_overlayDebugInfo4) return;
		if (!g_overlayDebugInfo5) return;

		static WORD num=0; // порядковый номер сообщения
		::sprintf(g_stringDebugBuffer, "[%d]   ", num);
		String string = g_stringDebugBuffer;
		num ++;

		va_list args;
		va_start( args, fmt );
		vsprintf( &g_stringDebugBuffer[0], fmt, args ); // форматирование строки
		va_end( args );

		// циклический сдвиг вверх
		//g_overlayDebugInfo1->setCaption(g_overlayDebugInfo2->getCaption());
		//g_overlayDebugInfo2->setCaption(g_overlayDebugInfo3->getCaption());
		//g_overlayDebugInfo3->setCaption(g_overlayDebugInfo4->getCaption());
		//g_overlayDebugInfo4->setCaption(g_overlayDebugInfo5->getCaption());

		//string += g_stringDebugBuffer; // приписываем порядковый номер
		switch(numLine)
		{
		case 1:
			g_overlayDebugInfo1->setCaption(g_stringDebugBuffer); // и выводим
			break;
		case 2:
			g_overlayDebugInfo2->setCaption(g_stringDebugBuffer); // и выводим
			break;
		case 3:
			g_overlayDebugInfo3->setCaption(g_stringDebugBuffer); // и выводим
			break;
		case 4:
			g_overlayDebugInfo4->setCaption(g_stringDebugBuffer); // и выводим
			break;
		case 5:
			g_overlayDebugInfo5->setCaption(g_stringDebugBuffer); // и выводим
			break;

		default:
			g_overlayDebugInfo5->setCaption(g_stringDebugBuffer); // и выводим
			break;
		}


	} catch (...) {}
}