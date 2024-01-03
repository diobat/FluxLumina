#pragma once
    
#include <string>
#include <vector>


void scene01_Setup(FluxLumina& engine)
{
    // Camera setup
    engine.create_Camera();

    // Skybox setup
    engine.create_Skybox({
        "res/models/skybox/right.jpg",
        "res/models/skybox/left.jpg",
        "res/models/skybox/top.jpg",
        "res/models/skybox/bottom.jpg",
        "res/models/skybox/front.jpg",
        "res/models/skybox/back.jpg"
        });

    // Models
    auto ground_ID = engine.create_Model("res/models/Ground/Ground.obj", "Basic");
    engine.setScale(ground_ID, 3.5f);
    engine.setPosition(ground_ID, {0.0f, 0.0f, 0.0f});

    // Create an n by n grid of statues centered at the origin and with a spacing of 10
    int n = 5;
    float spacing = 10.0f;
    float offset = (n - 1) * spacing / 2.0f;
    for(int i(0); i < n; ++i)
    {
        for(int j(0); j < n; ++j)
        {
            auto statue = engine.create_Model("res/models/Statue/Statue_fixed.obj", "Basic");
            engine.setPosition(statue, {(i * spacing) - offset, 0.0f, (j * spacing) - offset});
        }
    }

    // place columns long the outer edges of the grid of statues
    for(int i(0); i < n; ++i)
    {
        auto column1 = engine.create_Model("res/models/Column/column.glb", "Basic");
        engine.setScale(column1, 1.5f);
        engine.setPosition(column1, {(i * spacing) - offset, 0.0f, -offset - spacing});

        auto column2 = engine.create_Model("res/models/Column/column.glb", "Basic");
        engine.setScale(column2, 1.5f);
        engine.setPosition(column2, {(i * spacing) - offset, 0.0f, offset + spacing});

        auto column3 = engine.create_Model("res/models/Column/column.glb", "Basic");
        engine.setScale(column3, 1.5f);
        engine.setPosition(column3, {-offset - spacing, 0.0f, (i * spacing) - offset});

        auto column4 = engine.create_Model("res/models/Column/column.glb", "Basic");
        engine.setScale(column4, 1.5f);
        engine.setPosition(column4, {offset + spacing, 0.0f, (i * spacing) - offset});
    }

    // Point Lights
    auto light_A = engine.create_LightSource(1);
    engine.setColor(light_A, {0.4f, 0.4f, 0.4f});
    engine.setPosition(light_A, {-23.0f, 5.0f, 31.0f});
    engine.setAttenuationFactors(light_A, {1.0f, 0.0000009f, 0.00000032f});

    // Spot Lights
    std::array<float, 3> color = {0.2f, 0.2f, 0.2f};
    float cutoff = 12.5f;
    
    auto light_B = engine.create_LightSource(2);
    engine.setColor(light_B, color);
    engine.setSpotlightRadius(light_B, cutoff);
    engine.setPosition(light_B, {-23.0f, 10.0f, 31.0f});
    engine.setDirection(light_B, {-20.0f, 2.5f, 20.0f});

    auto light_C = engine.create_LightSource(2);
    engine.setColor(light_C, color);
    engine.setSpotlightRadius(light_C, cutoff);
    engine.setPosition(light_C, {-23.0f, 10.0f, 31.0f});
    engine.setDirection(light_C, {-20.0f, 2.5f, 10.0f});

    auto light_D = engine.create_LightSource(2);
    engine.setColor(light_D, color);
    engine.setSpotlightRadius(light_D, cutoff);
    engine.setPosition(light_D, {-23.0f, 10.0f, 31.0f});
    engine.setDirection(light_D, {-10.0f, 2.5f, 20.0f});


    // lets add like 25 random lights

    // float lower_bound = -50.0f;
    // float pos_range = 100.0f;
    // float color_range = 1.0f;

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

}

void scene02_Setup(FluxLumina& engine)
{
    engine.create_Camera();

    // IBL map
    engine.create_IBL("res/models/lightmap/meadow_8k.hdr", true);

    // Models
    std::vector<std::string> heightMaps_rusty_sphere({"rustedIron2_height.png"});
    std::vector<std::string> lightMaps_rusty_sphere({"rustedIron2_ao.png"});
    auto rusty_sphere_ID = engine.create_Model("res/models/rust-iron-sphere/rust-iron-sphere.glb", "PBR_basic", true, {heightMaps_rusty_sphere, lightMaps_rusty_sphere});
    engine.setPosition(rusty_sphere_ID, {0.0f, 2.5f, 0.0f});

    std::vector<std::string> heightMaps_gold({"gold-scuffed_height.png"});
    std::vector<std::string> lightMaps_gold({"gold-scuffed_ao.png"});
    auto gold_sphere_ID = engine.create_Model("res/models/gold-sphere/gold-sphere.glb", "PBR_basic", true, {heightMaps_gold, lightMaps_gold});
    engine.setPosition(gold_sphere_ID, {0.0f, 5.0f, 0.0f});

    std::vector<std::string> heightMaps_plastic({"plasticpattern1-height.png"});
    std::vector<std::string> lightMaps_plastic({"plasticpattern1-ao.png"});
    auto plastic_sphere_ID = engine.create_Model("res/models/plastic-sphere/plastic-sphere.glb", "PBR_basic", true, {heightMaps_plastic, lightMaps_plastic});
    engine.setPosition(plastic_sphere_ID, {2.5f, 2.5f, 0.0f});

    std::vector<std::string> heightMaps_asphalt({"rocky_asphalt1_Height.png"});
    std::vector<std::string> lightMaps_asphalt({"rocky_asphalt1_ao.png"});
    auto asphalt_sphere_ID = engine.create_Model("res/models/asphalt-sphere/asphalt_sphere.glb", "PBR_basic", true, {heightMaps_asphalt, lightMaps_asphalt});
    engine.setPosition(asphalt_sphere_ID, {2.5f, 5.0f, 0.0f});
}


void scene03_Setup(FluxLumina& engine)
{
    // Camera setup
    engine.create_Camera();

    // IBL map
    engine.create_IBL("res/models/lightmap/abandoned_tiled_room_8k.hdr", true);

    // Models

    auto maskID = engine.create_Model("res/models/mask/venice_mask_joined.glb", "PBR_basic", true);
    engine.setPosition(maskID, {0.0f, 0.0f, 0.0f});

    auto cubeID = engine.create_Model("res/models/mask/cube.glb", "PBR_basic", true);
    engine.setScale(cubeID, 10.0f);
    engine.setPosition(cubeID, {0.0f, 3.0f, -13.0f});

    // Lights 
    auto light_A_ID = engine.create_LightSource(1);
    engine.setColor(light_A_ID, {1.0f, 1.0f, 1.0f});
    engine.setPosition(light_A_ID, {1.0f, 1.0f, 0.5f});
    engine.setAttenuationFactors(light_A_ID, {1.0f, 0.0f, 0.040f});

}