; Auto-generated. Do not edit!


(cl:in-package all_msgs_srvs-msg)


;//! \htmlinclude settings.msg.html

(cl:defclass <settings> (roslisp-msg-protocol:ros-message)
  ((stamp
    :reader stamp
    :initarg :stamp
    :type cl:real
    :initform 0)
   (frame_id
    :reader frame_id
    :initarg :frame_id
    :type cl:fixnum
    :initform 0)
   (type
    :reader type
    :initarg :type
    :type cl:integer
    :initform 0)
   (which_param
    :reader which_param
    :initarg :which_param
    :type cl:integer
    :initform 0)
   (setdatas
    :reader setdatas
    :initarg :setdatas
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass settings (<settings>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <settings>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'settings)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name all_msgs_srvs-msg:<settings> is deprecated: use all_msgs_srvs-msg:settings instead.")))

(cl:ensure-generic-function 'stamp-val :lambda-list '(m))
(cl:defmethod stamp-val ((m <settings>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:stamp-val is deprecated.  Use all_msgs_srvs-msg:stamp instead.")
  (stamp m))

(cl:ensure-generic-function 'frame_id-val :lambda-list '(m))
(cl:defmethod frame_id-val ((m <settings>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:frame_id-val is deprecated.  Use all_msgs_srvs-msg:frame_id instead.")
  (frame_id m))

(cl:ensure-generic-function 'type-val :lambda-list '(m))
(cl:defmethod type-val ((m <settings>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:type-val is deprecated.  Use all_msgs_srvs-msg:type instead.")
  (type m))

(cl:ensure-generic-function 'which_param-val :lambda-list '(m))
(cl:defmethod which_param-val ((m <settings>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:which_param-val is deprecated.  Use all_msgs_srvs-msg:which_param instead.")
  (which_param m))

(cl:ensure-generic-function 'setdatas-val :lambda-list '(m))
(cl:defmethod setdatas-val ((m <settings>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:setdatas-val is deprecated.  Use all_msgs_srvs-msg:setdatas instead.")
  (setdatas m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <settings>) ostream)
  "Serializes a message object of type '<settings>"
  (cl:let ((__sec (cl:floor (cl:slot-value msg 'stamp)))
        (__nsec (cl:round (cl:* 1e9 (cl:- (cl:slot-value msg 'stamp) (cl:floor (cl:slot-value msg 'stamp)))))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __sec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 0) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __nsec) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __nsec) ostream))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'frame_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'frame_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'which_param)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'which_param)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'which_param)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'which_param)) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'setdatas))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'setdatas))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <settings>) istream)
  "Deserializes a message object of type '<settings>"
    (cl:let ((__sec 0) (__nsec 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __sec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 0) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __nsec) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __nsec) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'stamp) (cl:+ (cl:coerce __sec 'cl:double-float) (cl:/ __nsec 1e9))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'frame_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'frame_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'which_param)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'which_param)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'which_param)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'which_param)) (cl:read-byte istream))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'setdatas) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'setdatas)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<settings>)))
  "Returns string type for a message object of type '<settings>"
  "all_msgs_srvs/settings")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'settings)))
  "Returns string type for a message object of type 'settings"
  "all_msgs_srvs/settings")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<settings>)))
  "Returns md5sum for a message object of type '<settings>"
  "2c5fd6a4bb0c365ef3a2eff725118bfe")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'settings)))
  "Returns md5sum for a message object of type 'settings"
  "2c5fd6a4bb0c365ef3a2eff725118bfe")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<settings>)))
  "Returns full string definition for message of type '<settings>"
  (cl:format cl:nil "time stamp~%uint16 frame_id~%uint32 type~%uint32 which_param~%float32[] setdatas~%~%~%#type:~%#lan_info(0)~%#statis_info(1)~%#Transform_info(2)~%#Network_info(3)~%#Install_Info(4)~%~%~%#which_param~%#1101:给统计节点设置热参数[A,B,C,车道数,车道1断面数,车道1断面1距离,车道1断面2距离,...,车道n断面数,车道n断面1距离,车道n断面2距离...]~%#1102:[周期]~%#3001:[设备id]~%#1001:车道坐标设置 [车道总数,]~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'settings)))
  "Returns full string definition for message of type 'settings"
  (cl:format cl:nil "time stamp~%uint16 frame_id~%uint32 type~%uint32 which_param~%float32[] setdatas~%~%~%#type:~%#lan_info(0)~%#statis_info(1)~%#Transform_info(2)~%#Network_info(3)~%#Install_Info(4)~%~%~%#which_param~%#1101:给统计节点设置热参数[A,B,C,车道数,车道1断面数,车道1断面1距离,车道1断面2距离,...,车道n断面数,车道n断面1距离,车道n断面2距离...]~%#1102:[周期]~%#3001:[设备id]~%#1001:车道坐标设置 [车道总数,]~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <settings>))
  (cl:+ 0
     8
     2
     4
     4
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'setdatas) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <settings>))
  "Converts a ROS message object to a list"
  (cl:list 'settings
    (cl:cons ':stamp (stamp msg))
    (cl:cons ':frame_id (frame_id msg))
    (cl:cons ':type (type msg))
    (cl:cons ':which_param (which_param msg))
    (cl:cons ':setdatas (setdatas msg))
))
