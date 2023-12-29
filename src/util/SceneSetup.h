#pragma once
    
#include "scene/SceneObjectFactory.hpp"

void scene01_Setup(SceneObjectFactory& factory)
{

    // Camera setup
    factory.create_Camera();
    
    // ModelObject& backpack = factory.create_Model("res/models/backpack/backpack.obj", "Basic", true);
    // backpack.setPosition({15.0f, 3.0f, -16.5f});

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

    // TextureLocations textureLocations;
    // textureLocations.heightMaps.push_back("orange_brick_height.jpg");
    // auto& orangeWall = factory.create_Model("res/models/orangeBrickWall/orangeBrickWall.obj", "Basic", false, textureLocations);
    // orangeWall.setPosition({-2.0f, 5.0f, 29.0f});
    // orangeWall.setScale(3.5f);

    // auto& wall = factory.create_Model("res/models/brickWall/brickWall.obj", "Basic");
    // wall.setPosition({-10.0f, 5.0f, 29.0f});
    // wall.setScale(3.5f);

    ModelObject &ground = factory.create_Model("res/models/Ground/Ground.obj", "Basic");
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

    // place columns long the outer edges of the grid of statues
    for(int i(0); i < n; ++i)
    {
        auto& column1 = factory.create_Model("res/models/Column/column.glb", "Basic");
        column1.setScale(1.5f);
        column1.setPosition({(i * spacing) - offset, 0.0f, -offset - spacing});

        auto& column2 = factory.create_Model("res/models/Column/column.glb", "Basic");
        column2.setScale(1.5f);
        column2.setPosition({(i * spacing) - offset, 0.0f, offset + spacing});

        auto& column3 = factory.create_Model("res/models/Column/column.glb", "Basic");
        column3.setScale(1.5f);
        column3.setPosition({-offset - spacing, 0.0f, (i * spacing) - offset});

        auto& column4 = factory.create_Model("res/models/Column/column.glb", "Basic");
        column4.setScale(1.5f);
        column4.setPosition({offset + spacing, 0.0f, (i * spacing) - offset});
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
    light_A->setColor({0.4f, 0.4f, 0.4f});
    light_A->setPosition({-23.0f, 5.0f, 31.0f});
    light_A->setAttenuationFactors({1.0f, 0.0000009f, 0.00000032f});

    // auto light_B = factory.create_LightSource( E_LightType::POINT_LIGHT);
    // light_B->setColor({0.4f, 0.0f, 0.0f});
    // light_B->setPosition({0.0f, 5.0f, 17.5f});
    // light_B->setAttenuationFactors({1.0f, 0.0f, 0.0000340f});

    // auto light_C = factory.create_LightSource( E_LightType::POINT_LIGHT);
    // light_C->setColor({0.0f, 0.4f, 0.0f});
    // light_C->setPosition({15.0f, 5.0f, -15.0f});
    // light_C->setAttenuationFactors({1.0f, 0.0f, 0.0000340f});

    // auto light_D = factory.create_LightSource( E_LightType::POINT_LIGHT);
    // light_D->setColor({0.0f, 0.0f, 0.4f});
    // light_D->setPosition({-15.0f, 5.0f, -15.0f});
    // light_D->setAttenuationFactors({1.0f, 0.0f, 0.0000340f});

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

    std::array<float, 3> color = {0.2f, 0.2f, 0.2f};
    float cutoff = 12.5f;
    
    auto light3 = factory.create_LightSource( E_LightType::SPOT_LIGHT);
    auto light4 = std::dynamic_pointer_cast<SpotLight>(light3);
    light4->setColor(color);
    light4->setCutoff(cutoff);
    light4->setPosition({-23.0f, 10.0f, 31.0f});
    light4->pointAt({-20.0f, 2.5f, 20.0f});

    auto light5 = factory.create_LightSource( E_LightType::SPOT_LIGHT);
    auto light6 = std::dynamic_pointer_cast<SpotLight>(light5);
    light6->setColor(color);
    light6->setCutoff(cutoff);
    light6->setPosition({-23.0f, 10.0f, 31.0f});
    light6->pointAt({-20.0f, 2.5f, 10.0f});

    auto light7 = factory.create_LightSource( E_LightType::SPOT_LIGHT);
    auto light8 = std::dynamic_pointer_cast<SpotLight>(light7);
    light8->setColor(color);
    light8->setCutoff(cutoff);
    light8->setPosition({-23.0f, 10.0f, 31.0f});
    light8->pointAt({-10.0f, 2.5f, 20.0f});

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
    textureLocations1.lightMaps.push_back("rustedIron2_ao.png");
    ModelObject& rusty_sphere = factory.create_Model("res/models/rust-iron-sphere/rust-iron-sphere.glb", "PBR_basic", true, textureLocations1);
    rusty_sphere.setPosition({0.0f, 2.5f, 0.0f});

    // TextureLocations textureLocations2;
    // textureLocations2.heightMaps.push_back("worn-metal-studs_height.png");
    // textureLocations2.lightMaps.push_back("worn-metal-studs_ao.png");
    // auto& stud_sphere = factory.create_Model("res/models/metal-studs-sphere/metal-studs-sphere.glb", "PBR_basic", true, textureLocations2);
    // stud_sphere.setPosition({3.0f, 3.0f, 0.0f});

    TextureLocations textureLocations3;
    textureLocations3.heightMaps.push_back("gold-scuffed_height.png");
    textureLocations3.lightMaps.push_back("gold-scuffed_ao.png");
    auto& gold_sphere = factory.create_Model("res/models/gold-sphere/gold-sphere.glb", "PBR_basic", true, textureLocations3);
    gold_sphere.setPosition({0.0f, 5.0f, 0.0f});

    TextureLocations textureLocations4;
    textureLocations4.heightMaps.push_back("plasticpattern1-height.png");
    textureLocations4.lightMaps.push_back("plasticpattern1-ao.png");
    auto& plastic_sphere = factory.create_Model("res/models/plastic-sphere/plastic-sphere.glb", "PBR_basic", true, textureLocations4);
    plastic_sphere.setPosition({2.5f, 2.5f, 0.0f});

    // TextureLocations textureLocations5;
    // textureLocations5.heightMaps.push_back("Gravel023_4K-PNG_Displacement.png");
    // textureLocations5.lightMaps.push_back("Gravel023_4K-PNG_AmbientOcclusion.png");
    // auto& gravel_sphere = factory.create_Model("res/models/gravel-sphere/gravel-sphere.glb", "PBR_basic", true, textureLocations5);
    // gravel_sphere.setPosition({-6.0f, 3.0f, 0.0f});

    TextureLocations textureLocations6;
    textureLocations6.heightMaps.push_back("rocky_asphalt1_Height.png");
    textureLocations6.lightMaps.push_back("rocky_asphalt1_ao.png");
    auto& asphalt_sphere = factory.create_Model("res/models/asphalt-sphere/asphalt_sphere.glb", "PBR_basic", true, textureLocations6);
    asphalt_sphere.setPosition({2.5f, 5.0f, 0.0f});


    // // Lights 
    auto light_A = factory.create_LightSource(E_LightType::POINT_LIGHT);
    light_A->setColor({1.0f, 1.0f, 1.0f});
    light_A->setPosition({5.0f, 5.0f, 5.0f});
    light_A->setAttenuationFactors({1.0f, 0.0f, 3.40f});

    auto light_B = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light_B->setColor({1.0f, 1.0f, 1.0f});
    light_B->setPosition({5.0f, 5.0f, -5.0f});
    light_B->setAttenuationFactors({1.0f, 0.0f, 3.40});

    auto light_C = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light_C->setColor({1.0f, 1.0f, 1.0f});
    light_C->setPosition({-5.0f, 5.0f, -5.0f});
    light_C->setAttenuationFactors({1.0f, 0.0f, 3.40f});

    auto light_D = factory.create_LightSource( E_LightType::POINT_LIGHT);
    light_D->setColor({1.0f, 1.0f, 1.0f});
    light_D->setPosition({-5.0f, 5.0f, 5.0f});
    light_D->setAttenuationFactors({1.0f, 0.0f, 3.40f});
}


void scene03_Setup(SceneObjectFactory& factory)
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
    auto ibl = factory.create_IBL("res/models/lightmap/abandoned_tiled_room_8k.hdr", true);

    // Models

    TextureLocations textureLocations6;
    textureLocations6.lightMaps.push_back("mask-ao.png");
    ModelObject& mask = factory.create_Model("res/models/mask/venice_mask_joined.glb", "PBR_basic", true);
    mask.setPosition({0.0f, 0.0f, 0.0f});

    auto& cube5 = factory.create_Model("res/models/mask/cube.glb", "PBR_basic", true);
    cube5.setScale(10.0f);
    cube5.setPosition({0.0f, 3.0f, -13.0f});

    // Lights 
    auto light_A = factory.create_LightSource(E_LightType::POINT_LIGHT);
    light_A->setColor({1.0f, 1.0f, 1.0f});
    light_A->setPosition({1.0f, 1.0f, 0.5f});
    light_A->setAttenuationFactors({1.0f, 0.0f, 0.040f});

}