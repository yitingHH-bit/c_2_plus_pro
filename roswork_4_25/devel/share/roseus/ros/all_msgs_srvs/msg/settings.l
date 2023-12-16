;; Auto-generated. Do not edit!


(when (boundp 'all_msgs_srvs::settings)
  (if (not (find-package "ALL_MSGS_SRVS"))
    (make-package "ALL_MSGS_SRVS"))
  (shadow 'settings (find-package "ALL_MSGS_SRVS")))
(unless (find-package "ALL_MSGS_SRVS::SETTINGS")
  (make-package "ALL_MSGS_SRVS::SETTINGS"))

(in-package "ROS")
;;//! \htmlinclude settings.msg.html


(defclass all_msgs_srvs::settings
  :super ros::object
  :slots (_stamp _frame_id _type _which_param _setdatas ))

(defmethod all_msgs_srvs::settings
  (:init
   (&key
    ((:stamp __stamp) (instance ros::time :init))
    ((:frame_id __frame_id) 0)
    ((:type __type) 0)
    ((:which_param __which_param) 0)
    ((:setdatas __setdatas) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _stamp __stamp)
   (setq _frame_id (round __frame_id))
   (setq _type (round __type))
   (setq _which_param (round __which_param))
   (setq _setdatas __setdatas)
   self)
  (:stamp
   (&optional __stamp)
   (if __stamp (setq _stamp __stamp)) _stamp)
  (:frame_id
   (&optional __frame_id)
   (if __frame_id (setq _frame_id __frame_id)) _frame_id)
  (:type
   (&optional __type)
   (if __type (setq _type __type)) _type)
  (:which_param
   (&optional __which_param)
   (if __which_param (setq _which_param __which_param)) _which_param)
  (:setdatas
   (&optional __setdatas)
   (if __setdatas (setq _setdatas __setdatas)) _setdatas)
  (:serialization-length
   ()
   (+
    ;; time _stamp
    8
    ;; uint16 _frame_id
    2
    ;; uint32 _type
    4
    ;; uint32 _which_param
    4
    ;; float32[] _setdatas
    (* 4    (length _setdatas)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; time _stamp
       (write-long (send _stamp :sec) s) (write-long (send _stamp :nsec) s)
     ;; uint16 _frame_id
       (write-word _frame_id s)
     ;; uint32 _type
       (write-long _type s)
     ;; uint32 _which_param
       (write-long _which_param s)
     ;; float32[] _setdatas
     (write-long (length _setdatas) s)
     (dotimes (i (length _setdatas))
       (sys::poke (elt _setdatas i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; time _stamp
     (send _stamp :sec (sys::peek buf ptr- :integer)) (incf ptr- 4)  (send _stamp :nsec (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint16 _frame_id
     (setq _frame_id (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; uint32 _type
     (setq _type (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint32 _which_param
     (setq _which_param (sys::peek buf ptr- :integer)) (incf ptr- 4)
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

(setf (get all_msgs_srvs::settings :md5sum-) "2c5fd6a4bb0c365ef3a2eff725118bfe")
(setf (get all_msgs_srvs::settings :datatype-) "all_msgs_srvs/settings")
(setf (get all_msgs_srvs::settings :definition-)
      "time stamp
uint16 frame_id
uint32 type
uint32 which_param
float32[] setdatas


#type:
#lan_info(0)
#statis_info(1)
#Transform_info(2)
#Network_info(3)
#Install_Info(4)


#which_param
#1101:给统计节点设置热参数[A,B,C,车道数,车道1断面数,车道1断面1距离,车道1断面2距离,...,车道n断面数,车道n断面1距离,车道n断面2距离...]
#1102:[周期]
#3001:[设备id]
#1001:车道坐标设置 [车道总数,]



")



(provide :all_msgs_srvs/settings "2c5fd6a4bb0c365ef3a2eff725118bfe")


