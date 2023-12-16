;; Auto-generated. Do not edit!


(when (boundp 'all_msgs_srvs::systemState)
  (if (not (find-package "ALL_MSGS_SRVS"))
    (make-package "ALL_MSGS_SRVS"))
  (shadow 'systemState (find-package "ALL_MSGS_SRVS")))
(unless (find-package "ALL_MSGS_SRVS::SYSTEMSTATE")
  (make-package "ALL_MSGS_SRVS::SYSTEMSTATE"))

(in-package "ROS")
;;//! \htmlinclude systemState.msg.html


(defclass all_msgs_srvs::systemState
  :super ros::object
  :slots (_device_id _temperature _occ_RAM _occ_CPU _occ_GPU _occ_memory_sda1 _occ_memory_home _precess_state _camera_state _mmlidar_state ))

(defmethod all_msgs_srvs::systemState
  (:init
   (&key
    ((:device_id __device_id) 0)
    ((:temperature __temperature) 0.0)
    ((:occ_RAM __occ_RAM) 0.0)
    ((:occ_CPU __occ_CPU) 0.0)
    ((:occ_GPU __occ_GPU) 0.0)
    ((:occ_memory_sda1 __occ_memory_sda1) 0.0)
    ((:occ_memory_home __occ_memory_home) 0.0)
    ((:precess_state __precess_state) 0.0)
    ((:camera_state __camera_state) 0.0)
    ((:mmlidar_state __mmlidar_state) 0.0)
    )
   (send-super :init)
   (setq _device_id (round __device_id))
   (setq _temperature (float __temperature))
   (setq _occ_RAM (float __occ_RAM))
   (setq _occ_CPU (float __occ_CPU))
   (setq _occ_GPU (float __occ_GPU))
   (setq _occ_memory_sda1 (float __occ_memory_sda1))
   (setq _occ_memory_home (float __occ_memory_home))
   (setq _precess_state (float __precess_state))
   (setq _camera_state (float __camera_state))
   (setq _mmlidar_state (float __mmlidar_state))
   self)
  (:device_id
   (&optional __device_id)
   (if __device_id (setq _device_id __device_id)) _device_id)
  (:temperature
   (&optional __temperature)
   (if __temperature (setq _temperature __temperature)) _temperature)
  (:occ_RAM
   (&optional __occ_RAM)
   (if __occ_RAM (setq _occ_RAM __occ_RAM)) _occ_RAM)
  (:occ_CPU
   (&optional __occ_CPU)
   (if __occ_CPU (setq _occ_CPU __occ_CPU)) _occ_CPU)
  (:occ_GPU
   (&optional __occ_GPU)
   (if __occ_GPU (setq _occ_GPU __occ_GPU)) _occ_GPU)
  (:occ_memory_sda1
   (&optional __occ_memory_sda1)
   (if __occ_memory_sda1 (setq _occ_memory_sda1 __occ_memory_sda1)) _occ_memory_sda1)
  (:occ_memory_home
   (&optional __occ_memory_home)
   (if __occ_memory_home (setq _occ_memory_home __occ_memory_home)) _occ_memory_home)
  (:precess_state
   (&optional __precess_state)
   (if __precess_state (setq _precess_state __precess_state)) _precess_state)
  (:camera_state
   (&optional __camera_state)
   (if __camera_state (setq _camera_state __camera_state)) _camera_state)
  (:mmlidar_state
   (&optional __mmlidar_state)
   (if __mmlidar_state (setq _mmlidar_state __mmlidar_state)) _mmlidar_state)
  (:serialization-length
   ()
   (+
    ;; uint32 _device_id
    4
    ;; float32 _temperature
    4
    ;; float32 _occ_RAM
    4
    ;; float32 _occ_CPU
    4
    ;; float32 _occ_GPU
    4
    ;; float32 _occ_memory_sda1
    4
    ;; float32 _occ_memory_home
    4
    ;; float32 _precess_state
    4
    ;; float32 _camera_state
    4
    ;; float32 _mmlidar_state
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint32 _device_id
       (write-long _device_id s)
     ;; float32 _temperature
       (sys::poke _temperature (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _occ_RAM
       (sys::poke _occ_RAM (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _occ_CPU
       (sys::poke _occ_CPU (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _occ_GPU
       (sys::poke _occ_GPU (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _occ_memory_sda1
       (sys::poke _occ_memory_sda1 (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _occ_memory_home
       (sys::poke _occ_memory_home (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _precess_state
       (sys::poke _precess_state (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _camera_state
       (sys::poke _camera_state (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;; float32 _mmlidar_state
       (sys::poke _mmlidar_state (send s :buffer) (send s :count) :float) (incf (stream-count s) 4)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint32 _device_id
     (setq _device_id (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;; float32 _temperature
     (setq _temperature (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _occ_RAM
     (setq _occ_RAM (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _occ_CPU
     (setq _occ_CPU (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _occ_GPU
     (setq _occ_GPU (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _occ_memory_sda1
     (setq _occ_memory_sda1 (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _occ_memory_home
     (setq _occ_memory_home (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _precess_state
     (setq _precess_state (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _camera_state
     (setq _camera_state (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;; float32 _mmlidar_state
     (setq _mmlidar_state (sys::peek buf ptr- :float)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get all_msgs_srvs::systemState :md5sum-) "8e6672b93cb0d4c408677cdddcfc6c31")
(setf (get all_msgs_srvs::systemState :datatype-) "all_msgs_srvs/systemState")
(setf (get all_msgs_srvs::systemState :definition-)
      "uint32 device_id
float32 temperature
float32 occ_RAM
float32 occ_CPU
float32 occ_GPU
float32 occ_memory_sda1
float32 occ_memory_home
float32 precess_state
float32 camera_state
float32 mmlidar_state



")



(provide :all_msgs_srvs/systemState "8e6672b93cb0d4c408677cdddcfc6c31")


