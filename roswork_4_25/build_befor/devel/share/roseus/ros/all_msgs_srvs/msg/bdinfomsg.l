;; Auto-generated. Do not edit!


(when (boundp 'all_msgs_srvs::bdinfomsg)
  (if (not (find-package "ALL_MSGS_SRVS"))
    (make-package "ALL_MSGS_SRVS"))
  (shadow 'bdinfomsg (find-package "ALL_MSGS_SRVS")))
(unless (find-package "ALL_MSGS_SRVS::BDINFOMSG")
  (make-package "ALL_MSGS_SRVS::BDINFOMSG"))

(in-package "ROS")
;;//! \htmlinclude bdinfomsg.msg.html


(defclass all_msgs_srvs::bdinfomsg
  :super ros::object
  :slots (_node_name _device_id _isNew _timenow _lon _alt ))

(defmethod all_msgs_srvs::bdinfomsg
  (:init
   (&key
    ((:node_name __node_name) "")
    ((:device_id __device_id) 0)
    ((:isNew __isNew) 0)
    ((:timenow __timenow) "")
    ((:lon __lon) 0.0)
    ((:alt __alt) 0.0)
    )
   (send-super :init)
   (setq _node_name (string __node_name))
   (setq _device_id (round __device_id))
   (setq _isNew (round __isNew))
   (setq _timenow (string __timenow))
   (setq _lon (float __lon))
   (setq _alt (float __alt))
   self)
  (:node_name
   (&optional __node_name)
   (if __node_name (setq _node_name __node_name)) _node_name)
  (:device_id
   (&optional __device_id)
   (if __device_id (setq _device_id __device_id)) _device_id)
  (:isNew
   (&optional __isNew)
   (if __isNew (setq _isNew __isNew)) _isNew)
  (:timenow
   (&optional __timenow)
   (if __timenow (setq _timenow __timenow)) _timenow)
  (:lon
   (&optional __lon)
   (if __lon (setq _lon __lon)) _lon)
  (:alt
   (&optional __alt)
   (if __alt (setq _alt __alt)) _alt)
  (:serialization-length
   ()
   (+
    ;; string _node_name
    4 (length _node_name)
    ;; int32 _device_id
    4
    ;; int32 _isNew
    4
    ;; string _timenow
    4 (length _timenow)
    ;; float32 _lon
    4
    ;; float32 _alt
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; string _node_name
       (write-long (length _node_name) s) (princ _node_name s)
     ;; int32 _device_id
       (write-long _device_id s)
     ;; int32 _isNew
       (write-long _isNew s)
     ;; string _timenow
       (write-long (length _timenow) s) (princ _timenow s)
     ;; float32 _lon
       (sys::poke _lon (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _alt
       (sys::poke _alt (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; string _node_name
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _node_name (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; int32 _device_id
     (setq _device_id (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; int32 _isNew
     (setq _isNew (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; string _timenow
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _timenow (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; float32 _lon
     (setq _lon (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _alt
     (setq _alt (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get all_msgs_srvs::bdinfomsg :md5sum-) "8bc75b840511959862d72a0c796ab46a")
(setf (get all_msgs_srvs::bdinfomsg :datatype-) "all_msgs_srvs/bdinfomsg")
(setf (get all_msgs_srvs::bdinfomsg :definition-)
      "string node_name
int32 device_id
int32 isNew
string timenow
float32 lon
float32 alt

")



(provide :all_msgs_srvs/bdinfomsg "8bc75b840511959862d72a0c796ab46a")


