// Auto-generated. Do not edit!

// (in-package all_msgs_srvs.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class systemState {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.device_id = null;
      this.temperature = null;
      this.occ_RAM = null;
      this.occ_CPU = null;
      this.occ_GPU = null;
      this.occ_memory_sda1 = null;
      this.occ_memory_home = null;
      this.precess_state = null;
      this.camera_state = null;
      this.mmlidar_state = null;
    }
    else {
      if (initObj.hasOwnProperty('device_id')) {
        this.device_id = initObj.device_id
      }
      else {
        this.device_id = 0;
      }
      if (initObj.hasOwnProperty('temperature')) {
        this.temperature = initObj.temperature
      }
      else {
        this.temperature = 0.0;
      }
      if (initObj.hasOwnProperty('occ_RAM')) {
        this.occ_RAM = initObj.occ_RAM
      }
      else {
        this.occ_RAM = 0.0;
      }
      if (initObj.hasOwnProperty('occ_CPU')) {
        this.occ_CPU = initObj.occ_CPU
      }
      else {
        this.occ_CPU = 0.0;
      }
      if (initObj.hasOwnProperty('occ_GPU')) {
        this.occ_GPU = initObj.occ_GPU
      }
      else {
        this.occ_GPU = 0.0;
      }
      if (initObj.hasOwnProperty('occ_memory_sda1')) {
        this.occ_memory_sda1 = initObj.occ_memory_sda1
      }
      else {
        this.occ_memory_sda1 = 0.0;
      }
      if (initObj.hasOwnProperty('occ_memory_home')) {
        this.occ_memory_home = initObj.occ_memory_home
      }
      else {
        this.occ_memory_home = 0.0;
      }
      if (initObj.hasOwnProperty('precess_state')) {
        this.precess_state = initObj.precess_state
      }
      else {
        this.precess_state = 0.0;
      }
      if (initObj.hasOwnProperty('camera_state')) {
        this.camera_state = initObj.camera_state
      }
      else {
        this.camera_state = 0.0;
      }
      if (initObj.hasOwnProperty('mmlidar_state')) {
        this.mmlidar_state = initObj.mmlidar_state
      }
      else {
        this.mmlidar_state = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type systemState
    // Serialize message field [device_id]
    bufferOffset = _serializer.uint32(obj.device_id, buffer, bufferOffset);
    // Serialize message field [temperature]
    bufferOffset = _serializer.float32(obj.temperature, buffer, bufferOffset);
    // Serialize message field [occ_RAM]
    bufferOffset = _serializer.float32(obj.occ_RAM, buffer, bufferOffset);
    // Serialize message field [occ_CPU]
    bufferOffset = _serializer.float32(obj.occ_CPU, buffer, bufferOffset);
    // Serialize message field [occ_GPU]
    bufferOffset = _serializer.float32(obj.occ_GPU, buffer, bufferOffset);
    // Serialize message field [occ_memory_sda1]
    bufferOffset = _serializer.float32(obj.occ_memory_sda1, buffer, bufferOffset);
    // Serialize message field [occ_memory_home]
    bufferOffset = _serializer.float32(obj.occ_memory_home, buffer, bufferOffset);
    // Serialize message field [precess_state]
    bufferOffset = _serializer.float32(obj.precess_state, buffer, bufferOffset);
    // Serialize message field [camera_state]
    bufferOffset = _serializer.float32(obj.camera_state, buffer, bufferOffset);
    // Serialize message field [mmlidar_state]
    bufferOffset = _serializer.float32(obj.mmlidar_state, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type systemState
    let len;
    let data = new systemState(null);
    // Deserialize message field [device_id]
    data.device_id = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [temperature]
    data.temperature = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [occ_RAM]
    data.occ_RAM = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [occ_CPU]
    data.occ_CPU = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [occ_GPU]
    data.occ_GPU = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [occ_memory_sda1]
    data.occ_memory_sda1 = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [occ_memory_home]
    data.occ_memory_home = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [precess_state]
    data.precess_state = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [camera_state]
    data.camera_state = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [mmlidar_state]
    data.mmlidar_state = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 40;
  }

  static datatype() {
    // Returns string type for a message object
    return 'all_msgs_srvs/systemState';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '8e6672b93cb0d4c408677cdddcfc6c31';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint32 device_id
    float32 temperature
    float32 occ_RAM
    float32 occ_CPU
    float32 occ_GPU
    float32 occ_memory_sda1
    float32 occ_memory_home
    float32 precess_state
    float32 camera_state
    float32 mmlidar_state
    
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new systemState(null);
    if (msg.device_id !== undefined) {
      resolved.device_id = msg.device_id;
    }
    else {
      resolved.device_id = 0
    }

    if (msg.temperature !== undefined) {
      resolved.temperature = msg.temperature;
    }
    else {
      resolved.temperature = 0.0
    }

    if (msg.occ_RAM !== undefined) {
      resolved.occ_RAM = msg.occ_RAM;
    }
    else {
      resolved.occ_RAM = 0.0
    }

    if (msg.occ_CPU !== undefined) {
      resolved.occ_CPU = msg.occ_CPU;
    }
    else {
      resolved.occ_CPU = 0.0
    }

    if (msg.occ_GPU !== undefined) {
      resolved.occ_GPU = msg.occ_GPU;
    }
    else {
      resolved.occ_GPU = 0.0
    }

    if (msg.occ_memory_sda1 !== undefined) {
      resolved.occ_memory_sda1 = msg.occ_memory_sda1;
    }
    else {
      resolved.occ_memory_sda1 = 0.0
    }

    if (msg.occ_memory_home !== undefined) {
      resolved.occ_memory_home = msg.occ_memory_home;
    }
    else {
      resolved.occ_memory_home = 0.0
    }

    if (msg.precess_state !== undefined) {
      resolved.precess_state = msg.precess_state;
    }
    else {
      resolved.precess_state = 0.0
    }

    if (msg.camera_state !== undefined) {
      resolved.camera_state = msg.camera_state;
    }
    else {
      resolved.camera_state = 0.0
    }

    if (msg.mmlidar_state !== undefined) {
      resolved.mmlidar_state = msg.mmlidar_state;
    }
    else {
      resolved.mmlidar_state = 0.0
    }

    return resolved;
    }
};

module.exports = systemState;
