; Auto-generated. Do not edit!


(cl:in-package all_msgs_srvs-srv)


;//! \htmlinclude settingcallback-request.msg.html

(cl:defclass <settingcallback-request> (roslisp-msg-protocol:ros-message)
  ((fram_id
    :reader fram_id
    :initarg :fram_id
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

(cl:defclass settingcallback-request (<settingcallback-request>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <settingcallback-request>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'settingcallback-request)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name all_msgs_srvs-srv:<settingcallback-request> is deprecated: use all_msgs_srvs-srv:settingcallback-request instead.")))

(cl:ensure-generic-function 'fram_id-val :lambda-list '(m))
(cl:defmethod fram_id-val ((m <settingcallback-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-srv:fram_id-val is deprecated.  Use all_msgs_srvs-srv:fram_id instead.")
  (fram_id m))

(cl:ensure-generic-function 'type-val :lambda-list '(m))
(cl:defmethod type-val ((m <settingcallback-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-srv:type-val is deprecated.  Use all_msgs_srvs-srv:type instead.")
  (type m))

(cl:ensure-generic-function 'data-val :lambda-list '(m))
(cl:defmethod data-val ((m <settingcallback-request>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-srv:data-val is deprecated.  Use all_msgs_srvs-srv:data instead.")
  (data m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <settingcallback-request>) ostream)
  "Serializes a message object of type '<settingcallback-request>"
  (cl:write-byte (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'fram_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'fram_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'fram_id)) ostream)
  (cl:write-byte (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'fram_id)) ostream)
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <settingcallback-request>) istream)
  "Deserializes a message object of type '<settingcallback-request>"
    (cl:setf (cl:ldb (cl:byte 8 0) (cl:slot-value msg 'fram_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) (cl:slot-value msg 'fram_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) (cl:slot-value msg 'fram_id)) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) (cl:slot-value msg 'fram_id)) (cl:read-byte istream))
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<settingcallback-request>)))
  "Returns string type for a service object of type '<settingcallback-request>"
  "all_msgs_srvs/settingcallbackRequest")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'settingcallback-request)))
  "Returns string type for a service object of type 'settingcallback-request"
  "all_msgs_srvs/settingcallbackRequest")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<settingcallback-request>)))
  "Returns md5sum for a message object of type '<settingcallback-request>"
  "a893383d462b2192990ef4619919d478")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'settingcallback-request)))
  "Returns md5sum for a message object of type 'settingcallback-request"
  "a893383d462b2192990ef4619919d478")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<settingcallback-request>)))
  "Returns full string definition for message of type '<settingcallback-request>"
  (cl:format cl:nil "uint32 fram_id~%uint32 type~%float32[] data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'settingcallback-request)))
  "Returns full string definition for message of type 'settingcallback-request"
  (cl:format cl:nil "uint32 fram_id~%uint32 type~%float32[] data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <settingcallback-request>))
  (cl:+ 0
     4
     4
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'data) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <settingcallback-request>))
  "Converts a ROS message object to a list"
  (cl:list 'settingcallback-request
    (cl:cons ':fram_id (fram_id msg))
    (cl:cons ':type (type msg))
    (cl:cons ':data (data msg))
))
;//! \htmlinclude settingcallback-response.msg.html

(cl:defclass <settingcallback-response> (roslisp-msg-protocol:ros-message)
  ((setdatas
    :reader setdatas
    :initarg :setdatas
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass settingcallback-response (<settingcallback-response>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <settingcallback-response>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'settingcallback-response)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name all_msgs_srvs-srv:<settingcallback-response> is deprecated: use all_msgs_srvs-srv:settingcallback-response instead.")))

(cl:ensure-generic-function 'setdatas-val :lambda-list '(m))
(cl:defmethod setdatas-val ((m <settingcallback-response>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-srv:setdatas-val is deprecated.  Use all_msgs_srvs-srv:setdatas instead.")
  (setdatas m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <settingcallback-response>) ostream)
  "Serializes a message object of type '<settingcallback-response>"
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
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <settingcallback-response>) istream)
  "Deserializes a message object of type '<settingcallback-response>"
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
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<settingcallback-response>)))
  "Returns string type for a service object of type '<settingcallback-response>"
  "all_msgs_srvs/settingcallbackResponse")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'settingcallback-response)))
  "Returns string type for a service object of type 'settingcallback-response"
  "all_msgs_srvs/settingcallbackResponse")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<settingcallback-response>)))
  "Returns md5sum for a message object of type '<settingcallback-response>"
  "a893383d462b2192990ef4619919d478")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'settingcallback-response)))
  "Returns md5sum for a message object of type 'settingcallback-response"
  "a893383d462b2192990ef4619919d478")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<settingcallback-response>)))
  "Returns full string definition for message of type '<settingcallback-response>"
  (cl:format cl:nil "float32[] setdatas~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'settingcallback-response)))
  "Returns full string definition for message of type 'settingcallback-response"
  (cl:format cl:nil "float32[] setdatas~%~%~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <settingcallback-response>))
  (cl:+ 0
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'setdatas) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <settingcallback-response>))
  "Converts a ROS message object to a list"
  (cl:list 'settingcallback-response
    (cl:cons ':setdatas (setdatas msg))
))
(cl:defmethod roslisp-msg-protocol:service-request-type ((msg (cl:eql 'settingcallback)))
  'settingcallback-request)
(cl:defmethod roslisp-msg-protocol:service-response-type ((msg (cl:eql 'settingcallback)))
  'settingcallback-response)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'settingcallback)))
  "Returns string type for a service object of type '<settingcallback>"
  "all_msgs_srvs/settingcallback")