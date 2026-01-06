# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Nihilai Collective Corp
# https://github.com/nihilai-collective/rt-ut
# cmake/installation_setup.cmake

set(CONFIG_FILE_NAME ${PROJECT_NAME}Config.cmake)
set(EXPORTED_TARGETS_NAME ${PROJECT_NAME}Targets)
set(EXPORTED_TARGETS_FILE_NAME ${EXPORTED_TARGETS_NAME}.cmake)

include(CMakePackageConfigHelpers)
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/${CONFIG_FILE_NAME}.in
	${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME}
	INSTALL_DESTINATION share/rt-ut
)

install(
	FILES
	${CMAKE_CURRENT_BINARY_DIR}/${CONFIG_FILE_NAME}
	DESTINATION share/rt-ut
)

install(
	DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/include/
	DESTINATION include
)

install(
	TARGETS ${PROJECT_NAME}
	EXPORT ${EXPORTED_TARGETS_NAME}
)

install(
	EXPORT ${EXPORTED_TARGETS_NAME}
	FILE ${EXPORTED_TARGETS_FILE_NAME}
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION share/rt-ut
)