
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "Core/Root.hpp"
#include "Scene/Game.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Node.hpp"
#include "Graphics/TextComponent.hpp"
#include "Graphics/MeshComponent.hpp"
#include "Graphics/LightComponent.hpp"
#include "Physics/PhysicsBodyComponent.hpp"
#include <Logic/SimplePlayerComponent.hpp>
#include "Event/EventListener.hpp"
#include "Utils/Utils.hpp"

#include <OGRE/OgreVector3.h>
#include <OgreProcedural.h>

#define PLAYER_X 0
#define PLAYER_Y 0
#define PLAYER_Z 0

class Main: public dt::State {
public:
    Main() {
        mRuntime = 0;
    }

    void HandleEvent(std::shared_ptr<dt::Event> e) {
        if(e->GetType() == "DT_BEGINFRAMEEVENT") {
            mRuntime += std::dynamic_pointer_cast<dt::BeginFrameEvent>(e)->GetFrameTime();
            //update camera position
            camnode->SetPosition(Ogre::Vector3(spherenode->GetPosition().x, spherenode->GetPosition().y+3, spherenode->GetPosition().z-5));
            const std::string camstr("cam");
            camnode->FindComponent<dt::CameraComponent>(camstr)->LookAt(spherenode->GetPosition());
        }
    }

    void OnInitialize() {
        dt::Scene* scene = AddScene(new dt::Scene("testscene"));
        OgreProcedural::Root::getInstance()->sceneManager = scene->GetSceneManager();
        dt::InputManager::Get()->SetJailInput(true);

        dt::ResourceManager::Get()->AddResourceLocation("","FileSystem", true);
        Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        OgreProcedural::PlaneGenerator().setSizeX(100.f).setSizeY(100.f).setUTile(50.).setVTile(50.).realizeMesh("Plane");
        dt::Node* planenode = scene->AddChildNode(new dt::Node("planenode"));
        planenode->SetPosition(Ogre::Vector3(0, 0, 0));
        planenode->AddComponent(new dt::MeshComponent("Plane", "PrimitivesTest/Pebbles", "plane-mesh"));
        planenode->AddComponent(new dt::PhysicsBodyComponent("plane-mesh", "plane-body"))->SetMass(0.f);

	    OgreProcedural::SphereGenerator().setRadius(1.f).setUTile(5.).setVTile(5.).realizeMesh("Sphere");
        spherenode = scene->AddChildNode(new dt::Node("spherenode"));
        spherenode->SetPosition(Ogre::Vector3(0, 10, 0));
        spherenode->AddComponent(new dt::MeshComponent("Sphere", "PrimitivesTest/RedBrick", "sphere-mesh"));
        spherenode->AddComponent(new dt::PhysicsBodyComponent("sphere-mesh", "sphere-body"));
        dt::SimplePlayerComponent* player = new dt::SimplePlayerComponent("player");
        player->SetMoveSpeed(10.f);
        spherenode->AddComponent(player);

        camnode = scene->AddChildNode(new dt::Node("camnode"));
        camnode->SetPosition(Ogre::Vector3(spherenode->GetPosition().x, spherenode->GetPosition().y+3, spherenode->GetPosition().z-5));
        dt::CameraComponent* cam = camnode->AddComponent(new dt::CameraComponent("cam"));
        cam->LookAt(spherenode->GetPosition());

        dt::Node* lightnode = scene->AddChildNode(new dt::Node("lightnode"));
        lightnode->SetPosition(Ogre::Vector3(-20, 20, 10));
        lightnode->AddComponent(new dt::LightComponent("light"));

        scene->GetPhysicsWorld()->SetEnabled(true);
        // generate meshes
    }

private:
    double mRuntime;
    dt::Node* camnode;
    dt::Node* spherenode;
};

int main(int argc, char** argv) {
    dt::Game game;
    game.Run(new Main(), argc, argv);
    return 0;
}
