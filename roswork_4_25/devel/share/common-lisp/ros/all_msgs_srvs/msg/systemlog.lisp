; Auto-generated. Do not edit!


(cl:in-package all_msgs_srvs-msg)


;//! \htmlinclude systemlog.msg.html

(cl:defclass <systemlog> (roslisp-msg-protocol:ros-message)
  ((device_id
    :reader device_id
    :initarg :device_id
    :type cl:fixnum
    :initform 0)
   (node_id
    :reader node_id
    :initarg :node_id
    :type cl:string
    :initform "")
   (type
    :reader type
    :initarg :type
    :type cl:integer
    :initform 0)
   (describe
    :reader describe
    :initarg :describe
    :type cl:string
    :initform "")
   (data
    :reader data
    :initarg :data
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass systemlog (<systemlog>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <systemlog>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'systemlog)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name all_msgs_srvs-msg:<systemlog> is deprecated: use all_msgs_srvs-msg:systemlog instead.")))

(cl:ensure-generic-function 'device_id-val :lambda-list '(m))
(cl:defmethod device_id-val ((m <systemlog>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:device_id-val is deprecated.  Use all_msgs_srvs-msg:device_id instead.")
  (device_id m))

(cl:ensure-generic-function 'node_id-val :lambda-list '(m))
(cl:defmethod node_id-val ((m <systemlog>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:node_id-val is deprecated.  Use all_msgs_srvs-msg:node_id instead.")
  (node_id m))

(cl:ensure-generic-function 'type-val :lambda-list '(m))
(cl:defmethod type-val ((m <systemlog>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:type-val is deprecated.  Use all_msgs_srvs-msg:type instead.")
  (type m))

(cl:ensure-generic-function 'describe-val :lambda-list '(m))
(cl:defmethod describe-val ((m <systemlog>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:describe-val is deprecated.  Use all_msgs_srvs-msg:describe instead.")
  (describe m))

(cl:ensure-generic-function 'data-val :lambda-list '(m))
(cl:defmethod data-val ((m <systemlog>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:data-val is deprecated.  Use all_msgs_srvs-msg:data instead.")
  (data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <systemlog>) ostream)
  "Serializes a message object of type '<systemlog>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'device_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'device_id)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'node_id))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'node_id))
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'type)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'type)) ostream)
  (cl:let ((__ros_str_len (cl:length (cl:slot-value msg 'describe))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_str_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_str_len) ostream))
  (cl:map cl:nil #'(cl:lambda (c) (cl:write-byte (cl:char-code c) ostream)) (cl:slot-value msg 'describe))
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <systemlog>) istream)
  "Deserializes a message object of type '<systemlog>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'device_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'device_id)) (cl:read-byte istream))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'node_id) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'node_id) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'type)) (cl:read-byte istream))
    (cl:let ((__ros_str_len 0))
      (cl:setf (cl:ldb (cl:byte 8 0) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) __ros_str_len) (cl:read-byte istream))
      (cl:setf (cl:slot-value msg 'describe) (cl:make-string __ros_str_len))
      (cl:dotimes (__ros_str_idx __ros_str_len msg)
        (cl:setf (cl:char (cl:slot-value msg 'describe) __ros_str_idx) (cl:code-char (cl:read-byte istream)))))
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<systemlog>)))
  "Returns string type for a message object of type '<systemlog>"
  "all_msgs_srvs/systemlog")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'systemlog)))
  "Returns string type for a message object of type 'systemlog"
  "all_msgs_srvs/systemlog")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<systemlog>)))
  "Returns md5sum for a message object of type '<systemlog>"
  "fe73fa608b885c27025369005afbf02b")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'systemlog)))
  "Returns md5sum for a message object of type 'systemlog"
  "fe73fa608b885c27025369005afbf02b")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<systemlog>)))
  "Returns full string definition for message of type '<systemlog>"
  (cl:format cl:nil "uint16 device_id~%string node_id~%uint32 type~%string describe~%float32[] data~%~%~%#数据意义~%#device_id:表示设备id~%#node_id:表示哪个节点~%#type:表示什么类型的标识,1(warring),2(info),其他(debug),10(error致命错误),11(error一般错误)~%#describe:问题表述,操作表述~%#data:操作的数据~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'systemlog)))
  "Returns full string definition for message of type 'systemlog"
  (cl:format cl:nil "uint16 device_id~%string node_id~%uint32 type~%string describe~%float32[] data~%~%~%#数据意义~%#device_id:表示设备id~%#node_id:表示哪个节点~%#type:表示什么类型的标识,1(warring),2(info),其他(debug),10(error致命错误),11(error一般错误)~%#describe:问题表述,操作表述~%#data:操作的数据~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <systemlog>))
  (cl:+ 0
     2
     4 (cl:length (cl:slot-value msg 'node_id))
     4
     4 (cl:length (cl:slot-value msg 'describe))
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'data) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <systemlog>))
  "Converts a ROS message object to a list"
  (cl:list 'systemlog
    (cl:cons ':device_id (device_id msg))
    (cl:cons ':node_id (node_id msg))
    (cl:cons ':type (type msg))
    (cl:cons ':describe (describe msg))
    (cl:cons ':data (data msg))
))
