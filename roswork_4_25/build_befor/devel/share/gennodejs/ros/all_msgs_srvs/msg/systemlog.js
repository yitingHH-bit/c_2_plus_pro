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

class systemlog {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.device_id = null;
      this.node_id = null;
      this.type = null;
      this.describe = null;
      this.data = null;
    }
    else {
      if (initObj.hasOwnProperty('device_id')) {
        this.device_id = initObj.device_id
      }
      else {
        this.device_id = 0;
      }
      if (initObj.hasOwnProperty('node_id')) {
        this.node_id = initObj.node_id
      }
      else {
        this.node_id = '';
      }
      if (initObj.hasOwnProperty('type')) {
        this.type = initObj.type
      }
      else {
        this.type = 0;
      }
      if (initObj.hasOwnProperty('describe')) {
        this.describe = initObj.describe
      }
      else {
        this.describe = '';
      }
      if (initObj.hasOwnProperty('data')) {
        this.data = initObj.data
      }
      else {
        this.data = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type systemlog
    // Serialize message field [device_id]
    bufferOffset = _serializer.uint16(obj.device_id, buffer, bufferOffset);
    // Serialize message field [node_id]
    bufferOffset = _serializer.string(obj.node_id, buffer, bufferOffset);
    // Serialize message field [type]
    bufferOffset = _serializer.uint32(obj.type, buffer, bufferOffset);
    // Serialize message field [describe]
    bufferOffset = _serializer.string(obj.describe, buffer, bufferOffset);
    // Serialize message field [data]
    bufferOffset = _arraySerializer.float32(obj.data, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type systemlog
    let len;
    let data = new systemlog(null);
    // Deserialize message field [device_id]
    data.device_id = _deserializer.uint16(buffer, bufferOffset);
    // Deserialize message field [node_id]
    data.node_id = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [type]
    data.type = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [describe]
    data.describe = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [data]
    data.data = _arrayDeserializer.float32(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.node_id.length;
    length += object.describe.length;
    length += 4 * object.data.length;
    return length + 18;
  }

  static datatype() {
    // Returns string type for a message object
    return 'all_msgs_srvs/systemlog';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'fe73fa608b885c27025369005afbf02b';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint16 device_id
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
    
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new systemlog(null);
    if (msg.device_id !== undefined) {
      resolved.device_id = msg.device_id;
    }
    else {
      resolved.device_id = 0
    }

    if (msg.node_id !== undefined) {
      resolved.node_id = msg.node_id;
    }
    else {
      resolved.node_id = ''
    }

    if (msg.type !== undefined) {
      resolved.type = msg.type;
    }
    else {
      resolved.type = 0
    }

    if (msg.describe !== undefined) {
      resolved.describe = msg.describe;
    }
    else {
      resolved.describe = ''
    }

    if (msg.data !== undefined) {
      resolved.data = msg.data;
    }
    else {
      resolved.data = []
    }

    return resolved;
    }
};

module.exports = systemlog;
