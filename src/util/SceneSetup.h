#pragma once
    
#include "scene/SceneObjectFactory.hpp"

void scene01_Setup(SceneObjectFactory& factory)
{

    // Camera setup
    factory.create_Camera();
    
    ModelObject& backpack = factory.create_Model("res/models/backpack/backpack.obj", "Basic", true);
    backpack.setPosition({15.0f, 3.0f, -16.5f});

    // Skybox setup
    auto skybox = factory.create_Skybox(
        {"res/models/skybox/right.jpg",
         "res/models/skybox/left.jpg",
         "res/models/skybox/top.jpg",
         "res/models/skybox/bottom.jpg",
         "res/models/skybox/front.jpg",
         "res/models/skybox/back.jpg"});

    // Scene objects 
    // auto &window1 = factory.create_Model("res/models/window/window.obj", 2);
    // window1.setPosition({-25.0f, 10.0f, -20.0f});

    // auto &window2 = factory.create_Model("res/models/window/window.obj", 2);
    // window2.setPosition({-25.0f, 10.0f, -18.0f});

    // auto &window3 = factory.create_Model("res/models/window/window.obj", 2);
    // window3.setPosition({-25.0f, 10.0f, -16.0f});

    TextureLocations textureLocations;
    textureLocations.heightMaps.push_back("orange_brick_height.jpg");
    auto& orangeWall = factory.create_Model("res/models/orangeBrickWall/orangeBrickWall.obj", "Basic", false, textureLocations);
    orangeWall.setPosition({-2.0f, 5.0f, 29.0f});
    orangeWall.setScale(3.5f);

    auto& wall = factory.create_Model("res/models/brickWall/brickWall.obj", "Basic");
    wall.setPosition({-10.0f, 5.0f, 29.0f});
    wall.setScale(3.5f);

    ModelObject &ground = factory.create_Model("res/models/Ground3.obj", "Basic");
    ground.setScale(3.5f);
    ground.setPosition({0.0f, 0.0f, 0.0f});

    // ModelObject& mothership = factory.create_Model("res/models/Mothership/Mothership.obj", 0);
    // mothership.setPosition({0.0f, 10.0f, 7.5f});
    // mothership.setScale(0.001f);

    int width = 700;
    int height = 700;

    // for(int i(0); i < width * height; ++i)
    // {
    //     auto& cubeN = factory.create_Model("res/models/cube/cube.obj", 0);
    //     cubeN.setPosition({((i % width) * 2.0f) * 10.0f, -15.0f, ((i / width) * 2.0f) * 10.0f});
    //     //cubeN.rotate(45.0f, 45.0f, 45.0f);
    //     cubeN.setScale(5.0f);
    // }

    // Create an n by n grid of statues centered at the origin and with a spacing of 10.0f.
    // The statues whould all be at Y = 0.0f.
    int n = 5;
    float spacing = 10.0f;
    float offset = (n - 1) * spacing / 2.0f;
    for(int i(0); i < n; ++i)
    {
        for(int j(0); j < n; ++j)
        {
            auto& statue = factory.create_Model("res/models/Statue/Statue_fixed.obj", "Basic");
            statue.setPosition({(i * spacing) - offset, 0.0f, (j * spacing) - offset});
        }
    }


    // auto& statue = factory.create_Model("res/models/Statue/Statue_fixed.obj", 0);
    // statue.setPosition({0.0f, 0.0f, 5.0f});

    // auto &statue2 = factory.create_Model("res/models/Statue/Statue_fixed.obj", 0);
    // statue2.setPosition({0.0f, 0.0f, -5.0f});

    // auto &statue3 = factory.create_Model("res/models/Statue/Statue_fixed.obj", 0);
    // statue3.setPosition({-5.0f, 0.0f, 0.0f});

    // auto &statue4 = factory.create_Model("res/models/Statue/Statue_fixed.obj", 0);
    // statue4.setPosition({5.0f, 0.0f, 0.0f});

    // auto& grass = factory.create_Model("res/models/grassSquare/grassSquare.obj", 2);
    // grass.setPosition({10.0f, 1.0f, 10.0f});

    // auto& cube = factory.create_Model("res/models/cube/cubeBlank.obj", 5);
    // std::vector<Texture> cubeTextures = {scene->getSkybox().getCubemap()->getTexture()};
    // cube.getModel()->meshes[0]->attachTexture(cubeTextures);
    // cube.setPosition({18.0f, 15.0f, 18.0f});
    
    // Point Lights

    auto light_A = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light_A->setColor({20.0f, 20.0f, 20.0f});
    light_A->setPosition({13.0f, 25.0f, 13.0f});
    light_A->setAttenuationFactors({1.0f, 0.09f, 0.032f});

    auto light_B = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light_B->setColor({200.0f, 0.0f, 0.0f});
    light_B->setPosition({-4.0f, 8.0f, 17.5f});
    light_B->setAttenuationFactors({1.0f, 0.0f, 5.80f});

    auto light_C = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light_C->setColor({0.0f, 200.0f, 0.0f});
    light_C->setPosition({13.0f, 5.0f, -13.0f});
    light_C->setAttenuationFactors({1.0f, 0.0f, 3.40f});

    auto light_D = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light_D->setColor({0.0f, 0.0f, 200.0f});
    light_D->setPosition({-13.0f, 5.0f, -13.0f});
    light_D->setAttenuationFactors({1.0f, 0.55f, 0.40f});

    // lets add like 25 random lights

    float lower_bound = -50.0f;
    float pos_range = 100.0f;
    float color_range = 1.0f;

    // std::srand(std::time(nullptr)); // use current time as seed for random generator
    // for(int i(0); i < 25; ++i)
    // {
    //     float random_X = (static_cast<float>(std::rand()) / RAND_MAX) * pos_range + lower_bound;
    //     float random_Y = (static_cast<float>(std::rand()) / RAND_MAX) * pos_range/2.0f + 2.0f;
    //     float random_Z = (static_cast<float>(std::rand()) / RAND_MAX) * pos_range + lower_bound;

    //     float random_R = (static_cast<float>(std::rand()) / RAND_MAX) * color_range;
    //     float random_G = (static_cast<float>(std::rand()) / RAND_MAX) * color_range;
    //     float random_B = (static_cast<float>(std::rand()) / RAND_MAX) * color_range;

    //     auto light = factory.create_LightSource( E_LightType::POINT_LIGHT);
    //     light->setColor({random_R, random_G, random_B});
    //     light->setPosition({random_X, random_Y, random_Z});
    //     light->setAttenuationFactors({1.0f, 0.18f, 0.032f});
    // }


    // Spot Lights
    
    // auto light3 = factory.create_LightSource( E_LightType::SPOT_LIGHT);
    // auto light4 = std::dynamic_pointer_cast<SpotLight>(light3);
    // light4->setCutoff(18.5f);
    // light4->setPosition({0.0f, 10.0f, 0.0f});
    // light4->pointAt(statue.getPosition());

    // auto light5 = factory.create_LightSource( E_LightType::SPOT_LIGHT);
    // auto light6 = std::dynamic_pointer_cast<SpotLight>(light5);
    // light6->setCutoff(18.5f);
    // light6->setPosition({0.0f, 10.0f, 0.0f});
    // light6->pointAt(statue2.getPosition());

    // auto light7 = factory.create_LightSource( E_LightType::SPOT_LIGHT);
    // auto light8 = std::dynamic_pointer_cast<SpotLight>(light7);
    // light8->setCutoff(18.5f);
    // light8->setPosition({0.0f, 10.0f, 0.0f});
    // light8->pointAt(statue3.getPosition());

    // auto light9 = factory.create_LightSource( E_LightType::SPOT_LIGHT);
    // auto light10 = std::dynamic_pointer_cast<SpotLight>(light9);
    // light10->setCutoff(18.5f);
    // light10->setPosition({0.0f, 10.0f, 0.0f});
    // light10->pointAt(statue4.getPosition());

    // auto& cube5 = factory.create_Model("res/models/cube/cubeBlank.obj", 0);
    // cube5.setScale(3.0f);
    // cube5.setPosition(light4->getPosition());

}

void scene02_Setup(SceneObjectFactory& factory)
{

    // Camera setup
    factory.create_Camera();
    
    // Skybox
        auto skybox = factory.create_Skybox(
            {"res/models/skybox/right.jpg",
                "res/models/skybox/left.jpg",
                "res/models/skybox/top.jpg",
                "res/models/skybox/bottom.jpg",
                "res/models/skybox/front.jpg",
                "res/models/skybox/back.jpg"});

    // IBL map
    auto ibl = factory.create_IBL("res/models/lightmap/meadow_8k.hdr", true);

    // Models
    TextureLocations textureLocations1;
    textureLocations1.heightMaps.push_back("rustedIron2_height.png");
    ModelObject& rusty_sphere = factory.create_Model("res/models/rust-iron-sphere/rust-iron-sphere.glb", "PBR_basic", true, textureLocations1);
    rusty_sphere.setPosition({0.0f, 3.0f, 0.0f});


    TextureLocations textureLocations2;
    textureLocations2.heightMaps.push_back("worn-metal-studs_height.png");
    textureLocations2.lightMaps.push_back("worn-metal-studs_ao.png");
    auto& stud_sphere = factory.create_Model("res/models/metal-studs-sphere/metal-studs-sphere.glb", "PBR_basic", true, textureLocations2);
    stud_sphere.setPosition({3.0f, 3.0f, 0.0f});


    TextureLocations textureLocations3;
    textureLocations3.heightMaps.push_back("gold-scuffed_height.png");
    auto& gold_sphere = factory.create_Model("res/models/gold-sphere/gold-sphere.glb", "PBR_basic", true, textureLocations3);
    gold_sphere.setPosition({-3.0f, 3.0f, 0.0f});

    // // Lights 
    // auto light_A = factory.create_LightSource( E_LightType::POINT_LIGHT);
    // light_A->setColor({1.0f, 1.0f, 1.0f});
    // light_A->setPosition({5.0f, 5.0f, 5.0f});
    // light_A->setAttenuationFactors({1.0f, 0.0f, 3.40f});

    // auto light_B = factory.create_LightSource( E_LightType::POINT_LIGHT);
    // light_B->setColor({1.0f, 1.0f, 1.0f});
    // light_B->setPosition({5.0f, 5.0f, -5.0f});
    // light_B->setAttenuationFactors({1.0f, 0.0f, 3.40});

    // auto light_C = factory.create_LightSource( E_LightType::POINT_LIGHT);
    // light_C->setColor({1.0f, 1.0f, 1.0f});
    // light_C->setPosition({-5.0f, 5.0f, -5.0f});
    // light_C->setAttenuationFactors({1.0f, 0.0f, 3.40f});

    // auto light_D = factory.create_LightSource( E_LightType::POINT_LIGHT);
    // light_D->setColor({1.0f, 1.0f, 1.0f});
    // light_D->setPosition({-5.0f, 5.0f, 5.0f});
    // light_D->setAttenuationFactors({1.0f, 0.0f, 3.40f});
}