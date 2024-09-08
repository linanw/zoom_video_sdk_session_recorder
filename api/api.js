// const { exec } = require("child_process");
var fs = require('fs');
var express = require('express');
var app = express();
var PORT = 81;
var bot_proc;
var log_res;

var access = fs.createWriteStream('log.txt');
process.stdout.write = process.stderr.write = access.write.bind(access);

app.use('/', express.static('static'))

app.get('/start', (req, res) => {
  log_res = res;
  if (bot_proc) {
    res.json({ "success": "1", "message": "Linux bot is already started to record videos." });
  } else {
    // linanw: "stdbuf -oL -eL" to ensure the child_process stdout/stderr output immediately.
    bot_proc = require('child_process').spawn('stdbuf',
      ['-oL', '-eL', '../bin/zoom_v-sdk_linux_bot'], {stdio: ['pipe', access, access]}
    );
    // bot_proc.stdout.on("data", (data) => { console.log(new TextDecoder().decode(data)); });
    // bot_proc.stderr.on("data", (data) => { console.error(new TextDecoder().decode(data)); });
    res.json({ "success": "1", "message": "starting linux bot to record videos." });
  }
})

app.get('/stop', (req, res) => {
  if (bot_proc) {
    bot_proc.kill(2);
    bot_proc = undefined;
    res.json({ success: '1', message: 'Stopped.' });
  } else {
    res.json({ success: '1', message: 'Not started.' });
  }
  if (log_res) log_res.end();
})

app.get('/clean', (req, res) => {
  require('child_process').exec(
    'rm -f ../recording/*',
    (err, stdout, stderr) => {
      if (err) res.json({ success: '0', msg: `${err.message}` });
      else if (stderr) res.json({ success: '0', msg: `${stderr}` });
      else res.json({ success: '1', stdout: `${stdout}` });
    }
  );
})

app.get('/log', (req, res) => {
  require('child_process').exec(
    'tail -n 50 log.txt',
    (err, stdout, stderr) => {
      if (err) res.json({ success: '0', msg: `${err.message}` });
      else if (stderr) res.json({ success: '0', msg: `${stderr}` });
      else res.json({ success: '1', stdout: `${stdout}` });
    }
  );
})

app.listen(PORT, function (err) {
  if (err) console.log(err);
  console.log("Server listening on PORT", PORT);
}); 