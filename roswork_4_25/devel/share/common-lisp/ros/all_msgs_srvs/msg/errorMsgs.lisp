; Auto-generated. Do not edit!


(cl:in-package all_msgs_srvs-msg)


;//! \htmlinclude errorMsgs.msg.html

(cl:defclass <errorMsgs> (roslisp-msg-protocol:ros-message)
  ((device_id
    :reader device_id
    :initarg :device_id
    :type cl:integer
    :initform 0)
   (type
    :reader type
    :initarg :type
    :type cl:integer
    :initform 0)
   (data
    :reader data
    :initarg :data
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass errorMsgs (<errorMsgs>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <errorMsgs>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'errorMsgs)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name all_msgs_srvs-msg:<errorMsgs> is deprecated: use all_msgs_srvs-msg:errorMsgs instead.")))

(cl:ensure-generic-function 'device_id-val :lambda-list '(m))
(cl:defmethod device_id-val ((m <errorMsgs>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:device_id-val is deprecated.  Use all_msgs_srvs-msg:device_id instead.")
  (device_id m))

(cl:ensure-generic-function 'type-val :lambda-list '(m))
(cl:defmethod type-val ((m <errorMsgs>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:type-val is deprecated.  Use all_msgs_srvs-msg:type instead.")
  (type m))

(cl:ensure-generic-function 'data-val :lambda-list '(m))
(cl:defmethod data-val ((m <errorMsgs>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:data-val is deprecated.  Use all_msgs_srvs-msg:data instead.")
  (data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <errorMsgs>) ostream)
  "Serializes a message object of type '<errorMsgs>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'device_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'device_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'device_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'device_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'type)) ostream)
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'data))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'data))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <errorMsgs>) istream)
  "Deserializes a message object of type '<errorMsgs>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'device_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'device_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'device_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'device_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'type)) (cl:read-byte istream))
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'data) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'data)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<errorMsgs>)))
  "Returns string type for a message object of type '<errorMsgs>"
  "all_msgs_srvs/errorMsgs")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'errorMsgs)))
  "Returns string type for a message object of type 'errorMsgs"
  "all_msgs_srvs/errorMsgs")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<errorMsgs>)))
  "Returns md5sum for a message object of type '<errorMsgs>"
  "da293913ab09cf0b6203d2182912db2a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'errorMsgs)))
  "Returns md5sum for a message object of type 'errorMsgs"
  "da293913ab09cf0b6203d2182912db2a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<errorMsgs>)))
  "Returns full string definition for message of type '<errorMsgs>"
  (cl:format cl:nil "uint32 device_id~%uint32 type~%float32[] data~%~%#type:0错误信息,1警告消息,2系统消息~%~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'errorMsgs)))
  "Returns full string definition for message of type 'errorMsgs"
  (cl:format cl:nil "uint32 device_id~%uint32 type~%float32[] data~%~%#type:0错误信息,1警告消息,2系统消息~%~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <errorMsgs>))
  (cl:+ 0
     4
     4
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'data) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <errorMsgs>))
  "Converts a ROS message object to a list"
  (cl:list 'errorMsgs
    (cl:cons ':device_id (device_id msg))
    (cl:cons ':type (type msg))
    (cl:cons ':data (data msg))
))
