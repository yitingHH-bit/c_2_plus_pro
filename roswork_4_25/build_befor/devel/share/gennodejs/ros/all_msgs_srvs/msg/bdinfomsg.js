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

class bdinfomsg {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.node_name = null;
      this.device_id = null;
      this.isNew = null;
      this.timenow = null;
      this.lon = null;
      this.alt = null;
    }
    else {
      if (initObj.hasOwnProperty('node_name')) {
        this.node_name = initObj.node_name
      }
      else {
        this.node_name = '';
      }
      if (initObj.hasOwnProperty('device_id')) {
        this.device_id = initObj.device_id
      }
      else {
        this.device_id = 0;
      }
      if (initObj.hasOwnProperty('isNew')) {
        this.isNew = initObj.isNew
      }
      else {
        this.isNew = 0;
      }
      if (initObj.hasOwnProperty('timenow')) {
        this.timenow = initObj.timenow
      }
      else {
        this.timenow = '';
      }
      if (initObj.hasOwnProperty('lon')) {
        this.lon = initObj.lon
      }
      else {
        this.lon = 0.0;
      }
      if (initObj.hasOwnProperty('alt')) {
        this.alt = initObj.alt
      }
      else {
        this.alt = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type bdinfomsg
    // Serialize message field [node_name]
    bufferOffset = _serializer.string(obj.node_name, buffer, bufferOffset);
    // Serialize message field [device_id]
    bufferOffset = _serializer.int32(obj.device_id, buffer, bufferOffset);
    // Serialize message field [isNew]
    bufferOffset = _serializer.int32(obj.isNew, buffer, bufferOffset);
    // Serialize message field [timenow]
    bufferOffset = _serializer.string(obj.timenow, buffer, bufferOffset);
    // Serialize message field [lon]
    bufferOffset = _serializer.float32(obj.lon, buffer, bufferOffset);
    // Serialize message field [alt]
    bufferOffset = _serializer.float32(obj.alt, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type bdinfomsg
    let len;
    let data = new bdinfomsg(null);
    // Deserialize message field [node_name]
    data.node_name = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [device_id]
    data.device_id = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [isNew]
    data.isNew = _deserializer.int32(buffer, bufferOffset);
    // Deserialize message field [timenow]
    data.timenow = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [lon]
    data.lon = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [alt]
    data.alt = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.node_name.length;
    length += object.timenow.length;
    return length + 24;
  }

  static datatype() {
    // Returns string type for a message object
    return 'all_msgs_srvs/bdinfomsg';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '8bc75b840511959862d72a0c796ab46a';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    string node_name
    int32 device_id
    int32 isNew
    string timenow
    float32 lon
    float32 alt
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new bdinfomsg(null);
    if (msg.node_name !== undefined) {
      resolved.node_name = msg.node_name;
    }
    else {
      resolved.node_name = ''
    }

    if (msg.device_id !== undefined) {
      resolved.device_id = msg.device_id;
    }
    else {
      resolved.device_id = 0
    }

    if (msg.isNew !== undefined) {
      resolved.isNew = msg.isNew;
    }
    else {
      resolved.isNew = 0
    }

    if (msg.timenow !== undefined) {
      resolved.timenow = msg.timenow;
    }
    else {
      resolved.timenow = ''
    }

    if (msg.lon !== undefined) {
      resolved.lon = msg.lon;
    }
    else {
      resolved.lon = 0.0
    }

    if (msg.alt !== undefined) {
      resolved.alt = msg.alt;
    }
    else {
      resolved.alt = 0.0
    }

    return resolved;
    }
};

module.exports = bdinfomsg;
