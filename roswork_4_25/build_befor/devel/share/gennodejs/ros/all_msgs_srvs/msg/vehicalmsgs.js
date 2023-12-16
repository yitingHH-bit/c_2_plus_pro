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

class vehicalmsgs {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.stamp = null;
      this.frame_id = null;
      this.vehicaldatas = null;
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
      if (initObj.hasOwnProperty('vehicaldatas')) {
        this.vehicaldatas = initObj.vehicaldatas
      }
      else {
        this.vehicaldatas = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type vehicalmsgs
    // Serialize message field [stamp]
    bufferOffset = _serializer.time(obj.stamp, buffer, bufferOffset);
    // Serialize message field [frame_id]
    bufferOffset = _serializer.uint16(obj.frame_id, buffer, bufferOffset);
    // Serialize message field [vehicaldatas]
    bufferOffset = _arraySerializer.float32(obj.vehicaldatas, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type vehicalmsgs
    let len;
    let data = new vehicalmsgs(null);
    // Deserialize message field [stamp]
    data.stamp = _deserializer.time(buffer, bufferOffset);
    // Deserialize message field [frame_id]
    data.frame_id = _deserializer.uint16(buffer, bufferOffset);
    // Deserialize message field [vehicaldatas]
    data.vehicaldatas = _arrayDeserializer.float32(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += 4 * object.vehicaldatas.length;
    return length + 14;
  }

  static datatype() {
    // Returns string type for a message object
    return 'all_msgs_srvs/vehicalmsgs';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '3744cc7a9e5f2c7d66d5bd9cb6fa612d';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    time stamp
    uint16 frame_id
    float32[] vehicaldatas
    
    #[id,sort,lan,v,x,y]
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new vehicalmsgs(null);
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

    if (msg.vehicaldatas !== undefined) {
      resolved.vehicaldatas = msg.vehicaldatas;
    }
    else {
      resolved.vehicaldatas = []
    }

    return resolved;
    }
};

module.exports = vehicalmsgs;
