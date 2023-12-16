; Auto-generated. Do not edit!


(cl:in-package all_msgs_srvs-msg)


;//! \htmlinclude vehicalmsgs.msg.html

(cl:defclass <vehicalmsgs> (roslisp-msg-protocol:ros-message)
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
   (vehicaldatas
    :reader vehicaldatas
    :initarg :vehicaldatas
    :type (cl:vector cl:float)
   :initform (cl:make-array 0 :element-type 'cl:float :initial-element 0.0)))
)

(cl:defclass vehicalmsgs (<vehicalmsgs>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <vehicalmsgs>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'vehicalmsgs)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name all_msgs_srvs-msg:<vehicalmsgs> is deprecated: use all_msgs_srvs-msg:vehicalmsgs instead.")))

(cl:ensure-generic-function 'stamp-val :lambda-list '(m))
(cl:defmethod stamp-val ((m <vehicalmsgs>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:stamp-val is deprecated.  Use all_msgs_srvs-msg:stamp instead.")
  (stamp m))

(cl:ensure-generic-function 'frame_id-val :lambda-list '(m))
(cl:defmethod frame_id-val ((m <vehicalmsgs>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:frame_id-val is deprecated.  Use all_msgs_srvs-msg:frame_id instead.")
  (frame_id m))

(cl:ensure-generic-function 'vehicaldatas-val :lambda-list '(m))
(cl:defmethod vehicaldatas-val ((m <vehicalmsgs>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader all_msgs_srvs-msg:vehicaldatas-val is deprecated.  Use all_msgs_srvs-msg:vehicaldatas instead.")
  (vehicaldatas m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <vehicalmsgs>) ostream)
  "Serializes a message object of type '<vehicalmsgs>"
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
  (cl:let ((__ros_arr_len (cl:length (cl:slot-value msg 'vehicaldatas))))
    (cl:write-byte (cl:ldb (cl:byte 8 0) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) __ros_arr_len) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) __ros_arr_len) ostream))
  (cl:map cl:nil #'(cl:lambda (ele) (cl:let ((bits (roslisp-utils:encode-single-float-bits ele)))
    (cl:write-byte (cl:ldb (cl:byte 8 0) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 8) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 16) bits) ostream)
    (cl:write-byte (cl:ldb (cl:byte 8 24) bits) ostream)))
   (cl:slot-value msg 'vehicaldatas))
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <vehicalmsgs>) istream)
  "Deserializes a message object of type '<vehicalmsgs>"
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
  (cl:let ((__ros_arr_len 0))
    (cl:setf (cl:ldb (cl:byte 8 0) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 8) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 16) __ros_arr_len) (cl:read-byte istream))
    (cl:setf (cl:ldb (cl:byte 8 24) __ros_arr_len) (cl:read-byte istream))
  (cl:setf (cl:slot-value msg 'vehicaldatas) (cl:make-array __ros_arr_len))
  (cl:let ((vals (cl:slot-value msg 'vehicaldatas)))
    (cl:dotimes (i __ros_arr_len)
    (cl:let ((bits 0))
      (cl:setf (cl:ldb (cl:byte 8 0) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 8) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 16) bits) (cl:read-byte istream))
      (cl:setf (cl:ldb (cl:byte 8 24) bits) (cl:read-byte istream))
    (cl:setf (cl:aref vals i) (roslisp-utils:decode-single-float-bits bits))))))
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<vehicalmsgs>)))
  "Returns string type for a message object of type '<vehicalmsgs>"
  "all_msgs_srvs/vehicalmsgs")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'vehicalmsgs)))
  "Returns string type for a message object of type 'vehicalmsgs"
  "all_msgs_srvs/vehicalmsgs")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<vehicalmsgs>)))
  "Returns md5sum for a message object of type '<vehicalmsgs>"
  "3744cc7a9e5f2c7d66d5bd9cb6fa612d")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'vehicalmsgs)))
  "Returns md5sum for a message object of type 'vehicalmsgs"
  "3744cc7a9e5f2c7d66d5bd9cb6fa612d")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<vehicalmsgs>)))
  "Returns full string definition for message of type '<vehicalmsgs>"
  (cl:format cl:nil "time stamp~%uint16 frame_id~%float32[] vehicaldatas~%~%#[id,sort,lan,v,x,y]~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'vehicalmsgs)))
  "Returns full string definition for message of type 'vehicalmsgs"
  (cl:format cl:nil "time stamp~%uint16 frame_id~%float32[] vehicaldatas~%~%#[id,sort,lan,v,x,y]~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <vehicalmsgs>))
  (cl:+ 0
     8
     2
     4 (cl:reduce #'cl:+ (cl:slot-value msg 'vehicaldatas) :key #'(cl:lambda (ele) (cl:declare (cl:ignorable ele)) (cl:+ 4)))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <vehicalmsgs>))
  "Converts a ROS message object to a list"
  (cl:list 'vehicalmsgs
    (cl:cons ':stamp (stamp msg))
    (cl:cons ':frame_id (frame_id msg))
    (cl:cons ':vehicaldatas (vehicaldatas msg))
))
