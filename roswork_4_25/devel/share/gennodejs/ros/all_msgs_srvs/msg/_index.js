
"use strict";

let vehicalmsgs = require('./vehicalmsgs.js');
let systemlog = require('./systemlog.js');
let errorMsgs = require('./errorMsgs.js');
let settings = require('./settings.js');
let bdinfomsg = require('./bdinfomsg.js');
let systemState = require('./systemState.js');

module.exports = {
  vehicalmsgs: vehicalmsgs,
  systemlog: systemlog,
  errorMsgs: errorMsgs,
  settings: settings,
  bdinfomsg: bdinfomsg,
  systemState: systemState,
};
