add_definitions(-DGLM_FORCE_SILENT_WARNINGS)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm)

##set_target_properties(glm PROPERTIES FOLDER 3rdparty)

set(GLM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm/glm)
set(GLM_LIBRARIES ${GLM_LIBRARIES} glm)
