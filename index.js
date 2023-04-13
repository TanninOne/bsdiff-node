'use strict';

const bsdiff = require('./build/Release/bsdiff.node');

const enoentPattern = /"([^"]*)" No such file or directory/;

// terrible error handling in bsdiff 
function makeError(errString) {
  const res = new Error(errString);
  const enoentMatch = errString.match(enoentPattern);;
  if (enoentMatch != null) {
    res['code'] = 'ENOENT';
    res['path'] = enoentMatch[1];
  } else {
    res['code'] = 'EUNKNOWN';
  }
  return res;
}

function promisify (fnName) {
  const fn = bsdiff[fnName];
  bsdiff[fnName] = function () {
    var args = Array.from(arguments);
    var callback;
    if (typeof args[args.length - 1] === 'function') {
      callback = args.pop();
    }
    return new Promise(function (resolve, reject) {
      args.push(function (result, errString) {
        if (errString) {
          reject(makeError(errString));
        } else if (result >= 100) {
          process.nextTick(resolve);
        }
        if (callback) {
          callback(result, makeError(errString));
        }
      });
      fn.apply(bsdiff, args);
    });
  };
}

promisify('diff');
promisify('patch');

module.exports = bsdiff;
