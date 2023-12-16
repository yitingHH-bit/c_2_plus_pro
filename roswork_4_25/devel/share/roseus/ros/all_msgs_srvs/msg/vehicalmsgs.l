;; Auto-generated. Do not edit!


(when (boundp 'all_msgs_srvs::vehicalmsgs)
  (if (not (find-package "ALL_MSGS_SRVS"))
    (make-package "ALL_MSGS_SRVS"))
  (shadow 'vehicalmsgs (find-package "ALL_MSGS_SRVS")))
(unless (find-package "ALL_MSGS_SRVS::VEHICALMSGS")
  (make-package "ALL_MSGS_SRVS::VEHICALMSGS"))

(in-package "ROS")
;;//! \htmlinclude vehicalmsgs.msg.html


(defclass all_msgs_srvs::vehicalmsgs
  :super ros::object
  :slots (_stamp _frame_id _vehicaldatas ))

(defmethod all_msgs_srvs::vehicalmsgs
  (:init
   (&key
    ((:stamp __stamp) (instance ros::time :init))
    ((:frame_id __frame_id) 0)
    ((:vehicaldatas __vehicaldatas) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _stamp __stamp)
   (setq _frame_id (round __frame_id))
   (setq _vehicaldatas __vehicaldatas)
   self)
  (:stamp
   (&optional __stamp)
   (if __stamp (setq _stamp __stamp)) _stamp)
  (:frame_id
   (&optional __frame_id)
   (if __frame_id (setq _frame_id __frame_id)) _frame_id)
  (:vehicaldatas
   (&optional __vehicaldatas)
   (if __vehicaldatas (setq _vehicaldatas __vehicaldatas)) _vehicaldatas)
  (:serialization-length
   ()
   (+
    ;; time _stamp
    8
    ;; uint16 _frame_id
    2
    ;; float32[] _vehicaldatas
    (* 4    (length _vehicaldatas)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; time _stamp
       (write-long (send _stamp :sec) s) (write-long (send _stamp :nsec) s)
     ;; uint16 _frame_id
       (write-word _frame_id s)
     ;; float32[] _vehicaldatas
     (write-long (length _vehicaldatas) s)
     (dotimes (i (length _vehicaldatas))
       (sys::poke (elt _vehicaldatas i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; time _stamp
     (send _stamp :sec (sys::peek buf ptr- :integer)) (incf ptr- 4)  (send _stamp :nsec (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; uint16 _frame_id
     (setq _frame_id (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; float32[] _vehicaldatas
   (let (n)
     (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4)
     (setq _vehicaldatas (instantiate float-vector n))
     (dotimes (i n)
     (setf (elt _vehicaldatas i) (sys::peek buf ptr- :float)) (incf ptr- 4)
     ))
   ;;
   self)
  )

(setf (get all_msgs_srvs::vehicalmsgs :md5sum-) "3744cc7a9e5f2c7d66d5bd9cb6fa612d")
(setf (get all_msgs_srvs::vehicalmsgs :datatype-) "all_msgs_srvs/vehicalmsgs")
(setf (get all_msgs_srvs::vehicalmsgs :definition-)
      "time stamp
uint16 frame_id
float32[] vehicaldatas

#[id,sort,lan,v,x,y]

")



(provide :all_msgs_srvs/vehicalmsgs "3744cc7a9e5f2c7d66d5bd9cb6fa612d")


