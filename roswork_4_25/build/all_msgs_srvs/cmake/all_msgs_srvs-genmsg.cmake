# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "all_msgs_srvs: 6 messages, 1 services")

set(MSG_I_FLAGS "-Iall_msgs_srvs:/home/hsax/roswork/src/all_msgs_srvs/msg;-Igeometry_msgs:/opt/ros/melodic/share/geometry_msgs/cmake/../msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(all_msgs_srvs_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/errorMsgs.msg" NAME_WE)
add_custom_target(_all_msgs_srvs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "all_msgs_srvs" "/home/hsax/roswork/src/all_msgs_srvs/msg/errorMsgs.msg" ""
)

get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/systemlog.msg" NAME_WE)
add_custom_target(_all_msgs_srvs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "all_msgs_srvs" "/home/hsax/roswork/src/all_msgs_srvs/msg/systemlog.msg" ""
)

get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/systemState.msg" NAME_WE)
add_custom_target(_all_msgs_srvs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "all_msgs_srvs" "/home/hsax/roswork/src/all_msgs_srvs/msg/systemState.msg" ""
)

get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/srv/settingcallback.srv" NAME_WE)
add_custom_target(_all_msgs_srvs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "all_msgs_srvs" "/home/hsax/roswork/src/all_msgs_srvs/srv/settingcallback.srv" ""
)

get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/bdinfomsg.msg" NAME_WE)
add_custom_target(_all_msgs_srvs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "all_msgs_srvs" "/home/hsax/roswork/src/all_msgs_srvs/msg/bdinfomsg.msg" ""
)

get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/vehicalmsgs.msg" NAME_WE)
add_custom_target(_all_msgs_srvs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "all_msgs_srvs" "/home/hsax/roswork/src/all_msgs_srvs/msg/vehicalmsgs.msg" ""
)

get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/settings.msg" NAME_WE)
add_custom_target(_all_msgs_srvs_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "all_msgs_srvs" "/home/hsax/roswork/src/all_msgs_srvs/msg/settings.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/errorMsgs.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_cpp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/systemlog.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_cpp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/systemState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_cpp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/bdinfomsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_cpp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/vehicalmsgs.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_cpp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/settings.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/all_msgs_srvs
)

### Generating Services
_generate_srv_cpp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/srv/settingcallback.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/all_msgs_srvs
)

### Generating Module File
_generate_module_cpp(all_msgs_srvs
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/all_msgs_srvs
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(all_msgs_srvs_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(all_msgs_srvs_generate_messages all_msgs_srvs_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/errorMsgs.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_cpp _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/systemlog.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_cpp _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/systemState.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_cpp _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/srv/settingcallback.srv" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_cpp _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/bdinfomsg.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_cpp _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/vehicalmsgs.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_cpp _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/settings.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_cpp _all_msgs_srvs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(all_msgs_srvs_gencpp)
add_dependencies(all_msgs_srvs_gencpp all_msgs_srvs_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS all_msgs_srvs_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/errorMsgs.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_eus(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/systemlog.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_eus(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/systemState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_eus(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/bdinfomsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_eus(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/vehicalmsgs.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_eus(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/settings.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/all_msgs_srvs
)

### Generating Services
_generate_srv_eus(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/srv/settingcallback.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/all_msgs_srvs
)

### Generating Module File
_generate_module_eus(all_msgs_srvs
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/all_msgs_srvs
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(all_msgs_srvs_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(all_msgs_srvs_generate_messages all_msgs_srvs_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/errorMsgs.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_eus _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/systemlog.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_eus _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/systemState.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_eus _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/srv/settingcallback.srv" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_eus _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/bdinfomsg.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_eus _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/vehicalmsgs.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_eus _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/settings.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_eus _all_msgs_srvs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(all_msgs_srvs_geneus)
add_dependencies(all_msgs_srvs_geneus all_msgs_srvs_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS all_msgs_srvs_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/errorMsgs.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_lisp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/systemlog.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_lisp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/systemState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_lisp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/bdinfomsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_lisp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/vehicalmsgs.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_lisp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/settings.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/all_msgs_srvs
)

### Generating Services
_generate_srv_lisp(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/srv/settingcallback.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/all_msgs_srvs
)

### Generating Module File
_generate_module_lisp(all_msgs_srvs
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/all_msgs_srvs
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(all_msgs_srvs_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(all_msgs_srvs_generate_messages all_msgs_srvs_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/errorMsgs.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_lisp _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/systemlog.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_lisp _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/systemState.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_lisp _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/srv/settingcallback.srv" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_lisp _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/bdinfomsg.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_lisp _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/vehicalmsgs.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_lisp _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/settings.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_lisp _all_msgs_srvs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(all_msgs_srvs_genlisp)
add_dependencies(all_msgs_srvs_genlisp all_msgs_srvs_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS all_msgs_srvs_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/errorMsgs.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_nodejs(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/systemlog.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_nodejs(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/systemState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_nodejs(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/bdinfomsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_nodejs(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/vehicalmsgs.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_nodejs(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/settings.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/all_msgs_srvs
)

### Generating Services
_generate_srv_nodejs(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/srv/settingcallback.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/all_msgs_srvs
)

### Generating Module File
_generate_module_nodejs(all_msgs_srvs
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/all_msgs_srvs
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(all_msgs_srvs_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(all_msgs_srvs_generate_messages all_msgs_srvs_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/errorMsgs.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_nodejs _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/systemlog.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_nodejs _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/systemState.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_nodejs _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/srv/settingcallback.srv" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_nodejs _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/bdinfomsg.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_nodejs _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/vehicalmsgs.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_nodejs _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/settings.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_nodejs _all_msgs_srvs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(all_msgs_srvs_gennodejs)
add_dependencies(all_msgs_srvs_gennodejs all_msgs_srvs_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS all_msgs_srvs_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/errorMsgs.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_py(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/systemlog.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_py(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/systemState.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_py(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/bdinfomsg.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_py(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/vehicalmsgs.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/all_msgs_srvs
)
_generate_msg_py(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/msg/settings.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/all_msgs_srvs
)

### Generating Services
_generate_srv_py(all_msgs_srvs
  "/home/hsax/roswork/src/all_msgs_srvs/srv/settingcallback.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/all_msgs_srvs
)

### Generating Module File
_generate_module_py(all_msgs_srvs
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/all_msgs_srvs
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(all_msgs_srvs_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(all_msgs_srvs_generate_messages all_msgs_srvs_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/errorMsgs.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_py _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/systemlog.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_py _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/systemState.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_py _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/srv/settingcallback.srv" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_py _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/bdinfomsg.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_py _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/vehicalmsgs.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_py _all_msgs_srvs_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/hsax/roswork/src/all_msgs_srvs/msg/settings.msg" NAME_WE)
add_dependencies(all_msgs_srvs_generate_messages_py _all_msgs_srvs_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(all_msgs_srvs_genpy)
add_dependencies(all_msgs_srvs_genpy all_msgs_srvs_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS all_msgs_srvs_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/all_msgs_srvs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/all_msgs_srvs
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(all_msgs_srvs_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/all_msgs_srvs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/all_msgs_srvs
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(all_msgs_srvs_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/all_msgs_srvs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/all_msgs_srvs
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(all_msgs_srvs_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/all_msgs_srvs)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/all_msgs_srvs
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(all_msgs_srvs_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/all_msgs_srvs)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/all_msgs_srvs\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/all_msgs_srvs
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(all_msgs_srvs_generate_messages_py geometry_msgs_generate_messages_py)
endif()
