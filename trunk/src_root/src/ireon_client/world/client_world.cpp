/**
  * @file client_world.cpp
  * Client world manager
  */
 
 /* Copyright (C) 2005 ireon.org developers council
  * $Id: client_world.cpp 864 2007-08-20 07:28:11Z mip $
 
  *  See LICENSE for details
  */
#include "stdafx.h"
#include "ireon_client/world/client_world.h"
#include "common/world/managers/dynamic_collection.h"
#include "common/world/managers/dispatcher.h"
#include "common/world/managers/command.h"

#include "common/world/dynamic_object/character.h"
#include "common/world/client_zone.h"
#include "ireon_client/interface/client_event_manager.h"
#include "ireon_client/xsd/config_xsd.h"
#include "common/interface/debug_axis.h"
#include "common/world/mesh.h"
CClientWorld* CClientWorld::m_singleton = 0;

CClientWorld::CClientWorld()
{
};

CClientWorld::~CClientWorld()
{
	assert(zone_&&"zone_ don't may be destroyed, because ptr on her equal 0");
	delete zone_;

};

bool CClientWorld::init()
{

	CWorld::init();

	return true;
};

bool CClientWorld::createScene()
{
	if( !CWorld::createScene() )
		return false;

    // Set default mipmap level (NB some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	Camera* cam = sceneManager()->createCamera("PlayerCam");
	// Position it at 500 in Z direction
	cam->setPosition(Ogre::Vector3(0,0,0));
	
	cam->setNearClipDistance(1);
	cam->setFarClipDistance(1000);

	Ogre::Viewport* vp = CClientApp::instance()->getRenderWindow()->addViewport(cam);
	// Alter the camera aspect ratio to match the viewport
	cam->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));


	sceneManager()->setSkyBox(true,"Examples/MorningSkyBox");

	g_AxisCreate( sceneManager() );
	g_AxisShow(true);

	CDispatcher::getSingletonPtr()->executeNow(CommandPtr(new CCreateOwnPlayerCommand("ownPlayer")));

	COwnPlayer* ownPlayer = CDynamicCollection::getSingletonPtr()->getOwnPlayer();
	ownPlayer->setPosition3D(Vector3(-35,6.90349,-17));
	
	return true;
};

bool CClientWorld::loadZones()
{
	_log(_info,_loc,"Start loading zones");
	
	String pathToProto;
	if ( I_APP->getConfig()->world().present() &&
		I_APP->getConfig()->world().get().proto().present() &&
		I_APP->getConfig()->world().get().proto().get().directory().present())
		pathToProto = I_APP->getConfig()->world().get().proto().get().directory().get();
	else
		pathToProto = ireon::client::CProtoSettings::directory::default_value();
	
	String pathToZonesConfig = pathToProto.append("zones.xml");
	
	zone_ = new CClientZone("first_zone.xml");
	
 	CClientWorld::instance()->showSimpleNavigationMesh();

	_log(_info,_loc,"End loading zones\n");
 	return true;
 };
 
 bool CClientWorld::load()
 {
 
 	_log(_info,_loc,"Loading world...\n");


	String setting;
	//read ambient light rgb
	Ogre::ColourValue ambientLightColour(0,0,0,0);
	String pathToZones;
	if ( I_APP->getConfig()->world().present() &&
		 I_APP->getConfig()->world().get().light().present() 
		 )
		ambientLightColour += StringConverter::parseColourValue( I_APP->getConfig()->world().get().light().get().ambient() );
	else
		ambientLightColour += StringConverter::parseColourValue( ireon::client::CLightSettings::ambient::default_value() );

	//read main light rgb
	Ogre::ColourValue mainLightColour(0,0,0,0);
	if ( I_APP->getConfig()->world().present() &&
		I_APP->getConfig()->world().get().light().present() 
		)
		mainLightColour += StringConverter::parseColourValue( I_APP->getConfig()->world().get().light().get().main() );
	else
		mainLightColour += StringConverter::parseColourValue( ireon::client::CLightSettings::main::default_value() );

	bool shadowsEnabled;
	if ( I_APP->getConfig()->world().present() &&
		I_APP->getConfig()->world().get().light().present() &&
		I_APP->getConfig()->world().get().light().get().shadows().present() 
		)
		shadowsEnabled = I_APP->getConfig()->world().get().light().get().shadows().get().enabled() ;
	else
		shadowsEnabled =  ireon::client::CShadowSettings::enabled::default_value();

	Ogre::ColourValue shadowsColour(0, 0, 0, 0);
	if ( I_APP->getConfig()->world().present() &&
		I_APP->getConfig()->world().get().light().present() &&
		I_APP->getConfig()->world().get().light().get().shadows().present() 
		)
		shadowsColour += StringConverter::parseColourValue( I_APP->getConfig()->world().get().light().get().shadows().get().colour() );
	else
		shadowsColour += StringConverter::parseColourValue( ireon::client::CShadowSettings::colour::default_value() );

	bool lightEnabled;
	if ( I_APP->getConfig()->world().present() &&
		I_APP->getConfig()->world().get().light().present() 
		)
		lightEnabled = I_APP->getConfig()->world().get().light().get().enabled() ;
	else
		lightEnabled =  ireon::client::CLightSettings::enabled::default_value();

 	// Set ambient light
 	sceneManager()->setAmbientLight(ambientLightColour);
 
	if (lightEnabled)
	{
		// Create a light
		Light* l = sceneManager()->createLight("MainLight");
		l->setType(Ogre::Light::LT_POINT);
		l->setPosition(0,400,-400);
 		l->setSpecularColour(mainLightColour);
 		_log(_info,_loc,_("Created light.\n"));
		l->setCastShadows(shadowsEnabled);
	}
	else
		_log(_info,_loc,"Skip creating light.\n");
	// Create a skydome
	sceneManager()->setSkyBox(false,"");
	sceneManager()->setSkyDome(true, "Examples/CloudySky", 5, 8);

	sceneManager()->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
	sceneManager()->setShadowColour(shadowsColour);
	_log(_info,_loc,"Scene initialization completed.\n");

	//Plane plane( Vector3::UNIT_Y, 0 );
	//MeshManager::getSingleton().createPlane("ground",
	//	ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
	//	1500,1500,20,20,true,1,5,5,Vector3::UNIT_Z);
	//Entity* ent = sceneManager()->createEntity( "GroundEntity", "ground" );
	//sceneManager()->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
	//ent->setMaterialName("Examples/Rockwall");
	//ent->setCastShadows(false);
	//// simple terrain - mesh
	//Entity* terrainTest = sceneManager()->createEntity("terrain","cube.mesh");
	//SceneNode* node = sceneManager()->getRootSceneNode()->createChildSceneNode("terrainNode",Vector3( 1, 1, 1 ));
 //	node->attachObject(terrainTest);
	//node->setVisible(true);
	//node->scale(10,10,20);
	//node->setPosition(0,20,-50);

	//don't need
	//sceneManager()->setWorldGeometry("../data/world.ini");
	//sceneManager()->addListener(this);

	if( !loadZones() )
		return false;

	_log(_info,_loc,"Loading world complete.\n");

	return true;
}

void CClientWorld::terrainReady()
{
	if( !loaded() )
	{
		_log(_info,_loc,"Terrain ready.\n");
		m_loaded = true;
		COwnPlayer* ownPlayer = CDynamicCollection::getSingletonPtr()->getOwnPlayer();
		ownPlayer->setPosition2D(Vector2(ownPlayer->getPosition3D().x,ownPlayer->getPosition3D().z));
		//for( std::map<uint,CharacterPtr>::iterator it = m_characters.begin(); it != m_characters.end(); ++it )
		//	(*it).second->setPosition2D(Vector2((*it).second->getPosition3D().x,(*it).second->getPosition3D().z));
	}
}

//CharPlayerPtr CClientWorld::addPlayer(ClientCharPlayerData* data)
//{
//	assert(data);
//	if( findCharacter(data->m_id) )
//		return CharPlayerPtr();
//	CharPlayerPtr ch(new CCharPlayer(data,"Ninja.mesh"));
//	addCharacter(ch);
//	return ch;
//};

void CClientWorld::clear()
{
	CWorld::clear();
	//m_ownChar.reset();
};

//OwnCharPlayerPtr CClientWorld::createOwnCharacter(ClientOwnCharData*)
//{
//	/*ownChar.reset(new COwnCharPlayer(data,"Ninja.mesh"));
//	/// Fire event
//	CharacterEventArg arg;
//	arg.m_ch = m_ownChar;
//	CEventManager::instance()->execute(ON_CHAR_TO_WORLD,&arg);
//	return m_ownChar;*/
//};

void CClientWorld::update(Real time)
{
	CWorld::update(time);
	COwnPlayer* ownPlayer = CDynamicCollection::getSingletonPtr()->getOwnPlayer();
	if ( ownPlayer )
		ownPlayer->update(time);
}

void CClientWorld::updateCharactersInfo()
{
	//for( std::map<uint,CharacterPtr>::iterator it = m_characters.begin(); it != m_characters.end(); ++it )
	//{
	//	CharacterEventArg arg;
	//	arg.m_ch = (*it).second;
	//	CEventManager::instance()->execute(ON_CHAR_INFO_UPDATE,&arg);
	//};
	//CharacterEventArg arg;
	//arg.m_ch = m_ownChar;
	//CEventManager::instance()->execute(ON_CHAR_INFO_UPDATE,&arg);
};

bool characterLessByScreenCoordinates(CharacterPtr ch1, CharacterPtr ch2)
{
	return CClientWorld::instance()->translateTo2d(ch1->getPosition3D()).x < CClientWorld::instance()->translateTo2d(ch2->getPosition3D()).x;
};

void CClientWorld::makeVisibleList(std::vector<CharacterPtr> &list, bool sorted)
{
	list.clear();
	std::map<uint,CharacterPtr>::iterator it;
	for( it = m_characters.begin(); it != m_characters.end(); ++it )
		if( (*it).second->visible() )
			list.push_back((*it).second);
	if( sorted )
		std::sort(list.begin(),list.end(),characterLessByScreenCoordinates);
};

//CharacterPtr CClientWorld::findCharacter(uint )
//{
//	/*if( m_ownChar && m_ownChar->getId() == id )
//		return m_ownChar;
//	return CWorld::findCharacter(id);*/
//};

//void CClientWorld::showNavigationMesh()
//{
//	// Create the mesh via the MeshManager
//	Ogre::MeshPtr mesh = MeshManager::getSingleton().createManual("NavigationMesh", "General");
//
//	/// Create one submesh
//	SubMesh* submesh = mesh->createSubMesh();
//	/// Define the vertices 
//	const size_t nbVertices			= zone_->getNavigationMesh()->getVertexBufferSize();
//	const size_t nbFaces            = zone_->getNavigationMesh()->getTriangleBufferSize();
//	const size_t nbVerticesPerFaces = 3; // Triangle
//	
//	
//	submesh->useSharedVertices = false;
//	submesh->vertexData = new VertexData();
//	submesh->vertexData->vertexStart = 0;
//	submesh->vertexData->vertexCount = nbVertices;
//
//	// We must now declare what the vertex data contains
//	VertexDeclaration* declaration = submesh->vertexData->vertexDeclaration;
//	const unsigned short source = 0;
//	size_t offset = 0;
//	offset += declaration->addElement(source,offset,VET_FLOAT3,VES_POSITION).getSize();
//	offset += declaration->addElement(source,offset,VET_FLOAT3,VES_NORMAL).getSize();
//	offset += declaration->addElement(source,offset,VET_FLOAT2,VES_TEXTURE_COORDINATES).getSize();
//
//	// We create the hardware vertex buffer
//	HardwareVertexBufferSharedPtr vbuffer =
//		HardwareBufferManager::getSingleton().createVertexBuffer(declaration->getVertexSize(source), // == offset
//		submesh->vertexData->vertexCount,   // == nbVertices
//		HardwareBuffer::HBU_STATIC_WRITE_ONLY);
//
//	// No we get access to the buffer to fill it.  During so we record the bounding box.
//	AxisAlignedBox aabox;
//	float* vdata = static_cast<float*>(vbuffer->lock(HardwareBuffer::HBL_DISCARD));
//	//change ! use Triangle buffer
//	for (size_t i=0;i<nbVertices;++i)
//	{
//			// Position
//			Vector3 position;
//			position.x = zone_->getNavigationMesh()->getVertexBuffer()[i].x;
//			position.y = zone_->getNavigationMesh()->getVertexBuffer()[i].y;
//			position.z = zone_->getNavigationMesh()->getVertexBuffer()[i].z;
//			*vdata++ = position.x;
//			*vdata++ = position.y;
//			*vdata++ = position.z;
//			aabox.merge(position);
//			// Normal
//			Vector3 normal = position.normalisedCopy();
//			*vdata++ = normal.x;
//			*vdata++ = normal.y;
//			*vdata++ = normal.z;
//			// Texture coordinate
//			Vector2 tcoordinate(0.0f,1.0f); //tcoordinates[i];
//			*vdata++ = tcoordinate.x;
//			*vdata++ = tcoordinate.y;
//	}
//	vbuffer->unlock();
//
//	submesh->vertexData->vertexBufferBinding->setBinding(source,vbuffer);
//
//	// Creates the index data
//	submesh->indexData->indexStart = 0;
//	submesh->indexData->indexCount = nbFaces*nbVerticesPerFaces;
//	submesh->indexData->indexBuffer =
//		HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT,
//		submesh->indexData->indexCount,
//		HardwareBuffer::HBU_STATIC_WRITE_ONLY);
//	uint16* idata = static_cast<uint16*>(submesh->indexData->indexBuffer->lock(HardwareBuffer::HBL_DISCARD));
//	for (size_t i=0;i<nbFaces;++i)
//	{
//			for (size_t j=0;j<nbVerticesPerFaces;++j)
//			{
//					*idata++ = (zone_->getNavigationMesh()->getTriangleBuffer()[i]).vertexIndex[j];
//			}
//	}
//	submesh->indexData->indexBuffer->unlock();
//
//
//	// We must indicate the bounding box
//	mesh->_setBounds(aabox); 
//	mesh->_setBoundingSphereRadius((aabox.getMaximum()-aabox.getMinimum()).length()/2.0); 
//	// Finally we set a material to the submesh
//	submesh->setMaterialName("Examples/Rockwall"); 
//	// And we load the mesh
//	mesh->load();
//
//	// Create an entity with the mesh
//	Entity *entity = sceneManager()->createEntity("NavigationMeshEntity","NavigationMesh");
//	SceneNode* node = sceneManager()->getRootSceneNode()->createChildSceneNode("NavigationMeshNode");
//	node->attachObject(entity);
//}

void CClientWorld::showSimpleNavigationMesh()
{
	MaterialPtr materialAxisX = MaterialManager::getSingleton().create("materialDebugNavigationMesh", "debugger"); 
	materialAxisX->setReceiveShadows(false); 
	materialAxisX->getTechnique(0)->setLightingEnabled(true); 
	materialAxisX->getTechnique(0)->getPass(0)->setDiffuse(ColourValue(1,0,0)); 
	materialAxisX->getTechnique(0)->getPass(0)->setAmbient(ColourValue(1,0,0)); 
	materialAxisX->getTechnique(0)->getPass(0)->setSelfIllumination(ColourValue(1,0,0)); 

	ManualObject* lineAxis =  sceneManager()->createManualObject("lineNavigationMesh");
	lineAxis->setCastShadows(false);

	const size_t nbFaces            = zone_->getNavigationMesh()->getTriangleBufferSize();
	const size_t nbVerticesPerFaces = 3; // Triangle

	for (size_t i=0;i<nbFaces;++i)
	{
		lineAxis->begin("materialDebugNavigationMesh", Ogre::RenderOperation::OT_LINE_STRIP); 
		ireon::nav::Point* firstPoint = zone_->getNavigationMesh()->getTriangleBuffer()[i].vertice(0);
		for (size_t j=0;j<nbVerticesPerFaces;++j)
		{
			ireon::nav::Point* po = zone_->getNavigationMesh()->getTriangleBuffer()[i].vertice(j);
			// po - ogre system coords
			lineAxis->position(po->x, po->y ,  po->z);
			
		}
		// return to first point
		lineAxis->position(firstPoint->x, firstPoint->y ,  firstPoint->z);
		lineAxis->end(); 
	}

	SceneNode* g_nodeDebugAxis = sceneManager()->getRootSceneNode()->createChildSceneNode("nodeMeshNavigationMesh");
	g_nodeDebugAxis->attachObject(lineAxis);
	g_nodeDebugAxis->setVisible(true);


}
