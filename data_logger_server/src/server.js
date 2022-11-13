const express = require("express");
const bodyParser = require("body-parser");
const fs = require("fs/promises");

const app = express();

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

const PORT = 3000;

app.get("/test", (req, res) => {
  res.send("Hello world!");
});

app.post("/data", (req, res) => {
  //   console.log("**req.body**", req.body);
  writeToFile(
    "./log_aht10_sht41_bme280_v1_1nov_2h37.csv",
    JSON.stringify(req.body) + ",\n"
  );
  res.send().status(201);
});

app.post("/aht10_calibration", (req, res) => {
  console.log("aht10 data: ", req.body);
  writeToFile("./log_aht10_calibration.csv", JSON.stringify(req.body) + ",\n");
  res.send().status(201);
});

app.post("/aht20_calibration", (req, res) => {
  console.log("aht10 data: ", req.body);
  writeToFile("./log_aht20_calibration.csv", JSON.stringify(req.body) + ",\n");
  res.send().status(201);
});

app.post("/sht41_calibration", (req, res) => {
  console.log("sht41 data: ", req.body);
  writeToFile(
    "./log_sht41_2_calibration.csv",
    JSON.stringify(req.body) + ",\n"
  );
  res.send().status(201);
});

app.post("/bme280_calibration", (req, res) => {
  console.log("bme280 data: ", req.body);
  writeToFile(
    "./log_bme280_2_calibration.csv",
    JSON.stringify(req.body) + ",\n"
  );
  res.send().status(201);
});

async function writeToFile(filename, data) {
  try {
    await fs.writeFile(filename, data, { flag: "a" });
  } catch (e) {
    console.error(`Failed to write data to ${filename}. Error: ${e}`);
  }
}

app.listen(PORT, "0.0.0.0", function () {
  console.log(`Data logger server\nListening on PORT ${PORT}`);
});
