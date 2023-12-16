;; Auto-generated. Do not edit!


(when (boundp 'all_msgs_srvs::settingcallback)
  (if (not (find-package "ALL_MSGS_SRVS"))
    (make-package "ALL_MSGS_SRVS"))
  (shadow 'settingcallback (find-package "ALL_MSGS_SRVS")))
(unless (find-package "ALL_MSGS_SRVS::SETTINGCALLBACK")
  (make-package "ALL_MSGS_SRVS::SETTINGCALLBACK"))
(unless (find-package "ALL_MSGS_SRVS::SETTINGCALLBACKREQUEST")
  (make-package "ALL_MSGS_SRVS::SETTINGCALLBACKREQUEST"))
(unless (find-package "ALL_MSGS_SRVS::SETTINGCALLBACKRESPONSE")
  (make-package "ALL_MSGS_SRVS::SETTINGCALLBACKRESPONSE"))

(in-package "ROS")





(defclass all_msgs_srvs::settingcallbackRequest
  :super ros::object
  :slots (_fram_id _type _data ))

(defmethod all_msgs_srvs::settingcallbackRequest
  (:init
   (&key
    ((:fram_id __fram_id) 0)
    ((:type __type) 0)
    ((:data __data) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _fram_id (round __fram_id))
   (setq _type (round __type))
   (setq _data __data)
   self)
  (:fram_id
   (&optional __fram_id)
   (if __fram_id (setq _fram_id __fram_id)) _fram_id)
  (:type
   (&optional __type)
   (if __type (setq _type __type)) _type)
  (:data
   (&optional __data)
   (if __data (setq _data __data)) _data)
  (:serialization-length
   ()
   (+
    ;; uint32 _fram_id
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
     ;; uint32 _fram_id
       (write-long _fram_id s)
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
   ;; uint32 _fram_id
     (setq _fram_id (sys::peek buf ptr- :integer)) (incf ptr- 4)
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

(defclass all_msgs_srvs::settingcallbackResponse
  :super ros::object
  :slots (_setdatas ))

(defmethod all_msgs_srvs::settingcallbackResponse
  (:init
   (&key
    ((:setdatas __setdatas) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _setdatas __setdatas)
   self)
  (:setdatas
   (&optional __setdatas)
   (if __setdatas (setq _setdatas __setdatas)) _setdatas)
  (:serialization-length
   ()
   (+
    ;; float32[] _setdatas
    (* 4    (length _setdatas)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; float32[] _setdatas
     (write-long (length _setdatas) s)
     (dotimes (i (length _setdatas))
       (sys::poke (elt _setdatas i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; float32[] _setdatas
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _setdatas (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _setdatas i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;;
   self)
  )

(defclass all_msgs_srvs::settingcallback
  :super ros::object
  :slots ())

(setf (get all_msgs_srvs::settingcallback :md5sum-) "a893383d462b2192990ef4619919d478")
(setf (get all_msgs_srvs::settingcallback :datatype-) "all_msgs_srvs/settingcallback")
(setf (get all_msgs_srvs::settingcallback :request) all_msgs_srvs::settingcallbackRequest)
(setf (get all_msgs_srvs::settingcallback :response) all_msgs_srvs::settingcallbackResponse)

(defmethod all_msgs_srvs::settingcallbackRequest
  (:response () (instance all_msgs_srvs::settingcallbackResponse :init)))

(setf (get all_msgs_srvs::settingcallbackRequest :md5sum-) "a893383d462b2192990ef4619919d478")
(setf (get all_msgs_srvs::settingcallbackRequest :datatype-) "all_msgs_srvs/settingcallbackRequest")
(setf (get all_msgs_srvs::settingcallbackRequest :definition-)
      "uint32 fram_id
uint32 type
float32[] data
---
float32[] setdatas


")

(setf (get all_msgs_srvs::settingcallbackResponse :md5sum-) "a893383d462b2192990ef4619919d478")
(setf (get all_msgs_srvs::settingcallbackResponse :datatype-) "all_msgs_srvs/settingcallbackResponse")
(setf (get all_msgs_srvs::settingcallbackResponse :definition-)
      "uint32 fram_id
uint32 type
float32[] data
---
float32[] setdatas


")



(provide :all_msgs_srvs/settingcallback "a893383d462b2192990ef4619919d478")


