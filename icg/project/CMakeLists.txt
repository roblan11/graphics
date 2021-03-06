get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")

# these files are copied only once
file(GLOB TEXTURES
    terrain/grass.tga
    terrain/snow.tga
    terrain/rock.tga
    terrain/sand.tga
    terrain/uw.tga
    skybox/skybox_texture.tga
    water/waterDUDV.tga)
copy_files_once(${TEXTURES})

# list all the files you want to copy everytime
# you build (ie, you want the shaders there)
file(GLOB SHADERS
    heightmap/*.glsl
    terrain/*.glsl
    screenquad/*.glsl
    skybox/*.glsl
    skyplane/*.glsl
    water/*.glsl)
deploy_shaders_to_build_dir(${SHADERS})

add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS} ${SHADERS})
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS})
