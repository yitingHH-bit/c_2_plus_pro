// Auto-generated. Do not edit!

// (in-package all_msgs_srvs.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------


//-----------------------------------------------------------

class settingcallbackRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.fram_id = null;
      this.type = null;
      this.data = null;
    }
    else {
      if (initObj.hasOwnProperty('fram_id')) {
        this.fram_id = initObj.fram_id
      }
      else {
        this.fram_id = 0;
      }
      if (initObj.hasOwnProperty('type')) {
        this.type = initObj.type
      }
      else {
        this.type = 0;
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
    // Serializes a message object of type settingcallbackRequest
    // Serialize message field [fram_id]
    bufferOffset = _serializer.uint32(obj.fram_id, buffer, bufferOffset);
    // Serialize message field [type]
    bufferOffset = _serializer.uint32(obj.type, buffer, bufferOffset);
    // Serialize message field [data]
    bufferOffset = _arraySerializer.float32(obj.data, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type settingcallbackRequest
    let len;
    let data = new settingcallbackRequest(null);
    // Deserialize message field [fram_id]
    data.fram_id = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [type]
    data.type = _deserializer.uint32(buffer, bufferOffset);
    // Deserialize message field [data]
    data.data = _arrayDeserializer.float32(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 4 * object.data.length;
    return length + 12;
  }

  static datatype() {
    // Returns string type for a service object
    return 'all_msgs_srvs/settingcallbackRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'e5c7050b588d4aced7c3d6a605ad2ff0';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint32 fram_id
    uint32 type
    float32[] data
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new settingcallbackRequest(null);
    if (msg.fram_id !== undefined) {
      resolved.fram_id = msg.fram_id;
    }
    else {
      resolved.fram_id = 0
    }

    if (msg.type !== undefined) {
      resolved.type = msg.type;
    }
    else {
      resolved.type = 0
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

class settingcallbackResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.setdatas = null;
    }
    else {
      if (initObj.hasOwnProperty('setdatas')) {
        this.setdatas = initObj.setdatas
      }
      else {
        this.setdatas = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type settingcallbackResponse
    // Serialize message field [setdatas]
    bufferOffset = _arraySerializer.float32(obj.setdatas, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type settingcallbackResponse
    let len;
    let data = new settingcallbackResponse(null);
    // Deserialize message field [setdatas]
    data.setdatas = _arrayDeserializer.float32(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 4 * object.setdatas.length;
    return length + 4;
  }

  static datatype() {
    // Returns string type for a service object
    return 'all_msgs_srvs/settingcallbackResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '5c05e33076ac043125f4405803716ae5';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    float32[] setdatas
    
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new settingcallbackResponse(null);
    if (msg.setdatas !== undefined) {
      resolved.setdatas = msg.setdatas;
    }
    else {
      resolved.setdatas = []
    }

    return resolved;
    }
};

module.exports = {
  Request: settingcallbackRequest,
  Response: settingcallbackResponse,
  md5sum() { return 'a893383d462b2192990ef4619919d478'; },
  datatype() { return 'all_msgs_srvs/settingcallback'; }
};
