cmake_minimum_required (VERSION 3.13)

project(parser
    LANGUAGES    CXX
    HOMEPAGE_URL "https://github.com/pmx-to-txt/MMDFormats"
)

include(GNUInstallDirs)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

option(PMX2TXT_INSTALL "Install the parser" ON)

add_library(${PROJECT_NAME})
add_library(PMX2TXT::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE
    include/pmx2txt/parser/Pmx.h
    include/pmx2txt/parser/pmx/AnchorRigidBody.h
    include/pmx2txt/parser/pmx/Bone.h
    include/pmx2txt/parser/pmx/Frame.h
    include/pmx2txt/parser/pmx/IkLink.h
    include/pmx2txt/parser/pmx/Joint.h
    include/pmx2txt/parser/pmx/Material.h
    include/pmx2txt/parser/pmx/Model.h
    include/pmx2txt/parser/pmx/Morph.h
    include/pmx2txt/parser/pmx/RigidBody.h
    include/pmx2txt/parser/pmx/Setting.h
    include/pmx2txt/parser/pmx/SoftBody.h
    include/pmx2txt/parser/pmx/Vertex.h
    include/pmx2txt/parser/pmx/VertexSkinning.h
    include/pmx2txt/parser/converter.h
    include/pmx2txt/parser/Txt.h

    src/pmx/AnchorRigidBody.cpp
    src/pmx/Bone.cpp
    src/pmx/Frame.cpp
    src/pmx/IkLink.cpp
    src/pmx/Joint.cpp
    src/pmx/Material.cpp
    src/pmx/Model.cpp
    src/pmx/Morph.cpp
    src/pmx/RigidBody.cpp
    src/pmx/Setting.cpp
    src/pmx/SoftBody.cpp
    src/pmx/Vertex.cpp
    src/pmx/VertexSkinning.cpp

    src/pmx/util.cpp
    src/converter.cpp
    src/Txt.cpp
)

if(MSVC)
    target_compile_definitions(${PROJECT_NAME} PUBLIC MSVC)
    target_sources(${PROJECT_NAME} PRIVATE
        include/javacommons/strconv.h
    )
else(MSVC)
    find_package(ICU COMPONENTS data uc REQUIRED)
    target_link_libraries(${PROJECT_NAME} ICU::data ICU::uc)
endif(MSVC)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>  
    $<INSTALL_INTERFACE:include>
)

target_compile_features(${PROJECT_NAME} PUBLIC
    cxx_std_17
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CMAKE_CXX_STANDARD 17
    CMAKE_CXX_STANDARD_REQUIRED ON
    CMAKE_CXX_EXTENSIONS OFF
)

target_compile_options(${PROJECT_NAME} PRIVATE
    "$<$<C_COMPILER_ID:MSVC>:/utf-8>"
    "$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)

if(PMX2TXT_INSTALL)
    install(
        DIRECTORY   "${PROJECT_SOURCE_DIR}/include/"
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/"
    )
    install(TARGETS ${PROJECT_NAME}
        EXPORT "${PROJECT_NAME}Targets"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    )
    install(EXPORT "${PROJECT_NAME}Targets"
        FILE ${PROJECT_NAME}Config.cmake
        NAMESPACE PMX2TXT::
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}"
    )
endif(PMX2TXT_INSTALL)

### Filter Settings for Visual Studio
# Build the visual studio filter same as source directories form.
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

get_target_property(PARSER_SOURCES ${PROJECT_NAME} SOURCES)
assign_source_group(${PARSER_SOURCES})
get_target_property(PARSER_PRIVATE_HEADER ${PROJECT_NAME} PRIVATE_HEADER)
assign_source_group(${PARSER_HEADERS})
get_target_property(PARSER_PUBLIC_HEADER ${PROJECT_NAME} PUBLIC_HEADER)
assign_source_group(${PARSER_PUBLIC_HEADER})
