;; Auto-generated. Do not edit!


(when (boundp 'all_msgs_srvs::errorMsgs)
  (if (not (find-package "ALL_MSGS_SRVS"))
    (make-package "ALL_MSGS_SRVS"))
  (shadow 'errorMsgs (find-package "ALL_MSGS_SRVS")))
(unless (find-package "ALL_MSGS_SRVS::ERRORMSGS")
  (make-package "ALL_MSGS_SRVS::ERRORMSGS"))

(in-package "ROS")
;;//! \htmlinclude errorMsgs.msg.html


(defclass all_msgs_srvs::errorMsgs
  :super ros::object
  :slots (_device_id _type _data ))

(defmethod all_msgs_srvs::errorMsgs
  (:init
   (&key
    ((:device_id __device_id) 0)
    ((:type __type) 0)
    ((:data __data) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _device_id (round __device_id))
   (setq _type (round __type))
   (setq _data __data)
   self)
  (:device_id
   (&optional __device_id)
   (if __device_id (setq _device_id __device_id)) _device_id)
  (:type
   (&optional __type)
   (if __type (setq _type __type)) _type)
  (:data
   (&optional __data)
   (if __data (setq _data __data)) _data)
  (:serialization-length
   ()
   (+
    ;; uint32 _device_id
    4
    ;; uint32 _type
    4
    ;; float32[] _data
    (* 4    (length _data)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint32 _device_id
       (write-long _device_id s)
     ;; uint32 _type
       (write-long _type s)
     ;; float32[] _data
     (write-long (length _data) s)
     (dotimes (i (length _data))
       (sys::poke (elt _data i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint32 _device_id
     (setq _device_id (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _type
     (setq _type (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32[] _data
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _data (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _data i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;;
   self)
  )

(setf (get all_msgs_srvs::errorMsgs :md5sum-) "da293913ab09cf0b6203d2182912db2a")
(setf (get all_msgs_srvs::errorMsgs :datatype-) "all_msgs_srvs/errorMsgs")
(setf (get all_msgs_srvs::errorMsgs :definition-)
      "uint32 device_id
uint32 type
float32[] data

#type:0错误信息,1警告消息,2系统消息




")



(provide :all_msgs_srvs/errorMsgs "da293913ab09cf0b6203d2182912db2a")


