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

class settings {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.stamp = null;
      this.frame_id = null;
      this.type = null;
      this.which_param = null;
      this.setdatas = null;
    }
    else {
      if (initObj.hasOwnProperty('stamp')) {
        this.stamp = initObj.stamp
      }
      else {
        this.stamp = {secs: 0, nsecs: 0};
      }
      if (initObj.hasOwnProperty('frame_id')) {
        this.frame_id = initObj.frame_id
      }
      else {
        this.frame_id = 0;
      }
      if (initObj.hasOwnProperty('type')) {
        this.type = initObj.type
      }
      else {
        this.type = 0;
      }
      if (initObj.hasOwnProperty('which_param')) {
        this.which_param = initObj.which_param
      }
      else {
        this.which_param = 0;
      }
      if (initObj.hasOwnProperty('setdatas')) {
        this.setdatas = initObj.setdatas
      }
      else {
        this.setdatas = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type settings
    // Serialize message field [stamp]
    bufferOffset = _serializer.time(obj.stamp, buffer, bufferOffset);
    // Serialize message field [frame_id]
    bufferOffset = _serializer.uint16(obj.frame_id, buffer, bufferOffset);
    // Serialize message field [type]
    bufferOffset = _serializer.uint32(obj.type, buffer, bufferOffset);
    // Serialize message field [which_param]
    bufferOffset = _serializer.uint32(obj.which_param, buffer, bufferOffset);
    // Serialize message field [setdatas]
    bufferOffset = _arraySerializer.float32(obj.setdatas, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type settings
    let len;
    let data = new settings(null);
    // Deserialize message field [stamp]
    data.stamp = _deserializer.time(buffer, bufferOffset);
    // Deserialize message field [frame_id]
    data.frame_id = _deserializer.uint16(buffer, bufferOffset);
    // Deserialize message field [type]
    data.type = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [which_param]
    data.which_param = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [setdatas]
    data.setdatas = _arrayDeserializer.float32(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 4 * object.setdatas.length;
    return length + 22;
  }

  static datatype() {
    // Returns string type for a message object
    return 'all_msgs_srvs/settings';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '2c5fd6a4bb0c365ef3a2eff725118bfe';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    time stamp
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
    
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new settings(null);
    if (msg.stamp !== undefined) {
      resolved.stamp = msg.stamp;
    }
    else {
      resolved.stamp = {secs: 0, nsecs: 0}
    }

    if (msg.frame_id !== undefined) {
      resolved.frame_id = msg.frame_id;
    }
    else {
      resolved.frame_id = 0
    }

    if (msg.type !== undefined) {
      resolved.type = msg.type;
    }
    else {
      resolved.type = 0
    }

    if (msg.which_param !== undefined) {
      resolved.which_param = msg.which_param;
    }
    else {
      resolved.which_param = 0
    }

    if (msg.setdatas !== undefined) {
      resolved.setdatas = msg.setdatas;
    }
    else {
      resolved.setdatas = []
    }

    return resolved;
    }
};

module.exports = settings;
