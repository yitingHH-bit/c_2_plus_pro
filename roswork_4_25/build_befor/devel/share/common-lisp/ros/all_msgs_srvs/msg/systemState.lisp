; Auto-generated. Do not edit!


(cl:in-package all_msgs_srvs-msg)


;//! \htmlinclude systemState.msg.html

(cl:defclass <systemState> (roslisp-msg-protocol:ros-message)
  ((device_id
    :reader device_id
    :initarg :device_id
    :type cl:integer
    :initform 0)
   (temperature
    :reader temperature
    :initarg :temperature
    :type cl:float
    :initform 0.0)
   (occ_RAM
    :reader occ_RAM
    :initarg :occ_RAM
    :type cl:float
    :initform 0.0)
   (occ_CPU
    :reader occ_CPU
    :initarg :occ_CPU
    :type cl:float
    :initform 0.0)
   (occ_GPU
    :reader occ_GPU
    :initarg :occ_GPU
    :type cl:float
    :initform 0.0)
   (occ_memory_sda1
    :reader occ_memory_sda1
    :initarg :occ_memory_sda1
    :type cl:float
    :initform 0.0)
   (occ_memory_home
    :reader occ_memory_home
    :initarg :occ_memory_home
    :type cl:float
    :initform 0.0)
   (precess_state
    :reader precess_state
    :initarg :precess_state
    :type cl:float
    :initform 0.0)
   (camera_state
    :reader camera_state
    :initarg :camera_state
    :type cl:float
    :initform 0.0)
   (mmlidar_state
    :reader mmlidar_state
    :initarg :mmlidar_state
    :type cl:float
    :initform 0.0))
)

(cl:defclass systemState (<systemState>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <systemState>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'systemState)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name all_msgs_srvs-msg:<systemState> is deprecated: use all_msgs_srvs-msg:systemState instead.")))

(cl:ensure-generic-function 'device_id-val :lambda-list '(m))
(cl:defmethod device_id-val ((m <systemState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:device_id-val is deprecated.  Use all_msgs_srvs-msg:device_id instead.")
  (device_id m))

(cl:ensure-generic-function 'temperature-val :lambda-list '(m))
(cl:defmethod temperature-val ((m <systemState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:temperature-val is deprecated.  Use all_msgs_srvs-msg:temperature instead.")
  (temperature m))

(cl:ensure-generic-function 'occ_RAM-val :lambda-list '(m))
(cl:defmethod occ_RAM-val ((m <systemState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:occ_RAM-val is deprecated.  Use all_msgs_srvs-msg:occ_RAM instead.")
  (occ_RAM m))

(cl:ensure-generic-function 'occ_CPU-val :lambda-list '(m))
(cl:defmethod occ_CPU-val ((m <systemState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:occ_CPU-val is deprecated.  Use all_msgs_srvs-msg:occ_CPU instead.")
  (occ_CPU m))

(cl:ensure-generic-function 'occ_GPU-val :lambda-list '(m))
(cl:defmethod occ_GPU-val ((m <systemState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:occ_GPU-val is deprecated.  Use all_msgs_srvs-msg:occ_GPU instead.")
  (occ_GPU m))

(cl:ensure-generic-function 'occ_memory_sda1-val :lambda-list '(m))
(cl:defmethod occ_memory_sda1-val ((m <systemState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:occ_memory_sda1-val is deprecated.  Use all_msgs_srvs-msg:occ_memory_sda1 instead.")
  (occ_memory_sda1 m))

(cl:ensure-generic-function 'occ_memory_home-val :lambda-list '(m))
(cl:defmethod occ_memory_home-val ((m <systemState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:occ_memory_home-val is deprecated.  Use all_msgs_srvs-msg:occ_memory_home instead.")
  (occ_memory_home m))

(cl:ensure-generic-function 'precess_state-val :lambda-list '(m))
(cl:defmethod precess_state-val ((m <systemState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:precess_state-val is deprecated.  Use all_msgs_srvs-msg:precess_state instead.")
  (precess_state m))

(cl:ensure-generic-function 'camera_state-val :lambda-list '(m))
(cl:defmethod camera_state-val ((m <systemState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:camera_state-val is deprecated.  Use all_msgs_srvs-msg:camera_state instead.")
  (camera_state m))

(cl:ensure-generic-function 'mmlidar_state-val :lambda-list '(m))
(cl:defmethod mmlidar_state-val ((m <systemState>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:mmlidar_state-val is deprecated.  Use all_msgs_srvs-msg:mmlidar_state instead.")
  (mmlidar_state m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <systemState>) ostream)
  "Serializes a message object of type '<systemState>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'device_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'device_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'device_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'device_id)) ostream)
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'temperature))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'occ_RAM))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'occ_CPU))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'occ_GPU))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'occ_memory_sda1))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'occ_memory_home))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'precess_state))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'camera_state))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'mmlidar_state))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <systemState>) istream)
  "Deserializes a message object of type '<systemState>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'device_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'device_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'device_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'device_id)) (cl:read-byte istream))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'temperature) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'occ_RAM) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'occ_CPU) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'occ_GPU) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'occ_memory_sda1) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'occ_memory_home) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'precess_state) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'camera_state) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'mmlidar_state) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<systemState>)))
  "Returns string type for a message object of type '<systemState>"
  "all_msgs_srvs/systemState")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'systemState)))
  "Returns string type for a message object of type 'systemState"
  "all_msgs_srvs/systemState")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<systemState>)))
  "Returns md5sum for a message object of type '<systemState>"
  "8e6672b93cb0d4c408677cdddcfc6c31")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'systemState)))
  "Returns md5sum for a message object of type 'systemState"
  "8e6672b93cb0d4c408677cdddcfc6c31")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<systemState>)))
  "Returns full string definition for message of type '<systemState>"
  (cl:format cl:nil "uint32 device_id~%float32 temperature~%float32 occ_RAM~%float32 occ_CPU~%float32 occ_GPU~%float32 occ_memory_sda1~%float32 occ_memory_home~%float32 precess_state~%float32 camera_state~%float32 mmlidar_state~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'systemState)))
  "Returns full string definition for message of type 'systemState"
  (cl:format cl:nil "uint32 device_id~%float32 temperature~%float32 occ_RAM~%float32 occ_CPU~%float32 occ_GPU~%float32 occ_memory_sda1~%float32 occ_memory_home~%float32 precess_state~%float32 camera_state~%float32 mmlidar_state~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <systemState>))
  (cl:+ 0
     4
     4
     4
     4
     4
     4
     4
     4
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <systemState>))
  "Converts a ROS message object to a list"
  (cl:list 'systemState
    (cl:cons ':device_id (device_id msg))
    (cl:cons ':temperature (temperature msg))
    (cl:cons ':occ_RAM (occ_RAM msg))
    (cl:cons ':occ_CPU (occ_CPU msg))
    (cl:cons ':occ_GPU (occ_GPU msg))
    (cl:cons ':occ_memory_sda1 (occ_memory_sda1 msg))
    (cl:cons ':occ_memory_home (occ_memory_home msg))
    (cl:cons ':precess_state (precess_state msg))
    (cl:cons ':camera_state (camera_state msg))
    (cl:cons ':mmlidar_state (mmlidar_state msg))
))
