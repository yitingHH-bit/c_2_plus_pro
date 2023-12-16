;; Auto-generated. Do not edit!


(when (boundp 'all_msgs_srvs::systemlog)
  (if (not (find-package "ALL_MSGS_SRVS"))
    (make-package "ALL_MSGS_SRVS"))
  (shadow 'systemlog (find-package "ALL_MSGS_SRVS")))
(unless (find-package "ALL_MSGS_SRVS::SYSTEMLOG")
  (make-package "ALL_MSGS_SRVS::SYSTEMLOG"))

(in-package "ROS")
;;//! \htmlinclude systemlog.msg.html


(defclass all_msgs_srvs::systemlog
  :super ros::object
  :slots (_device_id _node_id _type _describe _data ))

(defmethod all_msgs_srvs::systemlog
  (:init
   (&key
    ((:device_id __device_id) 0)
    ((:node_id __node_id) "")
    ((:type __type) 0)
    ((:describe __describe) "")
    ((:data __data) (make-array 0 :initial-element 0.0 :element-type :float))
    )
   (send-super :init)
   (setq _device_id (round __device_id))
   (setq _node_id (string __node_id))
   (setq _type (round __type))
   (setq _describe (string __describe))
   (setq _data __data)
   self)
  (:device_id
   (&optional __device_id)
   (if __device_id (setq _device_id __device_id)) _device_id)
  (:node_id
   (&optional __node_id)
   (if __node_id (setq _node_id __node_id)) _node_id)
  (:type
   (&optional __type)
   (if __type (setq _type __type)) _type)
  (:describe
   (&optional __describe)
   (if __describe (setq _describe __describe)) _describe)
  (:data
   (&optional __data)
   (if __data (setq _data __data)) _data)
  (:serialization-length
   ()
   (+
    ;; uint16 _device_id
    2
    ;; string _node_id
    4 (length _node_id)
    ;; uint32 _type
    4
    ;; string _describe
    4 (length _describe)
    ;; float32[] _data
    (* 4    (length _data)) 4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint16 _device_id
       (write-word _device_id s)
     ;; string _node_id
       (write-long (length _node_id) s) (princ _node_id s)
     ;; uint32 _type
       (write-long _type s)
     ;; string _describe
       (write-long (length _describe) s) (princ _describe s)
     ;; float32[] _data
     (write-long (length _data) s)
     (dotimes (i (length _data))
       (sys::poke (elt _data i) (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
       )
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint16 _device_id
     (setq _device_id (sys::peek buf ptr- :short)) (incf ptr- 2)
   ;; string _node_id
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _node_id (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
   ;; uint32 _type
     (setq _type (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; string _describe
     (let (n) (setq n (sys::peek buf ptr- :integer)) (incf ptr- 4) (setq _describe (subseq buf ptr- (+ ptr- n))) (incf ptr- n))
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

(setf (get all_msgs_srvs::systemlog :md5sum-) "fe73fa608b885c27025369005afbf02b")
(setf (get all_msgs_srvs::systemlog :datatype-) "all_msgs_srvs/systemlog")
(setf (get all_msgs_srvs::systemlog :definition-)
      "uint16 device_id
string node_id
uint32 type
string describe
float32[] data


#数据意义
#device_id:表示设备id
#node_id:表示哪个节点
#type:表示什么类型的标识,1(warring),2(info),其他(debug),10(error致命错误),11(error一般错误)
#describe:问题表述,操作表述
#data:操作的数据



")



(provide :all_msgs_srvs/systemlog "fe73fa608b885c27025369005afbf02b")


