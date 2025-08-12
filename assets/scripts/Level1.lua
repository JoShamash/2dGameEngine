-- Load a different tilemap image depending on the current system time
local current_system_hour = os.date("*t").hour

-- Use a day-map or night-map texture (9am - 6pm)
local map_texture_asset_id
if current_system_hour >= 9 and current_system_hour < 18 then
    map_texture_asset_id = "tilemap-texture-day"
else
    map_texture_asset_id = "tilemap-texture-night"
end

-- Define a table with the start values of the first level
Level = {
    ----------------------------------------------------
    -- Table to define the list of assets
    ----------------------------------------------------
    Assets = {
        { type = "texture", id = "tilemap-texture-day",         path = "./assets/tilemaps/jungle.png" },
        { type = "texture", id = "tilemap-texture-night",       path = "./assets/tilemaps/jungle-night.png" },
        { type = "texture", id = "chopper-texture",             path = "./assets/images/chopper-green-spritesheet.png" },
        { type = "texture", id = "su27-texture",                path = "./assets/images/su27-spritesheet.png" },
        { type = "texture", id = "f22-texture",                 path = "./assets/images/f22-spritesheet.png" },
        { type = "texture", id = "fw190-texture",               path = "./assets/images/fw190-spritesheet.png" },
        { type = "texture", id = "upf7-texture",                path = "./assets/images/upf7-spritesheet.png" },
        { type = "texture", id = "bf109-texture",               path = "./assets/images/bf109-spritesheet.png" },
        { type = "texture", id = "bomber-texture",              path = "./assets/images/bomber-spritesheet.png" },
        { type = "texture", id = "carrier-texture",             path = "./assets/images/carrier.png" },
        { type = "texture", id = "boat-texture",                path = "./assets/images/boat.png" },
        { type = "texture", id = "tank-tiger-up-texture",       path = "./assets/images/tank-tiger-up.png" },
        { type = "texture", id = "tank-tiger-right-texture",    path = "./assets/images/tank-tiger-right.png" },
        { type = "texture", id = "tank-tiger-down-texture",     path = "./assets/images/tank-tiger-down.png" },
        { type = "texture", id = "tank-tiger-left-texture",     path = "./assets/images/tank-tiger-left.png" },
        { type = "texture", id = "tank-tiger-killed-texture",   path = "./assets/images/tank-tiger-killed.png" },
        { type = "texture", id = "tank-panther-up-texture",     path = "./assets/images/tank-panther-up.png" },
        { type = "texture", id = "tank-panther-right-texture",  path = "./assets/images/tank-panther-right.png" },
        { type = "texture", id = "tank-panther-down-texture",   path = "./assets/images/tank-panther-down.png" },
        { type = "texture", id = "tank-panther-left-texture",   path = "./assets/images/tank-panther-left.png" },
        { type = "texture", id = "tank-panther-killed-texture", path = "./assets/images/tank-panther-killed.png" },
        { type = "texture", id = "truck-ford-up-texture",       path = "./assets/images/truck-ford-up.png" },
        { type = "texture", id = "truck-ford-right-texture",    path = "./assets/images/truck-ford-right.png" },
        { type = "texture", id = "truck-ford-down-texture",     path = "./assets/images/truck-ford-down.png" },
        { type = "texture", id = "truck-ford-left-texture",     path = "./assets/images/truck-ford-left.png" },
        { type = "texture", id = "truck-ford-killed-texture",   path = "./assets/images/truck-ford-killed.png" },
        { type = "texture", id = "army-walk-up-texture",        path = "./assets/images/army-walk-up.png" },
        { type = "texture", id = "army-walk-right-texture",     path = "./assets/images/army-walk-right.png" },
        { type = "texture", id = "army-walk-down-texture",      path = "./assets/images/army-walk-down.png" },
        { type = "texture", id = "army-walk-left-texture",      path = "./assets/images/army-walk-left.png" },
        { type = "texture", id = "army-walk-killed-texture",    path = "./assets/images/army-walk-killed.png" },
        { type = "texture", id = "army-gun-up-texture",         path = "./assets/images/army-gun-up.png" },
        { type = "texture", id = "army-gun-right-texture",      path = "./assets/images/army-gun-right.png" },
        { type = "texture", id = "army-gun-down-texture",       path = "./assets/images/army-gun-down.png" },
        { type = "texture", id = "army-gun-left-texture",       path = "./assets/images/army-gun-left.png" },
        { type = "texture", id = "army-gun-killed-texture",     path = "./assets/images/army-gun-killed.png" },
        { type = "texture", id = "sam-truck-right-texture",     path = "./assets/images/sam-truck-right.png" },
        { type = "texture", id = "sam-tank-left-texture",       path = "./assets/images/sam-tank-left-spritesheet.png" },
        { type = "texture", id = "sam-tank-right-texture",      path = "./assets/images/sam-tank-right-spritesheet.png" },
        { type = "texture", id = "takeoff-base-texture",        path = "./assets/images/takeoff-base.png" },
        { type = "texture", id = "landing-base-texture",        path = "./assets/images/landing-base.png" },
        { type = "texture", id = "runway-texture",              path = "./assets/images/runway.png" },
        { type = "texture", id = "obstacles1-texture",          path = "./assets/images/obstacles-1.png" },
        { type = "texture", id = "obstacles2-texture",          path = "./assets/images/obstacles-2.png" },
        { type = "texture", id = "obstacles3-texture",          path = "./assets/images/obstacles-3.png" },
        { type = "texture", id = "obstacles4-texture",          path = "./assets/images/obstacles-4.png" },
        { type = "texture", id = "obstacles5-texture",          path = "./assets/images/obstacles-5.png" },
        { type = "texture", id = "obstacles6-texture",          path = "./assets/images/obstacles-6.png" },
        { type = "texture", id = "obstacles7-texture",          path = "./assets/images/obstacles-7.png" },
        { type = "texture", id = "tree1-texture",               path = "./assets/images/tree-1.png" },
        { type = "texture", id = "tree2-texture",               path = "./assets/images/tree-2.png" },
        { type = "texture", id = "tree3-texture",               path = "./assets/images/tree-3.png" },
        { type = "texture", id = "tree4-texture",               path = "./assets/images/tree-4.png" },
        { type = "texture", id = "tree5-texture",               path = "./assets/images/tree-5.png" },
        { type = "texture", id = "tree6-texture",               path = "./assets/images/tree-6.png" },
        { type = "texture", id = "tree7-texture",               path = "./assets/images/tree-7.png" },
        { type = "texture", id = "tree8-texture",               path = "./assets/images/tree-8.png" },
        { type = "texture", id = "tree9-texture",               path = "./assets/images/tree-9.png" },
        { type = "texture", id = "tree10-texture",              path = "./assets/images/tree-10.png" },
        { type = "texture", id = "tree11-texture",              path = "./assets/images/tree-11.png" },
        { type = "texture", id = "tree12-texture",              path = "./assets/images/tree-12.png" },
        { type = "texture", id = "tree13-texture",              path = "./assets/images/tree-13.png" },
        { type = "texture", id = "tree14-texture",              path = "./assets/images/tree-14.png" },
        { type = "texture", id = "tree15-texture",              path = "./assets/images/tree-15.png" },
        { type = "texture", id = "tree16-texture",              path = "./assets/images/tree-16.png" },
        { type = "texture", id = "tree17-texture",              path = "./assets/images/tree-17.png" },
        { type = "texture", id = "tree18-texture",              path = "./assets/images/tree-18.png" },
        { type = "texture", id = "tree19-texture",              path = "./assets/images/tree-19.png" },
        { type = "texture", id = "tree20-texture",              path = "./assets/images/tree-20.png" },
        { type = "texture", id = "bullet-texture",              path = "./assets/images/bullet.png" },
        { type = "texture", id = "radar-texture",               path = "./assets/images/radar-spritesheet.png" },
        { type = "font"   , id = "pico8-font-5",                path = "./assets/fonts/pico8.ttf", font_size = 5 },
        { type = "font"   , id = "pico8-font-10",               path = "./assets/fonts/pico8.ttf", font_size = 10 }
    },

    ----------------------------------------------------
    -- table to define the map config variables
    ----------------------------------------------------
    Tilemap = {
        map_file = "./assets/tilemaps/jungle.map",
        assetId = map_texture_asset_id,
        num_rows = 20,
        num_cols = 25,
        tile_size = 32,
        scale = 2.0
    },

    ----------------------------------------------------
    -- table to define entities and their components
    ----------------------------------------------------
    Entities = {
        {
            -- Player
            tag = "player",
            components = {
                transform = {
                    position = { x = 242, y = 110 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                rigidbody = {
                    speed = 0.0,
                    velocity = { x = 0.0, y = 0.0 },
                },
                camp = {
                    camp = "Friend",
                },
                sprite = {
                    assetId = "chopper-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                boxcollider = {
                    width = 32,
                    height = 25,
                    offset = { x = 0, y = 5 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 200, y = 200 },
                    projectile_duration = 10, -- seconds
                    repeat_frequency = 0, -- seconds
                    hit_percentage_damage = 10,
                    friendly = true
                },
                keyboard_controller = {
                    up_velocity = { x = 0, y = -50 },
                    right_velocity = { x = 50, y = 0 },
                    down_velocity = { x = 0, y = 50 },
                    left_velocity = { x = -50, y = 0 }
                },
                camera_follow = {
                    follow = true
                }
            }
        },
        {
            -- Takeoff base
            components = {
                transform = {
                    position = { x = 240, y = 113 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "takeoff-base-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                }
            }
        },
        {
            -- Landing base
            components = {
                transform = {
                    position = { x = 1396, y = 493 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "landing-base-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                }
            }
        },
        {
            -- Radar UI animation
            group = "ui",
            components = {
                transform = {
                    position = { x = 10, y = 10 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "radar-texture",
                    width = 64,
                    height = 64,
                    z_index = 10,
                    fixed = true
                },
                animation = {
                    num_frames = 8,
                    speed_rate = 7 -- fps
                }
            }
        },
        {
            -- Runway
            components = {
                transform = {
                    position = { x = 940, y = 65 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "runway-texture",
                    width = 21,
                    height = 191,
                    z_index = 1
                }
            }
        },
        {
            -- Runway
            components = {
                transform = {
                    position = { x = 470, y = 385 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "runway-texture",
                    width = 21,
                    height = 191,
                    z_index = 1
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 200, y = 497 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-tiger-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 2
                },
                boxcollider = {
                    width = 25,
                    height = 18,
                    offset = { x = 0, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = math.random(10, 200), y = 0 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 20,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 785, y = 170 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-up-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 17,
                    height = 18,
                    offset = { x = 7, y = 10 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -50 },
                    projectile_duration = 4, -- seconds
                    repeat_frequency = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 785, y = 250 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-tiger-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 20,
                    height = 18,
                    offset = { x = 5, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 20 },
                    projectile_duration = 3, -- seconds
                    repeat_frequency = 3, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 785, y = 350 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 25,
                    height = 18,
                    offset = { x = 5, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = -50, y = 0 },
                    projectile_duration = 3, -- seconds
                    repeat_frequency = 3, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 570, y = 520 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 25,
                    height = 18,
                    offset = { x = 5, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 60, y = 0 },
                    projectile_duration = 4, -- seconds
                    repeat_frequency = 4, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 570, y = 600 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 25,
                    height = 18,
                    offset = { x = 5, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = -60, y = 0 },
                    projectile_duration = 4, -- seconds
                    repeat_frequency = 4, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1050, y = 170 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 25,
                    height = 18,
                    offset = { x = 5, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 60, y = 0 },
                    projectile_duration = 4, -- seconds
                    repeat_frequency = 4, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1170, y = 116 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 17,
                    height = 18,
                    offset = { x = 8, y = 6 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 40 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1380, y = 116 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 17,
                    height = 18,
                    offset = { x = 8, y = 6 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 40 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1265, y = 240 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-killed-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1265, y = 290 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 20,
                    height = 17,
                    offset = { x = 7, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = -40, y = 0 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 640, y = 800 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-tiger-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 18,
                    height = 20,
                    offset = { x = 7, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 100 },
                    projectile_duration = 5, -- seconds
                    repeat_frequency = 5, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 790, y = 745 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-tiger-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 25,
                    height = 18,
                    offset = { x = 7, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = -60, y = 0 },
                    projectile_duration = 10, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 980, y = 790 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-tiger-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 25,
                    height = 18,
                    offset = { x = 0, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 60, y = 0 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1070, y = 870 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 17,
                    height = 20,
                    offset = { x = 8, y = 4 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 100 },
                    projectile_duration = 4, -- seconds
                    repeat_frequency = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1190, y = 790 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-up-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -200 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1210, y = 790 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-up-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -200 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1230, y = 790 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-up-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -200 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1250, y = 790 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-up-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -200 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1000, y = 445 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-up-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 17,
                    height = 20,
                    offset = { x = 7, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -200 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1426, y = 760 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 22,
                    height = 18,
                    offset = { x = 5, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 200, y = 0 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1423, y = 835 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-tiger-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 25,
                    height = 18,
                    offset = { x = 7, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = -200, y = 0 },
                    projectile_duration = 1, -- seconds
                    repeat_frequency = 2, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1450, y = 300 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-tiger-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 19,
                    height = 20,
                    offset = { x = 6, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 300 },
                    projectile_duration = 1, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 195, y = 980 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-tiger-up-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 18,
                    height = 25,
                    offset = { x = 7, y = 7 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -100 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 110, y = 1125 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 17,
                    height = 20,
                    offset = { x = 8, y = 4 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = 300 },
                    projectile_duration = 1, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                }
            }
        },
        {
            -- Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1395, y = 540 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tank-panther-killed-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 113, y = 580 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "truck-ford-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 180, y = 1045 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "truck-ford-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 195, y = 1055 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "truck-ford-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 210, y = 1065 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "truck-ford-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 545, y = 660 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "truck-ford-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 560, y = 670 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "truck-ford-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 12,
                    height = 25,
                    offset = { x = 10, y = 2 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1360, y = 880 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "truck-ford-up-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 12,
                    height = 20,
                    offset = { x = 10, y = 8 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1380, y = 880 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "truck-ford-up-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 12,
                    height = 20,
                    offset = { x = 10, y = 8 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1400, y = 880 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "truck-ford-up-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 12,
                    height = 20,
                    offset = { x = 10, y = 8 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1505, y = 780 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "truck-ford-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
                boxcollider = {
                    width = 25,
                    height = 16,
                    offset = { x = 3, y = 10 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1515, y = 790 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "truck-ford-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 2
                },
                boxcollider = {
                    width = 25,
                    height = 16,
                    offset = { x = 3, y = 10 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- SAM Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 495, y = 380 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "sam-tank-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 2
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 2 -- fps
                },
                boxcollider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = -50, y = -50 },
                    projectile_duration = 3, -- seconds
                    repeat_frequency = 2, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 495, y = 410 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "sam-tank-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 2
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 2 -- fps
                },
                boxcollider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = -50, y = -50 },
                    projectile_duration = 3, -- seconds
                    repeat_frequency = 2, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1290, y = 115 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "sam-tank-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 2
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 2 -- fps
                },
                boxcollider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = -100, y = -75 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 2, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Tank
            group = "enemies",
            components = {
                transform = {
                    position = { x = 935, y = 557 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "sam-tank-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 2
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 2 -- fps
                },
                boxcollider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = -100, y = -60 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 2, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 114, y = 700 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "sam-truck-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 2
                },
                boxcollider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 50, y = -50 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 114, y = 720 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "sam-truck-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 2
                },
                boxcollider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 50, y = -50 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 116, y = 499 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "sam-truck-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 2
                },
                boxcollider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 50, y = -70 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1454, y = 215 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "sam-truck-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 2
                },
                boxcollider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 50, y = -70 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1454, y = 231 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "sam-truck-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 2
                },
                boxcollider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 50, y = -70 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- SAM Truck
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1454, y = 247 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "sam-truck-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 2
                },
                boxcollider = {
                    width = 17,
                    height = 15,
                    offset = { x = 8, y = 8 }
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 50, y = -70 },
                    projectile_duration = 2, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 5,
                    friendly = false
                }
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 115, y = 633 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tree5-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 117, y = 650 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tree5-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 318, y = 490 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tree6-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 668, y = 526 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tree14-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 1018, y = 738 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tree17-texture",
                    width = 17,
                    height = 20,
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 1034, y = 738 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tree17-texture",
                    width = 17,
                    height = 20,
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 1028, y = 745 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tree18-texture",
                    width = 17,
                    height = 20,
                    z_index = 2
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 1390, y = 440 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tree10-texture",
                    width = 31,
                    height = 32,
                    z_index = 1
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 1400, y = 445 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tree10-texture",
                    width = 31,
                    height = 32,
                    z_index = 2
                },
            }
        },
        {
            -- Vegetation
            components = {
                transform = {
                    position = { x = 1365, y = 290 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "tree14-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 669, y = 549 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 685, y = 549 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 315, y = 505 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 3
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 330, y = 507 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 438, y = 390 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 1
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 449, y = 408 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 431, y = 416 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 940, y = 695 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 955, y = 705 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1085, y = 507 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1075, y = 527 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1075, y = 547 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1085, y = 567 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1355, y = 449 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles2-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1430, y = 446 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles2-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1435, y = 195 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1425, y = 215 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1425, y = 235 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1425, y = 255 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1435, y = 275 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 2
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1360, y = 310 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 1
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1380, y = 312 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles7-texture",
                    width = 16,
                    height = 16,
                    z_index = 1
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1330, y = 212 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles1-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Obstacles
            components = {
                transform = {
                    position = { x = 1360, y = 232 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "obstacles1-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 630, y = 405 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-walk-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 497, y = 450 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-walk-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 572, y = 560 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-gun-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 114, y = 980 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-walk-up-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 114, y = 1030 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-walk-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 883, y = 490 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-walk-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 113, y = 851 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-gun-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 750, y = 630 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-walk-left-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 800, y = 630 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-walk-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 856, y = 115 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-gun-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 900, y = 115 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-gun-up-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 1117, y = 530 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-walk-right-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 755, y = 440 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-gun-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 810, y = 440 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-gun-down-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 1390, y = 690 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-walk-killed-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 1425, y = 690 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-walk-killed-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Army
            components = {
                transform = {
                    position = { x = 1465, y = 690 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "army-walk-killed-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 1
                },
            }
        },
        {
            -- Boat
            components = {
                transform = {
                    position = { x = 80, y = 520 },
                    rotation = 0.0,
                    scale = { x = 0.8, y = 0.8 },
                },
                sprite = {
                    assetId = "boat-texture",
                    width = 21,
                    height = 126,
                    z_index = 1
                },
            }
        },
        {
            -- Boat
            components = {
                transform = {
                    position = { x = 80, y = 790 },
                    rotation = 0.0,
                    scale = { x = 0.8, y = 0.8 },
                },
                sprite = {
                    assetId = "boat-texture",
                    width = 21,
                    height = 126,
                    z_index = 1
                },
            }
        },
        {
            -- Boat
            components = {
                transform = {
                    position = { x = 345, y = 423 },
                    rotation = 270.0,
                    scale = { x = 0.8, y = 0.8 },
                },
                sprite = {
                    assetId = "boat-texture",
                    width = 21,
                    height = 126,
                    z_index = 1
                },
            }
        },
        {
            -- Boat
            components = {
                transform = {
                    position = { x = 1510, y = 460 },
                    rotation = 0.0,
                    scale = { x = 0.8, y = 0.8 },
                },
                sprite = {
                    assetId = "boat-texture",
                    width = 21,
                    height = 126,
                    z_index = 1
                },
            }
        },
        {
            -- Carrier
            components = {
                transform = {
                    position = { x = 670, y = 150 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "carrier-texture",
                    width = 59,
                    height = 191,
                    z_index = 1
                },
            }
        },
        {
            -- Carrier
            components = {
                transform = {
                    position = { x = 300, y = 975 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "carrier-texture",
                    width = 59,
                    height = 191,
                    z_index = 1
                },
            }
        },
        {
            -- F-22 fighter jet
            group = "enemies",
            components = {
                transform = {
                    position = { x = 688, y = 165 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "f22-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 4
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                boxcollider = {
                    width = 20,
                    height = 25,
                    offset = { x = 5, y = 5}
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- SU-27 fighter jet
            group = "enemies",
            components = {
                transform = {
                    position = { x = 685, y = 300 },
                    rotation = -10.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                rigidbody = {
                    velocity = { x = -5.5 , y = -35.0 }
                },
                sprite = {
                    assetId = "su27-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                boxcollider = {
                    width = 25,
                    height = 30,
                    offset = { x = 5, y = 0 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- Bomber plane
            group = "enemies",
            components = {
                transform = {
                    position = { x = 464, y = 520 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                sprite = {
                    assetId = "bomber-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                boxcollider = {
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    offset = { x = 0, y = 0 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- FW190 plane
            group = "enemies",
            components = {
                transform = {
                    position = { x = 1000, y = 143 },
                    rotation = -90.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                rigidbody = {
                    velocity = { x = -50.0, y = 0.0 }
                },
                sprite = {
                    assetId = "fw190-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 6
                },
                animation = {
                    num_frames = 3,
                    speed_rate = 15 -- fps
                },
                boxcollider = {
                    width = 32,
                    height = 30,
                    offset = { x = 0, y = 0 }
                },
                health = {
                    health_percentage = 100
                }
            }
        },
        {
            -- UPF7 plane
            group = "enemies",
            components = {
                transform = {
                    position = { x = 920, y = 179 },
                    rotation = 0.0,
                    scale = { x = 0.8, y = 0.8 },
                },
                sprite = {
                    assetId = "upf7-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                }
            }
        },
        {
            -- UPF7 plane
            group = "enemies",
            components = {
                transform = {
                    position = { x = 950, y = 179 },
                    rotation = 0.0,
                    scale = { x = 0.8, y = 0.8 },
                },
                sprite = {
                    assetId = "upf7-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                }
            }
        },
        {
            -- UPF7 plane
            group = "enemies",
            components = {
                transform = {
                    position = { x = 980, y = 179 },
                    rotation = 0.0,
                    scale = { x = 0.8, y = 0.8 },
                },
                sprite = {
                    assetId = "upf7-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                }
            }
        },
        {
            -- SU-27 fighter jet
            group = "enemies",
            components = {
                transform = {
                    position = { x = 317, y = 985 },
                    rotation = 0.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                rigidbody = {
                    velocity = { x = 0.0, y = -50.0 }
                },
                sprite = {
                    assetId = "su27-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                boxcollider = {
                    width = 32,
                    height = 32
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 0, y = -100 },
                    projectile_duration = 5, -- seconds
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                },
                on_update_script = {
                    [0] =
                    function(entity, delta_time, ellapsed_time)
                        -- this function makes the fighter jet move up and down the map shooting projectiles
                        local current_position_x, current_position_y = get_position(entity)
                        local current_velocity_x, current_velocity_y = get_velocity(entity)

                        -- if it reaches the top or the bottom of the map
                        if current_position_y < 10 or current_position_y > map_height - 32 then
                            set_velocity(entity, 0, current_velocity_y * -1); -- flip the entity y-velocity
                        else
                            set_velocity(entity, 0, current_velocity_y); -- do not flip y-velocity
                        end

                        -- set the transform rotation to match going up or down
                        if (current_velocity_y < 0) then
                            set_rotation(entity, 0) -- point up
                            set_projectile_velocity(entity, 0, -200) -- shoot projectiles up
                        else
                            set_rotation(entity, 180) -- point down
                            set_projectile_velocity(entity, 0, 200) -- shoot projectiles down
                        end
                    end
                }
            }
        },
        {
            -- F-22 fighter jet
            group = "enemies",
            components = {
                transform = {
                    position = { x = 10, y = 10 },
                    rotation = 90.0,
                    scale = { x = 1.0, y = 1.0 },
                },
                rigidbody = {
                    velocity = { x = 0.0, y = 0.0 }
                },
                sprite = {
                    assetId = "f22-texture",
                    size = { width = 32, height = 32 },
                    layer = "characters",
                    srcRect = { x = 0, y = 0 },
                    z_index = 5
                },
                animation = {
                    num_frames = 2,
                    speed_rate = 10 -- fps
                },
                boxcollider = {
                    width = 32,
                    height = 32
                },
                health = {
                    health_percentage = 100
                },
                projectile_emitter = {
                    projectile_velocity = { x = 200, y = 0 },
                    projectile_duration = 1, -- secondsm
                    repeat_frequency = 1, -- seconds
                    hit_percentage_damage = 10,
                    friendly = false
                },
                on_update_script = {
                    [0] =
                    function(entity, delta_time, ellapsed_time)
                        -- change the position of the the airplane to follow a sine wave movement
                        local new_x = ellapsed_time * 0.09
                        local new_y = 200 + (math.sin(ellapsed_time * 0.001) * 50)
                        set_position(entity, new_x, new_y) -- set the new position
                    end
                }
            }
        }
    },
}
-- Define some variables 
AssetsNum =  #Level.Assets
EntitiesNum =  #Level.Entities

-- Define some useful global variables
MapWidth = Level.Tilemap.num_cols * Level.Tilemap.tile_size * Level.Tilemap.scale
MapHeight = Level.Tilemap.num_rows * Level.Tilemap.tile_size * Level.Tilemap.scale