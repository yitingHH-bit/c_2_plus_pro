; Auto-generated. Do not edit!


(cl:in-package all_msgs_srvs-msg)


;//! \htmlinclude bdinfomsg.msg.html

(cl:defclass <bdinfomsg> (roslisp-msg-protocol:ros-message)
  ((node_name
    :reader node_name
    :initarg :node_name
    :type cl:string
    :initform "")
   (device_id
    :reader device_id
    :initarg :device_id
    :type cl:integer
    :initform 0)
   (isNew
    :reader isNew
    :initarg :isNew
    :type cl:integer
    :initform 0)
   (timenow
    :reader timenow
    :initarg :timenow
    :type cl:string
    :initform "")
   (lon
    :reader lon
    :initarg :lon
    :type cl:float
    :initform 0.0)
   (alt
    :reader alt
    :initarg :alt
    :type cl:float
    :initform 0.0))
)

(cl:defclass bdinfomsg (<bdinfomsg>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <bdinfomsg>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'bdinfomsg)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name all_msgs_srvs-msg:<bdinfomsg> is deprecated: use all_msgs_srvs-msg:bdinfomsg instead.")))

(cl:ensure-generic-function 'node_name-val :lambda-list '(m))
(cl:defmethod node_name-val ((m <bdinfomsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:node_name-val is deprecated.  Use all_msgs_srvs-msg:node_name instead.")
  (node_name m))

(cl:ensure-generic-function 'device_id-val :lambda-list '(m))
(cl:defmethod device_id-val ((m <bdinfomsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:device_id-val is deprecated.  Use all_msgs_srvs-msg:device_id instead.")
  (device_id m))

(cl:ensure-generic-function 'isNew-val :lambda-list '(m))
(cl:defmethod isNew-val ((m <bdinfomsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:isNew-val is deprecated.  Use all_msgs_srvs-msg:isNew instead.")
  (isNew m))

(cl:ensure-generic-function 'timenow-val :lambda-list '(m))
(cl:defmethod timenow-val ((m <bdinfomsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:timenow-val is deprecated.  Use all_msgs_srvs-msg:timenow instead.")
  (timenow m))

(cl:ensure-generic-function 'lon-val :lambda-list '(m))
(cl:defmethod lon-val ((m <bdinfomsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:lon-val is deprecated.  Use all_msgs_srvs-msg:lon instead.")
  (lon m))

(cl:ensure-generic-function 'alt-val :lambda-list '(m))
(cl:defmethod alt-val ((m <bdinfomsg>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:alt-val is deprecated.  Use all_msgs_srvs-msg:alt instead.")
  (alt m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <bdinfomsg>) ostream)
  "Serializes a message object of type '<bdinfomsg>"
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'node_name))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'node_name))
  (cl:let* ((signed (cl:slot-value msg 'device_id)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let* ((signed (cl:slot-value msg 'isNew)) (unsigned (cl:if (cl:< signed 0) (cl:+ signed 4294967296) signed)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) unsigned) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) unsigned) ostream)
    )
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'timenow))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'timenow))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'lon))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
  (cl:let ((bits (roslisp-utils:encode-single-float-bits (cl:slot-value msg 'alt))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <bdinfomsg>) istream)
  "Deserializes a message object of type '<bdinfomsg>"
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'node_name) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'node_name) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'device_id) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((unsigned 0))
      (cl:setf (cl:ldb (cl:byte 8 0) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) unsigned) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) unsigned) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'isNew) (cl:if (cl:< unsigned 2147483648) unsigned (cl:- unsigned 4294967296))))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'timenow) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'timenow) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'lon) (roslisp-utils:decode-single-float-bits bits)))
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:slot-value msg 'alt) (roslisp-utils:decode-single-float-bits bits)))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<bdinfomsg>)))
  "Returns string type for a message object of type '<bdinfomsg>"
  "all_msgs_srvs/bdinfomsg")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'bdinfomsg)))
  "Returns string type for a message object of type 'bdinfomsg"
  "all_msgs_srvs/bdinfomsg")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<bdinfomsg>)))
  "Returns md5sum for a message object of type '<bdinfomsg>"
  "8bc75b840511959862d72a0c796ab46a")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'bdinfomsg)))
  "Returns md5sum for a message object of type 'bdinfomsg"
  "8bc75b840511959862d72a0c796ab46a")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<bdinfomsg>)))
  "Returns full string definition for message of type '<bdinfomsg>"
  (cl:format cl:nil "string node_name~%int32 device_id~%int32 isNew~%string timenow~%float32 lon~%float32 alt~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'bdinfomsg)))
  "Returns full string definition for message of type 'bdinfomsg"
  (cl:format cl:nil "string node_name~%int32 device_id~%int32 isNew~%string timenow~%float32 lon~%float32 alt~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <bdinfomsg>))
  (cl:+ 0
     4 (cl:length (cl:slot-value msg 'node_name))
     4
     4
     4 (cl:length (cl:slot-value msg 'timenow))
     4
     4
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <bdinfomsg>))
  "Converts a ROS message object to a list"
  (cl:list 'bdinfomsg
    (cl:cons ':node_name (node_name msg))
    (cl:cons ':device_id (device_id msg))
    (cl:cons ':isNew (isNew msg))
    (cl:cons ':timenow (timenow msg))
    (cl:cons ':lon (lon msg))
    (cl:cons ':alt (alt msg))
))
