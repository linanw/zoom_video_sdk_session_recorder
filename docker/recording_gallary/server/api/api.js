const { exec } = require("child_process");
var express = require('express');
var app = express();
var PORT = 80;

app.use('/', express.static('static'))

app.get('/refresh', (req, res) => {
  // exec("rm -rf static/gallary && thumbsup --input recording/ --output static/gallary/ --title Recordings --thumb-size 200 --cleanup", (error, stdout, stderr) => {
  exec("thumbsup --input recording/ --output static/gallary/ --title Recordings --thumb-size 200 --cleanup", (error, stdout, stderr) => {
    if (error) {
      console.log(`error: ${error.message}`);
      res.send("Refresh failed: " + error.message);
      return;
    }
    if (stderr) {
      console.log(`stderr: ${stderr}`);
      res.send("Refresh failed: " + stderr);
      return;
    }
    console.log(`stdout: ${stdout}`);
    res.json({ success: '1', stdout: `${stdout}` });
  });
})

app.listen(PORT, function (err) {
  if (err) console.log(err);
  console.log("Server listening on PORT", PORT);
}); 